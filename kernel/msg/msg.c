/*
 * �����嵥����Ϣ��������
 *
 * �������ᴴ��2����̬�̣߳�һ���̻߳����Ϣ��������ȡ��Ϣ��һ���̻߳ᶨʱ����
 * Ϣ���з��� ��ͨ��Ϣ�ͽ�����Ϣ��
 */
#include <rtthread.h>

/* ��Ϣ���п��ƿ� */
static struct rt_messagequeue mq;
/* ��Ϣ�������õ��ķ�����Ϣ���ڴ�� */
static rt_uint8_t msg_pool[2048];

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;
/* �߳�1��ں��� */
static void thread1_entry(void *parameter)
{
    rt_uint8_t buf = 0;

    while (1)
    {
        /* ����Ϣ�����н�����Ϣ */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%d\n", buf);
            if (buf == 49)
            {
                break;
            }
        }
        /* ��ʱ100ms */
        rt_thread_delay(rt_tick_from_millisecond(100));
    }
    rt_kprintf("thread1: detach mq \n");
    rt_mq_detach(&mq);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;
/* �߳�2��� */
static void thread2_entry(void *parameter)
{
    int result;
    rt_uint8_t buf = 0;
    while (1)
    {
        if (buf == 25)
        {
            /* ���ͽ�����Ϣ����Ϣ������ */
            result = rt_mq_urgent(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_urgent ERR\n");
            }
            rt_kprintf("thread2: send urgent message - %d\n", buf);
        }
        else if (buf >= 50)/* ����50����Ϣ֮���˳� */
        {
            rt_kprintf("message queue stop send, thread2 quit\n");
            break;
        }
        else
        {
            /* ������Ϣ����Ϣ������ */
            result = rt_mq_send(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_send ERR\n");
            }

            rt_kprintf("thread2: send message - %d\n", buf);
        }
        buf++;
        /* ��ʱ10ms */
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
}

/* ��Ϣ����ʾ���ĳ�ʼ�� */
int messagequeue_sample_init()
{
    rt_err_t result;

    /* ��ʼ����Ϣ���� */
    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],               /* �ڴ��ָ��msg_pool */
                        1,                          /* ÿ����Ϣ�Ĵ�С�� 1 �ֽ� */
                        sizeof(msg_pool),           /* �ڴ�صĴ�С��msg_pool�Ĵ�С */
                        RT_IPC_FLAG_FIFO);          /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */

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
