
#include"codec.h"

#include<muduo/base/Logging.h>
#include<muduo/net/Endian.h>
#include<muduo/net/protorpc/google-inl.h>

#include<google/protobuf/descriptor.h>

#include<zlib.h>

void ProtobufCodec::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
	
}