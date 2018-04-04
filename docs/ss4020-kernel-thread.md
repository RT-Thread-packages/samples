# 线程的创建和删除 #

## 例程目的 ##

学会线程创建 rt_thread_create 和线程删除 rt_thread_delete 接口的使用，了解线程的抢占和睡眠机制。

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
/*
 * 程序清单：线程创建和删除
 *
 * 这个例子会创建两个线程，在一个线程中删除另外一个线程。
 */
#include <rtthread.h>

#if RT_THREAD_PRIORITY_MAX == 8
#define THREAD_PRIORITY        6
#elif RT_THREAD_PRIORITY_MAX == 32
#define THREAD_PRIORITY        25
#elif RT_THREAD_PRIORITY_MAX == 256
#define THREAD_PRIORITY        200
#endif
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE    5

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
        count ++;
    }
}
static void thread1_cleanup(struct rt_thread *tid)
{
    if (tid != tid1)
    {
        return ;
    }
    rt_kprintf("thread1 end\n");
    tid1 = RT_NULL;
}

/* 线程2的入口函数 */
static void thread2_entry(void* parameter)
{
    /* 线程2拥有较高的优先级，以抢占线程1而获得执行 */

    /* 线程2启动后先睡眠10个OS Tick */
    rt_thread_delay(RT_TICK_PER_SECOND);

    /*
     * 线程2唤醒后直接删除线程1，删除线程1后，线程1自动脱离就绪线程
     * 队列
     */
    rt_thread_delete(tid1);

    /*
     * 线程2继续休眠10个OS Tick然后退出，线程2休眠后应切换到idle线程
     * idle线程将执行真正的线程1控制块和线程栈的删除
     */
    rt_thread_delay(RT_TICK_PER_SECOND);
}

static void thread2_cleanup(struct rt_thread *tid)
{
    /*
     * 线程2运行结束后也将自动被删除(线程控制块和线程栈在idle线
     * 程中释放)
     */

    if (tid != tid2)
    {
        return ;
    }
    rt_kprintf("thread2 end\n");
    tid2 = RT_NULL;
}

/* 线程示例的初始化 */
int thread_sample_init()
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1", /* 线程1的名称是t1 */
        thread1_entry, RT_NULL,   /* 入口是thread1_entry，参数是RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */
    {
        tid1->cleanup = thread1_cleanup;
        rt_thread_startup(tid1);
    }

    /* 创建线程1 */
    tid2 = rt_thread_create("t2", /* 线程1的名称是t2 */
        thread2_entry, RT_NULL,   /* 入口是thread2_entry，参数是RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL) /* 如果获得线程控制块，启动这个线程 */
    {
        tid2->cleanup = thread2_cleanup;
        rt_thread_startup(tid2);
    }

    return 10 * RT_TICK_PER_SECOND;
}
INIT_APP_EXPORT(thread_sample_init);
```

### 例程设计 ###

该例程创建并运行了两个线程 t1 和 t2, t1 线程优先级低于 t2线程，t1 线程运行后一直打印计数值，t2 线程运行后先睡眠了10个 OS Tick，t2 线程被唤醒后删除了 t1 线程，继续睡眠 10 个 OS Tick，然后退出，被自动删除。

### 编译调试及观察输出信息 ###

```{.c .numberLines}
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Mar  8 2018
 2006 - 2018 Copyright by rt-thread team
counter: 0
counter: 1
counter: 2
counter: 3
counter: 4
counter: 5
counter: 6
```

## 本文所有相关核心API详解 ##

<!--
详细解析所有用到的核心API，理论上应该解释API列表里面所有出现的API，如果精力实在有限，就专注于核心API就好了。
-->

### 创建线程 rt_thread_create() ###

* 函数原型:

```{.c .numberLines}
rt_thread_t rt_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick)
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|name | 线程的名称；线程名称的最大长度由rtconfig.h中定义的RT_NAME_MAX宏指定，多余部分会被自动截掉。|
|entry  | 线程入口函数|
|parameter   | 线程入口函数参数；|
|stack_size   | 线程栈大小，单位是字节。在大多数系统中需要做栈空间地址对齐（例如ARM体系结构中需要向4字节地址对齐）。|
|priority   | 线程的优先级。优先级范围根据系统配置情况（rtconfig.h中的RT_THREAD_PRIORITY_MAX宏定义），如果支持的是256级优先级，那么范围是从0 ～ 255，数值越小优先级越高，0代表最高优先级。|
|tick   | 线程的时间片大小。时间片（tick）的单位是操作系统的时钟节拍。当系统中存在相同优先级线程时，这个参数指定线程一次调度能够运行的最大时间长度。这个时间片运行结束时，调度器自动选择下一个就绪态的同优先级线程进行运行。|

* 函数返回：

创建成功返回线程句柄；否则返回RT_NULL。

### 删除线程 rt_thread_delete() ###

* 函数原型:

```{.c .numberLines}
rt_err_t rt_thread_delete(rt_thread_t thread);
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| thread   | 要删除的线程句柄。|

* 函数返回：

返回RT_EOK。

### 线程睡眠 rt_thread_delay() ###

* 函数原型:

```{.c .numberLines}
rt_err_t rt_thread_delay(rt_tick_t tick);
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| tick     | 线程睡眠的时间。|

* 函数返回：

返回RT_EOK。

### 应用初始化 INIT_APP_EXPORT() ###

* 函数原型:

```{.c .numberLines}
INIT_APP_EXPORT()
```
