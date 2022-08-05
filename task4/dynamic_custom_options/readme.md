## Калькулятор(динамическая библиотека + использование <dlfcn.h>)

Для запуска сборки, в папке task4 введите команду: 
  ```
  make
  ```
Для запуска программы:
```
make run
```

### Работа программы:
```
Choose which functions to use in calc: 

(q - quit the selection menu)

0. Add
1. Sub
2. Mul
3. Div
0
'+' added!
2
'*' added!
q
Enter first number: 2
Enter second number: 3


Available operations:
0. Add
2. Mul
2
RESULT: 6
Again?(y/n): n
```

### Структура проекта: \
После сборки:
```
dynamic_custom_options/
  build/
    bin/
      main         <--- Бинарник
    libs/
      libmyadd.so  <--- Библиотека хранящая ТОЛЬКО функцию add
      libmydiv.so
      libmymenu.so
      libmymul.so
      libmysub.so
    obj/
      addition.o
      division.o
      main.o
      ...
```
