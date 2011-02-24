#include <iostream>
#include <fstream>
#include <vector>

#include <cstring>

#include "api/BamAux.h"
#include "api/BamAlignment.h"
#include "api/BamReader.h"

#include "bam_loader_global.h"
#include "cnvs_file_writer.h"

int loglvl = 3;

using namespace std;

typedef struct _target
{
	string  name;
	int32_t start;
	int32_t end;
} t_Target;

const char * fn_target_region = "";
const char * fn_output = "";
const char * program_name = "";
const char * fn_bam_list = "";



enum _algorithms
{
	ALGO_READ_COV,
	ALGO_BASE_COV
} algorithm;

vector<string> lst_fn_bam;

int compare_cnvs_cov_t(const void * d1, const void * d2)
{
	return *(cnvs_cov_t *)d1 - *(cnvs_cov_t *)d2;
}

int32_t int32_min(int32_t size1, int32_t size2)
{
	return size1<size2?size1:size2;
}

void usage(int errcode)
{
	cout<<"Usage : "<<program_name<<" <options> <bam filename1> <bam filename 2> ... "<<endl;
	cout<<"\nOptions: "<<endl;
	cout<<"\t -b <bed filename>  [REQUIRED] Specify the target region description file name."<<endl;
	cout<<"\t -o <out filename>  [REQUIRED] Specify the output filename in which the coverage is stored."<<endl;
	cout<<"\t -l <list filename> [OPTIONAL] Specify a file containing the filenames of bam files."<<endl;
	cout<<"\t                               Required if no bam file is given on the commandline."<<endl;
	cout<<"\t -B                 [OPTIONAL] Use base coverage instead of read coverage."<<endl;
	
	
	exit(errcode);
}

int main(int argc, char* argv[])
{

	program_name = *argv;

	argc--;
	argv++;

	algorithm = ALGO_READ_COV;

	while(argc > 0 && (*argv)[0] == '-')
	{
		switch((*argv)[1])
		{
			case 'o':
				argc--;
				argv++;
				fn_output = *argv;
				break;
			case 'b':
				argc--;
				argv++;
				fn_target_region = *argv;
				break;
			case 'l':
				argc--;
				argv++;
				fn_bam_list = *argv;
				break;
			case 'B':
				algorithm = ALGO_BASE_COV;
				break;
			default:
				cerr<<"Unrecognized option: -"<<(*argv)[1];
				usage(0);
		}

		argc--;
		argv++;
	}

	if(strlen(fn_target_region) == 0)  
	{
		cerr<<"Missing -b argument!"<<endl<<endl;
		usage(0);
	}

	if(strlen(fn_output) == 0)
	{
		cerr<<"Missing -o argument!"<<endl<<endl;
		usage(0);
	}

	// Generate bam file list
	
	if(strlen(fn_bam_list) != 0)
	{
		ifstream f_bam_list;
		f_bam_list.open(fn_bam_list, ifstream::in);
		if(!f_bam_list.good())
			cerr<<"Unable to open the specified list file: "<<fn_bam_list<<endl;
		else
		{
			string oneFn;
			while(!f_bam_list.eof())
			{
				oneFn = "";
				f_bam_list>>oneFn;
				if(oneFn.length() > 4)
					lst_fn_bam.push_back(oneFn);
			}
		}
	}

	while(argc > 0)
	{
		lst_fn_bam.push_back(*argv);
		argc--;
		argv++;
	}



	if(lst_fn_bam.size() <= 0)
	{
		cerr<<"Missing bam file(s)!"<<endl<<endl;
		usage(0);
	}


	// Load target regions
	
	vector<t_Target> Targets;

	if(loglvl >= 1)
		cerr<<"Opening bed file: "<<fn_target_region<<endl;
	
	ifstream f_target;
	f_target.open(fn_target_region, ifstream::in);

	if(!f_target.good()) {
		cerr<<"Unable to open target region file: "<<fn_target_region<<endl;
		exit(2);
	}

	while(!f_target.eof())
	{
		t_Target newTarget;
		f_target>>newTarget.name>>newTarget.start>>newTarget.end;
		if(newTarget.name.length() < 1)
			continue;
		newTarget.end--;
		Targets.push_back(newTarget);
	}

	if(loglvl >= 2)
		cerr<<"Total number of targets loaded: "<<Targets.size()<<endl;

	if(Targets.size() < 1)
	{
		cerr<<"No target region is loaded!"<<endl;
		exit(1);
	}
	// Iterate through all bam files

	size_t n_samples = lst_fn_bam.size();
	size_t n_targets = Targets.size();

	cnvs_cov_t **coverageData = (cnvs_cov_t **)malloc(sizeof(cnvs_cov_t *)*n_samples);
	if(coverageData == NULL)
	{
		cerr<<"Can't allocate space for coverage matrix"<<endl;
		exit(255);
	}

	char ** sampleNames = (char **)malloc(sizeof(char*)*n_targets);
	if(sampleNames == NULL)
	{
		cerr<<"Can't allocate space for sample name array"<<endl;
	}

	BamTools::BamReader *reader;
	
	
	// Iterate through all the remaining parameters, treating as samples
	size_t idx_sample;		
	for(idx_sample=0; idx_sample < n_samples; idx_sample++)
	{
		if(loglvl >=2)
			cerr<<"Processing "<<idx_sample<<"-th sample file: "<<lst_fn_bam[idx_sample]<<endl;

		string fn_bam = lst_fn_bam[idx_sample];

		sampleNames[idx_sample] = (char *)malloc(sizeof(char)*CNVS_SAMPLE_NAME_CHAR+1);
		if(sampleNames[idx_sample] == NULL)
		{
			cerr<<"Can't allocate space for sample name at "<<idx_sample<<endl;
			exit(255);
		}

		coverageData[idx_sample] = (cnvs_cov_t *)malloc(sizeof(cnvs_cov_t)*n_targets);
		if(coverageData[idx_sample] == NULL)
		{
			cerr<<"Can't allocate space for coverage data cells at "<<idx_sample<<endl;
			exit(255);
		}

		// Initialize the coverage matrix to be 0
		memset(coverageData[idx_sample], 0, sizeof(cnvs_cov_t)*n_targets);


		if(loglvl >= 3)
			cerr<<"memory allocation completed"<<endl;

		size_t basename_pos = fn_bam.find_last_of("/\\");
		if(basename_pos != fn_bam.npos)
			strncpy(sampleNames[idx_sample], fn_bam.c_str()+basename_pos+1, CNVS_SAMPLE_NAME_CHAR);
		else
			strncpy(sampleNames[idx_sample], fn_bam.c_str(), CNVS_SAMPLE_NAME_CHAR);
		
		// Open the bam file
		string fn_bai = fn_bam+".bai";

		reader = new BamTools::BamReader();

		if(!reader->Open(fn_bam, fn_bai))
		{
			cerr<<"Error happened opening bam file: "<<fn_bam<<endl;
			delete reader;
			continue;
		}

		if(!reader->IsOpen())
		{
			cerr<<"Cannot open bamfile: "<<fn_bam<<endl;
			delete reader;
			continue;
		}

		if(!reader->HasIndex())
		{
			cerr<<"Cannot open bai file: "<<fn_bai<<endl;
			reader->Close();
			delete reader;
			continue;
		}

		size_t idx_target;
		for(idx_target=0; idx_target<n_targets; idx_target++)
		{
			t_Target currentTarget = Targets[idx_target];
			int refID = reader->GetReferenceID(currentTarget.name);

			if(!reader->SetRegion(refID, currentTarget.start, refID, currentTarget.end))
			{
				cerr<<"Invalid interval: '"<<currentTarget.name<<"':"<<refID<<"\t"<<currentTarget.start<<"\t"<<currentTarget.end<<endl;
				continue;
			}

			BamTools::BamAlignment a;
			
			cnvs_cov_t * b_covs = NULL;
			if(algorithm == ALGO_BASE_COV)
			{
				b_covs = (cnvs_cov_t *)malloc(sizeof(cnvs_cov_t)*(currentTarget.end - currentTarget.start + 1));
				if(b_covs == NULL)
				{
					cerr<<"Unable to allocate memory for base coverage with in target "<<idx_target<<endl;
					exit(255);
				}

				memset(b_covs, 0, sizeof(cnvs_cov_t)*(currentTarget.end - currentTarget.start + 1));
			}

			while(reader->GetNextAlignmentCore(a))
			{

				// Flag-based filteration

				if(a.IsDuplicate() ||
				   a.IsFailedQC() )
					continue;

				if(a.Position < currentTarget.start)
					continue;

				switch(algorithm)
				{
					case ALGO_READ_COV:
						coverageData[idx_sample][idx_target]++;
						break;
					case ALGO_BASE_COV:
						int32_t p_pos;
						
						for(p_pos = a.Position; p_pos < int32_min(a.Position + a.Length, currentTarget.end+1); p_pos++)
							b_covs[p_pos - currentTarget.start]++;
						break;
					default:
						break;
				}
			}

			if(algorithm == ALGO_BASE_COV)
			{
				size_t length = currentTarget.end - currentTarget.start + 1;
			
				qsort(b_covs, length, sizeof(cnvs_cov_t), compare_cnvs_cov_t);
					
				if(length % 2 != 0)
					coverageData[idx_sample][idx_target] = b_covs[length / 2 + 1];
				else
					coverageData[idx_sample][idx_target] = ( b_covs[length / 2 ] + b_covs[length / 2 + 1] ) / 2;
			
				free(b_covs);
			}

			cout<<"\rProcessing sample "<<idx_sample+1<<"/"<<n_samples<<", target "<<idx_target<<"/"<<Targets.size()<<std::flush;


		} // for [Target Loop]

		reader->Close();
		delete reader;

	}	// for [Sample Loop]

	cout<<endl;


	cout<<"Number of Sample: "<<n_samples<<endl;
	cout<<"Number of Targets: "<<n_targets<<endl;

	for(size_t idx_sample = 0; idx_sample < n_samples; idx_sample++)
	{
		cout<<"Sample Name "<<idx_sample<<":  "<<sampleNames[idx_sample]<<endl;
	}

	cout<<endl;

	for(size_t idx_target = 0; idx_target < n_targets; idx_target++)
	{
		for(size_t idx_sample=0; idx_sample < n_samples; idx_sample++)
		{
			cout<<"\t"<<coverageData[idx_sample][idx_target];
		}
		cout<<endl;
	}

	cnvs_write_file(fn_output, n_samples, n_targets, sampleNames, coverageData);

	for(size_t idx_sample=0; idx_sample<n_samples; idx_sample++)
	{
		free(coverageData[idx_sample]);
	}


	free(coverageData);
	free(sampleNames);

	return 0;

}
