/*
 * =====================================================================================
 *
 *       Filename:  CNV_Vector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/13/2011 11:05:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <assert.h>
#include "CNV_Vector.h"
#include "CNV_Error.h"


//===============================
// Constructors and Destructors
//===============================

// create a new vector
CNV_Vector* CNV_VectorAlloc(cnv_size_t size)
{
    // newVector size should not be zero
    assert(size != 0);

    // create new newVector object
    CNV_Vector* newVector = malloc(sizeof(CNV_Vector));
    if (newVector == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to create a new newVector object.\n");

    // allocate memory for data storage
    newVector->data = malloc(sizeof(double) * size);
    if (newVector->data == NULL)
        CNV_ErrQuit("ERROR: Not enough memory for data storage in the newVector object.\n");

    // initialize members in newVector object
    newVector->size = size;
    newVector->stride = 1;

    return newVector;
}

// create a new vector and initialize every element into zero
CNV_Vector* CNV_VectorCalloc(cnv_size_t size)
{
    // newVector size should not be zero
    assert(size != 0);

    // create new newVector object
    CNV_Vector* newVector = malloc(sizeof(CNV_Vector));
    if (newVector == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to create a new newVector object.\n");

    // allocate memory for data storage and intialize every element to zero
    newVector->data = calloc(size, sizeof(double));
    if (newVector->data == NULL)
        CNV_ErrQuit("ERROR: Not enough memory for data storage in the newVector object.\n");

    // initialize members in newVector object
    newVector->size = size;
    newVector->stride = 1;

    return newVector;
}

// create a new vector view 
CNV_VectorView* CNV_VectorViewAlloc(void)
{
    // create new newVector object
    CNV_VectorView* newVectorView = malloc(sizeof(CNV_VectorView));
    if (newVectorView == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to create a new newVector object.\n");

    // initialize members in newVector object
    newVectorView->size = 0;
    newVectorView->stride = 1;

    return newVectorView;
}

// free a vector 
void CNV_VectorFree(CNV_Vector* vector)
{
    // should not free a null pointer
    assert(vector != NULL);

    if (vector->data != NULL)
        free(vector->data);

    free(vector);
}

// free a vector view
void CNV_VectorViewFree(CNV_VectorView* vectorView)
{
    // should not free a null pointer
    assert(vectorView != NULL);

    free(vectorView);
}


//===============================
// Constant methods
//===============================

void CNV_VectorPrint(const CNV_Vector* vector, FILE* output)
{
    // input arguments check
    assert(vector != NULL && vector->data != NULL && output != NULL);

    // position where to stop
    cnv_size_t stopPos = vector->size * vector->stride;

    for (unsigned i = 0; i < stopPos; i += vector->stride)
        fprintf(output, "%-10.2f ", vector->data[i]);

    fprintf(output, "\n\n");
}


// calculate the median value of all the elements in a vector
// Torben's method (without changing the element order in the vector)
double CNV_VectorGetMedian(const CNV_Vector* vector)
{
    // check input arguments
    assert(vector != NULL && vector->data != NULL);

    // number of elements in vector less than the guess median
    unsigned less; 

    // number of elements in vector greater than the guess median
    unsigned greater; 

    // number of elements in vector equal to the guess median
    unsigned equal;

    // the position that is after the last element in the vector
    unsigned stopPos = vector->stride * vector->size;

    unsigned breakPt = (vector->size + 1) / 2;

    // minimum value in search region
    double min; 

    // maximum value in search region
    double max; 

    // guess median
    double guess; 

    // maximum value that less than the guess median
    double maxltguess; 

    // minimum value that greater than the guess median
    double mingtguess;

    // find the max and min values in the vector
    min = vector->data[0];
    max = vector->data[0];
    for (unsigned i = vector->stride; i != stopPos; i += vector->stride) 
    {
        if (vector->data[i] < min) 
            min = vector->data[i];

        if (vector->data[i] > max) 
            max = vector->data[i];
    }

    while (TRUE) 
    {
        guess = (min + max) / 2;
        maxltguess = min;
        mingtguess = max;

        less = 0; 
        greater = 0; 
        equal = 0;

        for (unsigned i = 0; i != stopPos; i += vector->stride) 
        {
            if (vector->data[i] < guess) 
            {
                ++less;
                if (vector->data[i] > maxltguess) 
                    maxltguess = vector->data[i];
            } 
            else if (vector->data[i] > guess) 
            {
                ++greater;
                if (vector->data[i] < mingtguess) 
                    mingtguess = vector->data[i];
            } 
            else 
                ++equal;
        }

        if (less <= breakPt && greater <= breakPt) 
            break;
        else if (less > greater) 
            max = maxltguess;
        else 
            min = mingtguess;
    }

    // decide which value to be returned
    if (less < breakPt && greater < breakPt)
        return guess;
    else if (greater == breakPt && less == breakPt)
        return ((mingtguess + maxltguess) / 2);
    else if (less + equal == greater)
        return ((guess + mingtguess) / 2);
    else if (greater + equal == less)
        return ((guess + maxltguess) / 2);
    else if (greater > less)
        return mingtguess;
    else
        return maxltguess;
}


//===============================
// Non-constant methods
//===============================

// initialize a vector view
CNV_VectorView* CNV_VectorGetView(double* data, cnv_size_t size, cnv_size_t stride)
{
    // check input arguments
    assert(data != NULL && size != 0 && stride != 0);

    // create a new vector view and then initialize it
    CNV_VectorView* newVectorView = CNV_VectorViewAlloc();
    newVectorView->data = data;
    newVectorView->size = size / stride;
    newVectorView->stride = stride;

    return newVectorView;
}
