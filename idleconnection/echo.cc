#include"echo.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>

#include <assert.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;


EchoServer::EchoServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       int idleSeconds)
  : server_(loop, listenAddr, "EchoServer"),
    connectionBuckets_(idleSeconds)
{
  server_.setConnectionCallback(
      boost::bind(&EchoServer::onConnection, this, _1));
	//  server_.setConnectionCallback(std::bind());
  server_.setMessageCallback(
      boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
	loop->runEvery(1.0, boost::bind(&EchoServer::onTimer, this));
  connectionBuckets_.resize(idleSeconds);
  dumpConnectionBuckets();
}

void EchoServer::start()
{
  server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn) // boost::shared_ptr<TcpConnection>
{
	if(conn->connected())
	{
		WeakTcpConnectionPtr wp(conn);
		EntryPtr ep(new Entry(wp));
		
		//EntryPtr ep(new Entry(conn));
		
		 
		connectionBuckets_.back().insert(ep);
		dumpConnectionBuckets();
		conn->setContext(WeakEntryPtr(ep));
	}
	else
	{
		WeakEntryPtr wp(boost::any_cast<WeakEntryPtr>(conn->getContext()));
		LOG_DEBUG << "Entry use_count = " << wp.use_count();
	}
}



void EchoServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp time)
{
	  string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " echo " << msg.size()
           << " bytes at " << time.toString();
  conn->send(msg);
  
	WeakEntryPtr wp=boost::any_cast<WeakEntryPtr>(conn->getContext());
	//WeakEntryPtr wp(conn->getContext());
	EntryPtr ep=wp.lock();
	if(ep)
	{
		connectionBuckets_.back().insert(ep);
		dumpConnectionBuckets();
	}
}



void EchoServer::onTimer()
{
	connectionBuckets_.push_back(Bucket());
	dumpConnectionBuckets();
}


void EchoServer::dumpConnectionBuckets() const
{
	printf("size=%d\n", connectionBuckets_.size());
	int idx=0;
	//for(int i=0; i<connectionBuckets_.size(); i++)
	for(WeakConnectionList::const_iterator bucketI=connectionBuckets_.begin(); bucketI!=connectionBuckets_.end(); ++bucketI, idx++)
	{
		const Bucket& bucket=*bucketI; //connectionBuckets_[i];
		//printf("%d:\n", i);
		printf("%d, len=%d\n", idx, bucket.size());
		for(Bucket::const_iterator itr=bucket.cbegin(); itr!=bucket.cend(); itr++)
		{
			bool connectedDead=(*itr)->weakConn_.expired();
			printf("%p %ld \n", itr->get(), itr->use_count(), connectedDead ? "DEAD" : "");
		}
		puts("");
	}
}







