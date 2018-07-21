/*
 * File      : httpclient.c
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
 * 2018-07-20     flybreak     first version
 */

#include <webclient.h>  /* 使用 HTTP 协议与服务器通信需要包含此头文件 */
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include <netdb.h>
#include <cJSON.h>

#define BUF_SZ       4096        //缓冲区大小
#define URL_LEN_MAX  256         //网址最大长度
#define AREA_ID      "101021300" //上海浦东地区 ID

/* 天气数据解析 */
void weather_data_parse(rt_uint8_t *data)
{
    cJSON *root = RT_NULL, *object = RT_NULL, *item = RT_NULL;

    root = cJSON_Parse((const char *)data);
    if (!root)
    {
        rt_kprintf("No memory for cJSON root!\n");
        return;
    }
    object = cJSON_GetObjectItem(root, "sk_info");

    item = cJSON_GetObjectItem(object, "cityName");
    rt_kprintf("\ncityName:%s ", item->valuestring);

    item = cJSON_GetObjectItem(object, "temp");
    rt_kprintf("\ntemp    :%s ", item->valuestring);

    item = cJSON_GetObjectItem(object, "wd");
    rt_kprintf("\nwd      :%s ", item->valuestring);

    item = cJSON_GetObjectItem(object, "ws");
    rt_kprintf("\nws      :%s ", item->valuestring);

    item = cJSON_GetObjectItem(object, "sd");
    rt_kprintf("\nsd      :%s ", item->valuestring);

    item = cJSON_GetObjectItem(object, "date");
    rt_kprintf("\ndate    :%s", item->valuestring);

    item = cJSON_GetObjectItem(object, "time");
    rt_kprintf("\ntime    :%s \n", item->valuestring);

    if (root != RT_NULL)
        cJSON_Delete(root);
}
void weather(int argc, char **argv)
{
    rt_uint8_t *ptr = RT_NULL;
    int length = 0, result;
    struct webclient_session *session = RT_NULL;
    char *weather_url = RT_NULL;

    /* 为 weather_url 分配空间 */
    weather_url = rt_calloc(1, URL_LEN_MAX);
    if (!weather_url)
    {
        rt_kprintf("No memory for weather_url!\n");
        goto __exit;
    }
    /* 拼接 GET 网址 */
    rt_snprintf(weather_url, URL_LEN_MAX, "http://mobile.weather.com.cn/data/sk/%s.html", AREA_ID);

    /* 为结构体 webclient_session 分配空间 */
    session = (struct webclient_session *)rt_calloc(1, sizeof(struct webclient_session));
    if (!session)
    {
        rt_kprintf("No memory for session structure!\n");
        goto __exit;
    }
    
    /* 连接天气网站 */
    result = webclient_connect(session, weather_url);
    if (result < 0)
    {
        rt_kprintf("Webclient connect URI(%s) failed!\n", weather_url);
        goto __exit;
    }
    /* 发送官方标准 header */
    result = webclient_send_header(session, WEBCLIENT_GET, RT_NULL, RT_NULL);
    if (result < 0)
    {
        rt_kprintf("Webclient send header buffer failed return %d!", result);
        goto __exit;
    }
    /* 检查响应 */
    if (webclient_handle_response(session))
    {
        if (session->response != 200)
        {
            rt_kprintf("webclient handle response(%d) error!", session->response);
            goto __exit;
        }
    }
    /* 分配用于存放接收数据的缓冲 */
    ptr = rt_calloc(1, BUF_SZ);
    if(!ptr)
    {
        rt_kprintf("No memory for data receive buffer!\n");
        goto __exit;
    }
    
    length = webclient_read(session, ptr, BUF_SZ);
    ptr[length] = '\0';
    /* 天气数据解析 */
    weather_data_parse(ptr);

__exit:
    /* 释放网址空间 */
    if (weather_url != RT_NULL)
        rt_free(weather_url);
    /* 关闭会话 */
    if (session != RT_NULL)
        webclient_close(session);
    /* 释放缓冲区空间 */
    if (ptr != RT_NULL)
        rt_free(ptr);
}

#ifdef FINSH_USING_MSH
#include <finsh.h>

MSH_CMD_EXPORT(weather, Get weather by webclient);
#endif
