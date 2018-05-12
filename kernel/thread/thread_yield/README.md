# 线程让出处理器 #

## 介绍 ##

在这个例子中，将创建两个相同优先级的线程， 它们会通过rt_thread_yield
接口把处理器相互让给对方进行执行。

## 运行结果 ##

thread1: count = 0
thread2: count = 0
thread1: count = 1
thread2: count = 1
thread1: count = 2
thread2: count = 2
thread1: count = 3
thread2: count = 3
thread1: count = 4
thread2: count = 4
thread1: count = 5
thread2: count = 5
thread1: count = 6
thread2: count = 6
thread1: count = 7
thread2: count = 7
thread1: count = 8
thread2: count = 8
thread1: count = 9
thread2: count = 9
thread1: count = 10
thread2: count = 10
thread1: count = 11
thread2: count = 11
thread1: count = 12
thread2: count = 12
thread1: count = 13
thread2: count = 13
thread1: count = 14
thread2: count = 14
thread1: count = 15
thread2: count = 15
thread1: count = 16
thread2: count = 16
thread1thread2: count = 17
: count = 17
thread2: count = 18
thread1: count = 18