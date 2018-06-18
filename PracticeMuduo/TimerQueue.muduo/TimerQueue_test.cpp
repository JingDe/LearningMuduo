#include"reactor.muduo/EventLoop.h"
#include"TimerId.h"
#include"Timer.h"

#include<unistd.h>

#include"logging.muduo/Logging.h"

int cnt = 0;
EventLoop* g_loop;

void print(const char* msg)
{
	//printf("msg %lld %s\n", time(NULL), msg);
	LOG_INFO << "msg " << time(NULL) << " " << msg;
	if (++cnt == 20)
		g_loop->quit();
}

void cancel(TimerId timer)
{
	g_loop->cancel(timer);
	time_t now = time(NULL);
	printf("%lld cancelled at %lld\n", timer.timer()->expiration(), now);
}

void test1()
{
	Logger::setLogLevel(Logger::DEBUG);

	EventLoop loop;
	g_loop = &loop;


	loop.runAfter(1, std::bind(print, "once1"));
	loop.runAfter(4, std::bind(print, "once4"));
	TimerId t4 = loop.runAfter(4, std::bind(print, "once4"));
	loop.runAfter(5, std::bind(cancel, t4));
	loop.runAfter(6, std::bind(cancel, t4));
	loop.runEvery(2, std::bind(print, "every2"));
	TimerId t3 = loop.runEvery(3, std::bind(print, "every3"));
	loop.runAfter(9, std::bind(cancel, t3));

	loop.loop();
	print("main loop exits");

}

void test2()
{

}

int main()
{

	test1();

	return 0;
}