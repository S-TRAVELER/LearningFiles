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
void handler(int sig){
	printf("recv a sig=%d\n",sig);
	exit(EXIT_SUCCESS);
}


int main(void){
	int sock;
	if((sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		ERR_EXIT("socket");
	}	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5188);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//inet_aton("127.0.0.1",&servaddr.sin_addr);
	if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		ERR_EXIT("connect");
	}


	pid_t pid;
	pid=fork();
	if(pid==-1){
		ERR_EXIT("fork");
	}
	if(pid==0){
		signal(SIGUSR1,handler);
		printf("[ELSE] my pid: %d, my parent pid: %d\n",getpid(),getppid());
		char sendbuf[1024]={0};
		while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL){
			write(sock,sendbuf,strlen(sendbuf));
			memset(sendbuf,0,sizeof(sendbuf));
		}
		printf("child close\n");
		exit(EXIT_SUCCESS);
	}
	else{
		printf("[PID0] my pid: %d, my parent pid: %d\n",getpid(),getppid());
		char recvbuf[1024]={0};
		while(1){
			int ret=read(sock,recvbuf,sizeof(recvbuf));
			if(ret==-1){
				ERR_EXIT("read");
			}else if(ret==0){
				printf("peer close\n");
				break;
			} 
			fputs(recvbuf,stdout);
			memset(recvbuf,0,sizeof(recvbuf));
		}
		close(sock);
		kill(pid,SIGUSR1);
		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
	

