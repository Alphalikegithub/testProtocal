#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    /* 第一步：创建监听的套接字 */
    int fd = socket(AF_INET,SOCK_STREAM,0);//IPV4，流式传输协议，TCP
    //判断套接字是否创建成功
    if(fd == -1){
        perror("socket");
        return -1;
    }
    /* 第二步：把监听的套接字和本地IP 和端口进行绑定 */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);//将主机字节序转换为网络字节序，小端--->大端，htons
    saddr.sin_addr.s_addr = INADDR_ANY;//可以绑定任意的IP地址，0 = 0.0.0.0 对于0来说，大端和小端没有区别，因此不需要转换字节序
    //saddr.sin_addr.s_addr = inet_addr("172.16.4.91");

    int ret = bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));//把saddr强转为struct sockaddr类型
    if(ret == -1){
        perror("bind");
        return -1;
    }
    /* 第三步：设置监听 */
    ret = listen(fd,128);
    if(ret == -1){
        perror("listen");
        return -1;
    }
    /* 第四步：阻塞并等待客户端的连接 */
    struct sockaddr_in caddr;//为了方便服务器把客户端中的数据取出来，之后再做一次强转即可
    int addrlen = sizeof(caddr);
    int cfd = accept(fd,(struct sockaddr*)&caddr,&addrlen);//accept函数的第三个参数是传入传出参数，在上一行代码中，先将caddr的大小计算出来
    if(ret == -1){
        perror("accept");
        return -1;
    }
    //如果连接建立成功，打印客户端的IP地址和端口号
    //注意：需要先将IP地址和端口从网络字节序转换为主机字节序再输出（大端---->小端）
    /* const char *inet_ntop (int __af, const void *__restrict __cp,
			      char *__restrict __buf, socklen_t __len) */
    char ip[32];//inet_ntop函数中的第三个参数，需要给转换好之后的IP地址指定一块有效的内存
    printf("客户端IP地址: %s,客户端端口号:%d\n",
            inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip)),
            ntohs(caddr.sin_port));
    /* 第五步：通信 */
    while(1){
        //接收数据
        char buff[1024];
        char diy[1024];
        int len = recv(cfd,buff,sizeof(buff),0);//最后的flags标志默认填写0即可
        if(len > 0){
            printf("client say: %s\n",buff);
            //服务器将客户端发来的数据原样的回复给它
            send(cfd,buff,len,0);//该数据在buff里面，长度就是上一行代码中recv函数的返回值len
            /* //发送自定义消息
            int number = 0;
            
            sprintf(diy,"客户端你好, I have receive your message,%d...\n",number ++);
            send(cfd,diy,len,0); */
        }else if(len == 0){
            printf("客户端已经断开了连接...\n");
            break;
        }else{
            perror("recv");
            break;
        }
    }
    /* 第六步：关闭文件描述符 */
        close(fd);
        close(cfd);
    return 0;
}