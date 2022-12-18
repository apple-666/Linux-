#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h> // 套接字结构

int main(){
	// 客户端创建socket套接字
	printf("客户端创建套接字\n");
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd == -1){
		perror("socket error");
		return -1;
	}

	// 创建服务器地址结构
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(8080);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 业务处理
	printf("本次通信遇到bb 结束\n");
	while(1){
		char buf[128] = {};
		fgets(buf,sizeof(buf),stdin);
		
		if(strcmp(buf,"bb\n")==0) break;
		int rs = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&ser,sizeof(ser));
		if(rs == -1){
			perror("sendto error");
			return -1;
		}

		ssize_t size = recv(sockfd,buf,sizeof(buf)-sizeof(buf[0]),0);
		if(size == -1){
			perror("recv error");
			return -1;
		}
		printf("客户端接收到 %s\n",buf);
	}
	// 关闭套接字
	printf("客户端关闭套接字\n");
	close(sockfd);	
	return 0;
}
