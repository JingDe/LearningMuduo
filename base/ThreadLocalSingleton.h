
#ifndef LEARNINGMUDUO_THREADLOCALSIGLETON_H_
#define LEARNINGMUDUO_THREADLOCALSIGLETON_H_

#include<stdlib.h>

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
	
private:
	static __thread T* t_value_;

	ThreadLocalSingleton(const ThreadLocalSingleton&);
	void operator=(const ThreadLocalSingleton&);
};
template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_=NULL;

// 线程A: T& t1=ThreadLocalSingleton<T>::instance(); // 线程1执行到此处，创建一个 t_value_

// 线程B：T& t2=ThreadLocalSingleton<T>::instance(); // 线程2执行到此处，创建一个 t_value_



#endif


	

