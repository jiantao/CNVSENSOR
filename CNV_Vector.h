/*
 * =====================================================================================
 *
 *       Filename:  CNV_Vector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/13/2011 12:06:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CNV_VECTOR_H
#define  CNV_VECTOR_H

#include <stdio.h>

#include "CNV_Types.h"


//===============================
// Type and constant definition
//===============================

#define CNV_VECTOR_GET_VALUE(vector, i) ((vector->data)[(vector->stride) * (i)])

#define CNV_SIZE_VECTOR_PUSH(sizeVector, data)                    \
    do                                                            \
    {                                                             \
        (sizeVector)->sizeData[(sizeVector)->count++] = data;     \
    }while(0)

typedef struct
{
    double* data;
    size_t size;
    size_t stride;
}CNV_Vector, CNV_VectorView;


typedef struct
{
    size_t* sizeData;
    size_t  count;
    size_t  capacity;
}CNV_SizeVector;


//===============================
// Constructors and Destructors
//===============================

// create a new vector
CNV_Vector* CNV_VectorAlloc(size_t size);

// create a new vector and initialize every element into zero
CNV_Vector* CNV_VectorCalloc(size_t size);

// create a new vector view 
CNV_VectorView* CNV_VectorViewAlloc(void);

// free a vector 
void CNV_VectorFree(CNV_Vector* vector);

// free a vector view
void CNV_VectorViewFree(CNV_VectorView* vectorView);

// create a new size vector
CNV_SizeVector* CNV_SizeVectorAlloc(size_t size);

// free a size vector
void CNV_SizeVectorFree(CNV_SizeVector* sizeVector);


//===============================
// Constant methods
//===============================

// print all the elements in a vector
void CNV_VectorPrint(const CNV_Vector* vector, FILE* output);

// calculate the median value of all the elements in a vector
// Torben's method (without changing the element order in the vector)
double CNV_VectorGetMedian(const CNV_Vector* vector);

// calculate the k-th smallest value in the vector
int CNV_VectorGetKthSmallest(const CNV_Vector* vector, double* temp, double* results, size_t resultsLength, ...);

// compute the the best-fit linear regression coefficient of the model y = c1 * x and the r squared value of the fit
int CNV_VectorLinearFit(const CNV_Vector* x, const CNV_Vector* y, double* coeff, double* rSquare);

//===============================
// Non-constant methods
//===============================

// initialize a vector view
CNV_VectorView* CNV_VectorGetView(double* data, size_t size, size_t stride);

#endif  /*CNV_VECTOR_H*/
