
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
class ProtobufDispatcher : boost::noncopyable{
public:
	typedef boost::function<void (const muduo::net::TcpConnnectionPtr&, const MessagePtr&, muduo::Timestamp)> ProtobufMessageCallback;

	ProtobufDispatcher(const ProtobufMessageCallback& defaultCb)
	:defaultCallback_(defaultCb)
	{}

	void registerMessageCallback(const google::protobuf::Descriptor* desc, const ProtobufMessageCallback& callback)
	{
		callbacks_[desc]=callback;
	}

	void OnProtobufMessage(const muduo::net::TcpConnnectionPtr& conn, const MessagePtr& message, muduo::Timestamp receiveTime) const
	{
		Descriptor* desc=message->GetDescriptor();
		CallbackMap::iterator it=callbacks_.find(desc);
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