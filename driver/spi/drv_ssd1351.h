/*
 * File      : drv_ssd1351.h
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
#ifndef __DRV_SSD1351_H__
#define __DRV_SSD1351_H__

#include "drv_spi.h"

#define   MAX_COLUMN	127			// 128-1
#define   MAX_ROW		127			// 128-1
#define	  BRIGHTNESS	100

#define   RGB565_BLACK     0x0000          //黑色
#define   RGB565_NAVY      0x000F          //深蓝色 
#define   RGB565_DGREEN    0x03E0          //深绿色 
#define   RGB565_DCYAN     0x03EF          //深青色      
#define   RGB565_MAROON    0x7800          //深红色    
#define   RGB565_PURPLE    0x780F          //紫色    
#define   RGB565_OLIVE     0x7BE0          //橄榄绿    
#define   RGB565_LGRAY     0xC618          //灰白色  
#define   RGB565_DGRAY     0x7BEF          //深灰色 
#define   RGB565_BLUE      0x001F          //蓝色     
#define   RGB565_GREEN     0x07E0          //绿色      
#define   RGB565_CYAN      0x07FF          //青色         
#define   RGB565_RED       0xF800          //红色        
#define   RGB565_MAGENTA   0xF81F          //品红       
#define   RGB565_YELLOW    0xFFE0          //黄色       
#define   RGB565_WHITE     0xFFFF          //白色  

void ssd1351_clear(void);
void ssd1351_fill_ram(rt_uint16_t rgb565_color);
void ssd1351_fill_block(rt_uint8_t col_star, rt_uint8_t col_end, rt_uint8_t row_star, rt_uint8_t row_end, rt_uint16_t rgb565_color);
void draw_rectangle(rt_uint8_t a, rt_uint8_t b, rt_uint8_t c, rt_uint8_t d, rt_uint8_t e, rt_uint16_t rgb565_color);
void show_string(unsigned char a, unsigned char *Data_Pointer, rt_uint16_t rgb565_color, unsigned char d, unsigned char e);
void show_64k_pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void show_256k_pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void vertical_scroll(unsigned char a, unsigned char b, unsigned char c);
void horizontal_scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);

void deactivate_scroll(void);
void fade_in(void);
void fade_out(void);
void checker_board(void);
void test_string(void);
void test_rainbow(void);
void test_rectangle(void);

#endif
