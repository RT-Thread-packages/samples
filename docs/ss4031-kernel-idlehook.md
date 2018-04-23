# 空闲线程钩子的使用 #

## 例程目的 ##

学会设置空闲线程钩子。

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
/*
 * 程序清单：空闲任务钩子例程
 *
 * 这个程序设置了一个空闲函数钩子用于计算CPU使用率，并创建一个线程循环打印CPU使用率
 * 通过修改CPU使用率打印线程中的休眠tick时间可以看到不同的CPU使用率
 */
#include <rtthread.h>
#include <rthw.h>

#if RT_THREAD_PRIORITY_MAX == 8
#define THREAD_PRIORITY        6
#elif RT_THREAD_PRIORITY_MAX == 32
#define THREAD_PRIORITY        25
#elif RT_THREAD_PRIORITY_MAX == 256
#define THREAD_PRIORITY        200
#endif
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE    5

/* 指向线程控制块的指针 */
static rt_thread_t tid = RT_NULL;

#define CPU_USAGE_CALC_TICK    10
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;

/* 记录CPU使用率为0时的总count数 */
static rt_uint32_t total_count = 0;		

/* 空闲任务钩子函数 */
static void cpu_usage_idle_hook()
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;

    if (total_count == 0)
    {
        /* 获取 total_count */
        rt_enter_critical();
        tick = rt_tick_get();
        while(rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
        {
            total_count ++;
            loop = 0;
            while (loop < CPU_USAGE_LOOP) loop ++;
        }
        rt_exit_critical();
    }

    count = 0;
    /* 计算CPU使用率 */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count ++;
        loop  = 0;
        while (loop < CPU_USAGE_LOOP) loop ++;
    }

    /* 计算整数百分比整数部分和小数部分 */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;

        /* CPU使用率为0 */
        cpu_usage_major = 0;
        cpu_usage_minor = 0;
    }
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);

    *major = cpu_usage_major;
    *minor = cpu_usage_minor;
}

/* CPU使用率打印线程入口 */
static void thread_entry(void *parameter)
{
    rt_uint8_t major, minor;

    while(1)
    {
        cpu_usage_get(&major, &minor);
        rt_kprintf("cpu usage: %d.%d%\n", major, minor);

        /* 休眠50个OS Tick */
        /* 手动修改此处休眠 tick 时间，可以模拟实现不同的CPU使用率 */
        rt_thread_delay(50);
    }
}

int cpu_usage_init()
{
    /* 设置空闲线程钩子 */
    rt_thread_idle_sethook(cpu_usage_idle_hook);
	
    /* 创建线程 */
    tid = rt_thread_create("thread",
                            thread_entry, RT_NULL, /* 线程入口是thread_entry, 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(cpu_usage_init);


```

### 例程设计 ###

该例程在 `cpu_usage_init()` 中通过调用```rt_thread_idle_sethook()```设置了一个空闲任务钩子函数```cpu_usage_idle_hook()```用来计算CPU的使用率。
同时创建了一个线程```thread```来循环输出打印CPU使用率，可通过设置```thread```线程中的休眠```tick```时间来实现模拟不同的CPU使用率。


### 编译调试及观察输出信息 ###

仿真运行后，控制台一直循环输出打印CPU使用率:

		\ | /
	- RT -     Thread Operating System
	 / | \     3.0.3 build Apr 21 2018
	 2006 - 2018 Copyright by rt-thread team
	finsh >cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.2%
	cpu usage: 0.0%
	cpu usage: 0.2%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.2%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.2%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.0%
	cpu usage: 0.2%
	cpu usage: 0.2%
	cpu usage: 0.0%
	...
	

## 本文相关核心API ##

### 设置空闲线程钩子 rt_thread_idle_sethook() ###

* 函数原型:

```{.c}
    void rt_thread_idle_sethook(void (*hook)(void));
```

设置空闲线程运行时执行的钩子函数。   
当空闲线程运行时会自动执行设置的钩子函数，由于空闲线程具有系统的最低优先级，所以只有在空闲的时候才会执行此钩子函数。空闲线程是一个线程状态永远为就绪态的线程，因此设置的钩子函数必须保证空闲线程在任何时刻都不会处于挂起状态，例如```rt_thread_delay()``` ， ```rt_sem_take()``` 等可能会导致线程挂起的函数都不能使用。   

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|hook|设置的钩子函数；|

* 函数返回：

无
