#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **allocateMemory(const int matrixSize)
{
    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    if (matrix != NULL)
    {
        for (int index = 0; index < matrixSize; index++)
        {
            *(matrix + index) = (int *)malloc(matrixSize * sizeof(int));
            if (*(matrix + index) == NULL)
            {
                for (int row = 0; row < index ; row++)
                {
                    free(*(matrix + row));
                }
                free(matrix);
                matrix = NULL;
                break;
            }
        }
    }
    return matrix;
}

void freeMemory(int **matrix, const int matrixSize)
{
    if (matrix != NULL)
    {
        for (int index = 0; index < matrixSize; index++)
        {
            if (*(matrix + index) != NULL)
            {
                free(*(matrix + index));
            }
        }
    }
    free(matrix);
    matrix = NULL;
}

void matrixGeneration(int **matrix, const int matrixSize)
{
    srand((unsigned)time(NULL));
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            *( *(matrix + row)  + column)  = rand() % 256;
        }
    }
}

void printMatrix(int **matrix, const int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            printf("%5d ", *( *(matrix + row)  + column));
        }
        printf("\n");
    }
}

void matrixRotation(int **matrix, const int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = row+1; column < matrixSize; column++)
        {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + column) + row);
            *(*(matrix + column) + row) = temp;
        }
    }
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize/2; column++)
        {
            int *left = (*(matrix+row) + column);
            int *right = (*(matrix + row) + (matrixSize - column -1));

            int temp = *left;
            *left = *right;
            *right = temp;
        }
    }
}

void smoothingMatrix(int **matrix, const int matrixSize)
{
    int *previousRow = (int *)malloc(matrixSize * sizeof(int));
    int *currentRow = (int *)malloc(matrixSize * sizeof(int));
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            int sum = 0, count = 0;
            for (int rowIndex = row - 1; rowIndex <= row+1 ; rowIndex++)
            {
                for (int columnIndex = column - 1; columnIndex  <= column + 1; columnIndex++)
                {
                    if (rowIndex >= 0 && rowIndex < matrixSize && columnIndex >= 0 && columnIndex < matrixSize)
                    {
                        int value;
                        if (rowIndex == row -1 && row > 0)
                        {
                            value = *(previousRow + columnIndex);
                        }
                        else
                        {
                            value = *(*(matrix + rowIndex) + columnIndex);
                        }
                        sum += value;
                        count++;
                    }
                }
            }
            *(currentRow + column) = sum / count;
        }
        for (int column = 0; column < matrixSize; column++)
        {
            *(previousRow + column) = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(currentRow + column);
        }
    }
    free(previousRow);
    free(currentRow);
}

int main()
{
    int matrixSize, result = 1;
    printf("Enter Image size(2-10): ");
    scanf("%d", &matrixSize);
    if (matrixSize < 2 || matrixSize > 10)
    {
        printf("Value should between 2 and 10");
        result = 0;
    }
    if(result) 
    {
        int **matrix = allocateMemory(matrixSize);
        if(matrix != NULL)
        {
            matrixGeneration(matrix, matrixSize);
            printf("Original Randomly Generated Matrix:\n");
            printMatrix(matrix, matrixSize);
            matrixRotation(matrix, matrixSize);
            printf("Matrix after 90° Clockwise Rotation:\n");
            printMatrix(matrix, matrixSize);
            smoothingMatrix(matrix, matrixSize);
            printf("Matrix after Applying 3×3 Smoothing Filter:\n");
            printMatrix(matrix, matrixSize);
            freeMemory(matrix, matrixSize);
            matrix = NULL;
        }
    }
    return 0;
}
