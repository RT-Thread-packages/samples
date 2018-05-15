# 挂起线程 #

## 介绍 ##

这个例子中将创建两个动态线程(t1和t2)
低优先级线程t1在启动后将一直持续运行；
高优先级线程t2在一定时刻后唤醒并挂起低优先级线程。

## 程序清单 ##

```c
/*
 * 程序清单：挂起线程
 *
 * 这个例子中将创建两个动态线程(t1和t2)
 * 低优先级线程t1在启动后将一直持续运行；
 * 高优先级线程t2在一定时刻后唤醒并挂起低优先级线程。
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* 线程1入口 */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 线程1采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread count: %d\n", count ++);
    }
}

/* 线程2入口 */
static void thread2_entry(void* parameter)
{
    /* 延时10个OS Tick */
    rt_thread_delay(10);

    /* 挂起线程1 */
    rt_thread_suspend(tid1);

    /* 延时10个OS Tick */
    rt_thread_delay(10);

    /* 线程2自动退出 */
}

/* 挂起线程示例的初始化 */
int thread_suspend_init(void)
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1",
                            thread1_entry,  /* 线程入口是thread1_entry */
                            RT_NULL,        /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("t2",
                            thread2_entry,  /* 线程入口是thread2_entry */
                            RT_NULL,        /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_suspend_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_suspend_init, thread suspend);
```

## 运行结果 ##

```
thread count: 0
thread count: 1
thread count: 2
thread count: 3
thread count: 4
thread count: 5
thread count: 6
thread count: 7
thread count: 8
thread count: 9
thread count: 10
thread count: 11
thread count: 12
thread count: 13
thread count: 14
thread count: 15
thread count: 16
thread count: 17
thread count: 18
thread count: 19
thread count: 20
thread count: 21
thread count: 22
thread count: 23
thread count: 24
thread count: 25
thread count: 26
thread count: 27
thread count: 28
thread count: 29
thread count: 30
thread count: 31
thread count: 32
thread count: 33
thread count: 34
thread count: 35
thread count: 36
thread count: 37
thread count: 38
thread count: 39
thread count: 40
thread count: 41
thread count: 42
thread count: 43
thread count: 44
thread count: 45
thread count: 46
thread count: 47
thread count: 48
thread count: 49
thread count: 50
thread count: 51
thread count: 52
thread count: 53
thread count: 54
thread count: 55
thread count: 56
```
