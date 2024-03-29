## Подобие чата, используя messages queue в качестве IPC

### Сборка
```
make
```

### Запуск
```
terminal 1
/server        <---- Должен быть запущен первый
```
```
terminal 2
/client        <---- Должен быть запущен второй
```

### Работа программы

![Снимок экрана от 2022-08-25 01-05-10](https://user-images.githubusercontent.com/60806892/186491580-737b08a6-e5f5-4531-b25b-4d36f66a9962.png)

#### Как видно из скриншота, клиенту, который написал сообщение, также получает его. В итоге на его стороне сообщение на экране дублируется.

![Снимок экрана от 2022-08-25 01-07-10](https://user-images.githubusercontent.com/60806892/186491802-fe502416-3141-44e0-9585-f26ad16f20cc.png)

#### Выход из программы осуществляется CTRL+C. Эта комбинация вызывает handle_shutdown, который удаляет файл очереди.

![Снимок экрана от 2022-08-25 01-07-19](https://user-images.githubusercontent.com/60806892/186492308-91695b4b-e36d-4267-96e9-e4a1da13f44e.png)

#### Поскольку клиентская сторона использует два процесса(один принимает сообщения, другой их отправляет), то после CTRL+C видно два disconnect с pid-ами процессов
