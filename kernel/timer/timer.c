/*
 *程序清单：动态定时器例程和静态定时器例程
 *
 *动态定时器例程
 * 	创建3个定时器
 *		软件定时器周期性定时
 *		软件定时器单次  定时
 *		硬件定时器周期性定时
 *
 *		timer1: 1s
 *		timer2:	2s
 *		timer3: 3s(6s后timeout3中移除timer1)
 *
 *静态定时器例程
 *  创建2个定时器 
 *		单次  定时 timer4 1s
 *		周期性定时 timer5 2s
 */

#include <rtthread.h>

/* 定时器控制块 */
static rt_timer_t timer1;
static rt_timer_t timer2;
static rt_timer_t timer3;

/* 定时器控制块 */
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
	
	/*动态定时器创建*/

	/* create time1 */
	timer1 = rt_timer_create("timer1",
							timeout1,	/* 定时器超时调用的函数 */
							RT_NULL,
							1000,		/* 定时时间：1s*/
							RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);	/* 软件定时器 周期性定时*/
   
   /* start timer1 */
   if (timer1 != RT_NULL) rt_timer_start(timer1);
   
   /* create timer2 */
   timer2 = rt_timer_create("timer2",
						timeout2,		/* 定时器超时调用的函数 */
						RT_NULL, 
						2000,			/* 定时时间：2s*/
						RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_ONE_SHOT);	/* 软件定时器 单次定时*/

	/* start timer2 */
	if (timer2 != RT_NULL) rt_timer_start(timer2);
	
	/* create timer3 */
	timer3 = rt_timer_create("timer3",
						timeout3,
						RT_NULL,
						3000,		/* 定时时间：3s*/
						RT_TIMER_FLAG_HARD_TIMER|RT_TIMER_FLAG_PERIODIC);	/* 硬件定时器 周期性定时*/
						
	/* start timer3 */
	if (timer3 != RT_NULL) rt_timer_start(timer3);
	
	
	/*静态定时器创建*/
	rt_timer_init(&timer4, "timer4",	/* 定时器名字是 timer1 */
					timeout4,			/* 超时时回调的处理函数 */
					RT_NULL,			/* 超时函数的入口参数 */
					1000,				/* 定时长度，以OS Tick为单位，即10个OS Tick */
					RT_TIMER_FLAG_PERIODIC);	/* 周期性定时器 */
					
    rt_timer_init(&timer5, "timer5",	/* 定时器名字是 timer2 */
					timeout5,			/* 超时时回调的处理函数 */
                    RT_NULL,			/* 超时函数的入口参数 */
                    3000,				/* 定时长度为30个OS Tick */
                    RT_TIMER_FLAG_ONE_SHOT);	/* 单次定时器 */

	/* start timer */
	rt_timer_start(&timer4);
	rt_timer_start(&timer5);

	
	return 0;
}
INIT_APP_EXPORT(timer_sample_init);
