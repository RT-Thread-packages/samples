# 内存池的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用内存池。

## 运行结果 ##

在 ```memp.c``` 中第53行 ```rt_thread_delay(10);``` 处打断点，可以看到如下运行结果 

	 
	 \ | /   
	- RT -     Thread Operating System     
	 / | \     3.0.3 build Apr 11 2018   
	 2006 - 2018 Copyright by rt-thread team   
	finsh >allocate No.0   
	
	allocate No.1   
	allocate No.2   
	allocate No.3   
	allocate No.4   
	allocate No.5   
	allocate No.6   
	allocate No.7   
	allocate No.8   
	allocate No.9   
	allocate No.10   
	allocate No.11   
	allocate No.12   
	allocate No.13   
	allocate No.14   
	allocate No.15   
	allocate No.16   
	allocate No.17   
	allocate No.18   
	allocate No.19   
	allocate No.20   
	allocate No.21   
	allocate No.22   
	allocate No.23   
	allocate No.24   
	allocate No.25   
	allocate No.26   
	allocate No.27   
	allocate No.28   
	allocate No.29   
	allocate No.30   
	allocate No.31   
	allocate No.32   
	allocate No.33   
	allocate No.34   
	allocate No.35   
	allocate No.36   
	allocate No.37   
	allocate No.38   
	allocate No.39   
	allocate No.40   
	allocate No.41   
	allocate No.42   
	allocate No.43   
	allocate No.44   
	allocate No.45   
	allocate No.46   
	allocate No.47   
	try to release block 
	release block 0   
	allocate the block mem   

## 说明 ##
本例程会创建一个静态的内存池对象，2个动态线程。两个线程会试图分别从内存池中获得内存块，线程1申请内存块，线程2释放内存块，且线程1的优先级高于线程2。   
当线程1申请完48个内存块时，再执行申请内存块操作，线程1会因为没有空闲内存块可申请而被挂起，这时线程2执行，释放一个内存块后，内存池有可用空闲内存块，线程2被线程1抢占，线程1得到执行，执行到```rt_thread_delay(10);```时，线程1再次挂起线程2执行......
如此循环执行。