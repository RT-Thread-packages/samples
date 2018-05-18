# 线程让出处理器 #

## 介绍 ##

在这个例子中，将创建两个相同优先级的线程， 它们会通过rt_thread_yield
接口把处理器相互让给对方进行执行。

## 程序清单 ##

```{.c}
/*
 * 程序清单：线程让出处理器
 * 在这个例子中，将创建两个相同优先级的线程， 它们会通过rt_thread_yield
 * 接口把处理器相互让给对方进行执行。
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
        /* 打印线程1的输出 */
        rt_kprintf("thread1: count = %d\n", count ++);

        /* 执行yield后应该切换到thread2执行 */
        rt_thread_yield();
    }
}

/* 线程2入口 */
static void thread2_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 打印线程2的输出 */
        rt_kprintf("thread2: count = %d\n", count ++);

        /* 执行yield后应该切换到thread1执行 */
        rt_thread_yield();
    }
}

/* 线程让出处理器示例的初始化 */
int thread_yield_init(void)
{
    /* 创建线程1 */
    tid1 = rt_thread_create("thread",
                            thread1_entry,      /* 线程入口是thread1_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("thread",
                            thread2_entry,      /* 线程入口是thread2_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_yield_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_yield_init, thread yield);
```

## 运行结果 ##

```
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
```