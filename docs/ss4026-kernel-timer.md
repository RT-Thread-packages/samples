# timer 使用 #

## 例程目的 ##

学会 timer初始化，周期性定时，单次定时，软件定时器，硬件定时器. 

## 程序结构及例程原理 ##

### 程序清单 ###

```{.c .numberLines}
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
                            timeout1,   /* 定时器超时调用的函数 */
                            RT_NULL,
                            1000,       /* 定时时间：1s*/
                            RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);	/* 软件定时器 周期性定时*/

    /* start timer1 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);
   
    /* create timer2 */
    timer2 = rt_timer_create("timer2",
                        timeout2,       /* 定时器超时调用的函数 */
                        RT_NULL,
                        2000,           /* 定时时间：2s*/
                        RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_ONE_SHOT);	/* 软件定时器 单次定时*/

    /* start timer2 */
    if (timer2 != RT_NULL) rt_timer_start(timer2);
	
    /* create timer3 */
    timer3 = rt_timer_create("timer3",  
                        timeout3,   
                        RT_NULL,    
                        3000,   /* 定时时间：3s*/
                        RT_TIMER_FLAG_HARD_TIMER|RT_TIMER_FLAG_PERIODIC);   /* 硬件定时器 周期性定时*/
	
    /* start timer3 */
    if (timer3 != RT_NULL) rt_timer_start(timer3);
	
	
    /*静态定时器创建*/
    rt_timer_init(&timer4, "timer4",    /* 定时器名字是 timer1 */
                    timeout4,           /* 超时时回调的处理函数 */
                    RT_NULL,            /* 超时函数的入口参数 */
                    1000,               /* 定时长度，以OS Tick为单位，即10个OS Tick */
                    RT_TIMER_FLAG_PERIODIC);    /* 周期性定时器 */
					
    rt_timer_init(&timer5,  "timer5",   /* 定时器名字是 timer2 */
                    timeout5,           /* 超时时回调的处理函数 */
                    RT_NULL,            /* 超时函数的入口参数 */
                    3000,               /* 定时长度为30个OS Tick */
                    RT_TIMER_FLAG_ONE_SHOT);    /* 单次定时器 */

    /* start timer */
    rt_timer_start(&timer4);
    rt_timer_start(&timer5);
	
	return 0;
}
INIT_APP_EXPORT(timer_sample_init);

```

### 例程设计 ###
该例程在 timer_sample_init 中动态创建三个定时器timer1,timer2,timer3,并启动定时器,初始化静态定时器timer4,timer5,并启动定时器。

### timer1 ###
* 软件定时器1s周期性定时.
### timer2 ###
* 软件定时器2s单次定时.
### timer3 ###
* 硬件定时器3s周期性定时，经过6s移除timer1
### timer4 ###
* 静态周期性定时器定时
### timer5 ###
* 静态单次定时器定时


### 编译调试及观察输出信息 ###

```
  \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 16 2018
 2006 - 2018 Copyright by rt-thread team
[--------start--timer_sample_init-----]
[ soft periodic timer1 is timeout] = 1
[ static timer4  periodic is timeout] = 1
[ soft one shot timer2 is timeout] = 1
[ soft periodic timer1 is timeout] = 2
[ static timer4  periodic is timeout] = 2
[ hard periodic timer3 is timeout] = 1
[ static timer5  one shot is timeout] = 1
[ soft periodic timer1 is timeout] = 3
[ static timer4  periodic is timeout] = 3
[ soft periodic timer1 is timeout] = 4
[ static timer4  periodic is timeout] = 4
[ soft periodic timer1 is timeout] = 5
[ static timer4  periodic is timeout] = 5
[ hard periodic timer3 is timeout] = 2
[ static timer4  periodic is timeout] = 6
[ static timer4  periodic is timeout] = 7
[ static timer4  periodic is timeout] = 8
[ hard periodic timer3 is timeout] = 3
[ static timer4  periodic is timeout] = 9
[ static timer4  periodic is timeout] = 10
[ static timer4  periodic is timeout] = 11
[ hard periodic timer3 is timeout] = 4
[ static timer4  periodic is timeout] = 12
[ static timer4  periodic is timeout] = 13
[ static timer4  periodic is timeout] = 14
[ hard periodic timer3 is timeout] = 5
[ static timer4  periodic is timeout] = 15
[ static timer4  periodic is timeout] = 16
[ static timer4  periodic is timeout] = 17
[ hard periodic timer3 is timeout] = 6
[ static timer4  periodic is timeout] = 18
[ static timer4  periodic is timeout] = 19
[ static timer4  periodic is timeout] = 20
[ hard periodic timer3 is timeout] = 7
[ static timer4  periodic is timeout] = 21
[ static timer4  periodic is timeout] = 22
[ static timer4  periodic is timeout] = 23

``` 
## 本文相关核心API ##

### 当动态创建一个定时器时，可使用下面的函数接口 ###

* 函数原型:

```{.c .numberLines}
rt_timer_t rt_timer_create(const char* name, 
                        void (*timeout)(void* parameter), 
                        void* parameter, 
                        rt_tick_t time,rt_uint8_t flag);
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|name | 定时器的名称; |
|void (*timeout)(void* parameter); | 定时器超时函数指针（当定时器超时时，系统会调用这个函数）;|
|parameter| 定时器超时函数的入口参数（当定时器超时时，调用超时回调函数会把这个参数做为入口参数传递给超时函数); |
|time|定时器的超时时间，单位是系统节拍；|
|flag| 定时器创建时的参数，支持的值包括（可以用“或”关系取多个值）; |

*flag有如下值

```{.c .numberLines}
#define RT_TIMER_FLAG_ONE_SHOT          0x0             /**< one shot timer */
#define RT_TIMER_FLAG_PERIODIC          0x2             /**< periodic timer */

#define RT_TIMER_FLAG_HARD_TIMER        0x0             /**< hard timer,the timer's callback function will be called in tick isr. */
#define RT_TIMER_FLAG_SOFT_TIMER        0x4             /**< soft timer,the timer's callback function will be called in timer thread. */

#define RT_TIMER_CTRL_SET_TIME          0x0             /**< set timer control command */
#define RT_TIMER_CTRL_GET_TIME          0x1             /**< get timer control command */
#define RT_TIMER_CTRL_SET_ONESHOT       0x2             /**< change timer to one shot */
#define RT_TIMER_CTRL_SET_PERIODIC      0x3             /**< change timer to periodic */
```

* 函数返回：

创建成功返回rt_timer_t 类型定时器；失败为RT_NULL

### 启动定时器 ###

当定时器被创建或者初始化以后，并不会被立即启动，必须在调用启动定时器函数接口后，才开始工作，启动定时器函数接口如下

```{.c .numberLines}
rt_err_t rt_timer_start(rt_timer_t timer);
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|timer  | 定时器句柄，指向要启动的定时器控制块。|


* 函数返回：

如果timer已经处于激活状态，则返回-RT_ERROR；否则返回RT_EOK。

### 删除定时器 ###

系统不再使用特定定时器时，可使用下面的函数接口：

```{.c .numberLines}
rt_err_t rt_timer_delete(rt_timer_t timer);
```

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| timer   | 定时器句柄，指向要删除的定时器|

* 函数返回：

返回RT_EOK （如果参数timer句柄是一个RT_NULL，将会导致一个ASSERT断言）


### 停止定时器 ###

启动定时器以后，若想使它停止，可以使用下面的函数接口

```{.c .numberLines}
rt_err_t rt_timer_stop(rt_timer_t timer);
```

调用定时器停止函数接口后，定时器状态将更改为停止状态，并从rt_timer_list链表中脱离出来不参与定时器超时检查。当一个（周期性）定时器超时时，也可以调用这个函数接口停止这个（周期性）定时器本身。

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| timer   | 定时器句柄，指向要停止的定时器控制块|

如果timer已经处于停止状态，返回-RT_ERROR；否则返回RT_EOK。

### 控制定时器 ###

除了上述提供的一些编程接口，RT_thread也额外提供了定时器控制函数接口，以获取或设置更多定时器的信息。控制定时器函数接口如下：

```{.c .numberLines}
rt_err_t rt_timer_control(rt_timer_t timer, rt_uint8_t cmd, void* arg);
```

控制定时器函数接口可根据命令类型参数，来查看或改变定时器的设置。

* 函数参数：

|参数            | 描述 |
---------------|--------------------------------
| timer  | 定时器句柄，指向要进行控制的定时器控制块；|
|cmd|用于控制定时器的命令，当前支持四个命令接口，分别是设置定时时间，查看定时时间，设置单次触发，设置周期触发;|
|arg|与command相对应的控制命令参数;|

* cmd

```{.c .numberLines}
#define RT_TIMER_CTRL_SET_TIME      0x0     /* 设置定时器超时时间       */
#define RT_TIMER_CTRL_GET_TIME      0x1     /* 获得定时器超时时间       */
#define RT_TIMER_CTRL_SET_ONESHOT   0x2     /* 设置定时器为单一超时型   */
#define RT_TIMER_CTRL_SET_PERIODIC  0x3     /* 设置定时器为周期型定时器 */
```

* 函数返回：

函数返回RT_EOK。

### 静态创建定时器 初始化定时器 ###

当选择静态创建定时器时，可利用rt_timer_init接口来初始化该定时器，函数接口如下：

```{.c .numberLines}
void rt_timer_init(rt_timer_t timer,
                const char* name, void (*timeout)(void* parameter), 
                void* parameter,
                rt_tick_t time, rt_uint8_t flag);
```

使用该函数接口时会初始化相应的定时器控制块，初始化相应的定时器名称，定时器超时函数等等。

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|timer | 定时器句柄，指向要初始化的定时器控制块;|
|name | 定时器的名称；|
|void (*timeout)(void* parameter); | 定时器超时函数指针（当定时器超时时，系统会调用这个函数）；|
|parameter| 定时器超时函数的入口参数（当定时器超时时，调用超时回调函数会把这个参数做为入口参数传递给超时函数); |
|time|定时器的超时时间，单位是系统节拍；|
|flag| 定时器创建时的参数，支持的值包括（可以用“或”关系取多个值）; |

* flag 

```{.c .numberLines}
#define RT_TIMER_FLAG_ONE_SHOT      0x0     /* 单次定时     */
#define RT_TIMER_FLAG_PERIODIC      0x2     /* 周期定时     */
#define RT_TIMER_FLAG_HARD_TIMER    0x0     /* 硬件定时器   */
#define RT_TIMER_FLAG_SOFT_TIMER    0x4     /* 软件定时器   */
```

当指定的flag为RT_IMER_FLAG_HARD_TIMER时，如果定时器超时，定时器的回调函数将在时钟中断的服务例程上下文中被调用；当指定的flag为RT_TIMER_FLAG_SOFT_TIMER时，如果定时器超时，定时器的回调函数将在系统时钟timer线程的上下文中被调用

* 函数返回：

创建成功返回rt_timer_t 类型定时器；失败为RT_NULL。

### 脱离定时器 ###

当一个静态定时器不需要再使用时，可以使用下面的函数接口：

```{.c .numberLines}
rt_err_t rt_timer_detach(rt_timer_t timer);
```

脱离定时器时，系统会把定时器对象从系统容器的定时器链表中删除，但是定时器对象所占有的内存不会被释放。

* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
| timer   | 定时器句柄，指向要脱离的定时器控制块;|

* 函数返回：

返回RT_EOK。


