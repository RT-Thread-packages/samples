/*
 * File      : app.c
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
 * 2017-01-10     DQL        first implementation.
 */
#include <rthw.h>
#include <rtthread.h>

#include "drv_ssd1351.h"

#include <stdlib.h>


void oled_thread_entry(void *parameter)
{
     //0 indicate the end of string
    unsigned char Name[]={50 ,52 ,13 ,52 ,72 ,82 ,69 ,65 ,68,0};
    //RT-Thread
    unsigned char Tel[]={11,24,22,96,16,18,17,13,21,24,25,25,21,22,22,19,0};
    //+86 021-58995663
    
    uint16_t r=0;
    uint16_t g=0;
    uint16_t b=0;
    
    int i,j,k;
    
    uint16_t color=0;
   	  
    while(1)
    {
        
//        ssd1351_fill_block(0,127,0,127,RGB565_BLACK);
//        // Show String "RT-Thread +86 021-58995663"
//        //the font is 5*7
//        Show_String(1,Name,RGB565_NAVY,0,0);
//        Show_String(1,Tel,RGB565_DGREEN,0,2*7);
//        rt_thread_delay(500);
//        // Clear Screen
//        ssd1351_clear();
//        
//        // Color Bar 
//        test_rainbow();

//        rt_thread_delay(500);  
        
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_BLACK);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_DGREEN);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_DCYAN);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_MAROON);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_PURPLE);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_OLIVE);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_LGRAY);
//        rt_thread_delay(100);     

//        ssd1351_fill_ram(RGB565_DGRAY);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_BLUE);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_GREEN);
//        rt_thread_delay(100); 

//        ssd1351_fill_ram(RGB565_CYAN);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_RED);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_MAGENTA);
//        rt_thread_delay(100); 

//        ssd1351_fill_ram(RGB565_YELLOW);
//        rt_thread_delay(100); 
//        // Clear Screen
//        ssd1351_fill_ram(RGB565_WHITE);
//        rt_thread_delay(100); 

        r = rand()%(31-0+1)  + 0;   //产生[0，31]内的随机整数
        g = rand()%(63-0+1)  + 0;   //产生[0，63]内的随机整数
        b = rand()%(31-0+1)  + 0;   //产生[0，31]内的随机整数

        ssd1351_fill_ram(color);
        color = (r<<11) + (g<<5) + b;

        rt_thread_delay(1);  



             
             
//        // Clear Screen
//        ssd1351_fill_ram(0x0000);
//		
//        draw_rectangle(0x00,MAX_COLUMN,0x00,MAX_ROW,2,RGB565_WHITE);
//        rt_thread_delay(100); 
//        draw_rectangle(0x10,0x6F,0x10,0x6F,4,RGB565_RED);
//        rt_thread_delay(100); 
//        draw_rectangle(0x20,0x5F,0x20,0x5F,6,RGB565_GREEN);
//        rt_thread_delay(100); 
//        draw_rectangle(0x30,0x4F,0x30,0x4F,8,RGB565_BLUE);
//        rt_thread_delay(100); 
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