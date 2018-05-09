# 信号 #

信号用来通知线程发生了异步事件，又称为软中断信号，线程之间可以互相发送软中断信号。

## 介绍 ##

这个例子会创建两个线程线程1和线程2，每个线程会分别安装信号，然后给这两个线程发送信号，线程收到信号后会执行对应的信号处理函数。

## 运行结果 ##

thread2 count : 0
thread1 count : 0
thread1 count : 1
thread2 count : 1
thread1 count : 2
thread1 count : 3
thread2 count : 2
thread1 count : 4
thread2 received signal 11
thread2 count : 3
thread1 received signal 10
thread1 count : 5
thread1 count : 6
thread2 count : 4
thread1 count : 7
thread1 count : 8
thread2 count : 5
thread1 count : 9
thread2 count : 6
thread2 count : 7
thread2 count : 8
thread2 count : 9
