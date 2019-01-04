/*
 * This file is only used for doxygen document generation.
 */

/**
 * @defgroup example 示例程序
 *
 * @brief 提供内核、文件系统、网络和外设的使用例程
 */
 
/**
 * @addtogroup example
 */
 
/*@{*/

/**
 * @defgroup kernel_example 内核例程
 *
 * @brief 内核接口的使用例程。
 */
 
/**
 * @defgroup filesystem_example 文件系统例程
 *
 * @brief 文件系统接口的使用例程
 */

/**
 * @defgroup network_example 网络例程
 *
 * @brief 网络接口的使用例程
 *
 */
 
/**
 * @defgroup peripheral-sample 外设例程
 *
 * @brief 外设接口的使用例程
 *
 */

/*@}*/


/**
 * @addtogroup kernel_example
 */
/*@{*/

/**
 * @defgroup thread_example 线程
 * @brief 创建、初始化/脱离线程
 *
 * **源码**
 * @include thread_sample.c
 * @example thread_sample.c
 *
 */

/**
 * @defgroup timeslice_example 时间片调度
 * @brief 相同优先级线程按照时间片轮番调度
 *
 * **源码**
 * @include timeslice_sample.c
 * @example timeslice_sample.c
 */
 
/**
 * @defgroup timer_example 定时器
 * @brief 单次定时器和周期性定时器的使用
 *
 * **源码**
 * @include timer_sample.c
 * @example timer_sample.c
 */

/**
 * @defgroup interrupt_example 中断
 * @brief 关闭中断进行全局变量的访问
 *
 * **源码**
 * @include interrupt_sample.c
 * @example interrupt_sample.c
 */

/**
 * @defgroup signal_example 信号
 * @brief 给线程发送信号
 *
 * **源码**
 * @include signal_sample.c
 * @example signal_sample.c
 */

/**
 * @defgroup dynmem_example 动态内存
 * @brief 线程动态申请内存并释放
 *
 * **源码**
 * @include dynmem_sample.c
 * @example dynmem_sample.c
 */

/**
 * @defgroup memp_example 内存池
 * @brief 线程从内存池中获得内存块与释放内存块
 *
 * **源码**
 * @include memp_sample.c
 * @example memp_sample.c
 */

/**
 * @defgroup idlehook_example 空闲线程钩子函数
 * @brief 打印空闲线程钩子函数的运行次数
 *
 * **源码**
 * @include idlehook_sample.c
 * @example idlehook_sample.c
 */
 
/**
 * @defgroup scheduler_example 调度器钩子函数
 * @brief 在调度器钩子中打印线程切换信息
 *
 * **源码**
 * @include scheduler_hook.c
 * @example scheduler_hook.c
 */

/**
 * @defgroup semaphore_example 信号量
 * @brief 一个线程发送信号量，另外一个线程接收信号量
 *
 * **源码**
 * @include semaphore_sample.c
 * @example semaphore_sample.c
 *
 */

/**
 * @defgroup producer_example 生产者消费者模型
 * @brief 生产者线程与消费者线程使用信号量同步
 *
 * **源码**
 * @include producer_consumer.c
 * @example producer_consumer.c
 */

/**
 * @defgroup mutex_example 互斥量
 * @brief 线程使用互斥量保护共享资源
 *
 * **源码**
 * @include mutex_sample.c
 * @example mutex_sample.c
 *
 */

/**
 * @defgroup priority_example 优先级反转
 * @brief 互斥量反正线程优先级反转功能
 *
 * **源码**
 * @include priority_inversion.c
 * @example priority_inversion.c
 */
 
/**
 * @defgroup event_example 事件
 * @brief 一个线程等待事件，另一个线程发送事件
 *
 * **源码**
 * @include event_sample.c
 * @example event_sample.c
 */

/**
 * @defgroup mailbox_example 邮箱
 * @brief 一个线程往邮箱中发送邮件，另外一个线程往邮箱中收取邮件
 *
 * **源码**
 * @include mailbox_sample.c
 * @example mailbox_sample.c
 */

/**
 * @defgroup msgq_example 消息队列
 * @brief 一个线程会从消息队列中收取消息，另一个线程会定时给消息队列发送普通消息和紧急消息
 * 
 *
 * **源码**
 * @include msgq_sample.c
 * @example msgq_sample.c
 */

/*@}*/


/**
 * @addtogroup filesystem_example
 */
/*@{*/
 
/**
 * @defgroup mkdir_example 创建目录
 * @brief 创建目录
 *
 * **源码**
 * @include mkdir_sample.c
 * @example mkdir_sample.c
 */

/**
 * @defgroup opendir_example 打开目录
 * @brief 打开和关闭目录
 *
 * **源码**
 * @include opendir_sample.c
 * @example opendir_sample.c
 */

/**
 * @defgroup readdir_example 读取目录
 * @brief 读取目录
 *
 * **源码**
 * @include readdir_sample.c
 * @example readdir_sample.c
 */

/**
 * @defgroup readwrite_example 文件读写
 * @brief 打开文件及文件读写
 *
 * **源码**
 * @include readwrite_sample.c
 * @example readwrite_sample.c
 */

/**
 * @defgroup rename_example 文件重命名
 * @brief 文件重命名
 *
 * **源码**
 * @include rename_sample.c
 * @example rename_sample.c
 */

/**
 * @defgroup stat_example 文件状态
 * @brief 查看文件大小
 *
 * **源码**
 * @include stat_sample.c
 * @example stat_sample.c
 */

/**
 * @defgroup tell_seek_dir_example 目录位置操作
 * @brief 保存与设置读取目录位置
 *
 * **源码**
 * @include tell_seek_dir_sample.c
 * @example tell_seek_dir_sample.c
 */

/*@}*/


/**
 * @addtogroup network_example
 */
/*@{*/
 
 /**
 * @defgroup tcpclient_sample tcp 客户端
 * @brief tcp 客户端例程
 *
 * **源码**
 * @include tcpclient_sample.c
 * @example tcpclient_sample.c
 */

 /**
 * @defgroup udpclient_sample udp 客户端
 * @brief udp 客户端
 *
 * **源码**
 * @include udpclient_sample.c
 * @example udpclient_sample.c
 */


 /**
 * @defgroup httpclient_sample http 客户端
 * @brief http 客户端
 *
 * **源码**
 * @include httpclient_sample.c
 * @example httpclient_sample.c
 */


 /**
 * @defgroup tcpclient_select_sample select实现的tcp客户端
 * @brief 利用 select 实现的 tcp 客户端
 *
 * **源码**
 * @include tcpclient_select_sample.c
 * @example tcpclient_select_sample.c
 */


 /**
 * @defgroup tcpserver_sample tcp 服务端
 * @brief tcp 服务端
 *
 * **源码**
 * @include tcpserver_sample.c
 * @example tcpserver_sample.c
 */
 

 /**
 * @defgroup udpserver_sample udp 服务端
 * @brief udp 服务端
 *
 * **源码**
 * @include udpserver_sample.c
 * @example udpserver_sample.c
 */


/*@}*/


/**
 * @addtogroup peripheral-sample
 */
/*@{*/

/**
 * @defgroup i2c_aht10 I2C
 * @brief 通过 I2C 设备读取温湿度传感器 aht10 的温湿度数据
 *
 * **源码**
 * @include i2c_aht10_sample.c
 * @example i2c_aht10_sample.c
 */

/**
 * @defgroup iwdg Watchdog
 * @brief 独立看门狗设备使用
 *
 * **源码**
 * @include iwdg_sample.c
 * @example iwdg_sample.c
 */

/**
 * @defgroup led_blink Led
 * @brief 通过PIN设备管理接口控制LED亮灭
 *
 * **源码**
 * @include led_blink_sample.c
 * @example led_blink_sample.c
 */

/**
 * @defgroup pin_beep PIN
 * @brief 通过按键控制蜂鸣器对应引脚的电平状态进而控制蜂鸣器
 *
 * **源码**
 * @include pin_beep_sample.c
 * @example pin_beep_sample.c
 */

/**
 * @defgroup sd_sample SD卡
 * @brief SD卡设备的使用
 *
 * **源码**
 * @include sd_sample.c
 * @example sd_sample.c
 */

/**
 * @defgroup spi_w25q SPI
 * @brief 通过SPI设备读取 w25q 的 ID 数据
 *
 * **源码**
 * @include spi_w25q_sample.c
 * @example spi_w25q_sample.c
 */
 
/**
 * @defgroup uart_sample UART
 * @brief 通过串口输出字符串"hello RT-Thread!"，然后错位输出输入的字符
 *
 * **源码**
 * @include uart_sample.c
 * @example uart_sample.c
 */
 
/**
 * @defgroup adc_vol_sample ADC
 * @brief 通过 ADC 设备采样电压值并转换为数值
 *
 * **源码**
 * @include adc_vol_sample.c
 * @example adc_vol_sample.c
 */
 
/**
 * @defgroup hwtimer_sample HWTIMER
 * @brief 通过硬件定时器超时回调函数周期性的打印当前 tick 值
 *
 * **源码**
 * @include hwtimer_sample.c
 * @example hwtimer_sample.c
 */
 
/**
 * @defgroup pwm_sample PWM
 * @brief 通过 PWM 设备控制 LED 灯的亮度实现呼吸灯效果
 *
 * **源码**
 * @include pwm_led_sample.c
 * @example pwm_led_sample.c
 */

/**
 * @defgroup rtc_sample RTC
 * @brief 设置RTC设备的日期和时间，延时一段时间后获取当前时间
 *
 * **源码**
 * @include rtc_sample.c
 * @example rtc_sample.c
 */

/*@}*/
