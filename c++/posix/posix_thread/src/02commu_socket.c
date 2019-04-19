#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

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

void do_service(int conn){
	char recvBuf[1024];
	while(1){
		memset(recvBuf,0,sizeof(recvBuf));
		int ret=read(conn,recvBuf,sizeof(recvBuf));
		if(ret==0){
			printf("peer close\n");
			break;
		}
		if(ret==-1){
			ERR_EXIT("read");
		}
		fputs(recvBuf,stdout);
		//write(conn,recvBuf,ret);		
	}
}

void handler(int sig){
	printf("recv a sig=%d\n",sig);
	pthread_exit(NULL);
}

void* thread_routine(void* arg){
	signal(SIGUSR1,handler);

	char sendbuf[1024]={0};
	while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL){
		write(*(int*)arg,sendbuf,strlen(sendbuf));
		memset(sendbuf,0,sizeof(sendbuf));
	}
	printf("child close\n");

	return 0;
}

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
	
	int on=1;
	
	// 设置复用address
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&on,sizeof(on))<0)
		ERR_EXIT("set socket");
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind");
	if(listen(listenfd,SOMAXCONN)<0)
		ERR_EXIT("listen");
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen=sizeof(peeraddr);
	int conn; 

	while(1){	
		if((conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
			ERR_EXIT("accept");
		printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
		
		//启用多线程
		pthread_t tid;
		int ret;
		if((ret=pthread_create(&tid,NULL,thread_routine,(void*)&conn))!=0){
			fprintf(stderr,"pthread_create: %s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
		close(listenfd);
		do_service(conn);
		pthread_kill(tid,SIGUSR1);


		close(conn);
		break;
	}
	
	
	return 0.;
}
