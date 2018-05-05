# signal 使用 #

## 例程目的 ##

学会基本signal使用. 

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
/*
 * 程序清单：signal
 *
 * 这个例子会创建两个线程tid1,tid2。
 * thread1 send signal to thread2
 * thread2 send signal to thread1
 *
 */
#include <rtthread.h>

#define THREAD_PRIORITY			25
#define THREAD_STACK_SIZE		512
#define THREAD_TIMESLICE		5


static	rt_thread_t	tid1 = RT_NULL;
static	rt_thread_t	tid2 = RT_NULL;

/* thread1 注册signal需要的信号处理函数 */
void thread1_signal_handler(int sig)
{
	rt_kprintf("[------thread2 send signal to thread1---------[sig]-------- = %d\n",sig);
} 

/* thread2 注册signal需要的信号处理函数 */
void thread2_signal_handler(int sig)
{
	rt_kprintf("[------thread1 send signal to thread2---------[sig]-------- = %d\n",sig);
} 

/* 主进程注册signal需要的信号处理函数 */
void handler(int sig)
{
	rt_kprintf("[------main-------[sig]-------- = %d\n",sig);
} 

/* 线程1的入口函数 */
static void thread1_entry(void* parameter)
{	
	rt_uint32_t count = 0;
	
	rt_signal_install(7, thread1_signal_handler);
	rt_signal_unmask(7);
	
	while (1)
	{
		/* 线程1采用低优先级运行，一直打印计数值 */
		rt_kprintf("\n[ thread1 count : ]  %d\n", count );
		count ++;
		
		if ( (count % 3) == 0)			
			rt_thread_kill(tid2, 9);
			
		rt_thread_delay(RT_TICK_PER_SECOND*5);
	}
}

/* 线程2的入口函数 */
static void thread2_entry(void* parameter)
{
	rt_uint32_t Thread2count = 0;
	
	rt_signal_install(9, thread2_signal_handler);
	rt_signal_unmask(9);
	
	while(1) {	
		rt_kprintf("\n[ thread2 count : ]  %d\n", Thread2count );
		Thread2count++;
		
		if ((Thread2count % 3) == 0)
			rt_thread_kill(tid1, 7);
		
		rt_thread_delay(RT_TICK_PER_SECOND*5);
	}
}

/* signal示例的初始化 */
int signal_sample_init()
{	
	rt_signal_install(2, handler);
	rt_signal_unmask(2);
	rt_thread_kill(rt_thread_self(), 2);

	
	/* 创建线程1 */
	tid1 = rt_thread_create("t1",	/* 线程1的名称是t1 */
		thread1_entry, RT_NULL,	/* 入口是thread1_entry，参数是RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
	
	if (tid1 != RT_NULL)	/* 如果获得线程控制块，启动这个线程 */
		rt_thread_startup(tid1);
	
	
	/* 创建线程2 */
	tid2 = rt_thread_create("t2",	/* 线程2的名称是t2 */
		thread2_entry, RT_NULL,	/* 入口是thread2_entry，参数是RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
	
	if (tid2 != RT_NULL)	/* 如果获得线程控制块，启动这个线程 */	
		rt_thread_startup(tid2);

	rt_thread_free_sig(tid1);	/* 释放thread1注册的信号*/
	rt_thread_free_sig(tid2);	/* 释放thread1注册的信号*/
	rt_thread_free_sig(rt_thread_self());	/* 释放thread1注册的信号*/
	
	return 0;
}
INIT_APP_EXPORT(signal_sample_init);

```

### 例程设计 ###
该例程在 signal_sample_init 中创建两个线程tid1，tid2，每三次thread1 send signal to thread2，thread2 send signal to thread1


### 编译调试及观察输出信息 ###

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
