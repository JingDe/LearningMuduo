#include "codec.h"

#include "query.pb.h"

#include <iostream>
#include <typeinfo>

#include <assert.h>
#include <stdio.h>


//typedef muduo::Query T;
template<typename T>
void testDescriptor()
{
	//full_name() 返回 类型的完整名字string
	std::string type_name=T::descriptor()->full_name();
	std::cout<<type_name<<std::endl;

	// FindMessageTypeByName()根据 类型的完整名字 返回 类型的描述符Descriptor
	const google::protobuf::Descriptor* descriptor=google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	assert(descriptor==T::descriptor());
	std::cout<<"FindMessageTypeByName()="<<descriptor<<std::endl;
	std::cout<<"T::descriptor()="<<T::descriptor()<<std::endl;
	std::cout<<std::endl;

	// GetPrototype()根据 描述符 获得 类型基类Message*
	const google::protobuf::Message* prototype=google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	assert(prototype==&T::default_instance());
	std::cout<<"GetPrototype()="<<prototype<<std::endl;
	std::cout<<"T::default_instance()="<<&T::default_instance()<<std::endl;
	std::cout<<std::endl;

	// Message*的New()操作获得 该类型的对象
	T* new_obj=dynamic_cast<T*>(prototype->New());
	assert(new_obj!=NULL);
	assert(new_obj!=prototype);// T* 不等于 Message*
	assert(typeid(*new_obj)==typeid(T::default_instance()));// T::default_instance()是 T类型的
	std::cout<<"prototype->New()="<<new_obj<<std::endl;
	std::cout<<std::endl;
	delete new_obj;
};





int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	testDescriptor<muduo::Query>();
	
	testDescriptor<muduo::Answer>();
	
	testDescriptor<muduo::Empty>();
	
	
	google::protobuf::Message* newQuery=createMessage("muduo.Query");
	assert(newQuery !=NULL);
	assert(typeid(*newQuery) == typeid(muduo::Query::default_instance()));
	std::cout<<"createMessage(muduo.Query) = "<<newQuery<<std::endl;
	
	google::protobuf::Message* newAnswer=createMessage("muduo.Answer");
	assert(newAnswer !=NULL);
	assert(typeid(*newAnswer) == typeid(muduo::Answer::default_instance()));
	std::cout<<"createMessage(muduo.Answer) = "<<newAnswer<<std::endl;
	
	delete newQuery;
	delete newAnswer;

	puts("All pass!!!");
  
	google::protobuf::ShutdownProtobufLibrary();
	
	return 0;
}


