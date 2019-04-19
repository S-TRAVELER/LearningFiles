#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

typedef struct stu{
	char name[32];
	int age;
}STU;

int main(int argc,char* argv[]){
	if(argc!=2){
		fprintf(stderr,"Usage: %s <shm_name>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	shm_unlink(argv[1]);

	return 0;
}
