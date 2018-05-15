# 删除线程 #

## 介绍 ##

这个例子会创建两个线程，在一个线程中删除另外一个线程。

## 程序清单 ##

```c
/*
 * 程序清单：删除线程
 *
 * 这个例子会创建两个线程，在一个线程中删除另外一个线程。
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/*
 * 线程删除(rt_thread_delete)函数仅适合于动态线程，为了在一个线程
 * 中访问另一个线程的控制块，所以把线程块指针声明成全局类型以供全
 * 局访问
 */
static rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

/* 线程1的入口函数 */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 线程1采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread count: %d\n", count ++);
    }
}

/* 线程2的入口函数 */
static void thread2_entry(void* parameter)
{
    /* 线程2拥有较高的优先级，以抢占线程1而获得执行 */

    /* 线程2启动后先睡眠10个OS Tick */
    rt_thread_delay(10);

    /*
     * 线程2唤醒后直接删除线程1，删除线程1后，线程1自动脱离就绪线程
     * 队列
     */
    rt_thread_delete(tid1);
    tid1 = RT_NULL;

    /*
     * 线程2继续休眠10个OS Tick然后退出，线程2休眠后应切换到idle线程
     * idle线程将执行真正的线程1控制块和线程栈的删除
     */
    rt_thread_delay(10);

    /*
     * 线程2运行结束后也将自动被删除(线程控制块和线程栈依然在idle线
     * 程中释放)
     */
    tid2 = RT_NULL;
}

/* 删除线程示例的初始化 */
int thread_delete_init()
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1",                       /* 线程1的名称是t1 */
                            thread1_entry, RT_NULL,     /* 入口是thread1_entry，参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)                                /* 如果获得线程控制块，启动这个线程 */
        rt_thread_startup(tid1);
    else
        return -1;

    /* 创建线程1 */
    tid2 = rt_thread_create("t2",                       /* 线程1的名称是t2 */
                            thread2_entry, RT_NULL,     /* 入口是thread2_entry，参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)                                /* 如果获得线程控制块，启动这个线程 */
        rt_thread_startup(tid2);
    else
        return -1;

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
    INIT_APP_EXPORT(thread_delete_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_delete_init, thread delete);

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