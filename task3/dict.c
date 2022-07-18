#include <stdio.h>

/* Абонентский справочник:
    - Имя
    - Фамилия
    - Телефон

   Меню:
    * Добавить абонента             | 'a'
    * Удалить абонента              | 'r'
    * Искать абонента               | 's'
    * Вывести список всех абонентов | 'p'
    * Выйти из справочника          | 'q'
*/

#define SIZE 10

struct Abonent {
    char name[30];
    char surname[30];
    char number[12];
};

int free_places[SIZE] = {[0 ... SIZE-1] = 1};
struct Abonent abonents[SIZE];

int get_free_index()
{
    for(int i = 0; i < SIZE; i++)
    {
        if(free_places[i] == 1)
            return i;
    }
    return -1;
}

int is_equal(char* str1, char* str2)
{
    int i;
    for(i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
    {
        if(str1[i] != str2[i])
            return 0;
    }
    if(str1[i] == str2[i]) // compare \0
        return 1;
    return 0;
}

void print_abonents() {
    int counter = 0;
    for(int i = 0; i < SIZE; i++)
    {
        if(free_places[i] == 0)
        {
            counter++;
            printf("%d | %s | %s | %s\n", counter, 
                                        abonents[i].name, 
                                        abonents[i].surname, 
                                        abonents[i].number);
        }
    }
}

int search_abonent(int print_info)
{
    char data[30];
    printf("Enter abonent's data(name/surname/number): ");
    scanf("%29s", data);
    for(int i = 0; i < SIZE; i++)
    {
        if(is_equal(data, abonents[i].name) || is_equal(data, abonents[i].surname) || is_equal(data, abonents[i].number))
        {
            if(print_info)
                printf("%s | %s | %s\n",abonents[i].name, 
                                        abonents[i].surname, 
                                        abonents[i].number);
            return i;
        }
    }
    if(print_info)
        printf("There is no such user in the directory\n");
    return -1;
}

void remove_abonent() {
    int idx = search_abonent(0);
    if(idx == -1)
    {
        printf("There is no such user in the directory\n");
        return;
    }
    free_places[idx] = 1;
    printf("Abonent successfully deleted\n");
}

void add_abonent() {
    int idx = get_free_index();
    if(idx == -1)
    {
        print_abonents();
        printf("Free places in directory are over!\nWho will you rewrite?(1-10): ");
        scanf("%d", &idx);
        idx--;
    }
    printf("Enter name: ");
    scanf("%29s", abonents[idx].name);
    printf("Enter surname: ");
    scanf("%29s", abonents[idx].surname);
    printf("Enter number: ");
    scanf("%11s", abonents[idx].number);
    free_places[idx] = 0;
}

void menu()
{
    char c = 'r';
    printf("=======-------------------===\n");
    printf("Press 'a': Add abonent\n");
    printf("Press 'p': Print all abonents\n");
    printf("Press 'r': Remove abonent\n");
    printf("Press 's': Search abonent\n");
    printf("Press 'q': Quit\n");
    printf("=======-------------------===\n");
    while(c != 'q')
    {
        scanf(" %c", &c);
        switch (c)
        {
            case 'a':
                add_abonent();
                break;
            case 'p':
                print_abonents();
                break;
            case 'r':
                remove_abonent();
                break;
            case 's':
                search_abonent(1);
                break;
            case 'q':
                printf("Quit\n");
                break;
            default:
                printf("Undefined command\n");
                break;
        }
    }
    
}

int main()
{
    menu();
    return 0;
}