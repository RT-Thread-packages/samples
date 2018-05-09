# 事件的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用事件。

## 运行结果 ##

thread2: send event1
thread2 leave.
thread3: send event2
thread1: AND recv event 0x28
thread1: delay 1s to prepare second event
thread3: send event2
thread3 leave.
thread1: OR recv event 0x20
thread1 leave.
