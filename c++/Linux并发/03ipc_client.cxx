#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
{ \
	perror(m); \
	exit(EXIT_FAILURE); \
}
#define MSGMAX 8194

struct msgBuf{
	long mtype;
	char mtext[MSGMAX];
};

void echo_cli(int msgid){
	int n=0;
	struct msgBuf msg;
	memset(&msg,0,sizeof(msg));
	
	int pid=getpid();
	while(1){

		msg.mtype=1;
		*((int*)msg.mtext)=pid;
		if(fgets(msg.mtext+4,MSGMAX,stdin)==NULL){
			ERR_EXIT("fget");
		}

		if((n=msgsnd(msgid,&msg,strlen(msg.mtext+4)+4,0))<0){
			ERR_EXIT("msgsnd");
		}
		memset(&msg,0,sizeof(long)+MSGMAX);

		if((n=msgrcv(msgid,&msg,MSGMAX,pid,0))<0){
			ERR_EXIT("msgrcv");
		}
		fputs(msg.mtext+4,stdout);

		memset(&msg,0,sizeof(long)+MSGMAX);

	}
}

int main(){
	int msgid;
	msgid=msgget(1234,0);
	if(msgid==-1){
		ERR_EXIT("msgget");
	}
	echo_cli(msgid);
	return 0;
}

