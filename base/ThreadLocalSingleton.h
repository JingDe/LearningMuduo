
#ifndef LEARNINGMUDUO_THREADLOCALSIGLETON_H_
#define LEARNINGMUDUO_THREADLOCALSIGLETON_H_

#include<stdio.h>
#include<sys/syscall.h>



template<typename T>
class ThreadLocalSingleton
{
public:
	static T& instance()
	{
		if(!t_value_)
		{
			t_value_=new T();
			
		}
		return *t_value_;
	}

	static pid_t gettid()
	{
		return static_cast<pid_t>(syscall(SYS_gettid));
	}

/*
	class Deleter{
	public:
		Deleter()
		{
			printf("%d: Deleter\n", gettid());
		}
		~Deleter()
		{
			printf("%d: ~deleter\n", gettid());
			if(ThreadLocalSingleton::t_value_)
			{
				printf("%d: delete ThreadLocalSingleton\n", gettid());
				delete ThreadLocalSingleton::t_value_;
			}
			ThreadLocalSingleton::t_value_=0;
		}
	};*/
	class Deleter{
	public:
		Deleter()
		{
			pthread_key_create(&pkey_, &ThreadLocalSingleton::destructor);
		}
	};

	
private:
	static __thread T* t_value_; // 

	static Deleter deleter_; // 进程下所有线程共有

	ThreadLocalSingleton(const ThreadLocalSingleton&);
	void operator=(const ThreadLocalSingleton&);
};
template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_=0;
template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;

#endif
