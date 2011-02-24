#ifndef BAM_LOADER_GLOBAL_H
#define BAM_LOADER_GLOBAL_H

#define CNVS_SAMPLE_NAME_CHAR 100

#include <stdlib.h>

typedef double cnvs_cov_t;

typedef struct _coverage_file_handler
{
	size_t n_samples;
	size_t n_targets;

	char ** sample_names;				// This array contains all the sample names
										// Each element of this array sample_name[i]
										// represent the i-th sample name, 
										// i goes from 0 to n_samples - 1

	cnvs_cov_t  * coverage_data;		// The flat data structure storing the coverage
										// coverage_data [ i + j * n_samples ] represent
										// the coverage of i-th sample and j-th target
										// i goes from 0 to n_samples
										// j goes from 0 to n_targets
} cnvs_file_handler;

#endif
