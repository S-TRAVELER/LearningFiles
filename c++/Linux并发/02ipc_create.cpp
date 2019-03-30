#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

/*
* 注意：消息队列(ipc)是随内核持续的!!!
* ipc 查看：ipcs 管理：ipcrm 销毁消息队列：ipcrm -q msgid / ipcrm -Q key
*/

int main(void){

/*
* 1. int msgget(key_t key,int msgflg); //用来创建和访问一个消息队列,返回值为队列的标识，失败则是-1 @key 消息队列名字 @msgflg 权限控制
*/
	int msgid;
	msgid=msgget(1234,0666|IPC_CREAT);	//创建并打开消息，已存在则打开队列
//	msgid=msgget(1234,0666|IPC_CREAT|IPC_EXCL);	//
//	msgid=msgget(IPC_PRIVATE,0666|IPC_CREAT|IPC_EXCL);	//线程私有消息队列
//	msgid=msgget(IPC_PRIVATE,0666);		//可以不用PC_CREAT|IPC_EXC
//	msgid=msgget(1234,0400|IPC_CREAT);	//可以用不同权限，但不能超过权限



/*
* 2. int msgctl(int msqid, int cmd,struct msqid_ds* buf); //消息队列控制函数 @msgge：由msgget函数返回的消息队列的标识码 @cmd 将采取的动作（有三个可取值） @return succ返回0, failed返回-1
*/

// (1) IPC_STAT 消息队列状态
	if(msgid==-1){
		ERR_EXIT("mssget");
	}
	printf("msgget successfully!\n");
	printf("msgid=%d\n",msgid);

	struct msqid_ds buf;
	if(0==msgctl(msgid,IPC_STAT,&buf)){
		printf("mod=%o\n",buf.msg_perm.mode);	//权限模式
		printf("bytes=%ld\n",buf.__msg_cbytes);	//消息队列字节大小
		printf("number=%d\n",(int)buf.msg_qnum);	//消息队列消息数量
		printf("msgmnb=%d\n",(int)buf.msg_qbytes);	//单个消息最大容量
	}

// (2) IPC_SET 设置消息队列
/*	sscanf("600","%o",(unsigned int*)&buf.msg_perm.mode);
	msgctl(msgid,IPC_SET,&buf);
	
	struct msqid_ds newbuf;
	if(0==msgctl(msgid,IPC_STAT,&newbuf)){
		printf("new mod=%o\n",newbuf.msg_perm.mode);	//权限模式
	}
*/

// (3) IPC_RMID 删除消息队列
/*	if(0==msgctl(msgid,IPC_RMID,NULL))
		printf("removed ipc: %d\n",msgid);

*/


	return 0;
}

