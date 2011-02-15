#include <stdio.h>
#include <stdlib.h>

#include "bam_loader_global.h"
#include "cnvs_file_reader.h"

size_t cnvs_read_file(const char * fn,						// Filename to be written 
					  size_t* n_samples, 					// number of samples
					  size_t* n_targets, 					// number of targets
					  char*** sample_names,	 	// array of sample names
					  unsigned int *** coverage_data)	 	// array of coverage data, target stored as row
{
	size_t totalRead = 0;

	FILE *fp;
	fp = fopen(fn, "r");
	
	if(fp == NULL)
		perror("Cannot open file to read");

	totalRead += fread(n_samples, sizeof(size_t), 1, fp);
	totalRead += fread(n_targets, sizeof(size_t), 1, fp);

	*sample_names = (char **)malloc(sizeof(char*)*(*n_samples));
	size_t i;

	for(i=0; i<*n_samples; i++)
	{
		(*sample_names)[i] = malloc(sizeof(char)*CNVS_SAMPLE_NAME_CHAR);
		totalRead += fread((*sample_names)[i], sizeof(char), CNVS_SAMPLE_NAME_CHAR, fp);
	}

	*coverage_data = (unsigned int **)malloc(sizeof(unsigned int *)*(*n_targets));

	for(i=0; i<*n_targets; i++)
	{
		(*coverage_data)[i] = malloc(sizeof(unsigned int)*(*n_samples));
		totalRead += fread((*coverage_data)[i], sizeof(unsigned int), *n_samples, fp);
	}



	return totalRead;
}
