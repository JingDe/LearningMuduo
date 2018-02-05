#ifndef STATIC_TEST_H
#define STATIC_TEST_H

#include <assert.h>
#include <pthread.h>
#include<stdio.h>


class ThreadLocalSingleton 
{
 public:

  static int& instance()
  {
    if (!t_value_)
    {
      printf("instance()\n");
      t_value_ = new int();
      deleter_.set(t_value_);
    }
    printf("return\n");
    return *t_value_;
  }

  static int* pointer()
  {
    return t_value_;
  }

 private:
  ThreadLocalSingleton();
  ~ThreadLocalSingleton();

  static void destructor(void* obj)
  {
    printf("destructor\n");
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
      printf("Deleter\n");
      pthread_key_create(&pkey_, &ThreadLocalSingleton::destructor);
    }

    ~Deleter()
    {
      printf("~Deleter\n");
      pthread_key_delete(pkey_);
    }

    void set(int* newObj)
    {
      printf("set\n");
      printf("this=&p\n", this);
      //std::assert(pthread_getspecific(pkey_) == NULL);
      pthread_setspecific(pkey_, newObj);
    }

    pthread_key_t pkey_;
  };

  static __thread int* t_value_;
  static Deleter deleter_;
};


__thread int* ThreadLocalSingleton::t_value_ = 0;

ThreadLocalSingleton::Deleter ThreadLocalSingleton::deleter_;


#endif