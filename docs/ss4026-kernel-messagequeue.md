# 消息队列的使用 #

## 例程目的 ##

学会 消息队列的创建、初始化; 接收消息; 发送一般消息以及紧急消息; 删除消息队列、脱离消息队列. 

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c}
/*
 * 程序清单：消息队列例程
 *
 * 这个程序会创建2个动态线程，一个线程会从消息队列中收取消息；一个线程会定时给消
 * 息队列发送 普通消息和紧急消息。
 */
#include <rtthread.h>

/* 消息队列控制块 */
static struct rt_messagequeue mq;
/* 消息队列中用到的放置消息的内存池 */
static rt_uint8_t msg_pool[2048];

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;
/* 线程1入口函数 */
static void thread1_entry(void *parameter)
{
    rt_uint8_t buf = 0;

    while (1)
    {
        /* 从消息队列中接收消息 */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%d\n", buf);
            if (buf == 49)
            {
                break;
            }
        }
        /* 延时100ms */
        rt_thread_delay(rt_tick_from_millisecond(100));
    }
    rt_kprintf("thread1: detach mq \n");
    rt_mq_detach(&mq);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;
/* 线程2入口 */
static void thread2_entry(void *parameter)
{
    int result;
    rt_uint8_t buf = 0;
    while (1)
    {
        if (buf == 25)
        {
            /* 发送紧急消息到消息队列中 */
            result = rt_mq_urgent(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_urgent ERR\n");
            }
            rt_kprintf("thread2: send urgent message - %d\n", buf);
        }
        else if (buf >= 50)/* 发送50次消息之后退出 */
        {
            rt_kprintf("message queue stop send, thread2 quit\n");
            break;
        }
        else
        {
            /* 发送消息到消息队列中 */
            result = rt_mq_send(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_send ERR\n");
            }

            rt_kprintf("thread2: send message - %d\n", buf);
        }
        buf++;
        /* 延时10ms */
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
}

/* 消息队列示例的初始化 */
int messagequeue_sample_init()
{
    rt_err_t result;

    /* 初始化消息队列 */
    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],               /* 内存池指向msg_pool */
                        1,                          /* 每个消息的大小是 1 字节 */
                        sizeof(msg_pool),           /* 内存池的大小是msg_pool的大小 */
                        RT_IPC_FLAG_FIFO);          /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack), 10, 50);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack), 10, 50);
    rt_thread_startup(&thread2);

    return 0;
}
INIT_APP_EXPORT(messagequeue_sample_init);

```

### 例程设计 ###

该例程在 messagequeue_sample_init中首先初始化消息队列,最后创建启动线程thread1和thread2.

在线程thread1中进行如下操作:
1. 每成功从消息队列中接收消息一次,就延时100ms.(注意:这100ms表示该线程忙于做其他的事情,以达到无法及时接收消息的效果.)
2. 成功接收消息50次,就退出.调用rt_mq_detach脱离这个消息队列,不释放资源.

在线程thread2中进行如下操作:
1. 循环发送消息50次,时间间隔只有10ms,thread2发送数据的速度远远大于thread1接收数据的速度,消息的内容为数字0~49.当消息内容为25的时候,以紧急发送rt_mq_urgent的方式发送.其他内容通过rt_mq_send的方式发送消息.(注意:紧急发送的方式发送消息,会将此条消息内容插到消息队列最前面.优先被接收处理.)

两个线程互相作用,需要结合起来参考输出信息一起看.该例程主要演示消息队列 初始化、接收、发送、脱离这几个步骤的函数调用. 

### 编译调试及观察输出信息 ###

```
 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 27 2018
 2006 - 2018 Copyright by rt-thread team
thread2: send message - 0
thread1: recv msg from msg queue, the content:0
finsh >thread2: send message - 1
thread2: send message - 2
thread2: send message - 3
thread2: send message - 4
thread2: send message - 5
thread2: send message - 6
thread2: send message - 7
thread2: send message - 8
thread2: send message - 9
thread1: recv msg from msg queue, the content:1
thread2: send message - 10
thread2: send message - 11
thread2: send message - 12
thread2: send message - 13
thread2: send message - 14
thread2: send message - 15
thread2: send message - 16
thread2: send message - 17
thread2: send message - 18
thread2: send message - 19
thread1: recv msg from msg queue, the content:2
thread2: send message - 20
thread2: send message - 21
thread2: send message - 22
thread2: send message - 23
thread2: send message - 24
thread2: send urgent message - 25
thread2: send message - 26
thread2: send message - 27
thread2: send message - 28
thread2: send message - 29
thread1: recv msg from msg queue, the content:25
thread2: send message - 30
thread2: send message - 31
thread2: send message - 32
thread2: send message - 33
thread2: send message - 34
thread2: send message - 35
thread2: send message - 36
thread2: send message - 37
thread2: send message - 38
thread2: send message - 39
thread1: recv msg from msg queue, the content:3
thread2: send message - 40
thread2: send message - 41
thread2: send message - 42
thread2: send message - 43
thread2: send message - 44
thread2: send message - 45
thread2: send message - 46
thread2: send message - 47
thread2: send message - 48
thread2: send message - 49
thread1: recv msg from msg queue, the content:4
message queue stop send, thread2 quit
thread1: recv msg from msg queue, the content:5
thread1: recv msg from msg queue, the content:6
thread1: recv msg from msg queue, the content:7
thread1: recv msg from msg queue, the content:8
thread1: recv msg from msg queue, the content:9
thread1: recv msg from msg queue, the content:10
thread1: recv msg from msg queue, the content:11
thread1: recv msg from msg queue, the content:12
thread1: recv msg from msg queue, the content:13
thread1: recv msg from msg queue, the content:14
thread1: recv msg from msg queue, the content:15
thread1: recv msg from msg queue, the content:16
thread1: recv msg from msg queue, the content:17
thread1: recv msg from msg queue, the content:18
thread1: recv msg from msg queue, the content:19
thread1: recv msg from msg queue, the content:20
thread1: recv msg from msg queue, the content:21
thread1: recv msg from msg queue, the content:22
thread1: recv msg from msg queue, the content:23
thread1: recv msg from msg queue, the content:24
thread1: recv msg from msg queue, the content:26
thread1: recv msg from msg queue, the content:27
thread1: recv msg from msg queue, the content:28
thread1: recv msg from msg queue, the content:29
thread1: recv msg from msg queue, the content:30
thread1: recv msg from msg queue, the content:31
thread1: recv msg from msg queue, the content:32
thread1: recv msg from msg queue, the content:33
thread1: recv msg from msg queue, the content:34
thread1: recv msg from msg queue, the content:35
thread1: recv msg from msg queue, the content:36
thread1: recv msg from msg queue, the content:37
thread1: recv msg from msg queue, the content:38
thread1: recv msg from msg queue, the content:39
thread1: recv msg from msg queue, the content:40
thread1: recv msg from msg queue, the content:41
thread1: recv msg from msg queue, the content:42
thread1: recv msg from msg queue, the content:43
thread1: recv msg from msg queue, the content:44
thread1: recv msg from msg queue, the content:45
thread1: recv msg from msg queue, the content:46
thread1: recv msg from msg queue, the content:47
thread1: recv msg from msg queue, the content:48
thread1: recv msg from msg queue, the content:49
thread1: detach mq
```

由输出信息可知,在前几次打印输出中,基本上thread2每发送完10次消息,thread1才会去接受一次消息.当thread2发送完50次消息之后退出.thread1仍旧在读取消息队列的数据.(所以消息队列比较适合数据较多,但是处理不过来的情况.)另外thread2在发送消息内容为25的时候,是以紧急发送的方式发送的.所以在thread2发送完25这个内容之后,thread1的读取到的内容是25,而不是其他.
