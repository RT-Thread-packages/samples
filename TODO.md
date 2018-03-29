# TODO && How #

## 第一阶段目标 ##

完成 v2m-mps2 上的 kernel 例程和文档

| 名称 | 说明 | owner |
| ---- | ---- | ---- |
| v2m-mps2 platform| 模拟器平台搭建 | xxx |
| thread_create_delete | 线程的创建和删除 | xxx |
| interrupt | 中断和临界区保护 | xxx |
| semphore | 信号量 | xxx |
| mutex | 互斥量 | xxx |
| mailbox | 邮箱 | xxx |
| event | 事件 | xxx |
| messagequeue | 消息队列 | xxx |
| timer | 软件定时器 | xxx |
| heap | 堆的初始化和使用（数组方式） | xxx |
| memheap | 非连续内存堆 | xxx |
| mempool | 内存池 | xxx |
| idlehook | 空闲任务钩子 | xxx |
| producer_consumer | 生产者消费者 | xxx |
| signal | 信号 | xxx |

## 现状 ##

samples框架已经搭好，部分例程中加入了一些代码，但未整理，不能编译通过

## 如何开发 ##

参考以下模板的方式进行代码和文档开发；

### 模板 ###

* samples\kernel\thread\thread.c
* samples\kernel\thread\README.md
* samples\docs\thread.md

### 参考资料 ###

[RT-Thread编程指南](https://www.rt-thread.org/document/site/docs/tools/env/env-user-manual/)
[RealTouch例程列表](https://www.rt-thread.org/qa/thread-1877-1-1.html)
[realboard examples](https://github.com/RT-Thread/realboard-lpc4088/tree/master/software/rtthread_examples/examples)