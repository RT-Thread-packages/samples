# 邮箱的使用 #

## 例程目的 ##

学会 邮箱的初始化; 接收邮件; 发送邮件; 脱离邮箱.

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c}
/*
 * 程序清单：邮箱例程
 *
 * 这个程序会创建2个动态线程，一个静态的邮箱对象，其中一个线程往邮箱中发送邮件，
 * 一个线程往邮箱中收取邮件。
 */
#include <rtthread.h>

/* 邮箱控制块 */
static struct rt_mailbox mb;
/* 用于放邮件的内存池 */
static char mb_pool[128];

static char mb_str1[] = "I'm a mail!";
static char mb_str2[] = "this is another mail!";
static char mb_str3[] = "over";

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;

/* 线程1入口 */
static void thread1_entry(void *parameter)
{
    char *str;

    while (1)
    {
        rt_kprintf("thread1: try to recv a mail\n");

        /* 从邮箱中收取邮件 */
        if (rt_mb_recv(&mb, (rt_uint32_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: get a mail from mailbox, the content:%s\n", str);
            if (str == mb_str3)
                break;

            /* 延时10个OS Tick */
            rt_thread_delay(10);
        }
    }
    /* 执行邮箱对象脱离 */
    rt_mb_detach(&mb);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;
/* 线程2入口 */
static void thread2_entry(void *parameter)
{
    rt_uint8_t count;

    count = 0;
    while (count < 10)
    {
        count ++;
        if (count & 0x1)
        {
            /* 发送mb_str1地址到邮箱中 */
            rt_mb_send(&mb, (rt_uint32_t)&mb_str1[0]);
        }
        else
        {
            /* 发送mb_str2地址到邮箱中 */
            rt_mb_send(&mb, (rt_uint32_t)&mb_str2[0]);
        }

        /* 延时20个OS Tick */
        rt_thread_delay(20);
    }

    /* 发送邮件告诉线程1，线程2已经运行结束 */
    rt_mb_send(&mb, (rt_uint32_t)&mb_str3[0]);
}

int mailbox_sample_init()
{
    rt_err_t result;

    /* 初始化一个mailbox */
    result = rt_mb_init(&mb,
                        "mbt",                      /* 名称是mbt */
                        &mb_pool[0],                /* 邮箱用到的内存池是mb_pool */
                        sizeof(mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占4字节 */
                        RT_IPC_FLAG_FIFO);          /* 采用FIFO方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 10, 5);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack), 10, 5);
    rt_thread_startup(&thread2);
    return 0;
}
INIT_APP_EXPORT(mailbox_sample_init);
/*@}*/

```

### 例程设计 ###
该例程在 mailbox_sample_init中首先初始化静态邮箱,最后创建启动线程thread1和thread2.

在线程thread1中进行如下操作:
1. 不断的从邮箱中收取邮件,邮件内容为一个地址指针.
2. 收取到邮件后,将地址指针指向的内容在调试窗口打印出来.
3. 判断邮件的内容,如果地址指针指向的内容是 "over" 就break跳出循环,调用 rt_mb_detach 函数脱离这个静态邮箱.

在线程thread2中进行如下操作:
1. 先发送十封邮件,奇数发送的邮件内容为 mb_str1 的首地址 ,偶数发送邮件内容为 mb_str2 的首地址.
2. 发送完十封邮件后,发送一次邮件内容为 mb_str3 的首地址的邮件,最后结束.

两个线程互相作用,thread1负责接收邮件,thread2负责发送邮件,需要结合起来参考输出信息一起看.该例程主要演示邮箱 初始化、接收、发送、脱离这几个步骤的函数调用. 

注意:邮箱中一封邮件的最大长度是4字节，所以邮箱能够用于不超过4字节的消息传递，当传送的消息长度大于这个数目时就不能再采用邮箱的方式。 最重要的是，在32位系统上4字节的内容恰好适合放置一个指针，所以邮箱也适合这种仅传递指针的情况.

### 编译调试及观察输出信息 ###

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:I'm a mail!
finsh >thread1: try to recv a mail
thread1: get a mail from mailbox, the content:this is another mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:I'm a mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:this is another mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:I'm a mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:this is another mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:I'm a mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:this is another mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:I'm a mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:this is another mail!
thread1: try to recv a mail
thread1: get a mail from mailbox, the content:over
```

邮箱中一封邮件的最大长度是4字节,本例程发送的邮件为32位的地址指针. 
