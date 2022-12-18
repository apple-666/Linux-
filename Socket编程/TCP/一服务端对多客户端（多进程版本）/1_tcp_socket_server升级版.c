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

// 对子进程做资源回收
void waitchild(int sigo)
{
	printf("捕获到%号信号\n",sigo);
	while(1){
		pid_t pid = waitpid(-1,NULL,WNOHANG);
		if(pid == -1){
			if(errno == ECHILD){
				printf("没有子进程在运行了\n");
				break;
			}else {
				perror("waitpid error");
				exit(-1);
			}
		}else if(pid==0){
			printf("还有子进程在运行\n");
			break;
		}else {
			printf("回收了%d子进程\n",pid);
		}
	}
}

int main(){

	// 对sgichld做注册
	if(signal(SIGCHLD,waitchild)==SIG_ERR){
		perror("signal error");
		return -1;
	}

	// 服务端创建socket套接字
	printf("服务端创建套接字\n");
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
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

	// 服务端开启监听 （等待有客户发送连接请求）
	printf("服务器开启监听\n");
	int rl = listen(sockfd,1024);
	if(rl == -1){
		perror("listen error");
		return -1;
	}

	while(1){
		// 连接客户端
		printf("用accept方法做好连接准备 没有客户端请求连接则阻塞等待\n");
		struct sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int conn = accept(sockfd,(struct sockaddr*)&cli,&len);// 返回新的套接字 用于通信
		if(conn==-1){
			perror("conn error");
			return -1;
		}

		// 父进程管理 监听套接字
		// 多个子进程管理 通信套接字（处理业务)

		pid_t pid = fork();

		if(pid<0){
			perror("fork error");
			return -1;
		}else if(pid>0){
			// 父进程管 监听. 所以可以提前关闭通信套接字
			close(conn);

		}else {
			// 子进程管 通信 . 所以可以提前关闭监听套接字
			close(sockfd);
			printf("服务器接收的客户端的连接:  %s:%hu\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
			// 服务器做业务处理
			printf("服务器阻塞在read函数 读客户端的数据\n");
			printf("本次通信遇到bb 停止\n");
			while(1){
				char buf[128] = {};
				ssize_t size = read(conn,buf,sizeof(buf)-sizeof(buf[0]));	
				if(size == -1){
					perror("read error");
					return -1;
				}
				if(strcmp(buf,"bb\n")==0) break;
				printf("接收到客户端的数据:%s\n",buf);
				int i;
				for(i=0;i<size;i++){
					buf[i] = toupper(buf[i]);
				}
				int rw = write(conn,buf,size) ;
				if(rw == -1){
					perror("write error");
					return -1;
				}
			}
			close(conn);
			return 0;
		}
	}
	// 关闭套接字 和 连接
	printf("服务器关闭套接字\n");
	close(sockfd);
	return 0;
}
