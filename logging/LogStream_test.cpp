#include"LogStream.h"

//测试operator<<(int)  operator<<(double)
void test1()
{
	LogStream ls;
	
	double d=3.123;
	ls<<d;
	std::cout<<ls.buffer().ToString()<<std::endl;
	
	int i=888;
	ls<<d:；
	
}

int main()
{
	
}