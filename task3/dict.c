

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
struct PhoneBook {
    char name[30];
    char surname[30];
    char number[12];
    struct PhoneBook* next;
};
struct PhoneBook* head = NULL;  // Голову вынес из структуры,
                                // чтобы в каждом узле не было лишнего поля

struct RemoveInfo {
    char info;
    struct PhoneBook* prev;
    struct PhoneBook* to_remove;
};

int is_equal(char* str1, char* str2) {
    int i;
    for (i = 0; str1[i] != '\0' || str2[i] != '\0'; i++) {
        if (str1[i] != str2[i])
            return 0;
    }
    if (str1[i] == str2[i])  // compare \0
        return 1;
    return 0;
}

void print_abonents() {
    int counter = 0;
    for (struct PhoneBook* abonent = head; abonent != NULL; abonent = abonent->next) {
        counter++;
        printf("%d | %s | %s | %s\n", counter,
               abonent->name,
               abonent->surname,
               abonent->number);
    }
}

struct RemoveInfo* search_abonent_to_remove() {
    char data[30];
    struct RemoveInfo* r_info = (struct RemoveInfo*)malloc(sizeof(struct RemoveInfo));
    struct PhoneBook* prev = head;
    printf("Enter abonent's data(name/surname/number): ");
    scanf("%29s", data);
    for (struct PhoneBook* abonent = head; abonent != NULL; abonent = abonent->next) {
        if (is_equal(data, abonent->name) || is_equal(data, abonent->surname) || is_equal(data, abonent->number)) {
            r_info->to_remove = abonent;
            r_info->prev = prev;
            if (prev == head && abonent == head) {
                r_info->info = 'h';  // Если удаляемый элемент - голова списка
            } else if (abonent->next == NULL) {
                r_info->info = 'l';  // Если удаляемый элемент - хвост списка
            } else {
                r_info->info = 's';
            }
            return r_info;
        }
    }
    return NULL;
}

void search_abonent() {
    char data[30];
    printf("Enter abonent's data(name/surname/number): ");
    scanf("%29s", data);
    for (struct PhoneBook* abonent = head; abonent != NULL; abonent = abonent->next) {
        if (is_equal(data, abonent->name) || is_equal(data, abonent->surname) || is_equal(data, abonent->number)) {
            printf("%s | %s | %s\n", abonent->name,
                   abonent->surname,
                   abonent->number);
            return;
        }
    }
    printf("There is no such user in the directory\n");
}

void remove_abonent() {
    struct RemoveInfo* r_info = search_abonent_to_remove();
    if (r_info == NULL) {
        printf("There is no such user in the directory\n");
        free(r_info);
        return;
    }
    if (r_info->info == 'h') {
        struct PhoneBook* tmp = head->next;
        free(head);
        head = tmp;
    } else if (r_info->info == 'l') {
        free(r_info->to_remove);
        r_info->prev->next = NULL;
    } else {
        struct PhoneBook* nxt = r_info->prev->next;
        r_info->prev->next = nxt->next;
        free(nxt);
    }
    free(r_info);
    printf("Abonent successfully deleted\n");
}

void add_abonent() {
    struct PhoneBook* new_abonent = (struct PhoneBook*)malloc(sizeof(struct PhoneBook));
    printf("Enter name: ");
    scanf("%29s", new_abonent->name);
    printf("Enter surname: ");
    scanf("%29s", new_abonent->surname);
    printf("Enter number: ");
    scanf("%11s", new_abonent->number);
    if (head == NULL) {
        head = new_abonent;
        new_abonent->next = NULL;
    } else {
        struct PhoneBook* prev;
        for (struct PhoneBook* abonent = head; abonent != NULL; abonent = abonent->next) {
            prev = abonent;
        }
        prev->next = new_abonent;
        new_abonent->next = NULL;
    }
}

void destructor() {
    struct PhoneBook* tmp_next = head->next;
    struct PhoneBook* abonent = head;
    while (abonent != NULL) {
        free(abonent);
        abonent = tmp_next;
        if (tmp_next != NULL) {
            tmp_next = tmp_next->next;
        }
    }
}

void menu() {
    char c = 'r';
    printf("=======-------------------===\n");
    printf("Press 'a': Add abonent\n");
    printf("Press 'p': Print all abonents\n");
    printf("Press 'r': Remove abonent\n");
    printf("Press 's': Search abonent\n");
    printf("Press 'q': Quit\n");
    printf("=======-------------------===\n");
    while (c != 'q') {
        scanf(" %c", &c);
        switch (c) {
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
                search_abonent();
                break;
            case 'q':
                destructor();
                printf("Quit\n");
                break;
            default:
                printf("Undefined command\n");
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}