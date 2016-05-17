#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker( void* p){
	printf("Hello from worker\n");
	return(NULL);
}

int main(){
	int i;
	pthread_t OtherThreads[4];
	for(i=0; i<4; i++){
		pthread_create(&OtherThreads[i], NULL, worker, NULL);
	}
	printf("Hello main\n");

	for(i=0; i<4; i++){
		pthread_join(OtherThreads[i], NULL);
	}
	return(0);
}