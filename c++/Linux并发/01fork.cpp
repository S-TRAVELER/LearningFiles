#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	pid_t pid,ppid;
	pid=getpid();
	ppid=getppid();

	printf("当前进程的ID号为：%d\n",pid);
	printf("当前进程的父进程号ID号为：%d\n",ppid);

	pid=fork();
	if(-1==pid){
		printf("进程创建失败！\n");
		return -1;
	}
	else if(pid==0){
		/*子进程执行*/
		printf("子进程，fork返回值：%d，ID：%d, 父进程ID：%d\n",pid,getpid(),getppid());
	}
	else {
		printf("父进程，fork返回值：%d, ID: %d, 父进程ID：%d\n",pid,getpid(),getppid());
	}

	return 0;
}
