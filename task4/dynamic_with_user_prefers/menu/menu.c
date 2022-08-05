#include "menu.h"

#include <dlfcn.h>
#include <stdio.h>

#include "functional.h"

void menu() {
    int first = 0, second = 0, op = 0, res = -1;
    char quit = 'y';
    void* add_lib = NULL;
    void* sub_lib = NULL;
    void* mul_lib = NULL;
    void* div_lib = NULL;
    int (*add_func)(int, int);
    int (*sub_func)(int, int);
    int (*mul_func)(int, int);
    int (*div_func)(int, int);
    char oper_num;
    int used_oper_num = 0;
    printf("Choose which functions to use in calc: \n\n(q - quit the selection menu)\n\n");
    printf("0. Add\n1. Sub\n2. Mul\n3. Div\n");
    while(oper_num != 'q')
    {
        scanf(" %c", &oper_num);
        switch (oper_num) {
            case '0':
                add_lib = dlopen("libmyadd.so", RTLD_LAZY);
                if(!add_lib)
                {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                add_func = dlsym(add_lib, "add");
                printf("\'+\' added!\n");
                used_oper_num++;
                break;
            case '1':
                sub_lib = dlopen("libmysub.so", RTLD_LAZY);
                if(!sub_lib)
                {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                sub_func = dlsym(sub_lib, "subtr");
                printf("\'-\' added!\n");
                used_oper_num++;
                break;
            case '2':
                mul_lib = dlopen("libmymul.so", RTLD_LAZY);
                if(!mul_lib)
                {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                mul_func = dlsym(mul_lib, "multipl");
                printf("\'*\' added!\n");
                used_oper_num++;
                break;
            case '3':
                div_lib = dlopen("libmydiv.so", RTLD_LAZY);
                if(!div_lib)
                {
                    fprintf(stderr, "dlopen() error: %s\n", dlerror());
                    return;
                }
                div_func = dlsym(div_lib, "div");
                printf("\'/\' added!\n");
                used_oper_num++;
                break;
            case 'q':
                continue;
            default:
                printf("Oops, you may not see that I'm giving you a choice of 0-3!\n");
                break;
        }
    }



    while (quit != 'n' && quit != 'q') {
        printf("Enter first number: ");
        scanf(" %d", &first);
        printf("Enter second number: ");
        scanf(" %d", &second);
        int counter = 0;
        for(int counter = 0; counter < 4; counter++)
        {
                if(counter == 0 && add_lib != NULL) printf("0. Add\n");
                if(counter == 1 && sub_lib != NULL) printf("1. Sub\n");
                if(counter == 2 && mul_lib != NULL) printf("2. Mul\n");
                if(counter == 3 && div_lib != NULL) printf("3. Div\n");
        }
        scanf(" %d", &op);
        switch (op)
        {
        case 0:
            res = add_func(first, second);
            break;
        case 1:
            res = sub_func(first, second);
            break;
        case 2:
            res = mul_func(first, second);
            break;
        case 3:
            res = div_func(first, second);
            break;
        default:
            break;
        }
        if (res == -1 && op == 3) {
            printf("Something went wrong...TRY AGAIN!\n");
        } else {
            printf("RESULT: %d\n", res);
        }
        printf("Again?(y/n): ");
        scanf(" %c", &quit);
    }
    if(add_lib != NULL) dlclose(add_lib);
    if(sub_lib != NULL) dlclose(sub_lib);
    if(mul_lib != NULL) dlclose(mul_lib);
    if(div_lib != NULL) dlclose(div_lib);
}