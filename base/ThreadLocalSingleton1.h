
#ifndef LEARNINGMUDUO_THREADLOCALSIGLETON_H_
#define LEARNINGMUDUO_THREADLOCALSIGLETON_H_

#include<iostream>
#include<stdio.h>
#include<sys/syscall.h>
#include<unistd.h>
#include <assert.h>

template<typename T>
class ThreadLocalSingleton
{
public:
	static T& instance()
	{
		if(!t_value_)
		{
			std::cout<<gettid()<<", instance()\n";
			t_value_=new T();	
			
		}
		return *t_value_;
	}

	static pid_t gettid()
	{
		return static_cast<pid_t>(syscall(SYS_gettid));
	}

	static void destructor(void* obj)
    {
	    assert(obj == t_value_);
	    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
	    T_must_be_complete_type dummy; (void) dummy;
	    delete t_value_;
	    t_value_ = 0;
    }


	class Deleter2{
	public:
		Deleter2()
		{
			printf("%d, Deleter2=%p\n", ThreadLocalSingleton::gettid(), this);
		}
		~Deleter2()
		{
			printf("%d, ~Deleter2=%p\n", ThreadLocalSingleton::gettid(), this);
			if(ThreadLocalSingleton::t_value_)
			{
				delete ThreadLocalSingleton::t_value_;
			}
			ThreadLocalSingleton::t_value_=0;
		}

		bool dummy;
	};
	
  /*class Deleter
  {
   public:
    Deleter()
    {
      printf("%d, Deleter=%p\n", ThreadLocalSingleton::gettid(), this);
      pthread_key_create(&pkey_, &ThreadLocalSingleton::destructor);
    }

    ~Deleter()
    {
      printf("%d, ~Deleter=%p\n", ThreadLocalSingleton::gettid(), this);
      pthread_key_delete(pkey_);
    }

    void set(T* newObj)
    {
      //std::assert(pthread_getspecific(pkey_) == NULL);
      pthread_setspecific(pkey_, newObj);
    }

    pthread_key_t pkey_;
  };*/

	
private:
	static __thread T* t_value_; 

	static Deleter2 deleter2_; // 进程下所有线程共有


	ThreadLocalSingleton(const ThreadLocalSingleton&);
	void operator=(const ThreadLocalSingleton&);
};
template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_=0;
template<typename T>
typename ThreadLocalSingleton<T>::Deleter2 ThreadLocalSingleton<T>::deleter2_;

#endif
