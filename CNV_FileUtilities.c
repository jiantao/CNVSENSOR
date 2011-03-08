#include <stdio.h>
#include <stdlib.h>

#include "CNV_FileUtilities.h"

size_t CNV_ReadBinary(FILE * fp,							// File pointer to be written 
					  size_t* n_samples, 					// number of samples
					  size_t* n_targets, 					// number of targets
					  char*** sample_names,				 	// array of sample names
					  double ** coverage_data)	 			// array of coverage data, target stored as row
{
	size_t totalRead = 0;

	if(fp == NULL) {
		perror("Cannot open file to read");
	
		return 0;
	}

	totalRead += fread(n_samples, sizeof(size_t), 1, fp);
	totalRead += fread(n_targets, sizeof(size_t), 1, fp);

	*sample_names = (char **)malloc(sizeof(char*)*(*n_samples));


	if(*sample_names == NULL)
	{
		CNV_ErrSys("ERROR: Not enough memory for sample name array.");
		*n_samples = 0; *n_targets = 0;
		sample_names = NULL; coverage_data = NULL;
		return 0;
	}

	size_t i;

	for(i=0; i<*n_samples; i++)
	{
		(*sample_names)[i] = (char *)malloc(sizeof(char)*CNVS_SAMPLE_NAME_CHAR);

		if((*sample_names)[i] == NULL)
		{
			CNV_ErrSys("ERROR: Not enough memory gor a sample name.");
		}

		totalRead += fread((*sample_names)[i], sizeof(char), CNVS_SAMPLE_NAME_CHAR, fp);
	}


	*coverage_data = (cnvs_cov_t *)malloc(sizeof(double)*(*n_samples)*(*n_targets));
	if(*coverage_data == NULL) 
	{
		CNV_ErrSys("ERROR: Not enough memory for coverage data");
	}

	totalRead += fread(*coverage_data, sizeof(double), (*n_samples)*(*n_targets), fp);

	return totalRead;
}

size_t CNV_LoadBinary(FILE * fp, CNV_BinaryFileHandler* f_handle)
{
	return 
		CNV_ReadBinary(fp, 
					   &(f_handle->n_samples),
					   &(f_handle->n_targets),
					   &(f_handle->sample_names),
					   &(f_handle->coverage_data));
}

size_t CNV_WriteBinary(FILE * fp, size_t n_samples, size_t n_targets, char** sample_names, double * coverage_data)
{
	size_t totalWritten = 0;

	if(fp == NULL) {
		perror("Cannot open file to write");
	
		return 0;
	}
	totalWritten += fwrite(&n_samples, 		sizeof(size_t), 	1, fp);
	totalWritten += fwrite(&n_targets, 		sizeof(size_t), 	1, fp);

	size_t i;

	for(i=0; i<n_samples; i++)
		totalWritten += fwrite(sample_names[i], sizeof(char), 	CNVS_SAMPLE_NAME_CHAR, 	fp);

	//for(i=0; i<n_samples; i++)
	totalWritten += fwrite(coverage_data,	 	sizeof(double), n_targets*n_samples, 	fp);

	return totalWritten;
}

size_t CNV_SaveBinary(FILE *fp, CNV_BinaryFileHandler *f_handle)
{
	return
		CNV_WriteBinary(fp,
						f_handle->n_samples,
						f_handle->n_targets,
						f_handle->sample_names,
						f_handle->coverage_data);
}

void CNV_BinaryFileHandler_Free(CNV_BinaryFileHandler h)
{
	size_t i;
	for(i=0; i<h.n_samples; i++)
		free(h.sample_names[i]);
	
	free(h.sample_names);
	free(h.coverage_data);
}
