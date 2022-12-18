#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h> // 套接字结构
#include<signal.h>
#include<errno.h>
#include<sys/wait.h>

int main(){
	// 服务端创建socket套接字
	printf("服务端创建套接字\n");
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1){
		perror("socket error");
		return -1;
	}

	// 创建服务器地址结构
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(8080);// htons将主机字节序转网络字节序
	ser.sin_addr.s_addr = INADDR_ANY; // 可以接收任意客户端的ip

	// 绑定套接字和地址结构	绑定 sockfd 和 ser
	int rb = bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));	
	if(rb==-1){
		perror("bind error");
		return -1;
	}

	while(1){
		//UPD 连接客户端
		printf("UDP用recvfrom接收客户端数据\n");

		struct sockaddr_in cli;
		socklen_t len = sizeof(cli);
		char buf[128] = {};
		ssize_t size = recvfrom(sockfd,buf,sizeof(buf)-sizeof(buf[0]),0,(struct sockaddr*)&cli,&len);

		if(size == -1){
			perror("recvfrom error");
			return -1;
		}
		int i;
		for(i = 0;i<size;i++){
			buf[i] = toupper(buf[i]);
		}
		if(sendto(sockfd,buf,size,0,(struct sockaddr*)&cli,sizeof(cli))==-1){
			perror("sendto error");
			return -1;
		}		
	}
	printf("服务器关闭套接字\n");
	close(sockfd);
	return 0;
}
