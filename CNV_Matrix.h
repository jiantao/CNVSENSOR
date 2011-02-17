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

#include <stdio.h>
#include <stdlib.h>
#include "CNV_Types.h"
#include "CNV_Vector.h"

//===============================
// Type and constant definition
//===============================

// typedef of matrix object
typedef struct 
{
    // array to store the data
    double* data;
    // number of rows
    cnv_size_t rows;
    // number of columns
    cnv_size_t cols;
    // distance to the next element in the same column
    cnv_size_t stride;
}CNV_Matrix, CNV_MatrixView;

// constants that control the function-operation direction on a matrix
#define ROW_DIRECTION 0
#define COL_DIRECTION 1


//===============================
// Constructors and Destructors
//===============================

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


//===============================
// Constant methods
//===============================

// print the elements in the matrix
void CNV_MatrixPrint(const CNV_Matrix* matrix, FILE* output);

// get the element in i-th row and j-th column of the matrix
double CNV_MatrixGet(const CNV_Matrix* matrix, cnv_size_t i, cnv_size_t j);

// calculate the median of every row or every column of a matrix 
int CNV_MatrixGetMedian(CNV_Matrix* matrix, CNV_Vector* medians, short direction);


//===============================
// Non-constant methods
//===============================

// set the element in i-th row and j-th column of the matrix
int CNV_MatrixSet(CNV_Matrix* matrix, cnv_size_t i, cnv_size_t j, double value);

// Get a submatrix of the original one
int CNV_MatrixGetView(CNV_Matrix* matrix, CNV_MatrixView* matrixView, cnv_size_t beginRow, cnv_size_t endRow, cnv_size_t beginCol, cnv_size_t endCol);

// Get a view of a certain row of a matrix
int CNV_MatrixGetRowView(CNV_Matrix* matrix, CNV_VectorView* rowView, cnv_size_t rowNum);

// Get a view of a certain column of a matrix
int CNV_MatrixGetColView(CNV_Matrix* matrix, CNV_VectorView* colView, cnv_size_t colNum);

// add the elements of a matrix to the elements to another matrix
int CNV_MatrixAdd(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// substract the elements of the second matrix from the first one
int CNV_MatrixSub(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// times the elements of the first matrix by those of the second one
int CNV_MatrixTimes(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// divide the elements of the first matrix by those of the second one 
int CNV_MatrixDiv(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix);

// add a constant factor to the elements of the matrix
int CNV_MatrixAddConstant(CNV_Matrix* matrix, double factor);

// times the elements of the matrix by a constant factor
int CNV_MatrixScale(CNV_Matrix* matrix, double scale);

// get a matrix from the multiplication from two vectors
// first vector is a column vector where the second one is a row vector
int CNV_MatrixFromVectorsMultiply(CNV_Matrix* matrix, const CNV_Vector* vectorCol, const CNV_Vector* vectorRow);

#endif  /*CNV_MATRIX_H*/

