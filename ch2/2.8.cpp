
1) post()和traverse()死锁；
typedef std::vector<Foo> FooList;
typedef std::shared_ptr<FooList> FooListPtr;
MutexLock mutex;
FooListPtr g_foos;

// read端
void traverse()
{
	FooListPtr foos;
	{
		MutexLockGuard lock(mutex);
		foos=g_foos;
		asset(!g_foos.unique());
	}
	
	for(std::vector<Foo>::const_iterator it=foos->begin(); it!=foos->end(); it++)
	{
		it->doit();
	}
}

// write端
void post(const Foo& f)
{
	MutexLockGuard lock(mutex);
	if(!g_foos.unique())
	{
		g_foos.reset(new FooList(*g_foos));
	}
	assert(g_foos.unique());
	g_foos->push_back(f);
}




2) 把Request::print()移出Inventory::printAll()的临界区；
// 解法一：复制一份，在临界区外遍历副本
class Inventory{
public:
	void add(Request* req)
	{
		muduo::MutexLockGuard lock(mutex_);
		requests_.insert(req);
	}
	void remove(Request* req)
	{
		muduo::MutexLockGuard lock(mutex_);
		requests_.erase(req);
	}
	void printAll() const;

private:
	mutable muduo::MutexLock mutex_;
	std::set<Request*> requests_;
};
void Inventory::printAll() const
{
	std::set<Request*> requests;
	{
		muduo::MutexLockGuard lock(mutex_);
		requests=requests_; // 复制
	}
	for(std::set<Request*>::const_iterator it=requests.begin(); it!=requests.end(); it++)
	{
		(*it)->print(); 
	}
}

// 解法二：使用shared_ptr管理共享数据，遍历之前增加引用计数，阻止并发修改
class Inventory{
public:
	void add(Request* req) // write端
	{
		MutexLockGuard lock(mutex_);
		if(!requests_.unique())
		{
			requests_.reset(new std::set<Request*>(*requests_));
		}
		requests_.insert(req);
	}
	void remove(Request* req) // write端
	{
		MutexLockGuard lock(mutex_);
		if(!requests_.unique())
		{
			requests_.reset(new std::set<Request*>(*requests_));
		}
		requests_.erase(req);
	}

private:
	mutable muduo::MutexLock mutex_;
	std::shared_ptr<std::set<Request*> > requests_;
};

void Inventory::printAll() const // read端
{
	std::shared_ptr<std::set<Request*> > requests;
	{
		MutexLockGuard lock(mutex_);
		requests=requests_;
	}
	for(std::set<Request*>::const_iterator it=*requests.begin(); it!=*requests.end(); it++)
	{
		(*it)->print(); 
	}
}




3) 解决Request对象析构的race condition；

2.8.2.cpp






