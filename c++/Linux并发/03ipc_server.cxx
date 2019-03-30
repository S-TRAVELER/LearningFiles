/* 
 * 1. 查看ipc：ipcs
 * 2. 删除ipc：ipcrm -Q key 或 ipcrm -q msgid 
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

#define MSGMAX 8192
struct msgBuf{
	long mtype; /*message type, must be >0*/
	char mtext[MSGMAX];	/*message data*/
};

void echo_srv(int msgid)	//回射服务
{
	int n=0;
	struct msgBuf msg;
	memset(&msg,0,sizeof(msg));
	while(1){
		if((n=msgrcv(msgid,&msg,MSGMAX,1,0))<0){
			ERR_EXIT("msgsnd");
		}

		int pid;
		pid=*((int*)msg.mtext);	//前四个字节为进程id

		fputs(msg.mtext+4,stdout);
		msg.mtype=pid;
		msgsnd(msgid,&msg,n,0);
		memset(&msg,0,sizeof(msg));
	}
}

int main(){
	int msgid;
	msgid=msgget(1234,IPC_CREAT|0666);
	if(msgid==-1){
		ERR_EXIT("msgget");
	}
	echo_srv(msgid);
	return 0;
}
