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

#ifdef WIN32
const char OS_DIRECTORY_SEPARATOR = '\\';
#else
const char OS_DIRECTORY_SEPARATOR = '/';
#endif

#define DIRECTORY_NAME_LENGTH    255


// =======================
// Program specific types
// =======================

typedef uint64_t cnv_size_t;

#endif  /*CNV_TYPES_H*/
