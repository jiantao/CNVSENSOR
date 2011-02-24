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
#include <float.h>
#include <stdarg.h>
#include <gsl/gsl_fit.h>
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


// calculate the median value of all the elements in a vector (ignore NAN)
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

    // real size = vector size - number of NAN
    unsigned realSize = vector->size;

    // first index of the element whose value is not NAN
    unsigned firstNotNAN = 0;

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
    unsigned i;
    for (i = 0; i != stopPos; i += vector->stride)
    {
        min = vector->data[i];
        max = vector->data[i];

        // min and max can not be NAN
        if (!isnan(min))
            break;
        else
            --realSize;
    }

    // get the position of first element whose value is not NAN
    firstNotNAN = i;

    for (; i != stopPos; i += vector->stride) 
    {
        if (vector->data[i] < min) 
            min = vector->data[i];

        if (vector->data[i] > max) 
            max = vector->data[i];

        if (isnan(vector->data[i]))
            --realSize;
    }

    // the real size cannot be 0 or greater than the vector size
    assert(realSize > 0 && realSize <= vector->size);

    unsigned breakPt = (realSize + 1) / 2;
    while (TRUE) 
    {
        guess = (min + max) / 2;
        maxltguess = min;
        mingtguess = max;

        less = 0; 
        greater = 0; 
        equal = 0;

        for (unsigned i = firstNotNAN; i != stopPos; i += vector->stride) 
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
            else if (vector->data[i] == guess)
            {
                ++equal;
            }
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

// calculate the k-th smallest value in the vector
int CNV_VectorGetKthSmallest(const CNV_Vector* vector, double* temp, double* results, cnv_size_t resultsLength, ...)
{
    assert(vector != NULL && vector->data != NULL);
    assert(temp != NULL && results != NULL && resultsLength % 2 == 0);

    // how many elements greater than guess
    const cnv_size_t vectorSize = vector->size;
    const cnv_size_t stopPos    = vector->size * vector->stride;

    // max element in the vector
    double max = DBL_MIN;
    // copy all the elements in vector to a temporary array
    for (unsigned i = 0, j = 0; i != stopPos; i += vector->stride, ++j)
    {
        temp[j] = vector->data[i];
        if (temp[j] > max)
            max = temp[j];
    }

    va_list varArg;
    va_start(varArg, resultsLength);
    
    unsigned argNum = resultsLength / 2;
    for (unsigned count = 0; count != argNum; ++count)
    {
        cnv_size_t k = va_arg(varArg, cnv_size_t);

        cnv_size_t leftBound = 0;
        cnv_size_t rightBound = vectorSize - 1;
        while (leftBound < rightBound)
        {
            double swapTemp = 0.0;
            double guess = temp[k];
            cnv_size_t i = leftBound;
            cnv_size_t j = rightBound;
            do
            {
                while (temp[i] < guess)
                    ++i;

                while (temp[j] > guess)
                    --j;

                if (i <= j)
                {
                    // CNV_SWAP_NUM(temp[i], temp[j], swapTemp);
                    swapTemp = temp[i];
                    temp[i] = temp[j];
                    temp[j] = swapTemp;
                    ++i;
                    --j;
                }
            } while (i <= j);

            if (j < k) 
                leftBound = i;

            if (k < i) 
                rightBound = j;
        }

        results[2 * count] = temp[k];

        double kPlusOne = max;
        for (unsigned i = k + 1; i != vectorSize; ++i)
        {
            if (temp[i] < kPlusOne)
                kPlusOne = temp[i];
        }

        results[2 * count + 1] = kPlusOne;
    }

    va_end(varArg);
    return CNV_OK;
}

// compute the the best-fit linear regression coefficient of the model y = c1 * x and the r squared value of the fit
int CNV_VectorLinearFit(const CNV_Vector* x, const CNV_Vector* y, double* coeff, double* rSquare)
{
    assert(x != NULL && x->data != NULL);
    assert(y != NULL && y->data != NULL);
    assert(x->size == y->size);

    // covariance value of the coefficiency
    double cov = 0.0f;
    // sum squares of regression
    double sse = 0.0f;
    // least-squares fit with gsl library function
    gsl_fit_mul(x->data, x->stride, y->data, y->stride, x->size, coeff, &cov, &sse);

    double meanY = 0.0f;
    double sumSquareY = 0.0f;
    cnv_size_t stopPosY = y->stride * y->size;
    for (unsigned i = 0; i != stopPosY; i += y->stride)
    {
        meanY += y->data[i];
        sumSquareY += CNV_SQUARE(y->data[i]);
    }

    // mean value of y vector
    meanY = meanY / y->size;
    //total sum of squares (sum of squares about the mean)
    double sst = sumSquareY - (y->size * CNV_SQUARE(meanY));
    
    // r square (goodness of fit)
    *rSquare = 1 - (sse / sst);

    return CNV_OK;
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
