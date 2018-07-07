/*
 * File      : app.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
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
 * 2018-03-30     misonyo      first implementation.
 */

#include <rtthread.h>
#include "drv_ssd1351.h"

void oled_thread_entry(void *parameter)
{
    while (1)
    {
        test_rainbow();

        rt_thread_delay(200);
        ssd1351_clear();

        test_rectangle();

        rt_thread_delay(200);
        ssd1351_clear();


    }
}

int app_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("oled",
                           oled_thread_entry,
                           RT_NULL,
                           2048,
                           8,
                           20);
    if (tid == RT_NULL)
    {
        return -RT_ERROR;
    }

    rt_thread_startup(tid);

    return RT_EOK;
}
