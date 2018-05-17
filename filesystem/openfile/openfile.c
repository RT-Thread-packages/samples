/*
 * 程序清单：打开文件
 *
 * 程序会创建一个操作文件的线程
 * 在线程中调用 open() close() 函数
 * int open(const char *pathname, int oflag, int mode); 打开或创建一个文件
 * int close(int fd);关闭文件
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
    int fd, size;
    char s[] = "RT-Thread Programmer!\n", buffer[80];

    /* 打开/text.txt 作写入，如果该文件不存在则建立该文件*/
    fd = open("/text.txt", O_WRONLY | O_CREAT);
    if (fd >= 0)
    {
        write(fd, s, sizeof(s));
        close(fd);
    }

    /* 打开/text.txt 准备作读取动作*/
    fd = open("/text.txt", O_RDONLY);
    if (fd >= 0)
    {
        size = read(fd, buffer, sizeof(buffer));
        close(fd);
    }

    rt_kprintf("%s", buffer);
}
int openfile_sample_init(void)
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
INIT_APP_EXPORT(openfile_sample_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(openfile_sample_init, open file sample);
