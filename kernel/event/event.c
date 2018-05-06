/*
 * 程序清单：事件例程
 *
 * 这个程序会创建2个动态线程及初始化一个静态事件对象
 * 一个线程会以不同的方式接收事件；
 * 一个线程定时发送两个事件 (EVENT1) (EVENT2);
 */
#include <rtthread.h>

#define EVENT1 (1 << 3)
#define EVENT2 (1 << 5)

/* 事件控制块 */
static struct rt_event event;

/* 线程1入口函数 */
static void thread1_entry(void *param)
{
    rt_uint32_t e;
    rt_uint8_t i;

    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            /* “逻辑与”的方式接收事件,满足接收条件之后重置事件的相应标志位.*/
            if (rt_event_recv(&event, (EVENT1 | EVENT2),
                              RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                              RT_WAITING_FOREVER, &e) == RT_EOK)
            {
                rt_kprintf("thread1: AND recv event 0x%x\n", e);
            }
        }
        for (i = 0; i < 10; i++)
        {
            /* “逻辑或”的方式接收事件,满足接收条件之后重置事件的相应标志位.*/
            if (rt_event_recv(&event, (EVENT1 | EVENT2),
                              RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                              RT_WAITING_FOREVER, &e) == RT_EOK)
            {
                rt_kprintf("thread1: OR recv event 0x%x\n", e);
            }
        }
        while (1)
        {
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
}

/* 线程2入口函数 */
static void thread2_entry(void *param)
{
    rt_uint8_t i;
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            rt_kprintf("thread2: send event1\n");
            rt_event_send(&event, EVENT1);                     /* 发送事件(EVENT1)*/
            rt_thread_delay(rt_tick_from_millisecond(2000));
            rt_kprintf("thread2: send event2\n");
            rt_event_send(&event, EVENT2);                     /* 发送事件(EVENT2)*/
            rt_thread_delay(rt_tick_from_millisecond(2000));
        }
        rt_thread_delay(rt_tick_from_millisecond(500));
        rt_kprintf("thread2: detach event\n");
        rt_event_detach(&event);                               /* 脱离事件对象*/
        while (1)
        {
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
}

/* 事件示例的初始化 */
int event_simple_init()
{
    rt_thread_t tid = RT_NULL;
    /* 初始化事件对象 */
    rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);

    /* 创建线程1 */
    tid = rt_thread_create("thread1",
                           thread1_entry, RT_NULL, /* 线程入口是thread1_entry, 入口参数是RT_NULL */
                           1024, 5, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    /* 创建线程2 */
    tid = rt_thread_create("thread2",
                           thread2_entry, RT_NULL, /* 线程入口是thread2_entry, 入口参数是RT_NULL */
                           1024, 5, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(event_simple_init);
MSH_CMD_EXPORT(event_simple_init, event sample);