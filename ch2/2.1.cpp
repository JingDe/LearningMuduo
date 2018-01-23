// 案例一
MutexLock mutex;
std::vector<Foo> foos;

void post(const Foo& f)
{
	MutexLockGuard lock(mutex);
	foos.push_back(f);
}

void traverse()
{
	MutexLockGuard lock(mutex);
	for(std::vector<Foo>::const_iterator it=foos.begin(); it!=foos.end(); it++)
	{
		it->doit(); // 间接调用post()
	}
}

mutex非递归，死锁
mutex递归，push_back可能导致vector迭代器失效，crash


//解法一：
void post(const Foo& f)
{
	MutexLockGuard lock(mutex);
	postWithLockHold(f);
}

void postWithLockHold(const Foo& f)
{
	foos.push_back(f);
}



// 案例二
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
	muduo::MutexLockGuard lock(mutex_);
	for(std::set<Request*>::const_iterator it=requests_.begin(); it!=requests_.end(); it++)
	{
		(*it)->print(); // 解法一：把print() 移出printAll()的临界区
	}
}


Inventory g_inventory;




class Request{
public:
	void process()
	{
		muduo::MutexLockGuard lock(mutex_);
		g_inventory.add(this);
	}
	~Request() __attribute__ ((noinline))
	{
		muduo::MutexLockGuard lock(mutex_);
		g_inventory.remove(this); // 解法二：交换这两行
	}
	
	void print() const __attribute__ ((noinline))
	{
		muduo::MutexLockGuard lock(mutex_);
		//
	}
	
private:
	mutable muduo::MutexLock mutex_;
};


问题：
1) post()和traverse()死锁；
2) 把Request::print()移出Inventory::printAll()的临界区；
3) 解决Request对象析构的race condition；










