# 消息队列的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用消息队列。

## 程序清单 ##

```c
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
static struct rt_thread thread1;
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
static struct rt_thread thread2;
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
                   sizeof(thread1_stack), 25, 5);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack), 25, 5);
    rt_thread_startup(&thread2);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
    INIT_APP_EXPORT(messagequeue_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(messagequeue_sample_init, messagequeue sample);
```

## 运行结果 ##

```
thread2: send message - 0
thread1: recv msg from msg queue, the content:0
thread2: send message - 1
thread2: send message - 2
thread2: send message - 3
thread2: send message - 4
thread2: send message - 5
thread2: send message - 6
thread2: send message - 7
thread2: send message - 8
thread2: send message - 9
thread2: send message - 10
thread1: recv msg from msg queue, the content:1
thread2: send message - 11
thread2: send message - 12
thread2: send message - 13
thread2: send message - 14
thread2: send message - 15
thread2: send message - 16
thread2: send message - 17
thread2: send message - 18
thread2: send message - 19
thread2: send message - 20
thread1: recv msg from msg queue, the content:2
thread2: send message - 21
thread2: send message - 22
thread2: send message - 23
thread2: send message - 24
thread2: send urgent message - 25
thread2: send message - 26
thread2: send message - 27
thread2: send message - 28
thread2: send message - 29
thread2: send message - 30
thread1: recv msg from msg queue, the content:25
thread2: send message - 31
thread2: send message - 32
thread2: send message - 33
thread2: send message - 34
thread2: send message - 35
thread2: send message - 36
thread2: send message - 37
thread2: send message - 38
thread2: send message - 39
thread2: send message - 40
thread1: recv msg from msg queue, the content:3
thread2: send message - 41
thread2: send message - 42
thread2: send message - 43
thread2: send message - 44
thread2: send message - 45
thread2: send message - 46
thread2: send message - 47
thread2: send message - 48
thread2: send message - 49
message queue stop send, thread2 quit
thread1: recv msg from msg queue, the content:4
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


