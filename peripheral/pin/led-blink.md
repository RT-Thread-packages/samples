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

## 步骤一 编写示例代码

### 创建示例代码文件

创建 led_blink.c 文件，并在文件中加入引用头文件

```{.c}
#include <rtthread.h>
#include <rtdevice.h>
```

### 编写示例函数

编写 led_blink()函数，首先设置 PIN 脚模式

```{.c}
rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
```

PIN 脚模式设置好以后，就可以使用 rt_pin_write() 接口来拉高或者拉低 PIN 脚电平，控制对应灯的亮和灭，通过调整延时的长度来控制闪烁的频率，同时将灯的状态信息从串口打印出来。

```{.c}
    while (1)
    {
        /* led1 on */
        rt_kprintf("led on, count : %d\r\n", count);
        count++;
        rt_pin_write(LED_PIN, 0);
        rt_thread_delay(RT_TICK_PER_SECOND / 2); /* 延时 500 毫秒 */

        /* led1 off */
        rt_kprintf("led off\r\n");

        rt_pin_write(LED_PIN, 1);
        rt_thread_delay(RT_TICK_PER_SECOND / 2);
    }
```

这里的 `rt_thread_delay(RT_TICK_PER_SECOND/2)` 函数的作用是延迟一段时间， 即让 led 线程休眠 50 个 [OS Tick](os_tick.md) （按照 [rtconfig.h](rtconfig.md) 中的配置，1 秒 = RT_TICK_PER_SECOND 个 tick = 100 tick，即在这份代码中延迟时间等于 500ms）。

### 将示例函数导出到 msh 命令列表

通过如下的方式可以将示例函数 led_blink 导出到 msh 命令列表中：

```{.c}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(led_blink, led blink sample);
```

### 获取示例代码

RT-Thread samples 软件包中中已有一份该示例代码 [led_blink.c]()，可以通过 env 配置将示例代码加入到项目中。

```
 RT-Thread online packages  --->
     miscellaneous packages  --->
         samples: RT-Thread kernel and components samples  --->
             network sample options  --->
                 [*] [network] tcp client
```

[获取 samples 软件包示例代码]()

## 步骤二 运行示例代码

### 在 main 函数中运行

一种方式是在 main 程序中调用 led_blink() 函数

```{.c}
int main(int argc, char **argv)
{
    led_blink();
}
```

### 在 msh shell 中运行

另一种方式是通过 [msh shell](shell.md) 运行，在步骤一中已经将 led_blink 命令导出到了 msh 命令列表中，因此系统运行起来后，在 msh 命令行下输入 led_blink 命令即可让示例代码运行。

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
