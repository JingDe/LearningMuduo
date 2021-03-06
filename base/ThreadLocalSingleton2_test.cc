
#include "ThreadLocalSingleton2.h"
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<stdlib.h>

	ThreadLocalSingleton<int> tls1;
	ThreadLocalSingleton<double> tls2;

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
	int& t1=tls1.instance();
	
	int& t2=tls1.instance();
	
	double& t3=tls2.instance();
	
	printf("%d %p %p %p\n", gettid(), &t1, &t2, &t3);
	printf("%d %d %d %f\n", gettid(), t1, t2, t3);
	t1=3;
	printf("%d %d %d %f\n", gettid(), t1, t2, t3);
	
	return (void*)0; // NULL;
}


	
int main()
{
	pthread_t pt1;

	routine(NULL);
	
	
	pthread_Call("create thread1", pthread_create(&pt1, NULL, routine, NULL));
	
	pthread_join(pt1, NULL);	
	return 0;
}