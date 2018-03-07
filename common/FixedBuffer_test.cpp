#include"FixedBuffer.h"

//#include<cassert>
#include<assert.h>
#include<iostream>
#include<string>

void testChar()
{
	char buf[]="123";
	std::cout<<sizeof(buf)<<std::endl;
	
}

// 测试append函数
void testAppend()
{
	FixedBuffer<32> smallBuffer;
	assert(smallBuffer.empty()==true);
	
	std::string s="123";
	smallBuffer.append(s);
	assert(smallBuffer.ToString()=="123");
	
	char buf[]="456";
	smallBuffer.append(buf, sizeof buf);
	assert(smallBuffer.ToString()=="123456");
	
	//smallBuffer.append(buf, 
}


int main()
{
	testChar();
	
	
	return 0;
}