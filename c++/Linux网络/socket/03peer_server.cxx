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

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

struct packet{
	int len;
	char buf[1024];
};

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
		}
		bufp+=nwrite;
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

void handler(int sig){
	printf("recv sig=%d\n",sig);
	exit(EXIT_SUCCESS);
}
	
int main(){
	int listenfd;
	if((listenfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		ERR_EXIT("socket");
	}

	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family+AF_INET;
	servaddr.sin_port=htons(5188);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	int on=1;
	
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&on,sizeof(on))<0)
		ERR_EXIT("SET SOCKET");
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind");
	if(listen(listenfd,SOMAXCONN)<0)
		ERR_EXIT("listen");
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen=sizeof(peeraddr);
	int conn;

	// helo world
	pid_t pid;
	while(1){
		if((conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
			ERR_EXIT("accept");
		printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));	

	 	pid=fork();
		if(pid==-1){
			ERR_EXIT("fork");
		}
		if(pid==0){
			signal(SIGUSR1,handler);
			printf("[PID0] my pid=%d, my parent pid=%d\n",getpid(),getppid());
			
			struct packet sendbuf;
			memset(&sendbuf,0,sizeof(sendbuf));
			while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin)!=NULL){
				int n=strlen(sendbuf.buf);
				//host to net unsigned long 小端转到大端
				sendbuf.len=htonl(n);
				writen(conn,&sendbuf,n+4);
				memset(&sendbuf,0,sizeof(sendbuf));
			}
			printf("child close\n");
			exit(EXIT_SUCCESS);
		}
		else{
			printf("[ELSE] my pid=%d, my parent pid=%d\n",getpid(),getppid());
			
			close(listenfd);
			do_service(conn);
			kill(pid,SIGUSR1);
			close(conn);
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}
