#include <stdio.h>

#define N 5

void init_matrix(int matrix[N][N])
{
    int counter = 1;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            matrix[i][j] = counter++;
}

void print_matrix(int matrix[N][N]) {
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

int main()
{
    int matrix[N][N];
    init_matrix(matrix);
    print_matrix(matrix);
    
    return 0;
}