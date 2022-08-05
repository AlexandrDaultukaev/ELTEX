#include "menu.h"

#include <dlfcn.h>
#include <stdio.h>

#include "functional.h"

#define NUM_OPS 4

void init_interface(void* libs[], int (*ops[])(int, int)) {
    int oper_num;
    char q = 'n';
    int used_oper_num = 0;
    printf("Choose which functions to use in calc: \n\n(q - quit the selection menu)\n\n");
    printf("0. Add\n1. Sub\n2. Mul\n3. Div\n");
    while (q != 'q' && used_oper_num < NUM_OPS) {
        scanf(" %c", &q);
        oper_num = (int)q - 48;

        if ((oper_num < NUM_OPS+1 && oper_num >= 0) && libs[oper_num] != NULL) {
            printf("This operation is already added!\n");
            continue;
        }

        switch (oper_num) {
            case 0:
                libs[oper_num] = dlopen("libmyadd.so", RTLD_LAZY);

                if (!libs[oper_num]) {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }

                ops[oper_num] = dlsym(libs[oper_num], "add");
                printf("\'+\' added!\n");
                used_oper_num++;
                break;
            case 1:
                libs[oper_num] = dlopen("libmysub.so", RTLD_LAZY);
                if (!libs[oper_num]) {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                ops[oper_num] = dlsym(libs[oper_num], "subtr");
                printf("\'-\' added!\n");
                used_oper_num++;
                break;
            case 2:
                libs[oper_num] = dlopen("libmymul.so", RTLD_LAZY);
                if (!libs[oper_num]) {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                ops[oper_num] = dlsym(libs[oper_num], "multipl");
                printf("\'*\' added!\n");
                used_oper_num++;
                break;
            case 3:
                libs[oper_num] = dlopen("libmydiv.so", RTLD_LAZY);
                if (!libs[oper_num]) {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                ops[oper_num] = dlsym(libs[oper_num], "div");
                printf("\'/\' added!\n");
                used_oper_num++;
                break;
            case 65:  // if user enter 'q'
                continue;
            default:
                printf("Oops, you may not see that I'm giving you a choice of 0-3!\n");
                break;
        }
    }
}

void menu() {
    int first = 0, second = 0, op = 0, res = -1;
    char quit = 'y';
    /* In libs & ops information about certain func is stored at certain index:

       |  0  |  1  |  2  |  3  |
       | ADD | SUB | MUL | DIV |

    */
    void* libs[NUM_OPS] = {[0 ... NUM_OPS-1] = NULL};
    int (*ops[NUM_OPS])(int, int);
    init_interface(libs, ops); // <--- func that fills libs & ops with information about funcs
                               // <--- AND configures which funcs will be in calc

    while (quit != 'n' && quit != 'q') {
        printf("Enter first number: ");
        scanf(" %d", &first);
        printf("Enter second number: ");
        scanf(" %d", &second);
        int counter = 0;
        printf("\n\nAvailable operations:\n");
        for (int counter = 0; counter < NUM_OPS; counter++) {
            if (counter == 0 && libs[counter] != NULL) printf("0. Add\n");
            if (counter == 1 && libs[counter] != NULL) printf("1. Sub\n");
            if (counter == 2 && libs[counter] != NULL) printf("2. Mul\n");
            if (counter == 3 && libs[counter] != NULL) printf("3. Div\n");
        }
        scanf("%d", &op);
        res = ops[op](first, second); // <--- calling function from dynamic library

        if (res == -1 && op == 3) {
            /* If denominator == 0 */
            printf("Something went wrong...TRY AGAIN!\n");
        } else {
            printf("RESULT: %d\n", res);
        }

        printf("Again?(y/n): ");
        scanf(" %c", &quit);
    }

    for (int i = 0; i < NUM_OPS; i++) {
        if (libs[i] != NULL) dlclose(libs[i]);
    }
}