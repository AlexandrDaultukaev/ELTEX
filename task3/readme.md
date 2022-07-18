## Абонентский справочник

Сборка и запуск задания "Абонентский справочник":
  ```
  make build
  ```
  
Работа программы:
```
./dict
=======-------------------===
Press 'a': Add abonent
Press 'p': Print all abonents
Press 'r': Remove abonent
Press 's': Search abonent
Press 'q': Quit
=======-------------------===
```

### Search:
```
s
Enter abonent's data(name/surname/number): Frank
Frank | Sinatra | 576815
s
Enter abonent's data(name/surname/number): Martin
Dean | Martin | 78815
s
Enter abonent's data(name/surname/number): 88164
Harry | Crosby | 88164
```

### Add abonent:
```
a
Enter name: Frank
Enter surname: Sinatra
Enter number: 576815
a
Enter name: Dean
Enter surname: Martin
Enter number: 78815
a
Enter name: Louis
Enter surname: Armstrong
Enter number: 526826     
a
Enter name: Harry
Enter surname: Crosby
Enter number: 88164
p
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
```

### Rewriting:
```
a
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Nat | Coles | 7889274
7 | Eric | Nelson | 28814
8 | Bob | Hope | 155672
9 | Pierino | Ronald | 445633
10 | Nancy | Sinatra | 77252
Free places in directory are over!
Who will you rewrite?(1-10): 8
Enter name: Peter 
Enter surname: Lawford
Enter number: 88164
p
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Nat | Coles | 7889274
7 | Eric | Nelson | 28814
8 | Peter | Lawford | 88164
9 | Pierino | Ronald | 445633
10 | Nancy | Sinatra | 77252
```

### Deleting:
```
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Nat | Coles | 7889274
7 | Eric | Nelson | 28814
8 | Peter | Lawford | 88164
9 | Pierino | Ronald | 445633
10 | Nancy | Sinatra | 77252
r
Enter abonent's data(name/surname/number): Nat
Abonent successfully deleted
p
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Eric | Nelson | 28814
7 | Peter | Lawford | 88164
8 | Pierino | Ronald | 445633
9 | Nancy | Sinatra | 77252
r
Enter abonent's data(name/surname/number): Nelson
Abonent successfully deleted
p
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Peter | Lawford | 88164
7 | Pierino | Ronald | 445633
8 | Nancy | Sinatra | 77252
r
Enter abonent's data(name/surname/number): 77252
Abonent successfully deleted
p
1 | Frank | Sinatra | 576815
2 | Dean | Martin | 78815
3 | Louis | Armstrong | 526826
4 | Harry | Crosby | 88164
5 | Elvis | Presley | 76512
6 | Peter | Lawford | 88164
7 | Pierino | Ronald | 445633
```
