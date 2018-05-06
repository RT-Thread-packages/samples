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
	rt_kprintf("[thread2 send signal to thread1---------[sig]-------- = %d\n",sig);
} 

/* thread2 注册signal需要的信号处理函数 */
void thread2_signal_handler(int sig)
{
	rt_kprintf("[thread1 send signal to thread2---------[sig]-------- = %d\n",sig);
} 

/* 主进程注册signal需要的信号处理函数 */
void handler(int sig)
{
	rt_kprintf("[main-------[sig]-------- = %d\n",sig);
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
	tid1 = rt_thread_create("t1", /* 线程1的名称是t1 */
		thread1_entry, RT_NULL,   /* 入口是thread1_entry，参数是RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
	
	if (tid1 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */
		rt_thread_startup(tid1);
	
	
	/* 创建线程2 */
	tid2 = rt_thread_create("t2", /* 线程2的名称是t2 */
		thread2_entry, RT_NULL,   /* 入口是thread2_entry，参数是RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
	
	if (tid2 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */	
		rt_thread_startup(tid2);

	rt_thread_free_sig(tid1);
	rt_thread_free_sig(tid2);
	rt_thread_free_sig(rt_thread_self());
	
	return 0;
}
INIT_APP_EXPORT(signal_sample_init);