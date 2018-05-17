# 创建目录 #

## 介绍 ##

这个例程展示了如何创建目录。

## 程序清单 ##

```c
/*
 * 程序清单：创建目录
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 mkdir() 函数
 * int mkdir(const char *path, mode_t mode);
 * mkdir()函数用来创建一个目录，参数path 为目录名，
 * 参数mode 在当前版本未启用，输入0x777 即可。
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

ALIGN(RT_ALIGN_SIZE)
static char dir_thread_stack[1024];
static struct rt_thread dir_thread;
/* 假设文件操作是在一个线程中完成 */
static void dir_thread_entry(void *parameter)
{
    int ret;

    /* 创建目录*/
    ret = mkdir("/web", 0x777);
    if (ret < 0)
    {
        /* 创建目录失败*/
        rt_kprintf("mkdir error!\n");
    }
    else
    {
        /* 创建目录成功*/
        rt_kprintf("mkdir ok!\n");
    }
}
int mkdir_sample_init(void)
{
    rt_thread_init(&dir_thread,
                   "dir_thread",
                   dir_thread_entry,
                   RT_NULL,
                   &dir_thread_stack[0],
                   sizeof(dir_thread_stack), 8, 50);
    rt_thread_startup(&dir_thread);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
INIT_APP_EXPORT(mkdir_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(mkdir_sample_init, mkdir sample);
```

## 运行结果 ##

```
mkdir ok!
```

