# RT-Thread 示例代码 #

## 简介 ##

这是RT-Thread使用的典型示例代码，用来展示如何使用RT-Thread。

### 目录结构 ###

| 名称 | 说明 |
| ---- | ---- |
| kernel | 内核示例目录 |
| shell | shell 示例目录 |
| filesystem | 文件系统示例目录 |
| network | 网络示例目录 |
| driver | 驱动示例目录 |
| docs | 例程手册文档 |

### 许可证 ###

与RT-Thread许可一致

## 获取方式 ##

使用samples package需要在RT-Thread的包管理器中选择它：

    RT-Thread online packages
        miscellaneous packages --->
            [*] samples package

然后让RT-Thread的包管理器自动更新，或者使用`pkgs --update`命令更新包到BSP中。

### sample列表 ###

基本

| 名称 | 说明 |
| ---- | ---- |
| led | 跑马灯 |

内核

| 名称 | 说明 |
| ---- | ---- |
| thread_create_delete | 线程的创建和删除 |
| interrupt | 中断和临界区保护 |
| semphore | 信号量 |
| mutex | 互斥量 |
| mailbox | 邮箱 |
| event | 事件 |
| messagequeue | 消息队列 |
| timer | 软件定时器 |
| heap | 堆的初始化和使用（数组方式） |
| memheap | 非连续内存堆 |
| mempool | 内存池 |
| idlehook | 空闲任务钩子 |
| producer_consumer | 生产者消费者 |
| signal | 信号 |

Shell

| 名称 | 说明 |
| ---- | ---- |
| finsh/msh | shell |

设备驱动(和应用笔记结合，应用笔记使用的sample都来自这里)

| 名称 | 说明 |
| ---- | ---- |
| serial | 串口设备 |
| SPI | SPI |
| GPIO | 通用IO |
| I2C | I2C |
| Watchdog | 看门狗 |
| Ethernet | 以太网 |
| CAN | CAN设备 |
| BLOCK | 块设备 |
| USB | USB设备 |

文件系统

| 名称 | 说明 |
| ---- | ---- |
| create_mount | 文件系统创建和挂载 |
| read_write | 文件系统读写 |
| romfs | ROM文件系统 |
| ramfs | RAM文件系统 |

网络

| 名称 | 说明 |
| ---- | ---- |
| tcpclient | TCP客户端 |
| tcpserver | TCP服务器端 |
| udpclient | UDP客户端 |
| udpserver | UDP服务器端 |

