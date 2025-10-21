#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void matrixGeneration(int **matrix, int imageSize)
{
    srand((unsigned)time(NULL));
    for (int row = 0; row < imageSize; row++)
    {
        for (int column = 0; column < imageSize; column++)
        {
            *( *(matrix + row)  + column)  = rand() % 256;
        }
    }
}

void printMatrix(int **matrix, int imageSize)
{
    for (int row = 0; row < imageSize; row++)
    {
        for (int column = 0; column < imageSize; column++)
        {
            printf("%5d ", *( *(matrix + row)  + column));
        }
        printf("\n");
    }
}

void matrixRotation(int **matrix, int imageSize)
{
    for (int row = 0; row < imageSize; row++)
    {
        for (int column = row+1; column < imageSize; column++)
        {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + column) + row);
            *(*(matrix + column) + row) = temp;
        }
    }
    for (int row = 0; row < imageSize; row++)
    {
        for (int column = 0; column < imageSize/2; column++)
        {
            int *left = (*(matrix+row) + column);
            int *right = (*(matrix + row) + (imageSize - column -1));

            int temp = *left;
            *left = *right;
            *right = temp;
        }
    }
}

void smoothingMatrix(int **matrix, int imageSize)
{
    int *previousRow = (int *)malloc(imageSize * sizeof(int));
    int *currentRow = (int *)malloc(imageSize * sizeof(int));
    for (int row = 0; row < imageSize; row++)
    {
        for (int column = 0; column < imageSize; column++)
        {
            int sum = 0, count = 0;
            for (int rowIndex = row - 1; rowIndex <= row+1 ; rowIndex++)
            {
                for (int columnIndex = column - 1; columnIndex  <= column + 1; columnIndex++)
                {
                    if (rowIndex >= 0 && rowIndex < imageSize && columnIndex >= 0 && columnIndex < imageSize)
                    {
                        int value;
                        if (rowIndex == row -1 && row > 0)
                        {
                            value = previousRow[columnIndex];
                        }
                        else
                        {
                            value = *(*(matrix + rowIndex)+ columnIndex);
                        }
                        sum += value;
                        count++;
                    }
                }
            }
            currentRow[column] = sum / count;
        }
        for (int column = 0; column < imageSize; column++)
        {
            previousRow[column] = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = currentRow[column];
        }
    }
    free(previousRow);
    free(currentRow);
}

int main()
{
    int imageSize,result = 1;
    printf("Enter Image size(2-10): ");
    scanf("%d", &imageSize);
    if (imageSize < 2 || imageSize > 10)
    {
        printf("Value should between 2 and 10");
        result = 0;
    }
    if (result)
    {
        int **matrix = (int **)malloc(imageSize * sizeof(int *));
        for (int index = 0; index < imageSize; index++)
        {
            *(matrix + index) = (int *)malloc(imageSize * sizeof(int));
        }
        matrixGeneration(matrix, imageSize);
        printf("Original Randomly Generated Matrix:\n");
        printMatrix(matrix, imageSize);
        matrixRotation(matrix, imageSize);
        printf("Matrix after 90° Clockwise Rotation:\n");
        printMatrix(matrix, imageSize);
        smoothingMatrix(matrix, imageSize);
        printf("Matrix after Applying 3×3 Smoothing Filter:\n");
        printMatrix(matrix, imageSize);
        for (int index = 0; index < imageSize; index++)
        {
            free(*(matrix + index));
        }
        free(matrix);
    }
    return 0;
}
