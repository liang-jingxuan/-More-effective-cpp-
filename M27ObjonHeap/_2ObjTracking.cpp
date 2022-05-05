#include<iostream> 
#include "list"
#include"algorithm"
using namespace std;
//思路: 使用一个链表,只要调用了new就挂到链表上,在链表上能找到
//		说明对象在堆上 
//写一个堆追踪基类,想某个类的对象都在堆上生成,则继承这个类 
class HeapTracked{
	public:
		class MissingAddress{};
		virtual ~HeapTracked()=0;//表明是个抽象类,无法实例化
		static void* operator new(size_t size);
		//因为要访问static数据成员,所以设为static 
		static void operator delete(void* Ptr);
		bool isOnHeap() const;
	private:
		typedef const void* RawAddress;//把const void* 类简称为RawAddress
		static list<RawAddress> addresses; 
};
HeapTracked::~HeapTracked(){}

list<HeapTracked::RawAddress>HeapTracked::addresses;
void* HeapTracked::operator new(size_t size){
	void* memPtr=::operator new(size);
	addresses.push_front(memPtr);
	return memPtr;
}

void HeapTracked::operator delete(void* Ptr){
	auto it=
		find(addresses.begin(),addresses.end(),Ptr);
	if(it==addresses.end()){//说明不在链表中，不是operator new申请空间而来=不在堆
		throw MissingAddress();
	}
	else{
		addresses.erase(it);//从链表中移除
		::operator delete(Ptr);//释放空间
	}
}

bool HeapTracked::isOnHeap()const{
	//判断某个对象是否在堆上
	RawAddress Ptr=dynamic_cast<RawAddress>(this);

	auto it =
		find(addresses.begin(),addresses.end(),Ptr);
	if(it==addresses.end()){
		cout<<"Object is not on the Heap!"<<endl;
		return false;
	}
	cout<<"Object is on the Heap!"<<endl;
	return true;
}

class TestonHeap:public HeapTracked{
	//在堆上，说明都是new出来，而不是构造出来的
	public:
		void destroy() const{
			delete this;
		}
		TestonHeap(){}
	private:
		~TestonHeap(){}		//把析构设为private即可，因为退出main会析构对象，此时如果有构造就会调用
		//这里记得记得进行定义，因为即使在堆上，临时对象在退出main的时候也会调用析构
};



class TestnotonHeap:public HeapTracked{
	//不在堆上，说明都是构造出来而不是new出来的
	public:
		TestnotonHeap(){}
		~TestnotonHeap(){}
	private:
		static void *operator new(size_t size) ;
		static void operator delete(void * ptr){
			cout<<"In TestnotonHeap: operator delete called!"<<endl;
			delete ptr;
		}
};


class myClass:public HeapTracked{
	//...
	public:
		myClass(){}
		~myClass(){}
};

int main(){
	//TestonHeap T1;//错误，堆上对象不应该构造
	
	TestonHeap* p1=new TestonHeap();//正确,new出来说明在堆
	p1->isOnHeap();//来自继承,输出：在堆上
	p1->destroy();//来自继承
	
	//TestnotonHeap *p2=new TestnotonHeap();//错误,new出来的都在堆上
	TestnotonHeap T2;//正确
	T2.isOnHeap();//来自继承,输出：不在堆上

	myClass T4;
	T4.isOnHeap();//输出：不在堆上
	myClass *T5= new myClass();
	T5->isOnHeap();//输出：在堆上

	myClass *temp = &T4;
	try{
		temp->isOnHeap();//输出：不在堆上，因为T4是构造出来的
		delete temp;//理论上这里会引发异常，因为T4不在堆上，delelte的时候会抛出MissingAddress
	}
	catch(HeapTracked::MissingAddress&){
		cout<<"异常：企图删除不在堆上的对象！"<<endl;
	}
	cout<<"结束"<<endl;//这句没有被执行，因为delete里面抛出异常，在delete里抛出异常会终结程序,条款M11
	return 0;
}

