#ifndef  CNV_FILEUTILITIES_H
#define  CNV_FILEUTILITIES_H

#include <stdlib.h>
#include "CNV_Types.h"

#define CNVS_SAMPLE_NAME_CHAR 100

typedef struct _coverage_file_handler
{
    size_t n_samples;
    size_t n_targets;

    char** sample_names;			// This array contains all the sample names
                                   	// Each element of this array sample_name[i]
                                    // represent the i-th sample name, 
                                    // i goes from 0 to n_samples - 1

    double * coverage_data;			// The flat data structure storing the coverage
                                    // coverage_data [ i + j * n_samples ] represent
                                    // the coverage of i-th sample and j-th target
                                    // i goes from 0 to n_samples
                                    // j goes from 0 to n_targets
} CNV_BinaryFileHandler;

// Reader/Loader functions

size_t CNV_ReadBinary(FILE * fp,				// Filename to be read
					  size_t* n_samples, 		// number of samples
					  size_t* n_targets, 		// number of targets
					  char*** sample_names,		// array of sample names
					  double ** coverage_data); // array of coverage data, target stored as row

size_t CNV_LoadBinary(FILE * fp, CNV_BinaryFileHandler* f_handle);


// Writer/Saver functions

size_t CNV_WriteBinary(FILE * fp, 					// File pointer to which the data is to be stored
					   size_t* n_samples,			// number of samples
					   size_t* n_targets,			// number of targets
					   char ** sample_names,		// array of sample names
					   double ** coverage_data); 	// array of coverage data, target is stored as row

size_t CNV_SaveBinary(FILE * fp, CNV_BinaryFileHandler* f_handle);

void CNV_BinaryFileHandler_Free(CNV_BinaryFileHandler *h);


#endif  /*CNV_FILEUTILITIES_H*/
		
