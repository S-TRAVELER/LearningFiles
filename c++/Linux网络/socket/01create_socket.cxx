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

int main(void){
	int listenfd;
	if((listenfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		ERR_EXIT("socket");
	}	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5188);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//inet_aton("127.0.0.1",&servaddr.sin_addr);
	
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind");
	if(listen(listenfd,SOMAXCONN)<0)
		ERR_EXIT("listen");
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen=sizeof(peeraddr);
	int conn; 
	if((conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
		ERR_EXIT("accept");
	char recvBuf[1024];
	while(1){
		memset(recvBuf,0,sizeof(recvBuf));
		int ret=read(conn,recvBuf,sizeof(recvBuf));
		fputs(recvBuf,stdout);
		write(conn,recvBuf,ret);		
	}
	close(conn);
	close(listenfd);
	return 0.;
}
