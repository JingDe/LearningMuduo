
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
		(*it)->print(); // 把print() 移出printAll()的临界区
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
		g_inventory.remove(this); // 交 换这两行
	}
	
	void print() const __attribute__ ((noinline))
	{
		muduo::MutexLockGuard lock(mutex_);
		//
	}
	
private:
	mutable muduo::MutexLock mutex_;
};











