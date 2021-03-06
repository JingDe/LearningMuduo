
#ifndef MUDUO_BASE_THREADLOCALSINGLETON_H
#define MUDUO_BASE_THREADLOCALSINGLETON_H

#include <assert.h>
#include <pthread.h>
#include<stdio.h>

template<typename T>
class ThreadLocalSingleton 
{
 public:

  static T& instance()
  {
    if (!t_value_)
    {
      printf("instance()\n");
      t_value_ = new T();
      deleter_.set(t_value_);
    }
    printf("return\n");
    return *t_value_;
  }

  static T* pointer()
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
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
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

    void set(T* newObj)
    {
      printf("set\n");
      //std::assert(pthread_getspecific(pkey_) == NULL);
      pthread_setspecific(pkey_, newObj);
    }

    pthread_key_t pkey_;
  };

  static __thread T* t_value_;
  static Deleter deleter_;
};

template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_ = 0;

template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;


#endif