/*
 * 程序清单：打开目录
 *
 * 程序会创建一个操作文件的函数并导出到msh命令列表
 * 在函数中调用 opendir() 函数
 * DIR* opendir(const char* name);
 * opendir()函数用来打开一个目录，参数name 为目录路径名。
 * 若读取目录成功，返回该目录结构，若读取目录失败，返回RT_NULL。
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

static void opendir_sample(void)
{
    DIR *dirp;
    /* 打开/dir_test 目录*/
    dirp = opendir("/dir_test");
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
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(opendir_sample, open dir sample);
