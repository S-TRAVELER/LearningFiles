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

int main(int argc,char *argv[]){
	if(argc==2){
		mq_unlink(argv[1]);
		printf("unlink ipc: %s\n", argv[1]);
	}
	return 0;
}
