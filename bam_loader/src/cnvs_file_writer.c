#include <stdio.h>

#include "bam_loader_global.h"
#include "cnvs_file_writer.h"

size_t cnvs_write_file(const char * fn, size_t n_samples, size_t n_targets, char** sample_names, cnvs_cov_t ** coverage_data)
{
	FILE *fp = fopen(fn, "w");
	size_t totalWritten = 0;

	if(fp == NULL)
		perror("Cannot open file to write");

	totalWritten += fwrite(&n_samples, 		sizeof(size_t), 	1, fp);
	totalWritten += fwrite(&n_targets, 		sizeof(size_t), 	1, fp);

	size_t i;

	for(i=0; i<n_samples; i++)
		totalWritten += fwrite(sample_names[i], sizeof(char), 			CNVS_SAMPLE_NAME_CHAR, 	fp);

	for(i=0; i<n_samples; i++)
		totalWritten += fwrite(coverage_data[i], sizeof(cnvs_cov_t), 	n_targets, 				fp);

	fclose(fp);

	return totalWritten;
}
