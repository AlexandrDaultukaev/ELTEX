#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5



void init_matrix(int matrix[N][N], int mode)
{
    int counter;
    int value;
    switch(mode)
    {
        case 1:
            counter = 1;
            value = 0;
            break;
        case 2:
            counter = N-1;
            value = 1;
            break;
        case 3:
            counter = 1;
            value = 1;
            break;
        default:
            counter = N-1;
            value = 0;
    }
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(j == counter)
                value = value ^ 1;
            matrix[i][j] = value;
        }
        switch(mode)
        {
            case 3:
            case 1:
                counter++;
                break;
            default:
                counter--;
        }
        value = value ^ 1;
    }
}

void print_matrix(int matrix[N][N]) {
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void spin_around()
{
    int counter = 0;
    int matrix[N][N];
    while (1)
    {
        system("clear");
        init_matrix(matrix, counter);
        print_matrix(matrix);
        counter = (counter + 1) % 4;
        sleep(1);
    }

}

int main()
{
    int matrix[N][N];
    init_matrix(matrix, 0);
    print_matrix(matrix);
    // spin_around(); // for fun
    return 0;
}