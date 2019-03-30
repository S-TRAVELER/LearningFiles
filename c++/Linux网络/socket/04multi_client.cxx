#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*Address Format
struct sockaddr_in {
        sa_family_t    sin_family; //address family: AF_INET 
        in_port_t      sin_port;   // port in network byte order 
        struct in_addr sin_addr;   // internet address 
};

struct in_addr {
        uint32_t       s_addr;     // address in network byte order
};*/


#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

void echo_client(int sock){
	char sendbuf[1024]={0};
	char recvbuf[1024]={0};
	while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL){
		write(sock,sendbuf,strlen(sendbuf));
		read(sock,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
	}
}

int main(void){
	int sock[10];
	for(int i=0;i<10;++i){
		if((sock[i]=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
			ERR_EXIT("socket");
		}	
		struct sockaddr_in servaddr;
		memset(&servaddr,0,sizeof(servaddr));
		servaddr.sin_family=AF_INET;
		servaddr.sin_port=htons(5188);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		//servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
		//inet_aton("127.0.0.1",&servaddr.sin_addr);
		if(connect(sock[i],(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
			ERR_EXIT("connect");
		}
		struct sockaddr_in localaddr;
		socklen_t addrlen=sizeof(localaddr);
		if(getsockname(sock[i],(struct sockaddr*)&localaddr,&addrlen)<0){
			ERR_EXIT("getsockname");
		}
		printf("ip=%s, port=%d\n",inet_ntoa(localaddr.sin_addr),ntohs(localaddr.sin_port));
	}
	echo_client(sock[0]);
//	for(int i=0;i<5;++i){
//		close(sock[i]);
//	}
	return 0;
}
	

