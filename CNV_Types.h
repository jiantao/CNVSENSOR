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

#ifndef WIN32
#include <stdint.h>
#endif

// =================================
// Platform specific variable sizes
// =================================

#ifdef WIN32
typedef signed long long    int64_t;
typedef unsigned long long uint64_t;
#endif

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
typedef short Bool;

// boolean constants
#define FALSE 0
#define TRUE  1

// function return status
#define CNV_OK   0
#define CNV_ERR -1

#endif  /*CNV_TYPES_H*/
