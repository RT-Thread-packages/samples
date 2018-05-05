# 生产者消费者问题 #

## 例程目的 ##

在RT-Thread中实现生产者消费者问题，学会初始化信号量，获取和释放信号量。

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
/*
 * 程序清单：生产者消费者例子
 *
 * 这个例子中将创建两个线程用于实现生产者消费者问题
 */
#include <rtthread.h>

#if RT_THREAD_PRIORITY_MAX == 8
#define THREAD_PRIORITY        6
#elif RT_THREAD_PRIORITY_MAX == 32
#define THREAD_PRIORITY        25
#elif RT_THREAD_PRIORITY_MAX == 256
#define THREAD_PRIORITY        200
#endif
#define THREAD_STACK_SIZE    512
#define THREAD_TIMESLICE    5

/* 定义最大5个元素能够被产生 */
#define MAXSEM 5

/* 用于放置生产的整数数组 */
rt_uint32_t array[MAXSEM];
/* 指向生产者、消费者在array数组中的读写位置 */
static rt_uint32_t set, get;

/* 指向线程控制块的指针 */
static rt_thread_t producer_tid = RT_NULL;
static rt_thread_t consumer_tid = RT_NULL;

struct rt_semaphore sem_lock;
struct rt_semaphore sem_empty, sem_full;

/* 生成者线程入口 */
void producer_thread_entry(void* parameter)
{
    int cnt = 0;

    /* 运行100次 */
    while( cnt < 100)
    {
        /* 获取一个空位 */
        rt_sem_take(&sem_empty, RT_WAITING_FOREVER);

        /* 修改array内容，上锁 */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        array[set%MAXSEM] = cnt + 1;
        rt_kprintf("the producer generates a number: %d\n", array[set%MAXSEM]);
        set++;
        rt_sem_release(&sem_lock);

        /* 发布一个满位 */
        rt_sem_release(&sem_full);
        cnt++;

        /* 暂停一段时间 */
        rt_thread_delay(50);
    }

    rt_kprintf("the producer exit!\n");
}

/* 消费者线程入口 */
void consumer_thread_entry(void* parameter)
{
    rt_uint32_t no;
    rt_uint32_t sum;

    /* 第n个线程，由入口参数传进来 */
    no = (rt_uint32_t)parameter;

    sum = 0;
    while(1)
    {
        /* 获取一个满位 */
        rt_sem_take(&sem_full, RT_WAITING_FOREVER);

        /* 临界区，上锁进行操作 */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        sum += array[get%MAXSEM];
        rt_kprintf("the consumer[%d] get a number: %d\n", (get%MAXSEM), array[get%MAXSEM] );
        get++;
        rt_sem_release(&sem_lock);

        /* 释放一个空位 */
        rt_sem_release(&sem_empty);

        /* 生产者生产到100个数目，停止，消费者线程相应停止 */
        if (get == 100) break;

        /* 暂停一小会时间 */
        rt_thread_delay(10);
    }

    rt_kprintf("the consumer[%d] sum is %d \n ", no, sum);
    rt_kprintf("the consumer[%d] exit!\n");
}

int semaphore_producer_consumer_init()
{
    /* 初始化3个信号量 */
    rt_sem_init(&sem_lock , "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_empty, "empty",    MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_full , "full",     0,      RT_IPC_FLAG_FIFO);

    /* 创建线程1 */
    producer_tid = rt_thread_create("producer",
                                    producer_thread_entry, RT_NULL, /* 线程入口是producer_thread_entry, 入口参数是RT_NULL */
                                    THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (producer_tid != RT_NULL)
        rt_thread_startup(producer_tid);

    /* 创建线程2 */
    consumer_tid = rt_thread_create("consumer",
                                    consumer_thread_entry, RT_NULL, /* 线程入口是consumer_thread_entry, 入口参数是RT_NULL */
                                    THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (consumer_tid != RT_NULL)
        rt_thread_startup(consumer_tid);

    return 0;
}
INIT_APP_EXPORT(semaphore_producer_consumer_init);

```

### 例程设计 ###

本例程会创建了两个线程，一个作为生产者，一个作为消费者。   
1、生产者线程对array数组赋值，cnt累加1并依次放入array数组的成员内，每执行一次，完成一次赋值动作后线程调用```rt_thread_delay()```函数挂起，消费者线程得到执行，执行100次后，退出循环。   
2、消费者线程将生产者线程中的赋值的数据打印出来，并且进行累加计算，累计100次后退出循环。

### 编译调试及观察输出信息 ###

	\ | /
	- RT -     Thread Operating System
	 / | \     3.0.3 build Apr 23 2018
	 2006 - 2018 Copyright by rt-thread t
	finsh >the producer generates a numbe
	the consumer[0] get a number: 1
	the producer generates a number: 2
	the consumer[1] get a number: 2
	the producer generates a number: 3
	the consumer[2] get a number: 3
	the producer generates a number: 4
	the consumer[3] get a number: 4
	the producer generates a number: 5
	the consumer[4] get a number: 5
	the producer generates a number: 6
	the consumer[0] get a number: 6
	the producer generates a number: 7
	the consumer[1] get a number: 7
	the producer generates a number: 8
	the consumer[2] get a number: 8
	the producer generates a number: 9
	the consumer[3] get a number: 9
	the producer generates a number: 10
	the consumer[4] get a number: 10
	the producer generates a number: 11
	the consumer[0] get a number: 11
	the producer generates a number: 12
	the consumer[1] get a number: 12
	the producer generates a number: 13
	the consumer[2] get a number: 13
	the producer generates a number: 14
	the consumer[3] get a number: 14
	the producer generates a number: 15
	the consumer[4] get a number: 15
	the producer generates a number: 16
	the consumer[0] get a number: 16
	the producer generates a number: 17
	the consumer[1] get a number: 17
	the producer generates a number: 18
	the consumer[2] get a number: 18
	the producer generates a number: 19
	the consumer[3] get a number: 19
	the producer generates a number: 20
	the consumer[4] get a number: 20
	the producer generates a number: 21
	the consumer[0] get a number: 21
	the producer generates a number: 22
	the consumer[1] get a number: 22
	the producer generates a number: 23
	the consumer[2] get a number: 23
	the producer generates a number: 24
	the consumer[3] get a number: 24
	the producer generates a number: 25
	the consumer[4] get a number: 25
	the producer generates a number: 26
	the consumer[0] get a number: 26
	the producer generates a number: 27
	the consumer[1] get a number: 27
	the producer generates a number: 28
	the consumer[2] get a number: 28
	the producer generates a number: 29
	the consumer[3] get a number: 29
	the producer generates a number: 30
	the consumer[4] get a number: 30
	the producer generates a number: 31
	the consumer[0] get a number: 31
	the producer generates a number: 32
	the consumer[1] get a number: 32
	the producer generates a number: 33
	the consumer[2] get a number: 33
	the producer generates a number: 34
	the consumer[3] get a number: 34
	the producer generates a number: 35
	the consumer[4] get a number: 35
	the producer generates a number: 36
	the consumer[0] get a number: 36
	the producer generates a number: 37
	the consumer[1] get a number: 37
	the producer generates a number: 38
	the consumer[2] get a number: 38
	the producer generates a number: 39
	the consumer[3] get a number: 39
	the producer generates a number: 40
	the consumer[4] get a number: 40
	the producer generates a number: 41
	the consumer[0] get a number: 41
	the producer generates a number: 42
	the consumer[1] get a number: 42
	the producer generates a number: 43
	the consumer[2] get a number: 43
	the producer generates a number: 44
	the consumer[3] get a number: 44
	the producer generates a number: 45
	the consumer[4] get a number: 45
	the producer generates a number: 46
	the consumer[0] get a number: 46
	the producer generates a number: 47
	the consumer[1] get a number: 47
	the producer generates a number: 48
	the consumer[2] get a number: 48
	the producer generates a number: 49
	the consumer[3] get a number: 49
	the producer generates a number: 50
	the consumer[4] get a number: 50
	the producer generates a number: 51
	the consumer[0] get a number: 51
	the producer generates a number: 52
	the consumer[1] get a number: 52
	the producer generates a number: 53
	the consumer[2] get a number: 53
	the producer generates a number: 54
	the consumer[3] get a number: 54
	the producer generates a number: 55
	the consumer[4] get a number: 55
	the producer generates a number: 56
	the consumer[0] get a number: 56
	the producer generates a number: 57
	the consumer[1] get a number: 57
	the producer generates a number: 58
	the consumer[2] get a number: 58
	the producer generates a number: 59
	the consumer[3] get a number: 59
	the producer generates a number: 60
	the consumer[4] get a number: 60
	the producer generates a number: 61
	the consumer[0] get a number: 61
	the producer generates a number: 62
	the consumer[1] get a number: 62
	the producer generates a number: 63
	the consumer[2] get a number: 63
	the producer generates a number: 64
	the consumer[3] get a number: 64
	the producer generates a number: 65
	the consumer[4] get a number: 65
	the producer generates a number: 66
	the consumer[0] get a number: 66
	the producer generates a number: 67
	the consumer[1] get a number: 67
	the producer generates a number: 68
	the consumer[2] get a number: 68
	the producer generates a number: 69
	the consumer[3] get a number: 69
	the producer generates a number: 70
	the consumer[4] get a number: 70
	the producer generates a number: 71
	the consumer[0] get a number: 71
	the producer generates a number: 72
	the consumer[1] get a number: 72
	the producer generates a number: 73
	the consumer[2] get a number: 73
	the producer generates a number: 74
	the consumer[3] get a number: 74
	the producer generates a number: 75
	the consumer[4] get a number: 75
	the producer generates a number: 76
	the consumer[0] get a number: 76
	the producer generates a number: 77
	the consumer[1] get a number: 77
	the producer generates a number: 78
	the consumer[2] get a number: 78
	the producer generates a number: 79
	the consumer[3] get a number: 79
	the producer generates a number: 80
	the consumer[4] get a number: 80
	the producer generates a number: 81
	the consumer[0] get a number: 81
	the producer generates a number: 82
	the consumer[1] get a number: 82
	the producer generates a number: 83
	the consumer[2] get a number: 83
	the producer generates a number: 84
	the consumer[3] get a number: 84
	the producer generates a number: 85
	the consumer[4] get a number: 85
	the producer generates a number: 86
	the consumer[0] get a number: 86
	the producer generates a number: 87
	the consumer[1] get a number: 87
	the producer generates a number: 88
	the consumer[2] get a number: 88
	the producer generates a number: 89
	the consumer[3] get a number: 89
	the producer generates a number: 90
	the consumer[4] get a number: 90
	the producer generates a number: 91
	the consumer[0] get a number: 91
	the producer generates a number: 92
	the consumer[1] get a number: 92
	the producer generates a number: 93
	the consumer[2] get a number: 93
	the producer generates a number: 94
	the consumer[3] get a number: 94
	the producer generates a number: 95
	the consumer[4] get a number: 95
	the producer generates a number: 96
	the consumer[0] get a number: 96
	the producer generates a number: 97
	the consumer[1] get a number: 97
	the producer generates a number: 98
	the consumer[2] get a number: 98
	the producer generates a number: 99
	the consumer[3] get a number: 99
	the producer generates a number: 100
	the consumer[4] get a number: 100
	the consumer[0] sum is 5050
	 the consumer[0] exit!
	the producer exit!
	