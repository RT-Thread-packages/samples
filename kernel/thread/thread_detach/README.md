# 线程脱离 #

## 介绍 ##

这个例子会创建两个线程(t1和t2)，在t2中会对t1进行脱离操作；
t1脱离后将不在运行，状态也更改为初始状态。

## 程序清单 ##

```c
/*
 * 程序清单：线程脱离
 *
 * 这个例子会创建两个线程(t1和t2)，在t2中会对t1进行脱离操作；
 * t1脱离后将不在运行，状态也更改为初始状态
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 线程1控制块 */
static struct rt_thread thread1;
/* 线程1栈 */
static rt_uint8_t thread1_stack[THREAD_STACK_SIZE];
/* 线程2控制块 */
static struct rt_thread thread2;
/* 线程2栈 */
static rt_uint8_t thread2_stack[THREAD_STACK_SIZE];

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
    /* 线程2拥有较高的优先级，以抢占线程1而获得执行 */

    /* 线程2启动后先睡眠10个OS Tick */
    rt_thread_delay(10);

    /*
     * 线程2唤醒后直接执行线程1脱离，线程1将从就绪线程队列中删除
     */
    rt_thread_detach(&thread1);

    /*
     * 线程2继续休眠10个OS Tick然后退出
     */
    rt_thread_delay(10);

    /*
     * 线程2运行结束后也将自动被从就绪队列中删除，并脱离线程队列
     */
}

/* 线程脱离示例的初始化 */
int thread_detach_init(void)
{
    rt_err_t result;

    /* 初始化线程1 */
    result = rt_thread_init(&thread1, "t1",     /* 线程名：t1 */
                            thread1_entry,      /* 线程的入口是thread1_entr */
                            RT_NULL,            /* 入口参数是RT_NULL*/
                            &thread1_stack[0],  /* 线程栈是thread1_stack */
                            sizeof(thread1_stack),
                            THREAD_PRIORITY, 10);
    if (result == RT_EOK)                       /* 如果返回正确，启动线程1 */
        rt_thread_startup(&thread1);

    /* 初始化线程2 */
    result = rt_thread_init(&thread2, "t2",     /* 线程名：t2 */
                            thread2_entry,      /* 线程的入口是thread2_entry */
                            RT_NULL,            /* 入口参数是RT_NULL*/
                            &thread2_stack[0],  /* 线程栈是thread2_stack */
                            sizeof(thread2_stack),
                            THREAD_PRIORITY - 1, 10);
    if (result == RT_EOK)                       /* 如果返回正确，启动线程2 */
        rt_thread_startup(&thread2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_detach_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_detach_init, thread detach);

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

