# 文件匹配

## 支持平台

QEMU 模拟器           


## 软件要求

QEMU下运行文件系统

## 步骤一 编写例程

### 创建例程文件

创建 replacefile.c 文件，并在文件中加入引用头文件

```{.c}
#include <rtthread.h>
#include <dfs_posix.h>
```

### 编写示例函数

编写 replacefile_sample()函数，首先定义保存结果的结构体，以及待匹配的文件名

```{.c}
static struct dfs_fd fd;
static struct dirent dirent;
static char findfile[]={"1.txt"};
```
使用dfs_file_open()接口来打开文件夹,然后依次获取文件名,比对文件名是否与待查找文件名一致

```{.c}
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
```

### 将示例函数导出到 msh 命令列表

通过如下的方式可以将示例函数 findfile_sample 导出到 msh 命令列表中：

```{.c}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(findfile_sample, find file sample);
```

## 步骤二 运行例程

### 在 msh shell 中运行

在步骤一中已经将 replacefile_sample 命令导出到了 msh 命令列表中，因此系统运行起来后，在 msh 命令行下输入 findfile_sample 命令即可让例程运行，发现未找到1.txt，然后创建文件1.txt，再次通过 msh 命令行下输入 findfile_sample 命令让例程运行。

```{.c}
msh />findfile_sample
Didn't find the file 1.txt
msh />echo "hello" 1.txt
msh />findfile_sample
```

## 预期结果

串口打印出找到1.txt文件，并显示文件大小。

```{.c}
Find the file 1.txt:
1.txt               5
```

## 注意事项

* 示例代码指定了待查找文件名

