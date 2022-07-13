#include <stdio.h>

void print_matrix(int N) {
    int counter = 1;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            printf("%d ", counter++);
        printf("\n");
    }
}

int main()
{
    int N = 0;
    printf("Enter N: ");
    scanf("%d", &N);
    print_matrix(N);
}