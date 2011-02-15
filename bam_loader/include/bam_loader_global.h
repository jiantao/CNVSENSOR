#ifndef BAM_LOADER_GLOBAL_H
#define BAM_LOADER_GLOBAL_H

#define CNVS_SAMPLE_NAME_CHAR 100

#include <stdlib.h>

typedef struct _coverage_file_handler
{
	size_t number_of_samples;
	size_t number_of_targets;
	char ** sample_names;
	unsigned int  ** coverage_data;
} cnvs_file_handler;



#endif
