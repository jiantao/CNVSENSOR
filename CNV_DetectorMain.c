/*
 * =====================================================================================
 *
 *       Filename:  CNV_Detector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/23/2011 04:06:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <argtable2.h>

#include "CNV_Error.h"
#include "CNV_Types.h"
#include "CNV_GetOpt.h"
#include "CNV_Vector.h"
#include "CNV_Matrix.h"
#include "CNV_FileUtilities.h"

int main(int argc, char *argv[])
{
    // initialize the arguments structure for CNV detector
    CNV_Arguments arguments;
    CNV_IntialArguments(&arguments);

    // arguments table
    void* argTable[] = { arguments.inputFile, 
                         arguments.priorFile, 
                         arguments.sampleReadDepth, 
                         arguments.targetReadDepth, 
                         arguments.rSquare, 
                         arguments.checkData, 
                         arguments.help, 
                         arguments.end };

    // parse the command line arguments
    CNV_ParseOpt(argc, argv, argTable, &arguments);

    // check the options and initialize the detector's settings
    CNV_Settings settings;
    if (CNV_InitialSettings(&arguments, &settings) == CNV_ERR)
    {
        CNV_ShowHelp(argv[0], argTable, &arguments);
        exit(EXIT_FAILURE);
    }

    // show help if user ask to
    if (settings.ifShowHelp)
    {
        CNV_ShowHelp(argv[0], argTable, &arguments);
        exit(EXIT_SUCCESS);
    }

    // create a cnv binary file handle
    cnv_file_handler* cnvBinaryHandle = (cnv_file_handler*) malloc(sizeof(cnv_file_handler));
    if (cnvBinaryHandle == NULL)
        CNV_ErrSys("ERROR: Not enough memory for CNV binary file handler.");

    // load the binary file into a read count matrix
    if (CNV_LoadBinary("filename", cnvBinaryHandle) == 0)
    {
        CNV_ErrQuit("ERROR: Read count is zero.");
        exit(EXIT_FAILURE);
    }

    // create a matrix view of the read count matrix
    CNV_MatrixView* readCountMatrix = CNV_MatrixViewAlloc();
    if (readCountMatrix == NULL)
        CNV_ErrSys("ERROR: Not enough memory to create a matrix view of read count matrix.");

    // initialize the read count matrix view
    readCountMatrix->rows   = cnvBinaryHandle->n_targets;
    readCountMatrix->cols   = cnvBinaryHandle->n_samples;
    readCountMatrix->data   = cnvBinaryHandle->coverage_data;
    readCountMatrix->stride = readCountMatrix->cols;

    

    // free the resources used to hold arguments and settings
    CNV_CleanOpt(argTable, &arguments, &settings);

    // free the read count matrix view
    CNV_MatrixViewFree(readCountMatrix);
    return EXIT_SUCCESS;
}
