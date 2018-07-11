/*
 * 程序清单：文件匹配
 *
 * 程序会创建一个操作文件的函数并导出到msh命令列表
 * 在函数中调用 stat() open() close() read()  lseek() write()函数
 * int stat(const char *file_name, struct stat *buf); 读取文件状态，返回文件大小
 * int open(const char *pathname, int oflag, int mode); 打开或创建一个文件
 * int close(int fd);关闭文件
 * int read(int fd, void *buf, size_t len); 读取文件内容
 * off_t lseek(int fd, off_t offset, int whence); 指定打开文件的偏移量
 * int write(int fd, const void *buf, size_t len); 向指定文件位置写入
 *
*/

#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

static struct dfs_fd fd;
static struct dirent dirent;
static char findfile[]={"1.txt"};

void findfile_sample(const char *pathname)
{
    struct stat stat;
    int length;
    char *fullpath, *path;
	int findfile_result=0;

    fullpath = NULL;
    path = rt_strdup("/");
    if (path == NULL)
	{
		return ; /* out of memory */
    }
    else
    {
        path = (char *)pathname;
    }

    /* list directory */
    if (dfs_file_open(&fd, path, O_DIRECTORY) == 0)
    {
        do
        {
            memset(&dirent, 0, sizeof(struct dirent));
            length = dfs_file_getdents(&fd, &dirent, sizeof(struct dirent));
            if (length > 0)
            {
                memset(&stat, 0, sizeof(struct stat));

                /* build full path for each file */
                fullpath = dfs_normalize_path(path, dirent.d_name);
                if (fullpath == NULL)
                    break;

                if (dfs_file_stat(fullpath, &stat) == 0)
                {
					if(strcmp(dirent.d_name,findfile)==0)
					{
						findfile_result=1;
						rt_kprintf("Find the file 1.txt:\n");
						rt_kprintf("%-20s", dirent.d_name);
						rt_kprintf("%-25lu\n", stat.st_size);
					}
                }
                else
                    rt_kprintf("BAD file: %s\n", dirent.d_name);
                rt_free(fullpath);
            }
        }while(length > 0);

        dfs_file_close(&fd);
		if(findfile_result==0)
		{
			rt_kprintf("Didn't find the file 1.txt\n");
		}
    }
    else
    {
        rt_kprintf("No such directory\n");
    }
    if (pathname == NULL)
        rt_free(path);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(findfile_sample, find file sample);