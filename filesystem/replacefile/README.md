# 替换文件中的内容

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

编写 replacefile_sample()函数，首先宏定义要替换内容的文件名，以及测试用的数据和缓冲

```{.c}
#define TEST_FN     "/replace.txt"
/* 测试用的数据和缓冲 */
static char buffer[200];
static char test_data[3]="abc";
```
定义好待替换文件以及替换数据和测试缓冲之后，就可以使用stat()接口来查看待替换文件大小,然后使用open()接口打开文件并记录文件入口位置，使用read()接口读取出文件内容，查找是否含有字符串"123",找到有待替换字符串则记录当前偏移量，并使用write()接口用"abc"覆盖掉待替换字符串。

```{.c}
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
```

### 将示例函数导出到 msh 命令列表

通过如下的方式可以将示例函数 replacefile_sample 导出到 msh 命令列表中：

```{.c}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(replacefile_sample, replace file sample);
```

## 步骤二 运行例程

### 在 msh shell 中运行

在步骤一中已经将 replacefile_sample 命令导出到了 msh 命令列表中，因此系统运行起来后，先创建文件replace.txt并在其中写入待替换字符串，并查看文件内容，在 msh 命令行下输入 replacefile_sample 命令即可让例程运行。

```{.c}
msh />echo "12345632112345678902123" replace.txt
msh />cat replace.txt
12345632112345678902123
msh />replacefile_sample
```

## 预期结果

串口打印出待替换内容与替换内容，以及替换结果，使用cat命令查看文件内容，可以看到替换后的文件内容。

```{.c}
123 => adc
character string Replace completed
replace_times:3
msh />cat replace.txt
abc456321abc45678902abc
```

## 注意事项

* 示例代码指定了待替换文件名以及待替换内容
* 限定了文件大小不超过200字符

