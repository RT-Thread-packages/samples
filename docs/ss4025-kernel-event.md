# 事件的使用 #

## 例程目的 ##

学会 事件的创建/初始化; 接收事件; 发送事件; 删除事件/脱离事件. 

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

两个线程互相作用,需要结合起来参考输出信息一起看.该例程主要演示事件 初始化\接收\发送\脱离\这几个步骤的函数调用. 

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
## 本文相关核心API ##

### 初始化事件 rt_event_init() ###

* 函数原型:

```
rt_err_t rt_event_init(rt_event_t event, const char* name, rt_uint8_t flag);
```
对一个静态的事件进行初始化.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|event|事件对象的句柄，它由用户提供，并指向事件对象的内存块；|
|name  | 事件名称；|
|flag| 事件标志，可以取如下类型的数值：|
```
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```

* 函数返回：

创建成功返回RT_EOK；否则返回其它。

### 创建事件 rt_event_create() ###

* 函数原型:

```
rt_event_t rt_event_create (const char* name, rt_uint8_t flag);
```
创建一个动态的事件.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|name  | 事件名称；|
|flag|事件的标志，取值可以使用如下类型：|

```
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```


* 函数返回：

创建成功返回指向事件的事件句柄；否则返回RT_NULL。

### 删除事件 rt_event_delete() ###

* 函数原型:

```
rt_err_t rt_event_delete (rt_event_t event);
```
可以删除动态事件,会释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| event   | 事件对象的句柄；|

* 函数返回：

返回RT_EOK。

### 脱离事件 rt_event_detach() ###

* 函数原型:

```
rt_err_t rt_event_detach(rt_event_t event);
```
事件对象从内核对象管理器中脱离掉,但是不释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| event   | 事件对象的句柄；|

* 函数返回：

返回RT_EOK。

### 接收事件 rt_event_recv() ###

* 函数原型:

```
rt_err_t rt_event_recv(rt_event_t event, rt_uint32_t set, rt_uint8_t option,rt_int32_t timeout, rt_uint32_t* recved);
```
当用户调用这个接口时，系统首先根据set参数和接收选项来判断它要接收的事件是否发生，如果已经发生，则根据参数option上是否设置有RT_EVENT_FLAG_CLEAR来决定是否重置事件的相应标志位，然后返回（其中recved参数返回收到的事件）； 如果没有发生，则把等待的set和option参数填入线程本身的结构中，然后把线程挂起在此事件对象上，直到其等待的事件满足条件或等待时间超过指定的超时时间。如果超时时间设置为零，则表示当线程要接受的事件没有满足其要求时就不等待，而直接返回-RT_TIMEOUT。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| event  | 事件对象的句柄.|
|set|接收线程感兴趣的事件.|
|option|接受选项.|
|timeout|指定超时的时间,单位是操作系统时钟节拍(OS Tick)|
|recved|只想接收的事件.|

* 函数返回：

正确接收返回RT_EOK，超时返回-RT_TIMEOUT，其他返回-RT_ERROR。

### 发送事件 rt_event_send() ###

* 函数原型:

```
rt_err_t rt_event_send(rt_event_t event, rt_uint32_t set);
```
使用该函数接口时，通过参数set指定的事件标志来设定event对象的事件标志值，然后
遍历等待在event事件对象上的等待线程链表，判断是否有线程的事件激活要求与当前event
对象事件标志值匹配，如果有，则唤醒该线程。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| event| 事件对象句柄.|
|set|发送的事件集|

* 函数返回：

RT_EOK