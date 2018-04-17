/*
 *�����嵥����̬��ʱ�����̺;�̬��ʱ������
 *
 *��̬��ʱ������
 * 	����3����ʱ��
 *		�����ʱ�������Զ�ʱ
 *		�����ʱ������  ��ʱ
 *		Ӳ����ʱ�������Զ�ʱ
 *
 *		timer1: 1s
 *		timer2:	2s
 *		timer3: 3s(6s��timeout3���Ƴ�timer1)
 *
 *��̬��ʱ������
 *  ����2����ʱ�� 
 *		����  ��ʱ timer4 1s
 *		�����Զ�ʱ timer5 2s
 */

#include <rtthread.h>

/* ��ʱ�����ƿ� */
static rt_timer_t timer1;
static rt_timer_t timer2;
static rt_timer_t timer3;

/* ��ʱ�����ƿ� */
static struct rt_timer timer4;
static struct rt_timer timer5;

static void timeout1(void* parameter)
{
	static int count = 0;
	count ++;
	
	rt_kprintf("[ soft periodic timer1 is timeout] = %d\n", count);
}

static void timeout2(void* parameter)
{
	static int count = 0;
	count++;
	
	rt_kprintf("[ soft one shot timer2 is timeout] = %d\n", count);
}
static void timeout3(void* parameter)
{
	static int count = 0;
	count++;
	
	rt_kprintf("[ hard periodic timer3 is timeout] = %d\n", count);
	
	if ( count == 2) 
		rt_timer_delete(timer1);
}
 
static void timeout4(void* parameter)
{
	static int count = 0;
	count++;
	
	rt_kprintf("[ static timer4  periodic is timeout] = %d\n", count);
}

static void timeout5(void* parameter)
{
	static int count = 0;
	count++;
	
	rt_kprintf("[ static timer5  one shot is timeout] = %d\n", count);
}

int timer_sample_init(void)
{
	rt_kprintf("[--------start--timer_sample_init-----]\r\n");
	
	/*��̬��ʱ������*/

	/* create time1 */
	timer1 = rt_timer_create("timer1",
							timeout1,	/* ��ʱ����ʱ���õĺ��� */
							RT_NULL,
							1000,		/* ��ʱʱ�䣺1s*/
							RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);	/* �����ʱ�� �����Զ�ʱ*/
   
   /* start timer1 */
   if (timer1 != RT_NULL) rt_timer_start(timer1);
   
   /* create timer2 */
   timer2 = rt_timer_create("timer2",
						timeout2,		/* ��ʱ����ʱ���õĺ��� */
						RT_NULL, 
						2000,			/* ��ʱʱ�䣺2s*/
						RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_ONE_SHOT);	/* �����ʱ�� ���ζ�ʱ*/

	/* start timer2 */
	if (timer2 != RT_NULL) rt_timer_start(timer2);
	
	/* create timer3 */
	timer3 = rt_timer_create("timer3",
						timeout3,
						RT_NULL,
						3000,		/* ��ʱʱ�䣺3s*/
						RT_TIMER_FLAG_HARD_TIMER|RT_TIMER_FLAG_PERIODIC);	/* Ӳ����ʱ�� �����Զ�ʱ*/
						
	/* start timer3 */
	if (timer3 != RT_NULL) rt_timer_start(timer3);
	
	
	/*��̬��ʱ������*/
	rt_timer_init(&timer4, "timer4",	/* ��ʱ�������� timer1 */
					timeout4,			/* ��ʱʱ�ص��Ĵ����� */
					RT_NULL,			/* ��ʱ��������ڲ��� */
					1000,				/* ��ʱ���ȣ���OS TickΪ��λ����10��OS Tick */
					RT_TIMER_FLAG_PERIODIC);	/* �����Զ�ʱ�� */
					
    rt_timer_init(&timer5, "timer5",	/* ��ʱ�������� timer2 */
					timeout5,			/* ��ʱʱ�ص��Ĵ����� */
                    RT_NULL,			/* ��ʱ��������ڲ��� */
                    3000,				/* ��ʱ����Ϊ30��OS Tick */
                    RT_TIMER_FLAG_ONE_SHOT);	/* ���ζ�ʱ�� */

	/* start timer */
	rt_timer_start(&timer4);
	rt_timer_start(&timer5);

	
	return 0;
}
INIT_APP_EXPORT(timer_sample_init);
