#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>


int main(int argc,char* argv[]){
	char* ip = argv[1];
	char* name = argv[2];
	char* path = "";
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("socket error");
		return -1;
	}
	
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(80);
	ser.sin_addr.s_addr = inet_addr(ip);

	if(connect(sockfd,(struct sockaddr*)&ser,sizeof(ser)) == -1){
		perror("connect error");
		return -1;
	}

	char request[1000] = {};
	sprintf(request,
		"GET /%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Accept:*/*\r\n"
		"User-Agent:Mozilla/5.0\r\n"
		"Connection:close\r\n"
		"Referer:%s\r\n\r\n",path,name,name);
	if(send(sockfd,request,strlen(request),0) == -1){
		perror("send");
		return -1;
	}
	while(1){
		char buf[1000] = {};
		ssize_t size = recv(sockfd,buf,sizeof(buf)-sizeof(buf[0]),0);
		if(size == -1){
			perror("recv error");
			return -1;
		}
		if(size == 0){
			break;
		}
		printf("%s",buf);
	}
	close(sockfd);		
	return 0;
}
