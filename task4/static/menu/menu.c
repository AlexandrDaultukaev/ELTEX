#include "menu.h"

#include <stdio.h>

#include "functional.h"

void menu() {
    int first = 0, second = 0, op = 0, res = -1, div_flag = 0;
    char quit = 'y';
    while (quit != 'n' && quit != 'q') {
        printf("Enter first number: ");
        scanf(" %d", &first);
        printf("Enter second number: ");
        scanf(" %d", &second);
        printf("1. ADD\n2. SUB\n3. MUL\n4. DIV\n");
        scanf(" %d", &op);
        switch (op) {
            case 1:
                res = add(first, second);
                break;
            case 2:
                res = subtr(first, second);
                break;
            case 3:
                res = multipl(first, second);
                break;
            case 4:
                res = div(first, second);
                div_flag = 1;
                break;

            default:
                printf("Oops, you may not see that I'm giving you a choice of 1-4...only 1-4!\n");
                break;
        }
        if (res == -1 && div_flag) {
            printf("Something went wrong...TRY AGAIN!\n");
        } else {
            printf("RESULT: %d\n", res);
        }
        div_flag = 0;
        printf("Again?(y/n): ");
        scanf(" %c", &quit);
    }
}