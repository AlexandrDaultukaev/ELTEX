#include <stdio.h>
#include <stdlib.h>

#define N 5

void print_reverse(int* arr)
{
    for(int i = N-1; i >= 0; i--)
        printf("%d ", arr[i]);
}

int main()
{
    if(N > 0)
    {
        int arr[N] = {[0 ... N-1] = 0};
        printf("Enter N numbers: ");
        for(int i = 0; i < N; i++)
        {
            scanf("%d", (arr + i));
        }
        print_reverse(arr);
    }
    return 0;
}