# 内存池的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用内存池。

## 程序清单 ##

```c
/*
 * 程序清单：内存池例程
 *
 * 这个程序会创建一个静态的内存池对象，2个动态线程。两个线程会试图分别从内存池中获得
 * 内存块
 */
#include <rtthread.h>

static rt_uint8_t *ptr[48];
static rt_uint8_t mempool[4096];
static struct rt_mempool mp;

#define THREAD_PRIORITY      25
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE     5

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

/* 线程1入口 */
static void thread1_entry(void *parameter)
{
    int i, count = 0;
    char *block;

    while (1)
    {
        for (i = 0; i < 48; i++)
        {
            /* 申请内存块 */
            if (ptr[i] == RT_NULL)
            {
                ptr[i] = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
                rt_kprintf("allocate No.%d\n", i);
            }
        }

        /* 继续申请一个内存块，因为已经没有内存块，线程应该被挂起 */
        block = rt_mp_alloc(&mp, RT_WAITING_FOREVER);
        rt_kprintf("allocate the block mem\n");
        /* 释放这个内存块 */
        rt_mp_free(block);
        block = RT_NULL;

        /* 休眠10个OS Tick */
        rt_thread_delay(10);

        /* 循环10次后，退出线程1 */
        if (count++ >= 10) break;
    }
}

/* 线程2入口，线程2的优先级比线程1低，应该线程1先获得执行。*/
static void thread2_entry(void *parameter)
{
    int i, count = 0;

    while (1)
    {
        rt_kprintf("try to release block\n");

        for (i = 0 ; i < 48; i ++)
        {
            /* 释放所有分配成功的内存块 */
            if (ptr[i] != RT_NULL)
            {
                rt_kprintf("release block %d\n", i);

                rt_mp_free(ptr[i]);
                ptr[i] = RT_NULL;
            }
        }

        /* 休眠10个OS Tick */
        rt_thread_delay(10);

        /* 循环10次后，退出线程2 */
        if (count++ >= 10) break;
    }
}

int mempool_sample_init()
{
    int i;
    for (i = 0; i < 48; i ++) ptr[i] = RT_NULL;

    /* 初始化内存池对象 */
    rt_mp_init(&mp, "mp1", &mempool[0], sizeof(mempool), 80);

    /* 创建线程1 */
    tid1 = rt_thread_create("t1",
                            thread1_entry, RT_NULL, /* 线程入口是thread1_entry, 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    /* 创建线程2 */
    tid2 = rt_thread_create("t2",
                            thread2_entry, RT_NULL, /* 线程入口是thread2_entry, 入口参数是RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
    INIT_APP_EXPORT(mempool_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(mempool_sample_init, mempool sample);
```

## 运行结果 ##

```
allocate No.0
allocate No.1
allocate No.2
allocate No.3
allocate No.4
allocate No.5
allocate No.6
allocate No.7
allocate No.8
allocate No.9
allocate No.10
allocate No.11
allocate No.12
allocate No.13
allocate No.14
allocate No.15
allocate No.16
allocate No.17
allocate No.18
allocate No.19
allocate No.20
allocate No.21
allocate No.22
allocate No.23
allocate No.24
allocate No.25
allocate No.26
allocate No.27
allocate No.28
allocate No.29
allocate No.30
allocate No.31
allocate No.32
allocate No.33
allocate No.34
allocate No.35
allocate No.36
allocate No.37
allocate No.38
allocate No.39
allocate No.40
allocate No.41
allocate No.42
allocate No.43
allocate No.44
allocate No.45
allocate No.46
allocate No.47
try to release block
release block 0
allocate the block mem
release block 1
release block 2
release block 3
release block 4
release block 5
release block 6
release block 7
release block 8
release block 9
release block 10
release block 11
release block 12
release block 13
release block 14
release block 15
release block 16
release block 17
release block 18
release block 19
release block 20
release block 21
release block 22
release block 23
release block 24
release block 25
release block 26
release block 27
release block 28
release block 29
release block 30
release block 31
release block 32
release block 33
release block 34
release block 35
release block 36
release block 37
release block 38
release block 39
release block 40
release block 41
release block 42
release block 43
release block 44
release block 45
release block 46
release block 47
```