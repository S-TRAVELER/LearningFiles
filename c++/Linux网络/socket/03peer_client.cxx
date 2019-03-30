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

struct packet{
	int len;
	char buf[1024];
};

void handler(int sig){
	printf("recv a sig=%d\n",sig);
	exit(EXIT_SUCCESS);
}

size_t readn(int fd,void* buf,size_t count){
	size_t nleft=count;
	size_t nread=0;
	char* bufp=(char*)buf;
	while(nleft>0){
		if((nread=read(fd,buf,nleft))<0){
			if(errno=EINTR)
				continue;
			return -1;
		}
		else if(nread==0){
			return count-nleft;
		}
		bufp+=nread;
		nleft-=nread;
	}
	return count;
}

size_t writen(int fd,void* buf,size_t count){
	size_t nleft=count;
	size_t nwrite=0;
	char* bufp=(char*)buf;
	while(nleft>0){
		if((nwrite=write(fd,buf,nleft))<0){
			ERR_EXIT("wirte");
		}		bufp+=nwrite;
		nleft-=nwrite;
	}
	return count;
}


void do_service(int conn){
	struct packet recvbuf;
	while(1){
		memset(&recvbuf,0,sizeof(recvbuf));
		int ret=readn(conn,&recvbuf.len,4);
		if(ret==-1){
			ERR_EXIT("read");
		}
		if(ret==0){
			printf("peer close\n");
			break;
		}
		int n=ntohl(recvbuf.len);
		ret=readn(conn,recvbuf.buf,n);
		if(ret==-1){
			ERR_EXIT("read");
		}
		if(ret==0){
			printf("peer close\n");
			break;
		}
		printf("recv [%d bytes]:",n);
		fputs(recvbuf.buf,stdout);
	}
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
		struct packet sendbuf;
		memset(&sendbuf,0,sizeof(sendbuf));

		while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin)!=NULL){
			int n=strlen(sendbuf.buf);
			//host to net unsigned long 小端转到大端
			sendbuf.len=htonl(n);
			writen(sock,&sendbuf,n+4);
			memset(&sendbuf,0,sizeof(sendbuf));
		}
		printf("child close\n");
		exit(EXIT_SUCCESS);
	}
	else{
		printf("[PID0] my pid: %d, my parent pid: %d\n",getpid(),getppid());
		do_service(sock);
		close(sock);
		kill(pid,SIGUSR1);
		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
	

