
#ifndef LEARNINGMUDUO_THREADLOCALSIGLETON_H2_
#define LEARNINGMUDUO_THREADLOCALSIGLETON_H2_

#include<pthread.h>
#include<assert.h>

template<typename T>
class ThreadLocalSingleton{
public:
	ThreadLocalSingleton()
	{
		pthread_key_create(&pkey_, &destructor);
	}	
	~ThreadLocalSingleton()
	{
		pthread_key_delete(pkey_);
	}
	
	static void destructor(void* obj)
	{
		void* t=pthread_getspecific(pkey_);
		if(t!=NULL)
		{
			typedef char T_must_be_complete_type[sizeof(T)==0  ?  -1  :  1];
			T_must_be_complete_type dummy; (void) dummy;
			// assert(sizeof(T)>0);
			delete reinterpret_cast<T*>(t);
		}
	}
		
	static T& instance()
	{
		void* t=pthread_getspecific(pkey_);
		if(t==NULL)
		{
			t=new T();
			pthread_setspecific(pkey_, t);
		}
		return *(reinterpret_cast<T*>(t));
	}
	
private:
	static pthread_key_t pkey_; 
	
	ThreadLocalSingleton(const ThreadLocalSingleton&);
	void operator=(const ThreadLocalSingleton&);
};

template<typename T>
pthread_key_t ThreadLocalSingleton<T>::pkey_;

// 进程主线程A： ThreadLocalSingleton tls;  T t1=tls.instance();

// 线程B：T t2=tls.instance();

#endif