# RT-Thread Sample #

---

## 简介 ##

这是RT-Thread使用的典型示例代码，用来展示如何使用RT-Thread。

## 功能说明 ##

一方面，sample代码可以被RT-Thread开发者用来作为应用开发的参考，另一方面，Sample代码也可能在如下文档中被使用：

* RT-Thread编程指南
* 实验手册
* 应用笔记

### 目录结构 ###

| 名称 | 说明 |
| ---- | ---- |
| kernel | 内核示例目录 |
| filesystem | 文件系统示例目录 |
| network | 网络示例目录 |
| driver | 驱动示例目录 |
| pkginfo | Kconfig配置文件目录 |

### 许可证 ###

与RT-Thread许可一致

## 获取方式 ##

使用samples package需要在RT-Thread的包管理器中选择它：

    RT-Thread online packages
        miscellaneous packages --->
            [*] sample package

然后让RT-Thread的包管理器自动更新，或者使用`pkgs --update`命令更新包到BSP中。

## 使用 ##

* RT-Thread Nano

* RT-Thread

## 开发 ##

### 模板 ###

* [示例程序](kernel/sample/sample.c)
* [说明](kernel/sample/README.md)

### sample列表 ###

内核

| 名称 | 说明 |
| ---- | ---- |
| thread_create | 创建动态线程 |

文件系统

| 名称 | 说明 |
| ---- | ---- |

网络

| 名称 | 说明 |
| ---- | ---- |

### TODO ###

* 完善sample列表
* 根据模板开发sample列表中的示例程序和说明