
class QueryServer: boost::noncopyable{
public:
	QueryServer(EventLoop* loop, const InetAddress& listenAddr)
	:server_(loop, listenAddr, "QueryServer"),
	dispatcher_(boost::bind(&QueryServer::onUnknownMessage, this, _1, _2, _3)),
	codec_(boost::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3)
	{
		
	}

private:
	TcpServer server_;
	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
};