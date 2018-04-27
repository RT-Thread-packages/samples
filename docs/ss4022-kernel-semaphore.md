# 信号量的使用 #

## 例程目的 ##

学会 信号量的创建/初始化; 获取信号量; 释放信号量; 删除信号量/脱离信号量. 

## 程序结构及例程原理 ##

### 程序清单 ###

```
#include <stdio.h>
#include <rtthread.h>

/* 信号量控制块 */
static struct rt_semaphore static_sem;
/* 指向信号量的指针 */
static rt_sem_t dynamic_sem = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;
static void rt_thread_entry1(void *parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* 1. staic semaphore demo */
    /* 获得当前的OS Tick */
    tick = rt_tick_get();

    /* 试图持有信号量，最大等待10个OS Tick后返回 */
    result = rt_sem_take(&static_sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_detach(&static_sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* 因为没有其他地方释放信号量，所以不应该成功持有信号量，否则测试失败 */
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return;
    }

    /* 释放一次信号量 */
    rt_sem_release(&static_sem);

    /* 永久等待方式持有信号量 */
    result = rt_sem_take(&static_sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return;
    }

    rt_kprintf("take a staic semaphore, done.\n");

    /* 脱离信号量对象 */
    rt_sem_detach(&static_sem);

    /* 2. dynamic semaphore test */

    tick = rt_tick_get();

    /* 试图持有信号量，最大等待10个OS Tick后返回 */
    result = rt_sem_take(dynamic_sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        /* 超时后判断是否刚好是10个OS Tick */
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_delete(dynamic_sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* 因为没有其他地方释放信号量，所以不应该成功持有信号量，否则测试失败 */
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return;
    }

    /* 释放一次信号量 */
    rt_sem_release(dynamic_sem);

    /* 永久等待方式持有信号量 */
    result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* 不成功则测试失败 */
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return;
    }

    rt_kprintf("take a dynamic semaphore, done.\n");
    /* 删除信号量对象 */
    rt_sem_delete(dynamic_sem);
}
/* 信号量示例的初始化 */
int semphore_sample_init()
{
    rt_err_t result;

    /* 初始化静态信号量，初始值是0 */
    result = rt_sem_init(&static_sem, "ssem", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init dynamic semaphore failed.\n");
        return -1;
    }

    /* 创建一个动态信号量，初始值是0 */
    dynamic_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   rt_thread_entry1,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 11, 5);
    rt_thread_startup(&thread1);

    return 0;
}
INIT_APP_EXPORT(semphore_sample_init);

```

### 例程设计 ###

该例程在 thread_sample_init中首先初始化静态信号量,然后创建一个动态信号量.最后创建线程thread1.
在线程thread1中进行如下操作:
1. 试图获取静态信号量,如果获取不到就等待10个OS Tick后返回.
2. 通过调用 rt_sem_release 释放一次静态信号量.
3. 试图获取一次静态信号量.如果获取不到就永久等待.
4. 调用 rt_sem_detach 使得静态信号量对象从内核对象管理器中移除掉,但是不释放系统资源.
5. 试图获取动态信号量,如果获取不到就等待10个OS Tick后返回.
6. 通过调用 rt_sem_release 释放一次动态信号量.
7. 试图获取一次动态信号量.如果获取不到就永久等待.
8. 调用 rt_sem_delete 删除动态信号量释放系统资源.

事实上除了thread1线程以外没有第二个线程会去获取信号量,所以每次获取信号量都不需要等待其它线程释放就可以直接获取成功.该例程主要演示初始化\创建\获取\脱离\删除\这几个步骤的函数调用. 

### 编译调试及观察输出信息 ###

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
finsh >take semaphore timeout
take a staic semaphore, done.
take semaphore timeout
take a dynamic semaphore, done.
```

## 本文相关核心API ##

### 初始化信号量 rt_sem_init() ###

* 函数原型:

```
rt_err_t rt_sem_init (rt_sem_t sem, const char* name, rt_uint32_t value, rt_uint8_t flag);
```
对一个静态的信号量进行初始化.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|sem|信号量对象的句柄|
|name  | 信号量名称；|
|value | 信号量初始值；|
|flag|信号量标志，取值可以使用如下类型：|


```
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```


* 函数返回：

创建成功返回RT_EOK；否则返回其它。

### 创建信号量 rt_sem_create() ###

* 函数原型:

```
rt_sem_t rt_sem_create (const char* name, rt_uint32_t value, rt_uint8_t flag);
```
创建一个动态的信号量.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|name  | 信号量名称；|
|value | 信号量初始值；|
|flag|信号量标志，取值可以使用如下类型：|


```
#define RT_IPC_FLAG_FIFO 0x00 /* IPC参数采用FIFO方式*/
#define RT_IPC_FLAG_PRIO 0x01 /* IPC参数采用优先级方式*/
```


* 函数返回：

创建成功返回创建的信号量控制块指针；否则返回RT_NULL。

### 删除信号量 rt_sem_delete () ###

* 函数原型:

```
rt_err_t rt_sem_delete (rt_sem_t sem);
```
可以删除动态信号量,会释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| sem   | 要删除的信号量对象。|

* 函数返回：

返回RT_EOK。

### 脱离信号量 rt_sem_detach() ###

* 函数原型:

```
rt_err_t rt_sem_detach (rt_sem_t sem);
```
信号量对象从内核对象管理器中脱离掉,但是不释放系统资源.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| sem   | 要脱离的信号量对象。|

* 函数返回：

返回RT_EOK。

### 获取信号量 rt_sem_take() ###

* 函数原型:

```
rt_err_t rt_sem_take (rt_sem_t sem, rt_int32_t time);
```
线程通过获取信号量来获得信号量资源实例，当信号量值大于零时，线程将获得信号
量，并且相应的信号量值都会减1.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| sem   | 信号量对象句柄。|
|time|指定的等待时间,单位是操作系统时钟节拍(OS Tick)|

* 函数返回：

成功获得信号量返回RT_EOK；超时依然未获得信号量返回-RT_ETIMEOUT；其他错误返
回-RT_ERROR。

### 释放信号量 rt_sem_release() ###

* 函数原型:

```
rt_err_t rt_sem_release(rt_sem_t sem);
```
当线程完成资源的访问后，应尽快释放它持有的信号量，使得其他线程能获得该信号
量。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| sem   | 信号量对象句柄。|

* 函数返回：

成功获得信号量返回RT_EOK；超时依然未获得信号量返回-RT_ETIMEOUT；其他错误返
回-RT_ERROR。