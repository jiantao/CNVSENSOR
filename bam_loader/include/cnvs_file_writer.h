#ifndef CNVS_FILE_WRITER_H
#define CNVS_FILE_WRITER_H

#include <stdlib.h>

size_t cnvs_write_file(const char * fn,				// Filename to be written 
				    size_t n_samples, 				// number of samples
					size_t n_targets, 				// number of targets
					char** sample_names, 	// array of sample names
					unsigned int ** coverage_data); // array of coverage data, target stored as row

#endif
