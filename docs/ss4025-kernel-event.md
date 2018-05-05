# 事件的使用 #

## 例程目的 ##

学会 事件的创建、初始化; 接收事件; 发送事件; 删除事件、脱离事件. 

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c}
/*
 * 程序清单：事件例程
 *
 * 这个程序会创建2个动态线程及初始化一个静态事件对象
 * 一个线程会以不同的方式接收事件；
 * 一个线程定时发送两个事件 (EVENT1) (EVENT2);
 */
#include <rtthread.h>

#define EVENT1 (1 << 3)
#define EVENT2 (1 << 5)

/* 事件控制块 */
static struct rt_event event;

/* 线程1入口函数 */
static void thread1_entry(void *param)
{
    rt_uint32_t e;
    rt_uint8_t i;

    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            /* “逻辑与”的方式接收事件,满足接收条件之后重置事件的相应标志位.*/
            if (rt_event_recv(&event, (EVENT1 | EVENT2),
                              RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                              RT_WAITING_FOREVER, &e) == RT_EOK)
            {
                rt_kprintf("thread1: AND recv event 0x%x\n", e);
            }
        }
        for (i = 0; i < 10; i++)
        {
            /* “逻辑或”的方式接收事件,满足接收条件之后重置事件的相应标志位.*/
            if (rt_event_recv(&event, (EVENT1 | EVENT2),
                              RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                              RT_WAITING_FOREVER, &e) == RT_EOK)
            {
                rt_kprintf("thread1: OR recv event 0x%x\n", e);
            }
        }
        while (1)
        {
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
}

/* 线程2入口函数 */
static void thread2_entry(void *param)
{
    rt_uint8_t i;
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            rt_kprintf("thread2: send event1\n");
            rt_event_send(&event, EVENT1);                     /* 发送事件(EVENT1)*/
            rt_thread_delay(rt_tick_from_millisecond(2000));
            rt_kprintf("thread2: send event2\n");
            rt_event_send(&event, EVENT2);                     /* 发送事件(EVENT2)*/
            rt_thread_delay(rt_tick_from_millisecond(2000));
        }
        rt_thread_delay(rt_tick_from_millisecond(500));
        rt_kprintf("thread2: detach event\n");
        rt_event_detach(&event);                               /* 脱离事件对象*/
        while (1)
        {
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
}

/* 事件示例的初始化 */
int event_simple_init()
{
    rt_thread_t tid = RT_NULL;
    /* 初始化事件对象 */
    rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);

    /* 创建线程1 */
    tid = rt_thread_create("thread1",
                           thread1_entry, RT_NULL, /* 线程入口是thread1_entry, 入口参数是RT_NULL */
                           1024, 5, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    /* 创建线程2 */
    tid = rt_thread_create("thread2",
                           thread2_entry, RT_NULL, /* 线程入口是thread2_entry, 入口参数是RT_NULL */
                           1024, 5, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(event_simple_init);

```

### 例程设计 ###

该例程在 event_simple_init中首先初始化静态事件,最后创建启动线程thread1和thread2.

在线程thread1中进行如下操作:
1. 以“逻辑与”的方式接收事件,当EVENT1与EVENT2都发生的时候,重置事件的相应标志位,唤醒该线程,打印出接收的事件标志.共循环5次这样的过程.
2. 以“逻辑或”的方式接收事件,当EVENT1或EVENT2任意一个事件发生的时候,重置事件的相应标志位,唤醒该线程,打印出接收的事件标志.共循环10次这样的操作.

在线程thread2中进行如下操作:
1. 循环依次发送事件EVENT1,EVENT2,共循环10次,发送20次事件.
2. 调用rt_event_detach()函数脱离事件.

两个线程互相作用,需要结合起来参考输出信息一起看.该例程主要演示事件 初始化、接收、发送、脱离这几个步骤的函数调用. 

### 编译调试及观察输出信息 ###

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread2: send event1
finsh >thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread2: send event2
thread1: AND recv event 0x28
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: send event1
thread1: OR recv event 0x8
thread2: send event2
thread1: OR recv event 0x20
thread2: detach event
```

由输出信息可知,thread1的前5次循环,每当thread2发送了event1 event2两个事件之后thread1才会去打印输出;thread1的后10次循环,thread2发送event1 event2两个事件中的任意一个之后thread1就会去打印输出.
