/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-01-08     DQL        first implementation.
 */
#include <rthw.h>
#include <rtthread.h>

#include "drv_mpu6050.h"

void mpu6050_thread_entry(void *parameter)
{
    rt_int16_t temp;        //温度
    rt_int16_t gx,gy,gz;    //三轴加速度
    rt_int16_t ax,ay,az;    //三轴角速度
    
    rt_err_t ret;
    
    while(1)
    {
        ret = mpu6050_temperature_get(&temp);
        if (ret != RT_EOK)
        {
            rt_kprintf("mpu6050 : get temperature error\r\n");
        }            
        ret = mpu6050_accelerometer_get(&ax, &ay, &az);
        if (ret != RT_EOK)
        {
            rt_kprintf("mpu6050 : get acc error\r\n");
        } 
        ret = mpu6050_gyroscope_get(&gx, &gy, &gz);
        if (ret != RT_EOK)
        {
            rt_kprintf("mpu6050 : get gyro error\r\n");
        } 
        if (ret == RT_EOK)
        {
            rt_kprintf("mpu6050: temperature=%-6d gx=%-6d gy=%-6d gz=%-6d ax=%-6d ay=%-6d az=%-6d\r\n",temp/100,gx,gy,gz,ax,ay,az);
        }
        rt_thread_delay(rt_tick_from_millisecond(1000));    
    }
}

int main(void)
{ 
    rt_thread_t tid;
    
    tid = rt_thread_create("mpu6050", 
                            mpu6050_thread_entry,
                            RT_NULL,
                            1024,
                            24,
                            20);
    if (tid == RT_NULL)
    {
        return -RT_ERROR;
    }
    
    rt_thread_startup(tid);
    
    return RT_EOK;
}

