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

#include <assert.h>
#include "CNV_Error.h"
#include "CNV_Matrix.h"


//===============================
// Constructors and Destructors
//===============================

// create a new matrix
CNV_Matrix* CNV_MatrixAlloc(cnv_size_t rows, cnv_size_t cols)
{
    // both dimensions of the matrix cannot be zero
    assert(rows != 0 && cols != 0);

    // allocate the memory for the object
    CNV_Matrix* newMatrix = malloc(sizeof(CNV_Matrix));
    if (newMatrix == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to creat a matrix object.\n");

    // allocate the memory for the data inside the matrix
    newMatrix->data = malloc(sizeof(double) * rows * cols);
    if (newMatrix->data == NULL)
        CNV_ErrQuit("ERROR: Not enough memory for data storage in the matrix object.\n");

    // set the dimensions of the matrix
    newMatrix->rows = rows;
    newMatrix->cols = cols;
    newMatrix->stride = cols;

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
        CNV_ErrQuit("ERROR: Not enough memory for data storage in matrix object.\n");

    // set the dimensions of the matrix
    newMatrix->rows = rows;
    newMatrix->cols = cols;
    newMatrix->stride = cols;

    return newMatrix;
}

// create a new matrix view 
CNV_MatrixView* CNV_MatrixViewAlloc(void)
{
    // allocate the memory for the object
    CNV_MatrixView* newMatrixView = malloc(sizeof(CNV_MatrixView));
    if (newMatrixView == NULL)
        CNV_ErrQuit("ERROR: Not enough memory to creat matrix object.\n");

    // initialize the new created matrix view
    newMatrixView->data = NULL;
    newMatrixView->rows = 0;
    newMatrixView->cols = 0;
    newMatrixView->stride = 0;

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
    assert(matrixView != NULL);

    free(matrixView);
}


//===============================
// Constant methods
//===============================

void CNV_MatrixPrint(const CNV_Matrix* matrix, FILE* output)
{
    // cannot print an empty matrix
    assert(matrix != NULL && matrix->data != NULL);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            fprintf(output, "%-10.2f ", matrix->data[matrixIndex++]);

        fprintf(output, "\n");
    }

    fprintf(output, "\n");
}

// get the element in i-th row and j-th column of the matrix
double CNV_MatrixGet(const CNV_Matrix* matrix, cnv_size_t i, cnv_size_t j)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(i < matrix->rows && j < matrix->cols);

    return (matrix->data[i * matrix->stride + j]);
}

// calculate the median of every row or every column of a matrix 
int CNV_MatrixGetMedian(CNV_Matrix* matrix, CNV_Vector* medians, short direction)
{
    // check input arguments
    assert(matrix != NULL && matrix->data != NULL && medians != NULL && medians->data != 0);

    // vector view to hold a row or a column
    CNV_VectorView* vectorView = CNV_VectorViewAlloc();

    // decide direction of calculation
    if (direction == ROW_DIRECTION)
    {
        // vector size should be at least as large as the number of rows in the matrix
        assert(medians->size >= matrix->rows);
        for (unsigned i = 0; i != medians->size; ++i)
        {
            CNV_MatrixGetRowView(matrix, vectorView, i);
            medians->data[i] = CNV_VectorGetMedian(vectorView);
        }
    }
    else
    {
        // vector size should be at least as large as the number of column in the matrix
        assert(medians->size >= matrix->cols);
        for (unsigned i = 0; i != medians->size; ++i)
        {
            CNV_MatrixGetColView(matrix, vectorView, i);
            medians->data[i] = CNV_VectorGetMedian(vectorView);
        }
    }

    CNV_VectorViewFree(vectorView);
    return CNV_OK;
}

//===============================
// Non-constant methods
//===============================

// set the element in i-th row and j-th column of the matrix
int CNV_MatrixSet(CNV_Matrix* matrix, cnv_size_t i, cnv_size_t j, double value)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(i < matrix->rows && j < matrix->cols);

    matrix->data[i * matrix->stride + j] = value;
    
    return CNV_OK;
}

// get a submatrix of the original one
int CNV_MatrixGetView(CNV_Matrix* matrix, CNV_MatrixView* matrixView, cnv_size_t beginRow, cnv_size_t endRow, cnv_size_t beginCol, cnv_size_t endCol)
{
    // there should not be any null pointers for input
    assert(matrix != NULL && matrix->data != NULL && matrixView != NULL);

    // the dimensions of the submatrix should be within the original matrix
    assert(beginRow < matrix->rows && endRow < matrix->rows && beginCol < matrix->cols && endCol < matrix->cols);

    // start position should be less than end position
    assert(beginRow < endRow && beginCol < endCol);

    // get the dimension information for the submatrix 
    cnv_size_t beginInArray = beginRow * matrix->cols + beginCol;
    matrixView->data = matrix->data + beginInArray;
    matrixView->rows = endRow - beginRow + 1;
    matrixView->cols = endCol - beginCol + 1;
    matrixView->stride = matrix->stride;

    return CNV_OK;
}

// Get a view of a certain row of a matrix
int CNV_MatrixGetRowView(CNV_Matrix* matrix, CNV_VectorView* rowView, cnv_size_t rowNum)
{
    // check input arguments
    assert(matrix != NULL && matrix->data != NULL && rowView != NULL && rowNum < matrix->rows);

    // initialize the row view
    rowView->data = matrix->data + rowNum * matrix->stride;
    rowView->size = matrix->cols;
    rowView->stride = 1;

    return CNV_OK;
}

// Get a view of a certain column of a matrix
int CNV_MatrixGetColView(CNV_Matrix* matrix, CNV_VectorView* colView, cnv_size_t colNum)
{
    // check input arguments
    assert(matrix != NULL && matrix->data != NULL && colView != NULL && colNum < matrix->cols);

    // initialize the column view
    colView->data = matrix->data + colNum;
    colView->size = matrix->rows;
    colView->stride = matrix->stride;

    return CNV_OK;
}

// add the elements of a matrix to the elements to another matrix
int CNV_MatrixAdd(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix)
{
    // check matrices
    assert(matrix != NULL && otherMatrix != NULL);
    assert(matrix->data != NULL && otherMatrix->data != NULL);
    assert(matrix->rows == otherMatrix->rows && matrix->cols == otherMatrix->cols);


    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;
        cnv_size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] += otherMatrix->data[otherMatrixIndex++];
    }

    return CNV_OK;
}


// substract the elements of the second matrix from the first one
int CNV_MatrixSub(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix)
{
    // check matrices
    assert(matrix != NULL && otherMatrix != NULL);
    assert(matrix->data != NULL && otherMatrix->data != NULL);
    assert(matrix->rows == otherMatrix->rows && matrix->cols == otherMatrix->cols);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;
        cnv_size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] -= otherMatrix->data[otherMatrixIndex++];
    }

    return CNV_OK;
}

// times the elements of the first matrix by those of the second one
int CNV_MatrixTimes(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix)
{
    // check matrices
    assert(matrix != NULL && otherMatrix != NULL);
    assert(matrix->data != NULL && otherMatrix->data != NULL);
    assert(matrix->rows == otherMatrix->rows && matrix->cols == otherMatrix->cols);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;
        cnv_size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] *= otherMatrix->data[otherMatrixIndex++];
    }

    return CNV_OK;
}

// divide the elements of the first matrix by those of the second one 
int CNV_MatrixDiv(CNV_Matrix* matrix, const CNV_Matrix* otherMatrix)
{
    // check matrices
    assert(matrix != NULL && otherMatrix != NULL);
    assert(matrix->data != NULL && otherMatrix->data != NULL);
    assert(matrix->rows == otherMatrix->rows && matrix->cols == otherMatrix->cols);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;
        cnv_size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] /= otherMatrix->data[otherMatrixIndex++];
    }

    return CNV_OK;
}

// add a constant factor to the elements of the matrix
int CNV_MatrixAddConstant(CNV_Matrix* matrix, double factor)
{
    // check matrix
    assert(matrix != NULL && matrix->data != NULL);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] += factor; 
    }

    return CNV_OK;
}

// times the elements of the matrix by a constant factor
int CNV_MatrixScale(CNV_Matrix* matrix, double scale)
{
    // check matrix
    assert(matrix != NULL && matrix->data != NULL);

    for (unsigned i = 0; i != matrix->rows; ++i)
    {
        // get the begin position of current row
        cnv_size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        cnv_size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] *= scale; 
    }

    return CNV_OK;
}
