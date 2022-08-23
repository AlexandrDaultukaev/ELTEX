## Bash

## Сборка
```
make
```

## Работа программы
```
>>> ls | grep a
main
main.c
main.o
Makefile
readme.md
>>> ls | grep ake
Makefile
>>> ls
main  main.c  main.o  Makefile	script.py
>>> ls -la
итого 56
drwxrwxr-x 2 alex alex  4096 авг 16 16:55 .
drwxrwxr-x 5 alex alex  4096 авг 16 16:49 ..
-rwxrwxr-x 1 alex alex 20576 авг 16 16:55 main
-rw-rw-r-- 1 alex alex  1761 авг 16 16:49 main.c
-rw-rw-r-- 1 alex alex 10832 авг 16 16:55 main.o
-rw-rw-r-- 1 alex alex   135 авг 16 16:49 Makefile
-rw-rw-r-- 1 alex alex  1681 авг 16 16:49 script.py
>>> echo HELLO!
HELLO!
>>> exit
```

Также можно вызвать скрипт .py из fork_with_exec:

```
>>> python script.py Moscow
Weather in Moscow now: 16°C, wind: light 0.3-3.4m/s
>>> 
```
