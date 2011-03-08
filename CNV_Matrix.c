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
CNV_Matrix* CNV_MatrixAlloc(size_t rows, size_t cols)
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
CNV_Matrix* CNV_MatrixCalloc(size_t rows, size_t cols)
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
        size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            fprintf(output, "%-10.2f ", matrix->data[matrixIndex++]);

        fprintf(output, "\n");
    }

    fprintf(output, "\n");
}

// get the element in i-th row and j-th column of the matrix
double CNV_MatrixGet(const CNV_Matrix* matrix, size_t i, size_t j)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(i < matrix->rows && j < matrix->cols);

    return (matrix->data[i * matrix->stride + j]);
}

// calculate the median of every row or every column of a matrix 
int CNV_MatrixGetMedian(CNV_Matrix* matrix, CNV_Vector* medians, CNV_Direction direction)
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

// do the linear fit for each row or column in the matrix
int CNV_MatrixLinearFit(CNV_Matrix* matrix, const CNV_Vector* obs, CNV_Vector* coeffs, CNV_Vector* rSquares, CNV_Direction direction)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(obs != NULL && obs->data != NULL);
    assert(coeffs != NULL && coeffs->data != NULL);
    assert(rSquares != NULL && rSquares->data != NULL);

    CNV_VectorView vectorView;
    if (direction == ROW_DIRECTION)
    {
        for (unsigned i = 0; i != matrix->rows; ++i)
        {
            size_t coeffsIndex   = i * coeffs->stride;
            size_t rSquaresIndex = i * rSquares->stride;

            CNV_MatrixGetRowView(matrix, &vectorView, i);
            CNV_VectorLinearFit(obs, &vectorView, &(coeffs->data[coeffsIndex]), &(rSquares->data[rSquaresIndex]));
        }
    }
    else
    {
        for (unsigned i = 0; i != matrix->cols; ++i)
        {
            size_t coeffsIndex   = i * coeffs->stride;
            size_t rSquaresIndex = i * rSquares->stride;

            CNV_MatrixGetColView(matrix, &vectorView, i);
            CNV_VectorLinearFit(obs, &vectorView, &(coeffs->data[coeffsIndex]), &(rSquares->data[rSquaresIndex]));
        }
    }

    return CNV_OK;
}

//===============================
// Non-constant methods
//===============================

// set the element in i-th row and j-th column of the matrix
int CNV_MatrixSet(CNV_Matrix* matrix, size_t i, size_t j, double value)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(i < matrix->rows && j < matrix->cols);

    matrix->data[i * matrix->stride + j] = value;
    
    return CNV_OK;
}

// get a submatrix of the original one
int CNV_MatrixGetView(CNV_Matrix* matrix, CNV_MatrixView* matrixView, size_t beginRow, size_t endRow, size_t beginCol, size_t endCol)
{
    // there should not be any null pointers for input
    assert(matrix != NULL && matrix->data != NULL && matrixView != NULL);

    // the dimensions of the submatrix should be within the original matrix
    assert(beginRow < matrix->rows && endRow < matrix->rows && beginCol < matrix->cols && endCol < matrix->cols);

    // start position should be less than end position
    assert(beginRow < endRow && beginCol < endCol);

    // get the dimension information for the submatrix 
    size_t beginInArray = beginRow * matrix->cols + beginCol;
    matrixView->data = matrix->data + beginInArray;
    matrixView->rows = endRow - beginRow + 1;
    matrixView->cols = endCol - beginCol + 1;
    matrixView->stride = matrix->stride;

    return CNV_OK;
}

// Get a view of a certain row of a matrix
int CNV_MatrixGetRowView(CNV_Matrix* matrix, CNV_VectorView* rowView, size_t rowNum)
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
int CNV_MatrixGetColView(CNV_Matrix* matrix, CNV_VectorView* colView, size_t colNum)
{
    // check input arguments
    assert(matrix != NULL && matrix->data != NULL && colView != NULL && colNum < matrix->cols);

    // initialize the column view
    colView->data = matrix->data + colNum;
    colView->size = matrix->rows;
    colView->stride = matrix->stride;

    return CNV_OK;
}

// swap two rows in a matrix
void CNV_MatrixSwapRows(CNV_Matrix* matrix, size_t rowSwapFrom, size_t rowSwapTo)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(matrix->rows > rowSwapFrom && matrix->rows > rowSwapTo);

    // do noting if two rows are the same
    if (rowSwapFrom == rowSwapTo)
        return;

    double temp = 0;
    size_t stopPos = rowSwapFrom * matrix->stride + matrix->cols;
    for (unsigned i = rowSwapFrom * matrix->stride, j = rowSwapTo * matrix->stride; i != stopPos; ++i, ++j)
    {
        temp = matrix->data[i];
        matrix->data[i] = matrix->data[j];
        matrix->data[j] = temp;
    }
}

// swap two rows in a matrix
void CNV_MatrixSwapCols(CNV_Matrix* matrix, size_t colSwapFrom, size_t colSwapTo)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(matrix->cols > colSwapFrom && matrix->cols > colSwapTo);

    // do noting if two columns are the same
    if (colSwapFrom == colSwapTo)
        return;

    double temp = 0;
    size_t stopPos = matrix->rows * matrix->stride + colSwapFrom;
    for (unsigned i = colSwapFrom, j = colSwapTo; i != stopPos; i += matrix->stride, j += matrix->stride)
    {
        temp = matrix->data[i];
        matrix->data[i] = matrix->data[j];
        matrix->data[j] = temp;
    }
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
        size_t matrixIndex = i * matrix->stride;
        size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
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
        size_t matrixIndex = i * matrix->stride;
        size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
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
        size_t matrixIndex = i * matrix->stride;
        size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
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
        size_t matrixIndex = i * matrix->stride;
        size_t otherMatrixIndex = i * otherMatrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
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
        size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
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
        size_t matrixIndex = i * matrix->stride;

        // get the end position of current row
        size_t matrixRowEnd = matrixIndex + matrix->cols;
        
        while (matrixIndex != matrixRowEnd)
            matrix->data[matrixIndex++] *= scale; 
    }

    return CNV_OK;
}

// get a matrix from the multiplication from two vectors
// first vector is a column vector where the second one is a row vector
int CNV_MatrixFromVectorsMultiply(CNV_Matrix* matrix, const CNV_Vector* vectorCol, const CNV_Vector* vectorRow)
{
    // no null pointers in the input data
    assert(matrix != NULL && matrix->data != NULL);
    assert(vectorCol != NULL && vectorCol->data != NULL);
    assert(vectorRow != NULL && vectorRow->data != NULL);

    // dimensions of vector must agree must agree with the result matrix
    assert(vectorCol->size == matrix->rows && vectorRow->size == matrix->cols);

    for (int i = 0; i != vectorCol->size; ++i)
    {
        unsigned colVecIndex = i * vectorCol->stride;
        for (int j = 0; j != vectorRow->size; ++j)
        {
            unsigned rowVecIndex = j * vectorRow->stride;
            CNV_MatrixSet(matrix, i, j, vectorCol->data[colVecIndex] * vectorRow->data[rowVecIndex]);
        }
    }

    return CNV_OK;
}


// delete certain rows or columns from a matrix (swap the to-be-deleted rows or columns to the end of the matrix)
int CNV_MatrixDelVectors(CNV_Matrix* matrix, CNV_SizeVector* indexMap, const CNV_SizeList* delIndices, CNV_Direction direction)
{
    assert(matrix != NULL && matrix->data != NULL);
    assert(indexMap != NULL && indexMap->sizeData != NULL);
    assert(delIndices != NULL);

    // nothing to be deleted
    if (delIndices->count == 0)
        return CNV_OK;

    // number of vectors left to be deleted
    size_t leftDelNum = delIndices->count;

    // the index of the vector to be deleted
    CNV_SizeListNode* currDelIndex = delIndices->first;

    // last element in to-be-deleted list
    CNV_SizeListNode* lastDelIndex = delIndices->last;

    if (direction == ROW_DIRECTION) // delete rows
    {
        assert(indexMap->count == matrix->rows);

        // next available row for to-be-deleted row
        size_t nextRow = matrix->rows - 1;

        while (leftDelNum != 0)
        {
            // find the next avaiable row that to-be-deleted row can be swap to
            while (nextRow == lastDelIndex->sizeData && nextRow != currDelIndex->sizeData)
            {
                --nextRow;
                --leftDelNum;
                lastDelIndex = lastDelIndex->prev;
            }

            // swap the to-be-deleted row to the end of the matrix
            if (currDelIndex->sizeData < nextRow) 
            {
                // swap the two rows
                CNV_MatrixSwapRows(matrix, currDelIndex->sizeData, nextRow);

                // swap two indices in the index map
                size_t temp = 0;
                CNV_SWAP(indexMap->sizeData[currDelIndex->sizeData], indexMap->sizeData[nextRow], temp);

                // update variables
                --nextRow;
                --leftDelNum;
                currDelIndex = currDelIndex->next;
            }
            else
                break;
        }

        // update the number of rows in the matrix
        matrix->rows -= delIndices->count;
    }
    else // delete columns
    {
        assert(indexMap->count == matrix->cols);

        // next available row for to-be-deleted row
        size_t nextCol = matrix->cols - 1;

        while (leftDelNum != 0)
        {
            // find the next avaiable row that to-be-deleted row can be swap to
            while (nextCol == lastDelIndex->sizeData && nextCol != currDelIndex->sizeData)
            {
                --nextCol;
                --leftDelNum;
                lastDelIndex = lastDelIndex->prev;
            }

            // swap the to-be-deleted row to the end of the matrix
            if (currDelIndex->sizeData < nextCol) 
            {
                // swap two columns
                CNV_MatrixSwapCols(matrix, currDelIndex->sizeData, nextCol);

                // swap two indices in the index map
                size_t temp = 0;
                CNV_SWAP(indexMap->sizeData[currDelIndex->sizeData], indexMap->sizeData[nextCol], temp);

                // update variables
                --nextCol;
                --leftDelNum;
                currDelIndex = currDelIndex->next;
            }
            else 
                break;
        }

        // update the number of columns in the matrix
        matrix->cols -= delIndices->count;
    }

    indexMap->count -= delIndices->count;
    return CNV_OK;
}
