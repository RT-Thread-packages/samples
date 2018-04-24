/*
 * �����嵥���ļ�ϵͳ�����͹���
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
        /* ȷ������豸�Ѿ���ע��,���ز�ΪRT_NULL */
        if (rt_device_find(BLOCK_DEVICE) != RT_NULL)
        {
            /* �ȴ�SD������ */
            result = mmcsd_wait_cd_changed(RT_TICK_PER_SECOND);
            if (result == MMCSD_HOST_PLUGED)
            {
                /* ��װSD��FAT����1��Ϊ��Ŀ¼ */
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
