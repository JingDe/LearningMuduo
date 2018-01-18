

#ifndef PROTOBUF_DISPATCHER_H
#define PROTOBUF_DISPATCHER_H


#include <muduo/net/Callbacks.h>

#include <google/protobuf/message.h>

#include <map>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#ifndef NDEBUG
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#endif


typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;
//typedef boost::shared_ptr<T> TPtr;

class Callback : boost::noncopyable
{
public:
/* 	typedef boost::function<void (const muduo::net::TcpConnectionPtr&, const MessagePtr&, muduo::Timestamp)> ProtobufMessageCallback;

	void operator()(const muduo::net::TcpConnectionPtr& conn, const MessagePtr& message, muduo::Timestamp tm)
	{
		callback_(conn, message, tm);
	}
	
private:
	ProtobufMessageCallback callback_; */
	virtual ~Callback() {}
	virtual void onMessage(const muduo::net::TcpConnectionPtr&, const MessagePtr& message, muduo::Timestamp) const =0;
};


template<typename T>
class CallbackT : public Callback
{
#ifndef NDEBUG
  BOOST_STATIC_ASSERT((boost::is_base_of<google::protobuf::Message, T>::value));
#endif

public:
	//typedef boost::function<void (const muduo::net::TcpConnectionPtr&, const TPtr&, muduo::Timestamp)> ProtobufMessageTCallback;
	typedef boost::function<void (const muduo::net::TcpConnectionPtr&, const boost::shared_ptr<T>&, muduo::Timestamp)> ProtobufMessageTCallback;
	
	CallbackT(const ProtobufMessageTCallback& callback)
	:callback_(callback)
	{}
	
/* 	void operator()(const muduo::net::TcpConnectionPtr& conn, const TPtr& message, muduo::Timestamp tm)
	{
		callback_(conn, message, tm);
	} */
	virtual void onMessage(const muduo::net::TcpConnectionPtr& conn, const MessagePtr& message, muduo::Timestamp receiveTime) const
	{
		boost::shared_ptr<T> concrete=muduo::down_pointer_cast<T>(message);
		assert(concrete !=NULL);
		callback_(conn, concrete, receiveTime);
	}

private:
	ProtobufMessageTCallback callback_;
};


//ProtobufCodec decode Buffer获得Message后，根据Message类型，调用不同的函数
class ProtobufDispatcher: boost::noncopyable{
public:
	typedef boost::function<void (const muduo::net::TcpConnectionPtr&, const MessagePtr&, muduo::Timestamp)> ProtobufMessageCallback;
	
	ProtobufDispatcher(const ProtobufMessageCallback& defaultCb)
	:defaultCallback_(defaultCb)
	{}

	template<typename T>
	void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageTCallback& callback)
	{
		boost::shared_ptr<CallbackT<T> > pd(new CallbackT<T>(callback)); // new CallbackT<T>(callback) 根据用户注册的 处理特定消息的函数 创建CallbackT对象
		callbacks_[T::descriptor()]=pd;
	}

	void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn, const MessagePtr& message, muduo::Timestamp receiveTime) const
	{
		const google::protobuf::Descriptor* desc=message->GetDescriptor();
		CallbackMap::const_iterator it=callbacks_.find(desc); // 所指东西不可被改动
		if(it!=callbacks_.end())
		{
			it->second->onMessage(conn, message, receiveTime);
		}
		else
		{
			defaultCallback_(conn, message, receiveTime);
		}
	}

private:
	typedef std::map<const google::protobuf::Descriptor*, boost::shared_ptr<Callback> > CallbackMap;
	//使用 shared_ptr 因为 这里Callback是一个类，对应的对象 可能被释放
	// 原来的 ProtobufDispatcherLite 是一个函数，函数的地址不会被释放
	CallbackMap callbacks_;
	ProtobufMessageCallback defaultCallback_;
};

#endif