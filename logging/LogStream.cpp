

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;


// 整数转string，例如 32 --> "32", -32 --> "-32"
tempalte<typename T>
size_t convert(char buf[], T value)
{
	T i=value;
	char* p=buf;
	
	do{
		int lsd=static_cast<int>(i  %  10);
		i /=10;
		*p++ = zero[lsd];
	}while(i != 0);
	
	if(value <0)
	{
		*p++ ='-';
	}
	*p = '\0';
	std::reverse(buf, p);
	
	return p-buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
	uintptr_t i= value;
	char* p=buf;
	
	do{
		int lsd = static_cast<int>(i % 16);
		i /= 16;
		
	}
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if(buffer_.available() >= kMaxNumericSize)
	{
		size_t len=convert(buffer_.current(), v);
		buffer_.add(len);
	}
}

LogStream& LogStream::operator<<(short v)
{
	*this<< static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
	*this<< static_cast<unsigned int>(v);
	return *this;
}

LogStream& LogStream::operator<<(int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v=reinterpret_cast<uintptr_t>(p);
	if(buffer_.available() >= kMaxNumericSize)
	{
		char* buf=buffer_.current();
		buf[0]='0';
		buf[1]='x';
		size_t len = convertHex(buf+2, v);
		buffer_.add(len+2);
	}
	return *this;
}