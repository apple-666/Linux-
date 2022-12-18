#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netdb.h>

int main(int argc,char* argv[]){
	if(argc <2 ){
		fprintf(stderr,"用法：运行程序 后解域名\n");
		return -1;
	}
	
	struct hostent* host = gethostbyname(argv[1]);
	if(host==NULL){
		perror("gethostbyname errro");
		return -1;
	}

	printf("主机官方名：");
	printf("%s\n",host->h_name);
	printf("主机别名表：");
	char **pp = NULL;
	for(pp= host->h_aliases;*pp;pp++){
		printf("%s\n",*pp);
	}
	printf("IP地址表:");
	char *ip = NULL;
	int i;
	for(i=0;host->h_addr_list[i]!=NULL;i++){
		ip =  inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);
		printf("%s\n",ip);
	}
	return 0;
}
