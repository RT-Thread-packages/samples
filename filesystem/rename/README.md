# 更改名称 #

## 介绍 ##

这个例程展示了如何更改文件名称。

## 程序清单 ##

```{.c}
/*
 * 程序清单：更改名称
 *
 * 程序会创建一个操作文件的函数并导出到msh命令列表
 * 在函数中调用 rename()函数
 * int rename(const char *oldpath, const char *newpath);
 * rename()会将参数oldpath 所指定的文件名称改为参数newpath
 * 所指的文件名称。若newpath 所指定的文件已经存在，则该文件将会被覆盖。
 *
*/
#include <rtthread.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

static void rename_sample(void)
{
    rt_kprintf("%s => %s ", "/text.txt", "/text1.txt");

    if (rename("/text.txt", "/text1.txt") < 0)
        rt_kprintf("[error!]\n");
    else
        rt_kprintf("[ok!]\n");
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rename_sample, rename sample);
```

## 运行结果 ##

```
msh />echo "hello" text.txt
msh />ls
Directory /:
text.txt            5
msh />rename_sample
/text.txt => /text1.txt [ok!]
msh />ls
Directory /:
text1.txt           5
```

我们先使用 echo 命令创建文件 text.txt，接着运行示例代码将其文件名修改为 text1.txt。