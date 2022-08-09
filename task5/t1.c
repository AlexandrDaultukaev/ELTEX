#include <stdio.h>
#include "write_to_file.h"

#define BUF_SIZE 128

int main()
{
    char buffer[BUF_SIZE];
    char filename[BUF_SIZE];

    printf("Enter string: ");
    fgets(buffer, BUF_SIZE, stdin);
    printf("Enter filename: ");  
    fgets(filename, BUF_SIZE, stdin);

    write_to_file(buffer, filename);
    

    return 0;
}