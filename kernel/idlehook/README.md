# 空闲任务钩子的使用 #

## 介绍 ##

这个例程展示了如何在RT-Thread里使用空闲任务钩子。

## 运行结果 ##
仿真运行后，输入键盘```Tab```键，得到下面输出，可以看到新增命令```cpu_usage```:

	\ | /
	- RT -     Thread Operating System
	 / | \     3.0.3 build Apr 21 2018
	 2006 - 2018 Copyright by rt-thread team
	finsh >
	--function:
	cpu_usage        -- show cpu usage
	pinMode          -- set hardware pin mode
	pinWrite         -- write value to hardware pin
	pinRead          -- read status from hardware pin
	hello            -- say hello world
	version          -- show RT-Thread version information
	list_thread      -- list thread
	list_sem         -- list semaphone in system
	list_event       -- list event in system
	list_mutex       -- list mutex in system
	list_mailbox     -- list mail box in system
	list_msgqueue    -- list message queue in system
	list_memheap     -- list memory heap in system
	list_mempool     -- list memory pool in system
	list_timer       -- list timer in system
	list_device      -- list device in system
	list             -- list all symbol in system
	msh              -- use module shell
	--variable:
	dummy            -- dummy variable for finsh
	finsh >
	
输入```cpu_usage()```命令，可以获取CPU使用率，输出如下：

	finsh >cpu_usage() 
	cpu usage: 0.0 
	        0, 0x00000000 
	finsh > 
	finsh >cpu_usage()
	cpu usage: 0.2  
	        0, 0x00000000
## 说明 ##
本例程设置了一个空闲任务钩子函数用以计算CPU使用率，同时导出一个命令```cpu_usage```可以方便的在串口命令行中查询CPU使用情况。