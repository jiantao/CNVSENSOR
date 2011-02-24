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


// ===========================
// Platform specific file I/O 
// ===========================

#define DIRECTORY_NAME_LENGTH    255


// =======================
// Program specific types
// =======================

// size type
typedef uint64_t cnv_size_t;

// boolean type
typedef short CNV_Bool;


// boolean constants
#define FALSE 0
#define TRUE  1

// function return status
#define CNV_OK   0
#define CNV_ERR -1

// swap two numbers
#define CNV_SWAP_NUM(num1, num2, temp) \
    do                   \
    {                    \
        (temp) = (num1); \
        (num1) = (num2); \
        (num2) = (temp); \
    }while(0)        

// calculate square of a number 
#define CNV_SQUARE(x) ((x) * (x))

#endif  /*CNV_TYPES_H*/
