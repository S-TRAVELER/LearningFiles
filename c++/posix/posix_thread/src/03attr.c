#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>  	// random()
#include <string.h>	// strerror()

#define NUM_THREADS 3

void *thread_routine(void *arg){
	double result=0.0;
	for(int i=0;i<1000;++i){
		result+=(double)random();
	}
	printf("result = %e\n",result);
	return 0;
}

int main(int argc,char* argv[]){
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;

	int ret;
	void* status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	for(int i=0;i<NUM_THREADS;++i){
		printf("Creating thread %d\n",i);
		ret=pthread_create(&threads[i],&attr,thread_routine,NULL);
		if(ret!=0){
			fprintf(stderr,"pthread_create: %s\n",strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	pthread_attr_destroy(&attr);
	for(int i=0;i<NUM_THREADS;++i){
		ret=pthread_join(threads[i],&status);
		if(ret!=0){
			fprintf(stderr,"pthread_destroy: %s\n",strerror(ret));
			exit(EXIT_FAILURE);
		}

		printf("Completed join with thread %d status = %ld\n",i,(long)status);
	}

	return 0;
}

