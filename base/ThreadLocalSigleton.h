

template<typename T>
class ThreadLocalSigleton
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

};



全局单例：
	线程1：instance
	线程2：instance
	返回同一个 t_value_
	
线程单例：
	线程1：instance  instance任意次...       t_value_
	线程2：instance		  t_value_

__thread变量：
	线程1执行到此处，创建一个 t_value_
	线程2执行到此处，创建一个 t_value_

	
template<typename T>
class ThreadLocalSigleton{
public:
	ThreadLocalSigleton()
	{
		pthread_key_create(&pkey_, &destructor);
	}	
	~ThreadLocalSigleton()
	{
		pthread_key_delete(&pkey_);
	}
	
	static void destructor(void* obj)
	{
		assert(obj== t_value_);
		typedef char T_must_be_complete_type[sizeof(T)==0  ?  -1  :  1];
		T_must_be_complete_type dummy; (void) dummy;
		// assert(sizeof(T)>0);
		delete t_value_;
		t_value_=0;
	}
		
	static T& instance()
	{
		void* t=pthread_getspecific(pkey_);
		if(t==NULL)
		{
			t=new T();
			pthread_setspecific(pkey_, t);
		}
		return *t;
	}
	
private:
	pthread_key_t pkey_; // 必须先 pthread_key_create
	
};
