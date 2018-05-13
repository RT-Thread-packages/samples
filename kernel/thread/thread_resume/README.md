# 唤醒线程 #

## 介绍 ##

这个例子中将创建两个动态线程(t1和t2)，
低优先级线程t1将挂起自身
高优先级线程t2将在一定时刻后唤醒低优先级线程。

## 运行结果 ##

thread1 startup0
suspend thread self
thread1 resumed
