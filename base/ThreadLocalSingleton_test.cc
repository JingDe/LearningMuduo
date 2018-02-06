
#include "ThreadLocalSingleton.h"
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<stdlib.h>


void pthread_Call(const char* label, bool result)
{
	if(result)
	{
		printf("%s\n", label);
		abort();
	}
}

pid_t gettid()
{
	return static_cast<pid_t>(syscall(SYS_gettid));
}

void* routine(void *arg)
{
	int& t1=ThreadLocalSingleton<int>::instance();
	
	int& t2=ThreadLocalSingleton<int>::instance();
	
	double& t3=ThreadLocalSingleton<double>::instance();
	
	printf("%d %p %p %p\n", gettid(), &t1, &t2, &t3);
	printf("%d %d %d %f\n", gettid(), t1, t2, t3);
	t1=3;
	printf("%d %d %d %f\n", gettid(), t1, t2, t3);
	
	return (void*)0; 
}

int main()
{
	pthread_t pt[2];
	int len=sizeof(pt)/sizeof(pthread_t);
	routine(NULL);
	
	for(int i=0; i<len; i++)
		pthread_Call("create thread1", pthread_create(&pt[i], NULL, routine, NULL));
	
	for(int i=0; i<len; i++)
		pthread_join(pt[i], NULL);	
	return 0;
}