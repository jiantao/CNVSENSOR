/*
 * =====================================================================================
 *
 *       Filename:  CNV_GetOpt.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/24/2011 11:51:27 AM
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
#include <assert.h>
#include "CNV_Error.h"
#include "CNV_GetOpt.h"

// initialize the settings struct
int CNV_IntialSettings(CNV_Arguments* argumentss)
{
    assert(argumentss != NULL);
    // initialize the argumentss
    argumentss->inputFile       = arg_file1("i", "input",     "<cnv_data>",    "input CNV binary file");
    argumentss->priorFile       = arg_file0("p", "prior",     "<prior_data>",  "input prior probability file");
    argumentss->sampleReadDepth = arg_dbl0( "s", "sample_rd", "<real number>", "threshold for sample read depth");
    argumentss->targetReadDepth = arg_dbl0( "t", "target_rd", "<real number>", "threshold for target read depth");
    argumentss->rSquare         = arg_dbl0( "r", "r_square",  "[0, 1]",        "threshold for r square value from linear fit");
    argumentss->checkData       = arg_lit0( "c", "check",                      "check the uniformity of the target affinity");
    argumentss->help            = arg_lit0( "h", "help",                       "show help message");
    argumentss->end             = arg_end(CNV_ARG_MAX_ERR);

    return CNV_OK;
}

// get the options from command line arguments
int CNV_GetOpt(int argc, char* argv[], void* argTable[], CNV_Arguments* arguments)
{
    // check if we have enough memory for the arguments table
    if (arg_nullcheck(argTable) != 0)
        CNV_ErrQuit("Error: Not enough memory for command line arguments table.");

    // parse the command line arguments
    int numErr = arg_parse(argc, argv, argTable);
    if (numErr > 0)
    {
        // print error if there are any
        arg_print_errors(stderr, arguments->end, "CNVSENSOR");
        CNV_ErrQuit("Try '%s --help' for more information.\n", argv[0]);
    }

    return CNV_OK;
}

// check the options got from command line arguments
int CNV_CheckOpt(const CNV_Arguments* arguments, CNV_Settings* settings)
{
    assert(arguments != NULL && settings != NULL);

    // open the read count input file
    settings->readCountInput = fopen(*(arguments->inputFile->filename), "rb");
    if (settings->readCountInput == NULL)
    {
        CNV_ErrMsg("ERROR: Cannot open read count binary file \"%s\" for reading.", *(arguments->inputFile->filename));
        return CNV_ERR;
    }

    // open the prior probability input file
    if (arguments->priorFile->count == 0)
        settings->priorInput = NULL;
    else
    {
        settings->priorInput = fopen(*(arguments->priorFile->filename), "r");
        if (settings->priorInput == NULL)
        {
            CNV_ErrMsg("ERROR: Cannot open prior probability file \"%s\" for reading.", *(arguments->priorFile->filename));
            return CNV_ERR;
        }
    }

    // set up the threshold for sample coverage
    if (arguments->sampleReadDepth->count == 0)
        settings->sampleReadDepthCut = CNV_DEFAULT_SAMPLE_RD_CUT;
    else
        settings->sampleReadDepthCut = *(arguments->sampleReadDepth->dval);

    // set up the threshold for target coverage
    if (arguments->targetReadDepth->count == 0)
        settings->targetReadDepthCut = CNV_DEFAULT_TARGET_RD_CUT;
    else
        settings->targetReadDepthCut = *(arguments->targetReadDepth->dval);

    // set up the threshold for r squared value
    if (arguments->rSquare->count == 0)
        settings->rSquaredCut = CNV_DEFAULT_R_SQUARE_CUT;
    else if (*(arguments->rSquare->dval) < 0 || *(arguments->rSquare->dval) > 1)
    {
        CNV_ErrMsg("WARNING: User specified threshold for r squared value is out of range([0, 1]). Use default value %f instead.", CNV_DEFAULT_R_SQUARE_CUT);
        settings->rSquaredCut = CNV_DEFAULT_R_SQUARE_CUT;
    }
    else
        settings->rSquaredCut = *(arguments->rSquare->dval);

    // if we have to check the target affinity uniformity
    if (arguments->checkData->count > 0)
        settings->ifCheckData = TRUE;
    else
        settings->ifCheckData = FALSE;

    // if we show help message and quit
    if (arguments->help->count > 0)
        settings->ifShowHelp = TRUE;
    else
        settings->ifShowHelp = FALSE;

    return CNV_OK;
}

// show help message
void CNV_ShowHelp(const char* progName, void* argTable[], const CNV_Arguments* arguments)
{
    assert(progName != NULL && argTable != NULL && arguments != NULL);

    printf("Usage: %s", progName);
    arg_print_syntaxv(stdout, argTable, NULL);
    printf("Detect the CNV events from capture sequencing data with read depth algorithm.\n");
    arg_print_glossary(stdout, argTable, "  %-25s %s\n");
}
