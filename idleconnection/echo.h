#ifndef MUDUO_EXAMPLES_IDLECONNECTION_ECHO_H
#define MUDUO_EXAMPLES_IDLECONNECTION_ECHO_H

#include <muduo/net/TcpServer.h>
//#include <muduo/base/Types.h>

#include <boost/circular_buffer.hpp>
#include <boost/unordered_set.hpp>
//#include<unordered_set>
#include <boost/version.hpp>
//#include<memory>


#if BOOST_VERSION < 104700
namespace boost{
	template<typename T>
	inline sizze_t hash_value(const boost::shared_ptr<T>& x)
	{
		return boost::hash_value(x.get());
	}
}
#endif


class EchoServer
{
 public:
  EchoServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             int idleSeconds);

  void start();

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp time);

  void onTimer();

  void dumpConnectionBuckets() const; 
  
  typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;
  
	struct Entry
	{	
		Entry(const WeakTcpConnectionPtr& conn)
		:weakConn_(conn)
		{
			
		}
		~Entry()
		{
			boost::shared_ptr<muduo::net::TcpConnection> sConn=weakConn_.lock();
			if(sConn)
				sConn->shutdown();
		}

		WeakTcpConnectionPtr weakConn_;
	};
	
	
private:
	typedef boost::shared_ptr<Entry> EntryPtr;
	typedef boost::weak_ptr<Entry> WeakEntryPtr;
	typedef boost::unordered_set<EntryPtr> Bucket;
	typedef boost::circular_buffer<Bucket> WeakConnectionList;
	
	WeakConnectionList connectionBuckets_;
	muduo::net::TcpServer server_;
};

#endif