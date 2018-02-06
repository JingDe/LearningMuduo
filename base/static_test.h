#ifndef STATIC_TEST_H
#define STATIC_TEST_H

#include <assert.h>
#include<iostream>
#include <pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>

class ThreadLocalSingleton 
{
 public:

  static pid_t gettid()
  {
    return static_cast<pid_t>(syscall(SYS_gettid));
  }
  static int& instance()
  {
    if (!t_value_)
    {
      printf("%d, instance()\n", gettid());
      t_value_ = new int();
      //deleter_.set(t_value_);
    }
    return *t_value_;
  }

  static int* pointer()
  {
    return t_value_;
  }

 private:
  ThreadLocalSingleton();
  ~ThreadLocalSingleton();

  static void destructor(void* obj) // 主线程不调用，每个子线程调用释放各自线程本地变量
  {
    printf("%d, destructor\n", ThreadLocalSingleton::gettid());
   // std::assert(obj == t_value_);
    typedef char T_must_be_complete_type[sizeof(int) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete t_value_;
    t_value_ = 0;
  }

  class Deleter
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

    void set(int* newObj)
    {
      //std::assert(pthread_getspecific(pkey_) == NULL);
      pthread_setspecific(pkey_, newObj);
    }

    pthread_key_t pkey_;
  };

  class Deleter2
  {
  public:
    Deleter2()
    {
      //printf("%d: Deleter2\n", gettid());
      std::cout<<ThreadLocalSingleton::gettid()<<", Deleter2()\n";
    }
    ~Deleter2()
    {
      //printf("%d: ~Deleter2\n", gettid());     
      std::cout<<ThreadLocalSingleton::gettid()<<", ~Deleter2()\n";
      if(ThreadLocalSingleton::t_value_)
      {
        delete ThreadLocalSingleton::t_value_;
      }
      ThreadLocalSingleton::t_value_=0;
    }
  };

  static __thread int* t_value_;
  //static Deleter deleter_; // 进程下所有线程共有, 主线程构造和析构
  static Deleter2 deleter_;
};


__thread int* ThreadLocalSingleton::t_value_ = 0;

ThreadLocalSingleton::Deleter2 ThreadLocalSingleton::deleter_;


#endif