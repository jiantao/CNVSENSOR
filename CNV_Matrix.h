/*
 * =====================================================================================
 *
 *       Filename:  Matrix.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/11/2011 10:56:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CNV_MATRIX_H
#define  CNV_MATRIX_H

#include <stdlib.h>
#include "CNV_Types.h"

typedef struct 
{
    double* data;
    cnv_size_t rows;
    cnv_size_t cols;
}CNV_Matrix, CNV_MatrixView;

// create a new matrix
CNV_Matrix* CNV_MatrixAlloc(cnv_size_t rows, cnv_size_t cols);

// create a new matrix and initialize all the elements to zero
CNV_Matrix* CNV_MatrixCalloc(cnv_size_t rows, cnv_size_t cols);

// create a new matrix view 
CNV_MatrixView* CNV_MatrixViewAlloc(void);

// free the allocated memory for a matrix
void CNV_MatrixFree(CNV_Matrix* matrix);

// free the allocated memory for matrix view
void CNV_MatrixViewFree(CNV_Matrix* matrix);

// Get a submatrix of the original one
CNV_MatrixView* GetSubMatrix(CNV_Matrix* matrix, cnv_size_t beginRow, cnv_size_t endRow, cnv_size_t beginCol, cnv_size_t endCol);

// add the elements of a matrix to the elements to another matrix
int CNV_MatrixAdd(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// substract the elements of the second matrix from the first one
int CNV_MatrixSub(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// divide the elements of the first matrix by those of the second one 
int CNV_MatrixDiv(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// times the elements of the first matrix by those of the second one
int CNV_MatrixTimes(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// add a constant factor to the elements of the matrix
int CNV_MatrixAddConstant(CNV_Matrix* matrix, double factor);

// times the elements of the matrix by a constant factor
int CNV_MatrixScale(CNV_Matrix* matrix, double scale);

// calculate the medians of every row or every column of a matrix 
// Vector* CNV_MatrixMedian(const CNV_Matrix* matrix, short direction);

#endif  /*CNV_MATRIX_H*/

