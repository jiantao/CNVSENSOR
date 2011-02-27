#include <stdio.h>
#include <stdlib.h>

#include "CNV_Error.h"
#include "CNV_FileUtilities.h"

cnv_size_t CNV_ReadBinary(const char * fn,	// Filename to be read
        cnv_size_t* n_samples, 			// number of samples
        cnv_size_t* n_targets, 			// number of targets
        char*** sample_names,	 	        // array of sample names
        double** coverage_data)	 	        // array of coverage data, target stored as row
{
    cnv_size_t totalRead = 0;

    FILE *fp;
    fp = fopen(fn, "r");
    if(fp == NULL)
        CNV_ErrSys("ERROR: Cannot open CNV binary file %s for reading", fn);

    totalRead += fread(n_samples, sizeof(cnv_size_t), 1, fp);
    totalRead += fread(n_targets, sizeof(cnv_size_t), 1, fp);

    *sample_names = (char**) malloc(sizeof(char*) * (*n_samples));
    if (*sample_names == NULL)
        CNV_ErrSys("ERROR: Not enough memory for sample name array.");

    cnv_size_t i, j;
    for(i = 0; i < *n_samples; i++)
    {
        (*sample_names)[i] = (char *) malloc(sizeof(char) * CNVS_SAMPLE_NAME_CHAR);
        if ((*sample_names)[i] == NULL)
            CNV_ErrSys("ERROR: Not enough memory for a sample name.");

        totalRead += fread((*sample_names)[i], sizeof(char), CNVS_SAMPLE_NAME_CHAR, fp);
    }

    *coverage_data = (double*) malloc(sizeof(double) * (*n_samples) * (*n_targets));
    if(*coverage_data == NULL) 
        CNV_ErrSys("ERROR: Not enough memory for coverage data.")

    for(i = 0; i< *n_samples; i++)
    {
        for(j = 0; j<*n_targets; j++)
        {
            totalRead += fread((*coverage_data) + i + j * (*n_samples), sizeof(double), 1, fp);
        }
    }

    return totalRead;
}

cnv_size_t CNV_LoadBinary(const char* fn, cnv_file_handler* f_handle)
{
    return CNV_ReadBinary(fn, 
                &(f_handle->n_samples),
                &(f_handle->n_targets),
                &(f_handle->sample_names),
                &(f_handle->coverage_data));

}

