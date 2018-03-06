
const int kSmallBuffer =4000;

// 待写数据流
class LogStream : noncopyable 
{
public:
	LogStream& operator<<(bool v)
	{
		buffer_.append(v ? "1" : "0", 1);
		return *this;
	}
	
	LogStream& operator<<(short);
	
	void append(const char* data, int len) {
		buffer_.append(data, len);
	}
	void resetBuffer() { buffer_.reset(); }

private:
	FixedBuffer<kSmallBuffer> buffer_;
	
	static const int kMaxNumericSize = 32; // operator<<添加的数据不超过32个字节
};