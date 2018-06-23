# 读取目录 #

## 介绍 ##

这个例程展示了如何读取目录。

## 程序清单 ##

```{.c}
/*
 * 程序清单：读取目录
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 readdir() 函数
 * struct dirent* readdir(DIR *d);
 * readdir()函数用来读取目录，参数d 为目录路径名。
 * 返回值为读到的目录项结构，如果返回值为RT_NULL，则表示已经读到目录尾；
 * 此外，每读取一次目录，目录流的指针位置将自动往后递推1 个位置。
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
    struct dirent *d;

    /* 打开/web 目录*/
    dirp = opendir("/web");
    if (dirp == RT_NULL)
    {
        rt_kprintf("open directory error!\n");
    }
    else
    {
        /* 读取目录*/
        while ((d = readdir(dirp)) != RT_NULL)
        {
            rt_kprintf("found %s\n", d->d_name);
        }

        /* 关闭目录 */
        closedir(dirp);
    }
}
int readdir_sample_init(void)
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
INIT_APP_EXPORT(readdir_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(readdir_sample_init, readdir sample);
```

## 运行结果 ##

```
msh />ls
Directory /:
web                 <DIR>                    
msh />cd web
msh /web>echo "hello" hello.txt
msh /web>cd ..
msh />readdir_sample_init
found hello.txt
```

在 web 文件夹下创建 hello.txt 文件后，可以通过示例代码来获得文件夹下的内容。 