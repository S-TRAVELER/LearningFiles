#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

int getlocalip(char *ip){
	char host[100]={0};
	if(gethostname(host,sizeof(host))<0)
		ERR_EXIT("gethostname");
	printf("local host name: %s\n",host);
	struct hostent *hp;
	if((hp=gethostbyname(host))==NULL)
		ERR_EXIT("gethostbyname");

	strcpy(ip,inet_ntoa(*(struct in_addr*)hp->h_addr_list[0]));
	return 0;
}

int main(){
	//char ip[16]={0};
	//if(getlocalip(ip)<0){
	//	ERR_EXIT("getlocalip");
	//}
	//printf("local ip: %s\n",ip);
	char host[100]={0};
	if(gethostname(host,sizeof(host))<0)
		ERR_EXIT("gethostname");
	printf("local host name: %s\n",host);
	struct hostent *hp;
	if((hp=gethostbyname(host))==NULL)
		ERR_EXIT("gethostbyname");
	printf("local ip:\n");
	for(int i=0;hp->h_addr_list[i]!=NULL;++i){
		printf("\t%s\n",inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
	}
	return 0;
}
