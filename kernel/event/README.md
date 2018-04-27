# 事件的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用事件。

## 运行结果 ##

```{.c}
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread2: send event1
finsh >thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: detach event
```