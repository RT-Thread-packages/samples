# 空闲任务钩子的使用 #

## 例程目的 ##

学会设置空闲任务钩子。

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
    #include <rtthread.h>
#include <rthw.h>

#define CPU_USAGE_CALC_TICK    10
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

static void cpu_usage_idle_hook()
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;

    if (total_count == 0)
    {
        /* get total count */
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
    /* get CPU usage */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count ++;
        loop  = 0;
        while (loop < CPU_USAGE_LOOP) loop ++;
    }

    /* calculate major and minor */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;

        /* no CPU usage */
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

long cpu_usage(void)
{
    rt_uint8_t major, minor;
	
	cpu_usage_get(&major, &minor);
	rt_kprintf("cpu usage: %d.%d\n", major, minor);

    return 0;
}
FINSH_FUNCTION_EXPORT(cpu_usage, show cpu usage);
MSH_CMD_EXPORT(cpu_usage, show cpu usage);

int cpu_usage_init()
{
    /* set idle thread hook */
    rt_thread_idle_sethook(cpu_usage_idle_hook);
	return 0;
}
INIT_APP_EXPORT(cpu_usage_init);

```

### 例程设计 ###

该例程在 `cpu_usage_init()` 中通过调用```rt_thread_idle_sethook()```设置了一个空闲任务钩子函数```cpu_usage_idle_hook()```。   
```cpu_usage_idle_hook()```函数统计CPU的使用率，当在控制台输入命令```cpu_usage()```时通过```cpu_usage()```函数将CPU使用率以百分比形式打印出来。
 


### 编译调试及观察输出信息 ###

仿真运行后，输入键盘```Tab```键，得到下面输出，可以看到新增命令```cpu_usage```:

	\ | /
	- RT -     Thread Operating System
	 / | \     3.0.3 build Apr 21 2018
	 2006 - 2018 Copyright by rt-thread team
	finsh >
	--function:
	cpu_usage        -- show cpu usage
	pinMode          -- set hardware pin mode
	pinWrite         -- write value to hardware pin
	pinRead          -- read status from hardware pin
	hello            -- say hello world
	version          -- show RT-Thread version information
	list_thread      -- list thread
	list_sem         -- list semaphone in system
	list_event       -- list event in system
	list_mutex       -- list mutex in system
	list_mailbox     -- list mail box in system
	list_msgqueue    -- list message queue in system
	list_memheap     -- list memory heap in system
	list_mempool     -- list memory pool in system
	list_timer       -- list timer in system
	list_device      -- list device in system
	list             -- list all symbol in system
	msh              -- use module shell
	--variable:
	dummy            -- dummy variable for finsh
	finsh >
	
输入```cpu_usage()```命令，可以获取CPU使用率，输出如下：

	finsh >cpu_usage() 
	cpu usage: 0.0 
	        0, 0x00000000 
	finsh > 
	finsh >cpu_usage()
	cpu usage: 0.2  
	        0, 0x00000000




## 本文相关核心API ##

### 初始化内存池 rt_mp_init() ###

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
