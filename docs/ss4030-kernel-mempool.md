# 内存池的使用 #

## 例程目的 ##

学会内存池对象的初始化,内存块的申请，释放。

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c}
    /*
     * 程序清单：内存池例程
     *
     * 这个程序会创建一个静态的内存池对象, 2个动态线程.两个线程会试图分别从内存池中获得
     * 内存块
     */
    #include <rtthread.h>
    
    static rt_uint8_t *ptr[48];
    static rt_uint8_t mempool[4096];
    static struct rt_mempool mp;
    
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
    static rt_thread_t tid1 = RT_NULL;
    static rt_thread_t tid2 = RT_NULL;
    
    /* 线程1入口 */
    static void thread1_entry(void* parameter)
    {
        int i;
        char *block;
    
        while(1)
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
        }
    }
    
    /* 线程2入口，线程2的优先级比线程1低，应该线程1先获得执行。*/
    static void thread2_entry(void *parameter)
    {
        int i;
    
        while(1)
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
    INIT_APP_EXPORT(mempool_sample_init);
```

### 例程设计 ###

该例程在 `mempool_sample_init()` 中首先初始化静态内存池对象,最后创建启动线程 tid1 和 tid2 ,且线程 tid1 的优先级高于线程 tid2。

* 在线程 tid1 中进行如下操作:

1. 调用 `rt_mp_alloc()`,在 for 循环中先申请48个内存块,即：将内存池中所有内存块申请完. 再申请一个block内存块时,由于内存池中已无空闲内存块,所以会导致线程挂起.
2. 线程 tid1 挂起后,线程 tid2 得到执行,会释放内存块,一旦线程 tid2 释放一个内存块，由于内存池中有可用空闲内存块,线程 tid1 会被唤醒并抢占线程 tid2 而得到执行。此时block 内存块申请成功,执行到 `rt_thread_delay()` 后线程 tid1 因为延时函数而被挂起.

* 在线程tid2中进行如下操作:

调用 `rt_mp_free()` 函数,释放线程 tid1 的 for 循环中成功分配的内存块.

两个线程互相作用,需要结合起来参考输出信息一起看.

### 编译调试及观察输出信息 ###

在 `memp.c` 中第53行 `rt_thread_delay(10)`; 处打断点，可以看到如下运行结果：

```{.c}
      \ | /   
    - RT -     Thread Operating System
     / | \     3.0.3 build Apr 11 2018
     2006 - 2018 Copyright by rt-thread team
    finsh >allocate No.0
    
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
```

当线程1申请完48个内存块时，再执行申请内存块操作，线程1会因为没有空闲内存块可申请而被挂起，这时线程2执行，释放一个内存块后，内存池有可用空闲内存块，线程2被线程1抢占，线程1得到执行，执行到 `rt_thread_delay(10)` 时，线程1再次挂起线程2执行...... 如此循环执行。
