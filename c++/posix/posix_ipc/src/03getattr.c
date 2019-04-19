#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

int main(void){
	mqd_t mqid;
	mqid=mq_open("/abc",O_CREAT|O_RDWR,0666,NULL);
	if(mqid==(mqd_t)-1){
		printf("打开队列失败\n");
	}
	else{
		printf("打开队列成功\n");
	}
	struct mq_attr attr;
	mq_getattr(mqid,&attr);	//获取属性
	printf("max #msg=%ld max #bytes/msg=%ld #currently on queue=%ld\n",attr.mq_maxmsg,attr.mq_msgsize,attr.mq_curmsgs);

	mq_close(mqid);	//只是关闭，并没有销毁


	return 0;
}
