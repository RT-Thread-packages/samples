# 点亮第一盏灯 LED

## 支持平台

| 验证平台                 | 硬件连接     | 环境搭建    |
| ------------------------ | ---- | ---- |
| 野火 M3 M4 M7 开发板     | [硬件连接]()     |      |
| 正点原子 M3 M4 M7 开发板 | [硬件连接]()      |      |
| QEMU 模拟器              |      | [环境搭建]()      |
| Keil MDK 模拟器          |      | [环境搭建]()     |

## 硬件要求

要求硬件上：

* 至少有一路 GPIO， 能够用来接 LED 灯
* 有一路串口用来做 msh shell 终端

## 软件要求

BSP 中已经实现如下驱动：

* 串口驱动
* PIN 驱动

## 准备工作

### 获取示例代码

RT-Thread samples 软件包中中已有一份该示例代码 [led_blink.c]()，可以通过 env 配置将示例代码加入到项目中。

```
 RT-Thread online packages  --->
     miscellaneous packages  --->
         samples: RT-Thread kernel and components samples  --->
             network sample options  --->
                 [*] [network] tcp client
```

### 配置 LED 灯对应的 PIN 号

根据硬件连接情况配置与 LED 灯连接的 PIN 号，有两种配置方式:

* 手动方式

直接在新创建的示例代码文件中加入以下宏定义进行配置：

```{.c}
#define LED_PIN     xxx
```

* menuconfig 配置方式

通过使用 env 工具的 menuconfig 配置方式，会自动在 rtconfig.h 配置文件中生成如下的宏定义：

```{.c}
#define LED_PIN     xxx
```

### 示例代码文件

```{.c}
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

```

示例代码中的 `rt_thread_delay(rt_tick_from_millisecond(500))` 函数的作用是延迟 500ms，也相当于让出 CPU 500ms。

## 在 msh shell 中运行示例代码

示例代码中已经将 led_blink 命令导出到了 msh 命令列表中，因此系统运行起来后，在 msh 命令行下输入 led_blink 命令即可让示例代码运行。

```{.c}
msh> led_blink
```

## 预期结果

LED 灯能够进行亮灭交替，同时串口打印出 LED 灯的状态信息。

```{.c}
led on, count : 0
led off
led on, count : 1
led off
led on, count : 2
led off
led on, count : 3
led off
led on, count : 4
led off
led on, count : 5
led off
```

## 注意事项

* PIN 号和 GPIO 号是不相同的，需要注意区分。
* QEMU 和 Keil MDK 模拟器平台因为没有实际的 LED 灯硬件，因此只能通过串口日志信息来判断示例代码的运行情况。

## 引用参考

* 源码 [led_blink.c]()
* [如何使用 pin 设备](../../../../topics/driver/pin/user-guide.md)
* [如何导出 msh 命令]()
* [系统时钟 OS Tick]()
* [如何进行 menuconfig 系统配置]()
