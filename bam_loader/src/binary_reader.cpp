#include <iostream>
#include <stdio.h>

#include "CNV_FileUtilities.h"

using namespace std;

int main(int argc, char* argv[])
{

	if(argc <2)
		return 0;

	
	CNV_BinaryFileHandler fn_handle;

	FILE *in_f = fopen(argv[1], "r");

	if(in_f == NULL)
	{
		perror("Unable to open file to read");
		exit(0);
	}

	CNV_LoadBinary(in_f, &fn_handle);

	fclose(in_f);

	for(size_t idx_sample = 0; idx_sample < fn_handle.n_samples; idx_sample++)
	{
		cout<<fn_handle.sample_names[idx_sample];
		if(idx_sample != fn_handle.n_samples-1)
			cout<<",";
	}

	cout<<endl;

	for(size_t idx_target = 0; idx_target < fn_handle.n_targets; idx_target++)
	{
		for(size_t idx_sample=0; idx_sample < fn_handle.n_samples; idx_sample++)
		{
			cout<<fn_handle.coverage_data[CNVS_COVMAT_LOC(idx_sample, idx_target, fn_handle.n_samples)];
			if(idx_sample != fn_handle.n_samples)
				cout<<",";
		}
		cout<<endl;
	}

	CNV_BinaryFileHandler_Free(fn_handle);



}

