#include <fcntl.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "text_editor.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define msleep(msec) Sleep(msec)
#else
#include <unistd.h>
#define msleep(msec) usleep(msec * 1000)
#endif

enum CODE {
    NOEXIT_CODE = 0,
    EXIT_CODE = 1,
    SAVE_CODE = 2,
};

struct LineIndexer {
    int y;
    int x;
    int pos;
    int cur_line_idx;
    int line_ends[256];
    char associated_file[32];
    int modified;  // Флаг, контролирующий изменение в str.
    int fd;
} typedef LineIndexer;

void init(LineIndexer* idxer) {
    idxer->cur_line_idx = 0;
    idxer->x = 0;
    idxer->y = 0;
    idxer->pos = 0;
    for (int i = 0; i < 31; i++) {
        idxer->associated_file[i] = ' ';
    }
    idxer->associated_file[31] = '\0';
    idxer->modified = 0;
    idxer->fd = 0;
}

void save(LineIndexer* idxer, char* str, size_t len) {
    WINDOW* wnd = newwin(2, 44, getmaxy(stdscr) - 1, 0);
    if (idxer->modified)  // Если изменения в str произошли, значит записываем в файл
    {
        if (idxer->associated_file[0] == ' ') {
            waddstr(wnd, "Enter filename: ");
            wrefresh(wnd);
            wgetstr(wnd, idxer->associated_file);
        }
        idxer->fd = open(idxer->associated_file, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
        if (idxer->fd == -1) {
            perror("Open error");
        }
        int res = write(idxer->fd, str, len);

        if (res != -1) {
            waddstr(wnd, "Saving completed!\n");
        } else {
            perror("Write error");
        }
        wrefresh(wnd);
        msleep(270);
        close(idxer->fd);

        idxer->modified = 0;
    }

    wclear(wnd);
    wrefresh(wnd);
    delwin(wnd);
    refresh();
}

int confirm_exit(LineIndexer* idxer) {
    if (idxer->modified) {
        char y_n[5];
        WINDOW* wnd = newwin(2, 44, getmaxy(stdscr) - 2, 0);
        waddstr(wnd, "Are you sure want to exit?\nYou have unsaved changes.(yes/no/save)");
        wgetnstr(wnd, y_n, 4);
        y_n[4] = '\0';
        wclear(wnd);
        wrefresh(wnd);
        delwin(wnd);
        if (y_n[0] != 's' && y_n[0] != 'y') {
            return NOEXIT_CODE;
        }
        if (y_n[0] == 'y') {
            return EXIT_CODE;
        }
        if (y_n[0] == 's') {
            return SAVE_CODE;
        }
    } else {
        return EXIT_CODE;
    }
}

int main() {
    initscr();
    keypad(stdscr, true);
    scrollok(stdscr, TRUE);
    LineIndexer idxer;
    init(&idxer);
    char str[256];
    int ex = 0;
    int fd = 0;

    while (!ex) {
        int ch = mvgetch(idxer.y, idxer.x);

        switch (ch) {
            case ERR:
                mvaddstr(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, "Error has occured. Exit");
                break;

            case KEY_F(3):  //Выходим из программы, если была нажата F3
                ex = confirm_exit(&idxer);
                if (ex == SAVE_CODE) {
                    ex = EXIT_CODE;  // Без break переходим в case KEY_F(2), сохраняем и выходим из программы
                } else {
                    break;
                }

            case KEY_F(2):
                save(&idxer, str, strlen(str));
                break;

            case KEY_BACKSPACE:
                delch();
                str[--idxer.pos] = '\0';
                idxer.x--;
                if (idxer.x < 0) {
                    idxer.y--;
                    idxer.x = idxer.line_ends[--idxer.cur_line_idx];
                }
                if (!idxer.modified) idxer.modified = 1;
                break;
            case KEY_ENTER:
            case 10:
            case 13:
                idxer.line_ends[idxer.cur_line_idx++] = idxer.x;
                idxer.x = 0;
                str[idxer.pos++] = '\n';
                if (idxer.y + 1 != getmaxy(stdscr)) {
                    idxer.y++;
                }
                if (!idxer.modified) idxer.modified = 1;
                break;
            default:
                str[idxer.pos++] = ch;
                if (idxer.x + 1 != getmaxx(stdscr)) {
                    idxer.x++;
                }
                if (!idxer.modified) idxer.modified = 1;
                break;
        }

        refresh();
    }
    mvaddstr(getmaxy(stdscr) - 1, 0, "Press any key...");
    refresh();
    getch();
    endwin();
    return 0;
}