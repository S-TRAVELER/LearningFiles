#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}
#define MSGMAX 1000

int  main(int argc,char* argv[]){
	int flag=0;
	int type=0;
	int opt;

	while(1){
		opt=getopt(argc,argv,"nt:");
		if(opt=='?'){
			ERR_EXIT("GetOpt");
		}else if(opt==-1){
			break;
		}
		switch(opt){
		case 'n'://printf("AAA\n");
			flag|=IPC_NOWAIT;
			break;
		case 't':/*printf("BBB\n"); 
			int n=atoi(optarg);
			printf("n=%d\n",n);*/
			type=atoi(optarg);
			printf("type=%d\n",type);
			break;
		}
	}

	int msgid;
	msgid=msgget(1234,0);
	if(msgid==-1){
		ERR_EXIT("msgget");
	}
	
  	struct msgbuf* ptr;
	ptr=(struct msgbuf*) malloc(sizeof(long)+MSGMAX);
	ptr->mtype=type;
	int n=0;
	if((n=msgrcv(msgid,ptr,MSGMAX,type,flag))<0)
		ERR_EXIT("msgsnd");
	printf("read %d bytes type = %ld\n",n,ptr->mtype);
	return 0;
}
