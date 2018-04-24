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
当输出窗口输出 File System initialized! 的时候就说明运行成功了.
## 本文相关核心API ##

### 文件系统初始化 dfs_mount() ###

* 函数原型:

```
int dfs_mount(const char* device_name, const char* path, const char* filesystemtype,rt_uint32_t rwflag, const void* data);
```
dfs_mount 函数用于把以device_name 为名称的设备挂接到path 路径中。filesystemtype 指定了设备上的文件系统的类型（如elm、rom、nfs 等文件系统).data参数对某些文件系统是有意义的，如nfs，对elm 类型系统则没有意义。
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|device_name|设备名;|
|path  | 挂接路径;|
|filesystemtype| 文件系统的类型;|
|rwflag|文件系统的标志;|
|data|文件系统的数据;|

* 函数返回：

装载成功将返回0，否则返回-1。具体的错误需要查看errno。

### 检测sd卡插入 mmcsd_wait_cd_changed() ###

* 函数原型:

```
int mmcsd_wait_cd_changed(rt_int32_t timeout);
```
检测sd卡.
该函数内部有一个接收邮件的函数.接受的邮件为rt_mmcsd_host结构体的32位地址指针.当接收到的信息card不为空的时候,返回MMCSD_HOST_PLUGED,说明sd卡存在.
* 入口参数：

|参数            | 描述 |
---------------|--------------------------------
|timeout  | 最大等待时间|

* 函数返回：

成功返回MMCSD_HOST_PLUGED；否则返回MMCSD_HOST_UNPLUGED 或者 -RT_ETIMEOUT 。
