#include <fcntl.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "text_editor.h"

struct LineIndexer {
    int y;
    int x;
    int pos;
    int cur_line_idx;
    int line_ends[256];
} typedef LineIndexer;

void init(LineIndexer* idxer)
{
    idxer->cur_line_idx = 0;
    idxer->x = 0;
    idxer->y = 0;
    idxer->pos = 0;
}

int main() {
    initscr();
    keypad(stdscr, true);
    LineIndexer idxer;
    init(&idxer);
    char str[100];
    int ex = 0;
    

    while (!ex) {
        int ch = mvgetch(idxer.y, idxer.x);

        switch (ch) {
            case ERR:
                printw("Please, press any key...\n");  //Если нажатия не было, напоминаем пользователю, что надо нажать клавишу
                break;

            case KEY_F(2):
                WINDOW *wnd;
                // WINDOW *subwnd;
                wnd = newwin(2, 44, getmaxy(stdscr)-1, 0);
                char filename[20];
                //move(getmaxy(stdscr), 0);
                waddstr(wnd, "Enter filename: ");
                wrefresh(wnd);
                wgetstr(wnd, filename);
                int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
                write(fd, str, strlen(str));
                close(fd);
                wclear(wnd);
                wrefresh(wnd);
                refresh();
                delwin(wnd);
                break;
            case KEY_F(3):  //Выходим из программы, если была нажата F2
                ex = 1;
                break;
            case KEY_BACKSPACE:
                delch();
                str[--idxer.pos] = '\0';
                idxer.x--;
                if(idxer.x < 0)
                {
                    idxer.y--;
                    idxer.x = idxer.line_ends[--idxer.cur_line_idx];
                }
                break;
            case KEY_ENTER:
            case 10:
            case 13:
                idxer.line_ends[idxer.cur_line_idx++] = idxer.x;
                idxer.y++;
                idxer.x = 0;
                str[idxer.pos++] = '\n';
                break;
            default:
                str[idxer.pos++] = ch;
                idxer.x++;
                break;
        }

        refresh();
    }
    refresh();
    getch();
    endwin();
    return 0;
}