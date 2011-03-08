/*
 * =====================================================================================
 *
 *       Filename:  CNV_GetOpt.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/24/2011 11:35:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef  CNV_GETOPT_H
#define  CNV_GETOPT_H

#include <argtable2.h>
#include "CNV_Types.h"


//===============================
// Type and constant definition
//===============================

// max number of error that will be displayed because of command line arguments
#define CNV_ARG_MAX_ERR 30

// strut to hold the command line arguements
typedef struct arguments
{
    arg_file* inputFile;         // cnv binary file name
    arg_file* priorFile;         // prior file name
    arg_dbl* sampleReadDepth;    // threshold for sample read depth
    arg_dbl* targetReadDepth;    // threshold for target read depth
    arg_dbl* rSquare;            // threshold for r square value from linear fit
    arg_lit* checkData;          // if we check the uniformity of the target affinity
    arg_lit* help;               // display the help message and quit
    arg_finish* end;             // the end of arguments table
}CNV_Arguments;

// strut to hold the detector settings
typedef struct settings
{
    FILE*  readCountInput;       // input stream for read count matrix
    FILE*  priorInput;           // input stream for prior probability
    double sampleReadDepthCut;   // threshold for sample read depth
    double targetReadDepthCut;   // threshold for target read depth
    double rSquaredCut;          // threshold for r squared value from linear fit
    Bool   ifCheckData;          // boolean variable controls if we check the uniformity of the target affinity
    Bool   ifShowHelp;           // boolean variable controls if display the help message and quit
}CNV_Settings;


//===============================
// Constructors and Destructors
//===============================

// initialize the arguments struct
int CNV_IntialArguments(CNV_Arguments* arguments);

// check the options got from command line arguments and initialize the settings of detector
int CNV_InitialSettings(const CNV_Arguments* arguments, CNV_Settings* settings);

// clean up the detector's settings
void CNV_CleanOpt(void* argTable[], CNV_Arguments* arguments, CNV_Settings* settings);


//===============================
// Constant methods
//===============================

// show help message
void CNV_ShowHelp(const char* progName, void* argTable[], const CNV_Arguments* arguments);


//===============================
// Non-constant methods
//===============================

// parsing the options from command line arguments
int CNV_ParseOpt(int argc, char* argv[], void* argTable[], CNV_Arguments* arguments);

#endif  /*CNV_GETOPT_H*/
