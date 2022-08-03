## Калькулятор

### Для запуска сборки, в папке task4 введите команду: 
  ```
  make
  ```
### Для запуска программы:
```
make run
```
## Структура проекта:
### После сборки:
```
task4/
  build/
    bin/
      main         <--- Бинарник
    libs/
      libmycalc.a  <--- Библиотека хранящая все объектные файлы из obj(кроме main.o)
    obj/
      addition.o
      division.o
      main.o
      ...
```
## Внутри libmycalc.a:
```
menu.o:
                 U add
                 U div
                 U __isoc99_scanf
0000000000000000 T menu
                 U multipl
                 U printf
                 U puts
                 U __stack_chk_fail
                 U subtr

addition.o:
0000000000000000 T add

subtraction.o:
0000000000000000 T subtr

division.o:
0000000000000000 T div

multiplication.o:
0000000000000000 T multipl
```
