## Калькулятор(динамическая библиотека)

Для запуска сборки, в папке task4 введите команду: 
  ```
  make
  ```
Для запуска программы:
```
make run
```
Структура проекта:
После сборки:
```
task4/
  build/
    bin/
      main         <--- Бинарник
    libs/
      libmycalc.so  <--- Библиотека хранящая все объектные файлы из obj(кроме main.o)
    obj/
      addition.o
      division.o
      main.o
      ...
```
Внутри libmycalc.so:
```
00000000000013e9 T add
0000000000004060 b completed.0
                 w __cxa_finalize@GLIBC_2.2.5
0000000000001140 t deregister_tm_clones
0000000000001417 T div
00000000000011b0 t __do_global_dtors_aux
0000000000003e18 d __do_global_dtors_aux_fini_array_entry
0000000000004058 d __dso_handle
0000000000003e20 d _DYNAMIC
0000000000001454 t _fini
00000000000011f0 t frame_dummy
0000000000003e10 d __frame_dummy_init_array_entry
0000000000002238 r __FRAME_END__
0000000000004000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000000020d8 r __GNU_EH_FRAME_HDR
0000000000001000 t _init
                 U __isoc99_scanf@GLIBC_2.7
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000011f9 T menu
000000000000143b T multipl
                 U printf@GLIBC_2.2.5
                 U puts@GLIBC_2.2.5
0000000000001170 t register_tm_clones
                 U __stack_chk_fail@GLIBC_2.4
0000000000001401 T subtr
0000000000004060 d __TMC_END__
```
