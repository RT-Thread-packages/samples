# 互斥量的使用 #

## 例程目的 ##

学会 互斥量的创建、初始化; 获取互斥量; 释放互斥量; 删除互斥量、脱离互斥量.

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c}
    /*
     * 程序清单：互斥量的使用
     */

    #include <rtthread.h>

    /* 互斥量控制块 */
    static struct rt_mutex static_mutex;
    /* 指向互斥量的指针 */
    static rt_mutex_t dynamic_mutex = RT_NULL;

    ALIGN(RT_ALIGN_SIZE)
    static char thread1_stack[1024];
    struct rt_thread thread1;
    static void rt_thread_entry1(void *parameter)
    {
        rt_err_t result;
        rt_tick_t tick;

    /* 1. staic mutex demo */

    /* 试图持有互斥量，最大等待10个OS Tick后返回 */
    rt_kprintf("thread1 try to get static mutex, wait 10 ticks.\n");

    /* 获得当前的OS Tick */
    tick = rt_tick_get();
    result = rt_mutex_take(&static_mutex, 10);

    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_mutex_detach(&static_mutex);
            return;
        }
        rt_kprintf("thread1 take static mutex timeout\n");
    }
    else
    {
        /* 线程2持有互斥量，且在相当长的时间后才会释放互斥量，
         * 因此10个tick后线程1不可能获得 */
        rt_kprintf("thread1 take a static mutex, failed.\n");
        rt_mutex_detach(&static_mutex);
        return;
    }

    /* 永久等待方式持有互斥量 */
    rt_kprintf("thread1 try to get static mutex, wait forever.\n");
    result = rt_mutex_take(&static_mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("thread1 take a static mutex, failed.\n");
        rt_mutex_detach(&static_mutex);
        return;
    }

    rt_kprintf("thread1 take a staic mutex, done.\n");

    /* 脱离互斥量对象 */
    rt_mutex_detach(&static_mutex);

    /* 2. dynamic mutex test */

    /* 试图持有互斥量，最大等待10个OS Tick后返回 */
    rt_kprintf("thread1 try to get dynamic mutex, wait 10 ticks.\n");

    tick = rt_tick_get();
    result = rt_mutex_take(dynamic_mutex, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_mutex_delete(dynamic_mutex);
            return;
        }
        rt_kprintf("thread1 take dynamic mutex timeout\n");
    }
    else
    {
        /* 线程2持有互斥量，且在相当长的时间后才会释放互斥量，
         * 因此10个tick后线程1不可能获得 */
        rt_kprintf("thread1 take a dynamic mutex, failed.\n");
        rt_mutex_delete(dynamic_mutex);
        return;
    }

    /* 永久等待方式持有互斥量 */
    rt_kprintf("thread1 try to get dynamic mutex, wait forever.\n");
    result = rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("thread1 take a dynamic mutex, failed.\n");
        rt_mutex_delete(dynamic_mutex);
        return;
    }

    rt_kprintf("thread1 take a dynamic mutex, done.\n");
    /* 删除互斥量对象 */
    rt_mutex_delete(dynamic_mutex);
}


ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;
static void rt_thread_entry2(void *parameter)
{
    /* 1. static mutex test */
    rt_kprintf("thread2 try to get static mutex\n");
    rt_mutex_take(&static_mutex, 10);
    rt_kprintf("thread2 got static mutex\n");
    rt_thread_delay(RT_TICK_PER_SECOND);
    rt_kprintf("thread2 release static mutex\n");
    rt_mutex_release(&static_mutex);

    /* 2. dynamic mutex test */
    rt_kprintf("thread2 try to get dynamic mutex\n");
    rt_mutex_take(dynamic_mutex, 10);
    rt_kprintf("thread2 got dynamic mutex\n");
    rt_thread_delay(RT_TICK_PER_SECOND);
    rt_kprintf("thread2 release dynamic mutex\n");
    rt_mutex_release(dynamic_mutex);
}

/* 互斥量示例的初始化 */
int mutex_sample_init()
{
    rt_err_t result;

    /* 初始化静态互斥量 */
    result = rt_mutex_init(&static_mutex, "smutex", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init static mutex failed.\n");
        return -1;
    }

    /* 创建一个动态互斥量 */
    dynamic_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (dynamic_mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   rt_thread_entry1,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 11, 5);
    rt_thread_startup(&thread1);


    rt_thread_init(&thread2,
                   "thread2",
                   rt_thread_entry2,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack), 10, 5);
    rt_thread_startup(&thread2);
    return 0;
}
INIT_APP_EXPORT(mutex_sample_init);

```

### 例程设计 ###

该例程在 mutex_sample_init中首先初始化静态互斥量,然后创建一个动态互斥量.最后创建启动线程thread1和thread2.

在线程thread1中进行如下操作:
1. 试图持有静态互斥量，最大等待10个OS Tick后返回.
2. 试图持有静态互斥量，永久等待.
3. 成功持有静态互斥量之后,使用rt_mutex_detach()脱离这个静态互斥量.
4. 试图持有动态互斥量，最大等待10个OS Tick后返回.
5. 试图持有动态互斥量,永久等待.
6. 成功持有静态互斥量之后,使用rt_mutex_delete()删除这个动态互斥量.

在线程thread2中进行如下操作:
1. 获取静态互斥量,最大等待10个OS Tick后返回.
2. 延时一段时间.
3. 释放静态互斥量.
4. 获取动态互斥量,最大等待10个OS Tick后返回.
5. 延时一段时间.
6. 释放动态互斥量.

两个线程互相作用,需要结合起来参考输出信息一起看.该例程主要演示互斥量 初始化、创建、获取、释放、脱离、删除这几个步骤的函数调用.

### 编译调试及观察输出信息 ###

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread2 try to get static mutex
thread2 got static mutex
thread1 try to get static mutex, wait 10 ticks.
finsh >thread1 take static mutex timeout
thread1 try to get static mutex, wait forever.
thread2 release static mutex
thread2 try to get dynamic mutex
thread2 got dynamic mutex
thread1 take a staic mutex, done.
thread1 try to get dynamic mutex, wait 10 ticks.
thread1 take dynamic mutex timeout
thread1 try to get dynamic mutex, wait forever.
thread2 release dynamic mutex
thread1 take a dynamic mutex, done.
```

由输出信息可知,thread2率先获取了静态互斥量,thread1这个时候想要获取静态互斥量,等待10个tick期间没有成功获取,所以返回.再次获取静态互斥量,这次是永久等待,直到thread2释放静态互斥量.(之后的动态互斥量的操作一样,不在赘述.)

## 本文相关核心API ##

### 初始化互斥量 rt_mutex_init() ###

* 函数原型:

```{.c}
rt_err_t rt_mutex_init (rt_mutex_t mutex, const char* name, rt_uint8_t flag);
```

对一个静态的互斥量进行初始化.

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|mutex|互斥量对象的句柄，它由用户提供，并指向互斥量对象的内存块；|
|name  | 互斥量名称；|
|flag| 互斥量标志，可以取如下类型的数值：|

```{.c}
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```

* 函数返回：

创建成功返回RT_EOK；否则返回其它。

### 创建互斥量 rt_mutex_create() ###

* 函数原型:

```{.c}
rt_mutex_t rt_mutex_create (const char* name, rt_uint8_t flag);
```
创建一个动态的互斥量.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|name  | 互斥量名称；|
|flag|互斥量标志，取值可以使用如下类型：|

```{.c}
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```

* 函数返回：

创建成功返回指向互斥量的互斥量句柄；否则返回RT_NULL。

### 删除互斥量 rt_mutex_delete() ###

* 函数原型:

```{.c}
rt_err_t rt_mutex_delete (rt_mutex_t mutex);
```
可以删除动态互斥量,会释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| mutex   | 互斥量对象的句柄；|

* 函数返回：

返回RT_EOK。

### 脱离互斥量 rt_mutex_detach() ###

* 函数原型:

```{.c}
rt_err_t rt_mutex_detach (rt_mutex_t mutex);
```
互斥量对象从内核对象管理器中脱离掉,但是不释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| mutex   | 互斥量对象的句柄；|

* 函数返回：

返回RT_EOK。

### 获取互斥量 rt_mutex_take() ###

* 函数原型:

```{.c}
rt_err_t rt_mutex_take (rt_mutex_t mutex, rt_int32_t time);
```
如果互斥量没有被其他线程控制，那么申请该互斥量的线程将成功获得该互斥量。如果
互斥量已经被当前线程线程控制，则该互斥量的持有计数加1，当前线程也不会挂起等待。
如果互斥量已经被其他线程占有，则当前线程在该互斥量上挂起等待，直到其他线程释放它
或者等待时间超过指定的超时时间。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| mutex  | 互斥量对象的句柄；。|
|time|指定的等待时间,单位是操作系统时钟节拍(OS Tick)|

* 函数返回：

成功获得互斥量返回RT_EOK；超时返回-RT_ETIMEOUT；其他返回-RT_ERROR。

### 释放互斥量 rt_mutex_release() ###

* 函数原型:

```{.c}
rt_err_t rt_mutex_release(rt_mutex_t mutex);
```
使用该函数接口时，只有已经拥有互斥量控制权的线程才能释放它，每释放一次该互斥
量，它的持有计数就减1。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| mutex   | 互斥量对象句柄。|

* 函数返回：

RT_EOK
