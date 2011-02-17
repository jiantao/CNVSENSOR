#include <iostream>
#include <fstream>

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

const char * fn_target_region;
const char * fn_output;
const char * program_name;

void usage()
{
	cout<<"Usage : "<<program_name<<" <options> <bam filename1> <bam filename 2> ... "<<endl;
	cout<<"\t Options: "<<endl;
	cout<<"\t -b <bed filename>:\t [REQUIRED] Specify the target region description file name"<<endl;
	cout<<"\t -o <out filename>:\t [REQUIRED] Specify the output filename in which the coverage is stored"<<endl;
}

int main(int argc, char* argv[])
{

	program_name = *argv;

	argc--;
	argv++;

	if(argc < 3)
	{
		usage();
		exit(0);
	}

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
			default:
				cerr<<"Unrecognized option: -"<<(*argv)[1];
				usage();
		}

		argc--;
		argv++;
	}

	if(strlen(fn_target_region) == 0)  
	{
		cerr<<"Missing -b argument!"<<endl<<endl;
		usage();
		exit(0);
	}

	if(strlen(fn_output) == 0)
	{
		cerr<<"Missing -o argument!"<<endl<<endl;
		usage();
		exit(0);
	}

	if(argc == 0)
	{
		cerr<<"Missing bam file(s)!"<<endl<<endl;
		usage();
		exit(0);
	}


	// Load target regions
	
	vector<t_Target> Targets;

	if(loglvl >= 1)
		cerr<<"Opening bed file: "<<fn_target_region<<endl;
	
	ifstream f_target;
	f_target.open(fn_target_region, ifstream::in);

	if(!f_target.good()) {
		cerr<<"Unable to open target region file: "<<*argv<<endl;
		exit(2);
	}

	while(!f_target.eof())
	{
		t_Target newTarget;
		f_target>>newTarget.name>>newTarget.start>>newTarget.end;
		if(newTarget.name.length() < 1)
			break;
		newTarget.end--;
		Targets.push_back(newTarget);
	}

	if(loglvl >= 2)
		cerr<<"Total number of targets loaded: "<<Targets.size()<<endl;

	// Iterate through all bam files

	size_t n_samples = argc;
	size_t n_targets = Targets.size();

	unsigned int **coverageData = (unsigned int **)malloc(sizeof(unsigned int *)*n_samples);
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

	BamTools::BamReader reader;
		
	while(argc > 0)	// Iterate through all the remaining parameters, treating as samples
	{


		if(loglvl >=2)
			cerr<<"Processing "<<n_samples - argc + 1<<" sample file: "<<*argv<<endl;

		string bamFn(*argv);

		size_t idx_sample = n_samples - argc;

		sampleNames[idx_sample] = (char *)malloc(sizeof(char)*CNVS_SAMPLE_NAME_CHAR+1);
		if(sampleNames[idx_sample] == NULL)
		{
			cerr<<"Can't allocate space for sample name at "<<idx_sample<<endl;
			exit(255);
		}

		coverageData[idx_sample] = (unsigned int *)malloc(sizeof(unsigned int)*n_targets);
		if(coverageData[idx_sample] == NULL)
		{
			cerr<<"Can;t allocate space for coverage data cells at "<<idx_sample<<endl;
			exit(255);
		}

		memset(coverageData[idx_sample], 0, sizeof(unsigned int)*n_targets);


		if(loglvl >= 3)
			cerr<<"memory allocation completed"<<endl;

		char *base_name = strrchr(*argv, '/');
		if(base_name != NULL)
			strncpy(sampleNames[idx_sample], base_name + 1, CNVS_SAMPLE_NAME_CHAR);
		else
			strncpy(sampleNames[idx_sample], *argv, CNVS_SAMPLE_NAME_CHAR);

		// Open the bam file
		char *fn_bai = (char*)malloc(sizeof(char)*strlen(*argv)+6);
		strcat(fn_bai, *argv);
		strcat(fn_bai, ".bai");
		reader.Open(*argv, fn_bai);

		if(!reader.IsOpen())
		{
			cerr<<"Cannot open bamfile: "<<*argv<<endl;
			free(fn_bai);
			argc--;
			argv++;
			continue;
		}

		if(!reader.HasIndex())
		{
			cerr<<"Cannot open bai file: "<<fn_bai<<endl;
			free(fn_bai);
			argc--;
			argv++;
			continue;
		}

		free(fn_bai);

		size_t idx_target;
		for(idx_target=0; idx_target<Targets.size(); idx_target++)
		{
			t_Target currentTarget = Targets[idx_target];
			int refID = reader.GetReferenceID(currentTarget.name);

			if(!reader.SetRegion(refID, currentTarget.start, refID, currentTarget.end))
			{
				cerr<<"Invalid interval: '"<<currentTarget.name<<"':"<<refID<<"\t"<<currentTarget.start<<"\t"<<currentTarget.end<<endl;
				continue;
			}

			BamTools::BamAlignment a;
			while(reader.GetNextAlignmentCore(a))
			{
				if(a.Position >= currentTarget.start)
					coverageData[idx_sample][idx_target]++;
			}

			cout<<"\rProcessing target "<<idx_target<<"/"<<Targets.size()<<std::flush;


		}

		reader.Close();

		argc--;
		argv++;
	}

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