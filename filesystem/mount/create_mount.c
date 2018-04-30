/*
 * 程序清单：文件系统创建和挂载
 */
#include <rtthread.h>
#include <dfs_file.h>

#define  BLOCK_DEVICE "sd0"

void rt_thread_entry1(void *parameter)
{
    rt_uint32_t result;
#if defined(RT_USING_DFS)
    while (1)
    {
        /* 确认这个设备已经被注册,返回不为RT_NULL */
        if (rt_device_find(BLOCK_DEVICE) != RT_NULL)
        {
            /* 等待SD卡插入 */
            result = mmcsd_wait_cd_changed(RT_TICK_PER_SECOND);
            if (result == MMCSD_HOST_PLUGED)
            {
                /* 安装SD卡FAT分区1作为根目录 */
                if (dfs_mount(BLOCK_DEVICE, "/", "elm", 0, 0) == 0)
                    rt_kprintf("File System initialized!\n");
                else
                    rt_kprintf("File System init failed!\n");
            }
            else
            {
                rt_kprintf("sdcard init fail or timeout: %d!\n", result);
            }
            break;
        }
        else
        {
            rt_kprintf("%s waiting find !\n", BLOCK_DEVICE);
            rt_thread_delay(rt_tick_from_millisecond(500));
        }
    }
#else
    while (1)
    {
        rt_kprintf("Undefined macro RT_USING_DFS !\n");
        rt_thread_delay(rt_tick_from_millisecond(1000));
    }
#endif
}

int create_mount_sample_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("thread1",
                           rt_thread_entry1,
                           RT_NULL,
                           2048,
                           RT_THREAD_PRIORITY_MAX / 3,
                           20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    return 0;
}
INIT_APP_EXPORT(create_mount_sample_init);
