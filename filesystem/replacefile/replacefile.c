/*
 * 程序清单：替换内容
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

#define TEST_FN     "/replace.txt"
/* 测试用的数据和缓冲 */
static char buffer[200];
static char test_data[3]="abc";

static void replacefile_sample(void *parameter)
{
	int fd,fd_begin,read_length,write_length1;
	int i=0,replace_times=0;
	int offset=0,result=0;	
	struct stat buf;
   
	rt_kprintf("%s => %s \n", "123", "adc");
	stat(TEST_FN, &buf);
	/* 读写打开进行 数据读取与替换 */
    fd = open(TEST_FN, O_RDONLY|O_WRONLY, 0);
	if (fd < 0)
    {
        rt_kprintf("open file for write failed\n");
        return;
    }
	fd_begin=fd;
	read_length = read(fd, buffer, buf.st_size);
	if(read_length)
	{
		for(i=0;i<read_length-2;i++)
		{
			if(buffer[i]==49&&buffer[i+1]==50&&buffer[i+2]==51)
			{
				replace_times++;
				lseek(fd_begin, i, SEEK_SET);
				write_length1=write(fd_begin, test_data, sizeof(test_data));
				if (write_length1 != sizeof(test_data))
				{
					rt_kprintf("write data failed\n");
					close(fd);
					return;
				}
			}
		}
	}
	close(fd);
	rt_kprintf("character string Replace completed\n");
	rt_kprintf("replace_times:%d\n",replace_times);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(replacefile_sample, replace file sample);
