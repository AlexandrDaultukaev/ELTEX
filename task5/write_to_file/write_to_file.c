#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "write_to_file.h"

void write_to_file(char buffer[], char filename[])
{
    ssize_t ret;
    size_t text_length = strlen(buffer);
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if(fd == -1)
    {
        perror("Error open file");
        return;
    }
    
    ret = write(fd, buffer, text_length);
    if(ret == -1)
    {
        perror("Error write to file");
        goto closeFile;
    } else if(ret < text_length)
    {
        printf("Only %ld of %ld bytes were written\n", ret, text_length);
        goto closeFile;
    }

closeFile:
    if(close(fd) == -1) {
        perror("Error close fd");
    }

}