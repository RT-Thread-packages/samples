# 打开目录 #

## 介绍 ##

这个例程展示了如何打开目录。

## 程序清单 ##

```{.c}
/*
 * 程序清单：打开目录
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 opendir() 函数
 * DIR* opendir(const char* name);
 * opendir()函数用来打开一个目录，参数name 为目录路径名。
 * 若读取目录成功，返回该目录结构，若读取目录失败，返回RT_NULL。
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

ALIGN(RT_ALIGN_SIZE)
static char dir_thread_stack[1024];
static struct rt_thread dir_thread;
/* 假设文件操作是在一个线程中完成 */
static void dir_thread_entry(void *parameter)
{
    DIR *dirp;

    /* 打开/web 目录*/
    dirp = opendir("/web");
    if (dirp == RT_NULL)
    {
        rt_kprintf("open directory error!\n");
    }
    else
    {
        rt_kprintf("open directory OK!\n");
        /* 在这儿进行读取目录相关操作*/
        /* ...... */

        /* 关闭目录 */
        closedir(dirp);
    }
}
int opendir_sample_init(void)
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
INIT_APP_EXPORT(opendir_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(opendir_sample_init, open dir sample);
```

## 运行结果 ##

```
open directory OK!
```

