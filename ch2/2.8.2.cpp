
typedef std::shared_ptr<Request*> RequestPtr;

class Inventory{
public:
	void add(RequestPtr req) // write端
	{
		MutexLockGuard lock(mutex_);
		if(!requests_.unique())
		{
			requests_.reset(new std::set<RequestPtr>(*requests_));
		}
		requests_.insert(req);
	}
	void remove(RequestPtr req) // write端
	{
		MutexLockGuard lock(mutex_);
		if(!requests_.unique())
		{
			requests_.reset(new std::set<RequestPtr>(*requests_));
		}
		requests_.erase(req);
	}

private:
	mutable muduo::MutexLock mutex_;
	std::shared_ptr<std::set<RequestPtr> > requests_;
};

Inventory g_inventory;

void Inventory::printAll() const // read端
{
	std::shared_ptr<std::set<RequestPtr> > requests;
	{
		MutexLockGuard lock(mutex_);
		requests=requests_;
	}
	for(std::set<RequestPtr>::const_iterator it=*requests.begin(); it!=*requests.end(); it++)
	{
		(*it)->print(); 
	}
}


class Request : public std::enable_shared_from_this<Request> {
public:
	void process()
	{
		muduo::MutexLockGuard lock(mutex_);
		g_inventory.add(shared_from_this());
	}
	~Request() __attribute__ ((noinline))
	{
		muduo::MutexLockGuard lock(mutex_);
		g_inventory.remove(shared_from_this()); // 解法二：交换这两行
	}
	
	void print() const __attribute__ ((noinline))
	{
		muduo::MutexLockGuard lock(mutex_);
		//
	}
	
private:
	mutable muduo::MutexLock mutex_;
};
