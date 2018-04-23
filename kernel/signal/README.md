
## 实验现象 ##

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 20 2018
 2006 - 2018 Copyright by rt-thread team
 
[------main-------[sig]-------- = 2

[ thread2 count : ]  0

[ thread1 count : ]  0

[ thread2 count : ]  1

[ thread1 count : ]  1

[ thread2 count : ]  2
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  2
[------thread1 send signal to thread2---------[sig]-------- = 9

[ thread2 count : ]  3

[ thread2 count : ]  4

[ thread1 count : ]  3

[ thread2 count : ]  5
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  4

[ thread2 count : ]  6

[ thread1 count : ]  5
[------thread1 send signal to thread2---------[sig]-------- = 9

[ thread2 count : ]  7

[ thread2 count : ]  8
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  6

[ thread2 count : ]  9

[ thread1 count : ]  7

[ thread2 count : ]  10

[ thread1 count : ]  8
[------thread1 send signal to thread2---------[sig]-------- = 9

[ thread2 count : ]  11
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread2 count : ]  12

[ thread1 count : ]  9

[ thread2 count : ]  13

[ thread1 count : ]  10

[ thread2 count : ]  14
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  11
[------thread1 send signal to thread2---------[sig]-------- = 9

[ thread2 count : ]  15

[ thread2 count : ]  16

[ thread1 count : ]  12

[ thread2 count : ]  17
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  13

[ thread2 count : ]  18

[ thread1 count : ]  14
[------thread1 send signal to thread2---------[sig]-------- = 9

[ thread2 count : ]  19

[ thread2 count : ]  20
[------thread2 send signal to thread1---------[sig]-------- = 7

[ thread1 count : ]  15
```
