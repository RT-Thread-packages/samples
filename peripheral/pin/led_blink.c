/*
 * 程序清单：跑马灯例程
 *
 * 跑马灯大概是最简单的例子，就类似于每种编程语言中程序员接触的第一个程序
 * Hello World一样，所以这个例子就从跑马灯开始。创建一个线程，让它定时地对
 * LED进行更新（亮或灭）
 */

#include <rtthread.h>
#include <rtdevice.h>

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_stack[ 512 ];
/* 线程的TCB控制块 */
static struct rt_thread led_thread;

static void led_thread_entry(void *parameter)
{
    unsigned int count = 0;
    /* 设置 PIN 脚模式 */
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        /* 点亮 led */
        rt_kprintf("led on, count : %d\n", count);
        rt_pin_write(LED_PIN, 0);
        rt_thread_delay(rt_tick_from_millisecond(500));

        /* 关灭 led */
        rt_kprintf("led off\n");
        rt_pin_write(LED_PIN, 1);
        rt_thread_delay(rt_tick_from_millisecond(500));

        count++;
    }
}

int led_sample_init(void)
{
    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry,
                            RT_NULL,
                            (rt_uint8_t *)&led_stack[0],
                            sizeof(led_stack),
                            20,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }
    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(led_sample_init, led sample);
