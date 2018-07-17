/*
 * 程序清单：保存与设置读取目录位置
 *
 * 程序会创建一个操作文件的函数并导出到msh命令列表
 * 在函数中调用 telldir() 函数
 * off_t telldir(DIR *d); 获取目录流的读取位置
 * void seekdir(DIR *d, off_t offset); 设置下次读取目录的位置
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

/* 假设文件操作是在一个线程中完成 */
static void telldir_sample(void)
{
    DIR *dirp;
    int save3 = 0;
    int cur;
    int i = 0;
    struct dirent *dp;

    /* 打开根目录 */
    dirp = opendir("/");
    for (dp = readdir(dirp); dp != RT_NULL; dp = readdir(dirp))
    {
        /* 保存第三个目录项的目录指针*/
        i++;
        if (i == 3)
            save3 = telldir(dirp);

        rt_kprintf("%s\n", dp->d_name);
    }

    /* 回到刚才保存的第三个目录项的目录指针*/
    seekdir(dirp, save3);

    /* 检查当前目录指针是否等于保存过的第三个目录项的指针. */
    cur = telldir(dirp);
    if (cur != save3)
    {
        rt_kprintf("seekdir (d, %ld); telldir (d) == %ld\n", save3, cur);
    }

    /* 从第三个目录项开始打印*/
    for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp))
        rt_kprintf("%s\n", dp->d_name);

    /* 关闭目录*/
    closedir(dirp);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(telldir_sample, telldir sample);
