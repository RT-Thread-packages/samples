# 唤醒线程 #

## 介绍 ##

这个例子中将创建两个动态线程(t1和t2)，
低优先级线程t1将挂起自身
高优先级线程t2将在一定时刻后唤醒低优先级线程。

## 程序清单 ##

```{.c}
/*
 * 程序清单：唤醒线程
 *
 * 这个例子中将创建两个动态线程(t1和t2)，
 * 低优先级线程t1将挂起自身
 * 高优先级线程t2将在一定时刻后唤醒低优先级线程。
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
    /* 低优先级线程1开始运行 */
    rt_kprintf("thread1 startup%d\n");

    /* 挂起自身 */
    rt_kprintf("suspend thread self\n");
    rt_thread_suspend(tid1);
    /* 主动执行线程调度 */
    rt_schedule();

    /* 当线程1被唤醒时 */
    rt_kprintf("thread1 resumed\n");
}

/* 线程2入口 */
static void thread2_entry(void* parameter)
{
    /* 延时10个OS Tick */
    rt_thread_delay(10);

    /* 唤醒线程1 */
    rt_thread_resume(tid1);

    /* 延时10个OS Tick */
    rt_thread_delay(10);

    /* 线程2自动退出 */
}

/* 唤醒线程示例的初始化 */
int thread_resume_init(void)
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1",
                            thread1_entry,      /* 线程入口是thread1_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("t2",
                            thread2_entry,      /* 线程入口是thread2_entry */
                            RT_NULL,            /* 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY - 1,
                            THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_resume_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_resume_init, thread resume);
```

## 运行结果 ##

```
thread1 startup0
suspend thread self
thread1 resumed
```
