# 互斥量的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用互斥量。

## 运行结果 ##

```{.c}
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread2 try to get static mutex
thread2 got static mutex
thread1 try to get static mutex, wait 10 ticks.
finsh >thread1 take static mutex timeout
thread1 try to get static mutex, wait forever.
thread2 release static mutex
thread2 try to get dynamic mutex
thread2 got dynamic mutex
thread1 take a staic mutex, done.
thread1 try to get dynamic mutex, wait 10 ticks.
thread1 take dynamic mutex timeout
thread1 try to get dynamic mutex, wait forever.
thread2 release dynamic mutex
thread1 take a dynamic mutex, done.
```