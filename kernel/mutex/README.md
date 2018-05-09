# 互斥量的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用互斥量。

## 运行结果 ##

thread1 try to get static mutex, wait 10 ticks.
thread1 take a static mutex, failed.
thread2 try to get static mutex
thread2 got static mutex
thread2 release static mutex
thread2 try to get dynamic mutex
thread2 got dynamic mutex
thread2 release dynamic mutex
