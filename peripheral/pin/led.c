/*
 * File      : led.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2018, RT-Thread Development Team
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
 */

#include <rtthread.h>
#include <rtdevice.h>

#define LED_STAUTS_ON    0
#define LED_STATUS_OFF   1

#define LED_LEVEL_HIGH   0
#define LED_LEVEL_LOW    1

struct led{
    int pin;             /* pin number */
    rt_bool_t level;     
    rt_bool_t status;    /* led on/off status */
    rt_bool_t init_ok;
};

struct led *rt_led_create(int pin, rt_bool_t level)
{
    struct led *node;
    
    node = (struct led*) rt_malloc(sizeof(struct led));
    node->pin = pin;
    node->level = level;
    node->status = LED_STAUTS_OFF;
    
    rt_pin_mode(pin, PIN_MODE_OUTPUT_OD); 
    
    return node;
}

int rt_led_destroy(struct led *node)
{
    RT_ASSERT(node != RT_NULL);

    rt_free(node);

    return RT_EOK;
}

int rt_led_turn_on(struct led *node)
{
    RT_ASSERT(node != RT_NULL);

    node->status = 1;
    rt_pin_write(node->pin, 0x01&(~node->level));

    return RT_EOK;
}

int rt_led_turn_off(struct led *node)
{
    if(NULL == node)
    {
        return -1;
    }
    else
    {
        node->status = 0;
        rt_pin_write(node->pin, node->level);
    }
    return 0;
}

int rt_led_is_on(struct led *node)
{
    if(NULL == node)
    {
        return -1;
    }
    else
        return node->status;
}

int rt_led_turn_over(struct led *node)
{
    if(NULL == node)
    {
        return -1;
    }
    else
    {
        if(node->status)
        {
            node->status = 0;
            rt_pin_write(node->pin, node->level);
        }
        else
        {
            node->status = 1;
            rt_pin_write(node->pin, 0x01&(~node->level));
        }
    }
    return 0;
    
}

#ifdef RT_USING_FINSH
#include <finsh.h>

#ifdef FINSH_USING_MSH
static int led_test(int argc, char **argv)
{
    rt_int8_t pin;
    rt_int8_t level;
    
    if(argc == 3)
    {
        struct led *node;
        
        pin = atol(argv[1]);
        level = atol(argv[2]);
        if(pin < 1)
        {
            return -1;
        }
        if(level > 1)
        {
            rt_kprintf("Level must be 0 or 1!\n");
            return -1;
        }
        else
        {
            node = rt_led_create(pin, level);
            rt_led_on(node);
            rt_thread_delay(1000);
            rt_led_off(node);
            rt_led_destroy(node);
        }
        return 0;
    }
    else
    {
        rt_kprintf("Usage: led_test [pin] [level]\n");
    }
        
    return -1;
}
MSH_CMD_EXPORT(led_test, led pin test.);


#endif /* FINSH_USING_MSH */


#endif /* RT_USING_FINSH */
