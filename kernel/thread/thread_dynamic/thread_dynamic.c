/*
 * 程序清单：动态线程
 *
 * 这个程序会初始化2个动态线程：
 * 它们拥有共同的入口函数，相同的优先级
 * 但是它们的入口参数不相同
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* 线程入口 */
static void thread_entry(void* parameter)
{
    rt_uint32_t count = 0;
    rt_uint32_t no = (rt_uint32_t) parameter; /* 获得线程的入口参数 */

    while (1)
    {
        /* 打印线程计数值输出 */
        rt_kprintf("thread%d count: %d\n", no, count ++);

        /* 休眠10个OS Tick */
        rt_thread_delay(10);
    }
}

/* 动态线程线程示例的初始化 */
int thread_dynamic_init()
{
    /* 创建线程1 */
    tid1 = rt_thread_create("t1",
                            thread_entry, (void*)1, /* 线程入口是thread_entry, 入口参数是1 */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else
        return -1;

    /* 创建线程2 */
    tid2 = rt_thread_create("t2",
                            thread_entry, (void*)2, /* 线程入口是thread_entry, 入口参数是2 */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else
        return -1;

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_dynamic_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_dynamic_init, thread dynamic init);
