/*
 * 程序清单：跑马灯例程
 *
 * 跑马灯大概是最简单的例子，就类似于每种编程语言中程序员接触的第一个程序
 * Hello World一样，所以这个例子就从跑马灯开始。创建一个线程，让它定时地对
 * LED进行更新（关或灭）
 */

#include <rtthread.h>
#include <rtdevice.h>

void led_blink(void)
{
    unsigned int count = 0;

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        /* led1 on */
        rt_kprintf("led on, count : %d\r\n", count);
        count++;
        rt_pin_write(LED_PIN, 0);
        rt_thread_delay(RT_TICK_PER_SECOND / 2); /* sleep 0.5 second and switch to other thread */

        /* led1 off */
        rt_kprintf("led off\r\n");

        rt_pin_write(LED_PIN, 1);
        rt_thread_delay(RT_TICK_PER_SECOND / 2);
    }
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(led_blink, led blink sample);
