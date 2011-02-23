#include <iostream>

#include "bam_loader_global.h"
#include "cnvs_file_writer.h"
#include "cnvs_file_reader.h"


using namespace std;

int main(int argc, char* argv[])
{

	if(argc <2)
		return 0;

	
	cnvs_file_handler fn_handle;

	cnvs_read_file(argv[1], &fn_handle);

	cout<<"Number of Sample: "<<fn_handle.n_samples<<endl;
	cout<<"Number of Targets: "<<fn_handle.n_targets<<endl;

	for(size_t idx_sample = 0; idx_sample < fn_handle.n_samples; idx_sample++)
	{
		cout<<"Sample Name "<<idx_sample<<":  "<<fn_handle.sample_names[idx_sample]<<endl;
	}

	cout<<endl;

	for(size_t idx_target = 0; idx_target < fn_handle.n_targets; idx_target++)
	{
		for(size_t idx_sample=0; idx_sample < fn_handle.n_samples; idx_sample++)
		{
			cout<<"\t"<<fn_handle.coverage_data[idx_sample + idx_target * fn_handle.n_samples];
		}
		cout<<endl;
	}



}

