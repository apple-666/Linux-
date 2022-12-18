//http
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main(int argc,char* argv[]){
    // ./a.out 12.34.56.78 www.baidu.com [abc/def.html]
    if(argc < 3){
        fprintf(stderr,"用法：./a.out <IP地址> <域名> [<资源路径>]\n");
        return -1;
    }
    char* ip = argv[1];
    char* name = argv[2];
    char* path = argc < 4 ? "" : argv[3];

    //创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        perror("socket");
        return -1;
    }
    //组织服务器的地址结构
    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(80);
    ser.sin_addr.s_addr = inet_addr(ip);

    //向服务器发起连接
    if(connect(sockfd,(struct sockaddr*)&ser,sizeof(ser)) == -1){
        perror("connect");
        return -1;
    }
    //发送HTTP请求
    char request[1024] = {};
    sprintf(request,
            "GET /%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Accept: */*\r\n"
            "User-Agent: Mozilla/5.0\r\n"
            "Connection: close\r\n"
            "Referer: %s\r\n\r\n",path,name,name);
    if(send(sockfd,request,strlen(request),0) == -1){
        perror("send");
        return -1;
    }
    //接收响应
    for(;;){
        char buf[1024] = {};
        ssize_t size = recv(sockfd,buf,sizeof(buf)-sizeof(buf[0]),0);
        if(size == -1){
            perror("recv");
            return -1;
        }
        if(size == 0){
            break;
        }
        printf("%s",buf);
    }
    printf("\n");
    //关闭套接字
    close(sockfd);
    return 0;
}








