#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    /* 第一步：创建用于通信的套接字 */
    int fd = socket(AF_INET,SOCK_STREAM,0);//IPV4，流式传输协议，TCP
    //判断套接字是否创建成功
    if(fd == -1){
        perror("socket");
        return -1;
    }
    /* 第二步：连接服务器，需要先知道服务器的IP和Port */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);//将主机字节序转换为网络字节序，小端--->大端，htons
    //将客户端的IP地址转换为网络字节序
    inet_pton(AF_INET,"172.16.4.132",&saddr.sin_addr.s_addr);
    //进行connect连接
    int ret = connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));//把saddr强转为struct sockaddr类型
    if(ret == -1){
        perror("connect");
        return -1;
    }
    
    /* 第三步：通信 */
    int number = 0;
    while(1){
        //发送数据
        char buff[1024];//把要发送的数据先初始化到buff内存块里面
        //sprintf() 函数可以将格式化字符串的输出结果保存到字符数组中，并且可以通过其他的输出语句输出这个字符数组。
        sprintf(buff,"你好,hello tcp,%d...\n",number ++);
        send(fd,buff,strlen(buff)+1,0);//strlen(buff)是计算buff中实际的字节数，加1是为了把字符串最后的"\0"一并发送出去
        //接收数据
        memset(buff,0,sizeof(buff));//先将缓冲区中的数据清空
        int len = recv(fd,buff,sizeof(buff),0);//当recv函数检测到对应的读缓冲区有数据就解除阻塞
        if(len > 0){
            printf("sever say: %s\n",buff);
        }else if(len == 0){
            printf("服务器已经断开了连接...\n");
            break;
        }else{
            perror("recv");
            break;
        }
        sleep(3);//让客户端发送数据的频率慢一点
    }
    /* 第六步：关闭文件描述符 */
        close(fd);
    return 0;
}