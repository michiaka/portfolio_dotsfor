#include <stdio.h>
#include <stdlib.h>

int main() {
    int initialRows = 2; // Initial number of rows
    int initialCols = 2; // Initial number of columns

    int **matrix = (int **)malloc(initialRows * sizeof(int *));
    for (int i = 0; i < initialRows; ++i) {
        matrix[i] = (int *)malloc(initialCols * sizeof(int));
    }

    int rows = initialRows;
    int cols = initialCols;

    printf("Enter elements for the matrix. Enter -1 to stop.\n");

    int input;
    int i = 0, j = 0;

    while (1) {
        printf("Enter element at position (%d, %d): ", i, j);
        scanf("%d", &input);

        if (input == -1) {
            break; // Stop taking input if -1 is entered
        }

        matrix[i][j] = input;

        j++; // Move to the next column

        if (j == cols) {
            // Resize the matrix if the current row is full
            cols *= 2;
            for (int k = 0; k < rows; ++k) {
                matrix[k] = (int *)realloc(matrix[k], cols * sizeof(int));
            }
        }

        if (j == cols) {
            // Move to the next row and reset column index
            i++;
            j = 0;

            // Resize the matrix if the current rows are full
            if (i == rows) {
                rows *= 2;
                matrix = (int **)realloc(matrix, rows * sizeof(int *));
                for (int k = i; k < rows; ++k) {
                    matrix[k] = (int *)malloc(cols * sizeof(int));
                }
            }
        }
    }

    // Display the matrix
    printf("Matrix:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Deallocate the memory
    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
