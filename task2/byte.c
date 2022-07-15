#include <stdio.h>


int main()
{
    int number = 270533154;
    char* ptr = (char*)&number;
    printf("\nBefore\n");
    printf("\tHex: %x\n", number);
    printf("\tDec: %d\n", number);
    ptr += 2;
    *ptr = 128;
    printf("\nAfter\n");
    printf("\tHex: %x\n", number);
    printf("\tDec: %d\n", number);
    return 0;
}