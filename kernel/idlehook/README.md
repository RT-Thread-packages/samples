# 空闲任务钩子的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用空闲任务钩子。

## 运行结果 ##
仿真运行后，控制台一直循环输出打印CPU使用率:

cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.2%
cpu usage: 0.0%
cpu usage: 0.2%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.2%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.2%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.0%
cpu usage: 0.2%
cpu usage: 0.2%
cpu usage: 0.0%
	
## 说明 ##
本例程设置了一个空闲任务钩子用于计算CPU使用率，并创建一个线程循环打印CPU使用率。通过修改CPU使用率打印线程中的休眠tick时间可以看到不同的CPU使用率