# 更改名称 #

## 介绍 ##

这个例程展示了如何更改文件名称。

## 程序清单 ##

```c
/*
 * 程序清单：更改名称
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 rename()函数
 * int rename(const char *oldpath, const char *newpath);
 * rename()会将参数oldpath 所指定的文件名称改为参数newpath
 * 所指的文件名称。若newpath 所指定的文件已经存在，则该文件将会被覆盖。
 *
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

ALIGN(RT_ALIGN_SIZE)
static char file_thread_stack[1024];
static struct rt_thread file_thread;
/* 假设文件操作是在一个线程中完成 */
static void file_thread_entry(void *parameter)
{
    rt_kprintf("%s => %s ", "/text.txt", "/text1.txt");

    if (rename("/text.txt", "/text1.txt") < 0)
        rt_kprintf("[error!]\n");
    else
        rt_kprintf("[ok!]\n");
}
int rename_sample_init(void)
{

    rt_thread_init(&file_thread,
                   "file_thread",
                   file_thread_entry,
                   RT_NULL,
                   &file_thread_stack[0],
                   sizeof(file_thread_stack), 8, 50);
    rt_thread_startup(&file_thread);

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
INIT_APP_EXPORT(rename_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rename_sample_init, rename sample);
```

## 运行结果 ##

```
/text.txt => /text1.txt [ok!]
```

