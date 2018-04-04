# TODO && How #

## 第一阶段目标 ##

完成 v2m-mps2 上的 kernel 例程和文档

| 名称 | 说明 | owner |
| ---- | ---- | ---- |
| v2m-mps2 platform| 模拟器平台搭建 | skawu |
| led  | led 跑马灯 | maxwell |
| thread_create_delete | 线程的创建和删除 | yqiu |
| interrupt | 中断和临界区保护 | flight |
| semphore | 信号量 | 来一颗糖 |
| mutex | 互斥量 | 来一颗糖 |
| mailbox | 邮箱 | 来一颗糖 |
| event | 事件 | 来一颗糖 |
| messagequeue | 消息队列 | 来一颗糖 |
| timer | 软件定时器 | flight |
| heap | 堆的初始化和使用（数组方式） | maxwell |
| memheap | 非连续内存堆 | maxwell |
| mempool | 内存池 | skawu |
| idlehook | 空闲任务钩子 | skawu |
| producer_consumer | 生产者消费者 | skawu |
| signal | 信号 | flight |

## 现状 ##

samples框架已经搭好，部分例程中加入了一些代码，但未整理，不能编译通过

## 如何开发 ##

参考以下模板的方式进行代码和文档开发；

### 模板 ###

* samples\kernel\thread\thread.c
* samples\kernel\thread\README.md
* samples\docs\ss4020-kernel-thread.md

### 参考资料 ###

* [RT-Thread编程指南](https://www.rt-thread.org/document/site/docs/tools/env/env-user-manual/)
* [RealTouch例程列表](https://www.rt-thread.org/qa/thread-1877-1-1.html)
* [realboard examples](https://github.com/RT-Thread/realboard-lpc4088/tree/master/software/rtthread_examples/examples)