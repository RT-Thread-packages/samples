/*
 * 程序清单：打开文件
 *
 * 程序会创建一个操作文件的函数并导出到msh命令列表
 * 在函数中调用 open() close() 函数
 * int open(const char *pathname, int oflag, int mode); 打开或创建一个文件
 * int close(int fd);关闭文件
 *
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

static void openfile_sample(void *parameter)
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
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(openfile_sample, open file sample);