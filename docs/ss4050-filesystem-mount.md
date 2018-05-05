# 文件系统创建和挂载 #

## 例程目的 ##

文件系统的创建和挂载 

## 程序结构及例程原理 ##

### 程序清单 ###

```
/*
 * 程序清单：文件系统创建和挂载
 */
#include <rtthread.h>
#include <dfs_file.h>

#define  BLOCK_DEVICE "sd0"

void rt_thread_entry1(void *parameter)
{
    rt_uint32_t result;
#if defined(RT_USING_DFS)
    while (1)
    {
        /* 确认这个设备已经被注册,返回不为RT_NULL */
        if (rt_device_find(BLOCK_DEVICE) != RT_NULL)
        {
            /* 等待SD卡插入 */
            result = mmcsd_wait_cd_changed(RT_TICK_PER_SECOND);
            if (result == MMCSD_HOST_PLUGED)
            {
                /* 安装SD卡FAT分区1作为根目录 */
                if (dfs_mount(BLOCK_DEVICE, "/", "elm", 0, 0) == 0)
                    rt_kprintf("File System initialized!\n");
                else
                    rt_kprintf("File System init failed!\n");
            }
            else
            {
                rt_kprintf("sdcard init fail or timeout: %d!\n", result);
            }
            break;
        }
        else
        {
            rt_kprintf("%s waiting find !\n", BLOCK_DEVICE);
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
#else
    while (1)
    {
        rt_kprintf("Undefined macro RT_USING_DFS !\n");
        rt_thread_delay(rt_tick_from_millisecond(1000));
    }
#endif
}

int create_mount_sample_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("thread1",
                           rt_thread_entry1,
                           RT_NULL,
                           2048,
                           RT_THREAD_PRIORITY_MAX / 3,
                           20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    return 0;
}
INIT_APP_EXPORT(create_mount_sample_init);

```

### 例程设计 ###

注意:本例程在不改动的情况下正常运行首先要确保 RT_USING_DFS 宏是被打开的,sd0设备是存在的.
该例程在 create_mount_sample_init中创建启动线程thread1.

在线程thread1中进行如下操作:

1. 首先调用rt_device_find函数,确认sd0设备存在且被注册.
2. 其次调用mmcsd_wait_cd_changed函数,检测SD卡是否插入.
3. 最后调用dfs_mount函数,安装SD卡FAT分区1作为根目录.

### 编译调试及观察输出信息 ###

```

 \ | /
- RT -     Thread Operating System
 / | \     3.0.3 build Apr 23 2018
 2006 - 2018 Copyright by rt-thread team
lwIP-2.0.2 initialized!
sd0 waiting find !
msh />SD card capacity 123008 KB
switching card to high speed failed
probe mmcsd block device!
found part[0], begin: 32256, size: 120.96MB
File System initialized!

msh />

```

当输出窗口输出 File System initialized! 的时候就说明运行成功了。
