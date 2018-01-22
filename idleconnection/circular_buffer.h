
/* 
boost的circular_buffer:
stl::list、stl::deque
随机访问迭代器、常数时间插入、开头和末尾的删除操作、适用std算法
固定容量，根据插入操作在开头或末尾覆盖元素
只在创建时、resize、assign操作分配内存


支持操作：
	长度为参数的构造函数	
	size()  			返回长度
	push_back()			末尾插入一个元素，同时删除开头一个元素
	back()				返回最后一个元素
	begin()				指向开头元素的迭代器
	end()				指向末尾元素后一个位置的迭代器
	resize()			改变长度
*/

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

class circular_buffer{
private:
	
};

#endif
