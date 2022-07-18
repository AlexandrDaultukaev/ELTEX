#include <stdio.h>

struct test_packed {
    char a;
    int b;
} __attribute__((packed));

struct test_padded {
    char a;
    int b;
};

void print_bin(unsigned int value)
{
    for (int i = sizeof(int) * 8-1; i >= 0; i--)
    {
        printf("%d", (value & (1 << i)) >> i );
        if(i % 8 == 0)
            printf(" ");
    }
    putc('\n', stdout);
}

int main()
{
    char str[10] = {'A', 0, 0, 0, 0, 'B', 0, 0, 0, 0};
    struct test_padded* ptr_pad = (struct test_padded *)str;
    struct test_packed* ptr_pack = (struct test_packed *)str;


    printf("==========------------------===\n");
    printf("\tStruct with padding\n");
    printf("==========------------------===\n");
    
    printf("CHAR: %c\n", ptr_pad->a);
    printf("INT BIN: ");
    print_bin(ptr_pad->b);
    printf("INT: %d\n", ptr_pad->b);
    printf("---\nPointer++\n---\n");
    ptr_pad++;
    printf("CHAR: %c\n", ptr_pad->a);
    printf("INT: %d\n", ptr_pad->b);

    printf("==========------------------===\n");
    printf("\tPacked struct\n");
    printf("==========------------------===\n");
    
    printf("CHAR: %c\n", ptr_pack->a);
    printf("INT BIN: ");
    print_bin(ptr_pack->b);
    printf("INT: %d\n", ptr_pack->b);
    printf("---\nPointer++\n---\n");
    ptr_pack++;
    printf("CHAR: %c\n", ptr_pack->a);
    printf("INT: %d\n", ptr_pack->b);
    return 0;
}