
#ifndef PROTOBUF_DISPATCHER_H
#define PROTOBUF_DISPATCHER_H


#include <muduo/net/Callbacks.h>

#include <google/protobuf/message.h>

#include <map>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;

//ProtobufCodec decode Buffer获得Message后，根据Message类型，调用不同的函数
class ProtobufDispatcherLite : boost::noncopyable{
public:
	typedef boost::function<void (const muduo::net::TcpConnectionPtr&, const MessagePtr&, muduo::Timestamp)> ProtobufMessageCallback;
	// 要求注册的函数都是 ProtobufMessageCallback 类型的函数，这样的函数需要自己 对MessagePtr做转型
	
	ProtobufDispatcherLite(const ProtobufMessageCallback& defaultCb)
	:defaultCallback_(defaultCb)
	{}

	void registerMessageCallback(const google::protobuf::Descriptor* desc, const ProtobufMessageCallback& callback)
	{
		callbacks_[desc]=callback;
	}

	void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn, const MessagePtr& message, muduo::Timestamp receiveTime) const
	{
		const google::protobuf::Descriptor* desc=message->GetDescriptor();
		CallbackMap::const_iterator it=callbacks_.find(desc); // 所指东西不可被改动
		if(it!=callbacks_.end())
		{
			it->second(conn, message, receiveTime);
		}
		else
		{
			defaultCallback_(conn, message, receiveTime);
		}
	}

private:
	typedef std::map<const google::protobuf::Descriptor*, ProtobufMessageCallback> CallbackMap;
	CallbackMap callbacks_;
	ProtobufMessageCallback defaultCallback_;
};

#endif