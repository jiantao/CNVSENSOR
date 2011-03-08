/*
 * =====================================================================================
 *
 *       Filename:  CNV_Types.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/12/2011 02:17:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CNV_TYPES_H
#define  CNV_TYPES_H

#include <stdint.h>
#include <math.h>
#include <argtable2.h>


// ===========================
// Platform specific file I/O 
// ===========================

#define DIRECTORY_NAME_LENGTH    255


// =======================
// Program specific types
// =======================

// size type
typedef size_t cnv_size_t;

// boolean type
typedef short Bool;

// type define for argtable structures
typedef struct arg_int  arg_int;
typedef struct arg_dbl  arg_dbl;
typedef struct arg_lit  arg_lit;
typedef struct arg_str  arg_str;
typedef struct arg_rex  arg_rex;
typedef struct arg_file arg_file;
typedef struct arg_date arg_date;
typedef struct arg_end  arg_finish;

// boolean constants
#define FALSE 0
#define TRUE  1

// function return status
#define CNV_OK   0
#define CNV_ERR -1

// swap two objects
#define CNV_SWAP(obj1, obj2, temp) \
    do                   \
    {                    \
        (temp) = (obj1); \
        (obj1) = (obj2); \
        (obj2) = (temp); \
    }while(0)        

// calculate square of a number 
#define CNV_SQUARE(x) ((x) * (x))

#endif  /*CNV_TYPES_H*/
