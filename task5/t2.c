#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "write_to_file.h"

#define BUF_SIZE 128

void reverse_print(char* filename, size_t text_length)
{
    int fd = open(filename, O_RDONLY);
    ssize_t ret;
    off_t ret_lseek;
    char c = 'r';

    if(fd == -1)
    {
        perror("Error open file");
        return;
    }

    for(int i = 2; i <= text_length; i++)
    {
        ret_lseek = lseek(fd, -i, 2);
        if(ret_lseek == -1)
        {
            perror("Error lseek");
            goto closeFile;
        }
        ret = read(fd, &c, 1);
        if(ret == -1)
        {
            perror("Error read");
            goto closeFile;
        }
        printf("%c", c);
    }
    printf("\n");

closeFile:
    close(fd);
}

int main()
{
    char buffer[BUF_SIZE];
    char filename[BUF_SIZE];
    
    

    printf("Enter string: ");
    fgets(buffer, BUF_SIZE, stdin);
    printf("Enter filename: ");  
    fgets(filename, BUF_SIZE, stdin);

    write_to_file(buffer, filename);

    size_t text_length = strlen(buffer);
    reverse_print(filename, text_length);

    
    return 0;
}