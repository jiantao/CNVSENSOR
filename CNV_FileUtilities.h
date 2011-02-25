#ifndef  CNV_FILEUTILITIES_H
#define  CNV_FILEUTILITIES_H

#include <stdlib.h>
#include "CNV_Types.h"

#define CNVS_SAMPLE_NAME_CHAR 100

typedef struct _coverage_file_handler
{
    cnv_size_t n_samples;
    cnv_size_t n_targets;

    char** sample_names;			// This array contains all the sample names
                                                                // Each element of this array sample_name[i]
                                                                // represent the i-th sample name, 
                                                                // i goes from 0 to n_samples - 1

    double* coverage_data;		        // The flat data structure storing the coverage
                                                                // coverage_data [ i + j * n_samples ] represent
                                                                // the coverage of i-th sample and j-th target
                                                                // i goes from 0 to n_samples
                                                                // j goes from 0 to n_targets
} cnv_file_handler;

cnv_size_t CNV_ReadBinary(const char* fn,					// Filename to be read
					  cnv_size_t* n_samples, 		// number of samples
					  cnv_size_t* n_targets, 		// number of targets
					  char*** sample_names,			// array of sample names
					  double** coverage_data); 	        // array of coverage data, target stored as row

cnv_size_t CNV_LoadBinary(const char* fn, cnv_file_handler* f_handle);

#endif  /*CNV_FILEUTILITIES_H*/
		
