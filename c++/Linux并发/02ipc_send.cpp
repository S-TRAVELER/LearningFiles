#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

int main(int argc,char* argv[]){
	if(argc!=3){
		fprintf(stderr,"Usage: %s <bytes> <type>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int len=atoi(argv[1]);
	int type=atoi(argv[2]);
	int msgid=msgget(1234,0);
	if(msgid==-1){
		ERR_EXIT("msgget");
	}


/*
* 1. int msgsnd(int msqid, const void* msgp, size_t msgsz, msgflg);
* @msgid 由msgget函数返回的消息队列标识
* @msgp 是一个指针，指向准备发送的消息
* @msgsz 是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
* struct msgbuf{
8	long int mtype;
*	char mtext[1];
* }
* @msgflg 控制着当前消息队列满或达到系统上限时将要发生的事情
* @return 成功 0 失败 -1
*/

	struct msgbuf* ptr;
	ptr=(struct msgbuf*)malloc(sizeof(long)+len);
	ptr->mtype=type;
	if(0>msgsnd(msgid,ptr,len,0)){
		ERR_EXIT("msgsnd");
	}
	fprintf(stdout,"%s","send successfully\n");


/*
* 2. ssize_t msgrcv(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg);
* @msgid 由msgget函数返回的消息队列标识
* @msgp 是一个指针，指向准备接收的消息
* @msgsz 是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
* struct msgbuf{
8	long int mtype;
*	char mtext[1];
* }
* @msgflg 控制着当前消息队列满或达到系统上限时将要发生的事情
* @return 成功 返回实际放到缓存的字符数 失败 -1
*/
	
	return 0;
}
