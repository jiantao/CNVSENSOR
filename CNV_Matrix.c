/*
 * =====================================================================================
 *
 *       Filename:  CNV_Matrix.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/12/2011 11:05:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CNV_Error.h"
#include "CNV_Matrix.h"

// create a new matrix
CNV_Matrix* CNV_MatrixAlloc(cnv_size_t rows, cnv_size_t cols)
{
    // both dimensions of the matrix cannot be zero
    assert(rows != 0 && cols != 0);

    // allocate the memory for the object
    CNV_Matrix* newMatrix = malloc(sizeof(CNV_Matrix));
    if (newMatrix == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to creat matrix object.\n");

    // allocate the memory for the data inside the matrix
    newMatrix->data = malloc(sizeof(double) * rows * cols);
    if (newMatrix->data == NULL)
        CNV_ErrQuit("ERROR: Not enough memory for data storage in matrix object\n");

    // set the dimensions of the matrix
    newMatrix->rows = rows;
    newMatrix->cols = cols;

    return newMatrix;
}

// create a new matrix and initialize all the elements to zero
CNV_Matrix* CNV_MatrixCalloc(cnv_size_t rows, cnv_size_t cols)
{
    // both dimensions of the matrix cannot be zero
    assert(rows != 0 && cols != 0);

    // allocate the memory for the object
    CNV_Matrix* newMatrix = malloc(sizeof(CNV_Matrix));
    if (newMatrix == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to creat matrix object.\n");

    // allocate the memory for the data inside the matrix
    newMatrix->data = calloc(rows * cols, sizeof(double));
    if (newMatrix->data == NULL)
        CNV_ErrQuit("ERROR: Not enough memory for data storage in matrix object\n");

    // set the dimensions of the matrix
    newMatrix->rows = rows;
    newMatrix->cols = cols;

    return newMatrix;
}

// create a new matrix view 
CNV_MatrixView* CNV_MatrixViewAlloc(void)
{
    // allocate the memory for the object
    CNV_MatrixView* newMatrixView = malloc(sizeof(CNV_MatrixView));
    if (newMatrixView == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to creat matrix object.\n");

    return newMatrixView;
}

// free the allocated memory for a matrix
void CNV_MatrixFree(CNV_Matrix* matrix)
{
    // can not free NULL pointer
    assert(matrix != NULL);

    // free the stored data if there are any
    if (matrix->data != NULL)
        free(matrix->data);

    // free the object
    free(matrix);
}

// free the allocated memory for matrix view
void CNV_MatrixViewFree(CNV_MatrixView* matrixView)
{
    CNV_MatrixFree(matrixView);
}

// Get a submatrix of the original one
CNV_MatrixView* GetSubMatrix(CNV_Matrix* matrix, cnv_size_t beginRow, cnv_size_t endRow, cnv_size_t beginCol, cnv_size_t endCol)
{
    // the dimensions of the submatrix should be within the original matrix
    assert(beginRow < matrix->rows && endRow < matrix->rows && beginCol < matrix->cols && endCol < matrix->cols);

    // start position should be less than end position
    assert(beginRow < endRow && beginCol < endCol);

    // create a matrix view
    CNV_MatrixView* matrixView = CNV_MatrixViewAlloc();

    // get the dimension information for the submatrix 
    cnv_size_t beginInArray = beginRow * matrix->cols + beginCol;
    matrixView->data = matrix->data[beginInArray];
    matrixView->rows = endRow - startRow + 1;
    matrixView->cols = endCol - startCol + 1;

    return CNV_MatrixView;
}
