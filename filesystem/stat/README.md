# 取得状态 #

这个例程展示了如何取得文件状态。

## 程序清单 ##

```{.c}
/*
 * 程序清单：取得状态
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 stat() 函数
 * int stat(const char *file_name, struct stat *buf);
 * stat()函数用来将参数file_name 所指向的文件状态，
 * 复制到buf 指针所指的结构中(struct stat)。
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

ALIGN(RT_ALIGN_SIZE)
static char file_thread_stack[1024];
static struct rt_thread file_thread;
/* 假设文件操作是在一个线程中完成 */
static void file_thread_entry(void *parameter)
{
    struct stat buf;
    stat("/text.txt", &buf);
    rt_kprintf("text.txt file size = %d\n", buf.st_size);
}
int stat_sample_init(void)
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
INIT_APP_EXPORT(stat_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(stat_sample_init, show text.txt stat sample);
```

## 运行结果 ##

```
msh />echo "hello" text.txt
msh />stat_sample_init
text.txt file size = 5
```

通过 echo 命令创建文件 text.txt 后，可以通过示例代码来获取文件状态信息。