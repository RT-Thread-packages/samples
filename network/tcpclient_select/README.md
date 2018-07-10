# TCP 客户端 #

## 介绍 ##

这个例程展示了如何使用 DFS NET 提供的 select 接口来创建一个 TCP 客户端，跟远端服务器进行通信。

## 程序清单 ##

```{.c}
/*
 * 程序清单：tcp 客户端
 *
 * 这是一个 tcp 客户端的例程
 * 导出 tcpclient 命令到控制终端
 * 命令调用格式：tcpclient URL PORT
 * URL：服务器地址 PORT:：端口号
 * 程序功能：接收并显示从服务端发送过来的信息，接收到开头是 'q' 或 'Q' 的信息退出程序
*/
#include <rtthread.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include "netdb.h"
#include <sys/select.h> /* 使用 dfs select 功能  */

#define BUFSZ   1024

static const char send_data[] = "This is TCP Client from RT-Thread."; /* 发送用到的数据 */
void tcpclient_select(int argc, char **argv)
{
    int ret;
    char *recv_data;
    struct hostent *host;
    int sock, bytes_received;
    struct sockaddr_in server_addr;
    const char *url;
    int port;
    fd_set readset;
    int i, maxfdp1;

    if (argc < 3)
    {
        rt_kprintf("Usage: tcpclient URL PORT\n");
        rt_kprintf("Like: tcpclient 192.168.12.44 5000\n");
        return ;
    }

    url = argv[1];
    port = strtoul(argv[2], 0, 10);

    /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析） */
    host = gethostbyname(url);

    /* 分配用于存放接收数据的缓冲 */
    recv_data = rt_malloc(BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    /* 创建一个socket，类型是SOCKET_STREAM，TCP类型 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* 创建socket失败 */
        rt_kprintf("Socket error\n");

        /* 释放接收缓冲 */
        rt_free(recv_data);
        return;
    }

    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* 连接到服务端 */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 连接失败 */
        rt_kprintf("Connect fail!\n");
        closesocket(sock);

        /*释放接收缓冲 */
        rt_free(recv_data);
        return;
    }
    
    /* 获取需要监听的描述符号最大值 */
    maxfdp1 = sock + 1;

    while (1)
    {
        /* 清空可读事件描述符列表 */
        FD_ZERO(&readset);
        
        /* 将需要监听可读事件的描述符加入列表 */
        FD_SET(sock, &readset);

        /* 等待设定的网络描述符有事件发生 */
        i = select(maxfdp1, &readset, 0, 0, 0);
         
        /* 至少有一个文件描述符有指定事件发生再向后运行 */
        if (i == 0) continue;

        /* 查看 sock 描述符上有没有发生可读事件 */
        if (FD_ISSET(sock, &readset))
        {
            /* 从sock连接中接收最大BUFSZ - 1字节数据 */
           bytes_received = recv(sock, recv_data, BUFSZ - 1, 0);
           if (bytes_received < 0)
           {
               /* 接收失败，关闭这个连接 */
               closesocket(sock);
               rt_kprintf("\nreceived error,close the socket.\r\n");

               /* 释放接收缓冲 */
               rt_free(recv_data);
               break;
           }
           else if (bytes_received == 0)
           {
               /* 打印recv函数返回值为0的警告信息 */
               rt_kprintf("\nReceived warning,recv function return 0.\r\n");

               continue;
           }

           /* 有接收到数据，把末端清零 */
           recv_data[bytes_received] = '\0';

           if (strncmp(recv_data, "q", 1) == 0 || strncmp(recv_data, "Q", 1) == 0)
           {
               /* 如果是首字母是q或Q，关闭这个连接 */
               closesocket(sock);
               rt_kprintf("\n got a 'q' or 'Q',close the socket.\r\n");

               /* 释放接收缓冲 */
               rt_free(recv_data);
               break;
           }
           else
           {
               /* 在控制终端显示收到的数据 */
               rt_kprintf("\nReceived data = %s ", recv_data);
           }

           /* 发送数据到sock连接 */
           ret = send(sock, send_data, strlen(send_data), 0);
           if (ret < 0)
           {
               /* 接收失败，关闭这个连接 */
               closesocket(sock);
               rt_kprintf("\nsend error,close the socket.\r\n");

               rt_free(recv_data);
               break;
           }
           else if (ret == 0)
           {
               /* 打印send函数返回值为0的警告信息 */
               rt_kprintf("\n Send warning,send function return 0.\r\n");
           }
        }
    }
    return;
}
MSH_CMD_EXPORT(tcpclient_select, a tcp client sample by select api);

```

## 运行结果 ##

```
Received data = xxx

Received data = xxx

Received data = xxx

Received data = xxx

Received data = xxx
...
got a 'q' or 'Q',close the socket.
```

## 程序介绍 ##

第一步，需要用网络调试工具在电脑上搭建一个 TCP 客户端，

第二步，在终端调用命令 tcpclient 192.168.xxx.xxx 5000 去连接服务端。

在服务端发送 quit 结束程序运行 。