#include"codec.h"
#include"query.pb.h"

#include<stdio.h>

void print(const std::string& buf)
{
  printf("encoded to %zd bytes\n", buf.size());
  for (size_t i = 0; i < buf.size(); ++i)
  {
    printf("%2zd:  0x%02x  %c\n",
           i,
           (unsigned char)buf[i],
           isgraph(buf[i]) ? buf[i] : ' ');
  }
}

void testQuery()
{
	muduo::Query query;
	query.set_id(1);
	query.set_questioner("chen shuo");
	query.add_question("running?");
	
	std::string transport=encode(query);
	print(transport);
	
	int32_t be32=0;
	std::copy(transport.begin(), transport.begin()+sizeof be32, reinterpret_cast<char*>(&be32));
	int32_t len=::ntohl(be32);
	assert(len==transport.size()-sizeof(be32));
	
	std::string buf=transport.substr(sizeof(int32_t));
	assert(len==buf.size());
	
	muduo::Query* newQuery=dynamic_cast<muduo::Query*>(decode(buf));
	assert(newQuery != NULL);
	newQuery->PrintDebugString();
	assert(newQuery->DebugString() == query.DebugString());
	delete newQuery;	
	
	buf[buf.size() - 6]++;  // oops, some data is corrupted
	muduo::Query* badQuery = dynamic_cast<muduo::Query*>(decode(buf));
	assert(badQuery == NULL);
}

void testAnswer()
{
	muduo::Answer answer;
	answer.set_id(1);
	answer.set_questioner("Chen Shuo");
	answer.set_answerer("blog.csdn.net/Solstice");
	answer.add_solution("Jump!");
	answer.add_solution("Win!");

	std::string transport=encode(answer);
	print(transport);
	
	int32_t be32=0;
	std::copy(transport.begin(), transport.begin()+sizeof be32, reinterpret_cast<char*>(&be32));
	int32_t len=::ntohl(be32);
	assert(len==transport.size()-sizeof(be32));
	
	std::string buf=transport.substr(sizeof(int32_t));
	assert(len==buf.size());
	
	muduo::Answer* newQuery=dynamic_cast<muduo::Answer*>(decode(buf));
	assert(newQuery != NULL);
	newQuery->PrintDebugString();
	assert(newQuery->DebugString() == answer.DebugString());
	delete newQuery;	
	
	buf[buf.size() - 6]++;  // oops, some data is corrupted
	muduo::Answer* badQuery = dynamic_cast<muduo::Answer*>(decode(buf));
	assert(badQuery == NULL);
}


void testEmpty()
{
  muduo::Empty empty;

  std::string transport = encode(empty);
  print(transport);

  std::string buf = transport.substr(sizeof(int32_t));

  muduo::Empty* newEmpty = dynamic_cast<muduo::Empty*>(decode(buf));
  assert(newEmpty != NULL);
  newEmpty->PrintDebugString();
  assert(newEmpty->DebugString() == empty.DebugString());
  delete newEmpty;
}


int main()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  testQuery();
  puts("");
  testAnswer();
  puts("");
  testEmpty();
  puts("");

  puts("All pass!!!");

  google::protobuf::ShutdownProtobufLibrary();
}
