#include<iostream> 
using namespace std;
//在堆中的对象一般是临时对象, 不具有名字,可能是传值、传返回值时候生成的临时对象 
//想让所有的对象都建立在堆中,只需要把析构函数设置为private即可
// 只在堆上建立对象: 构造&析构(NO) new&delete(Yes) 
class UPNumber{
	public:
		UPNumber(){}
		UPNumber(int initValue);
		UPNumber(double initValue);
		UPNumber(const UPNumber&);
		
		void destroy() const{delete this;}
		//new 和delete用系统自动生成的办法足矣 
	private:
		~UPNumber(){}//把析构禁用了 
};

int main(){
	//UPNumber n1;// 报错,因为退出main会调用析构函数对n1进行析构,但是UPNumber的析构是私有的
	UPNumber *p=new UPNumber;//正确
	//delete p;//错误,试图调用析构函数
	//delete会调用析构的原因:delete会先调用析构再释放内存 
	p->destroy();// 
	
	return 0;
}
//Q:不管是用户自己delete还是调用了destroy(),都会调用delete进而
//使用构造函数,那为什么要进行包装呢?
// A:因为系统会自动生成各种构造函数,没办法枚举,但是析构只能有一个
//所以把析构设为private。但是为了防止内存泄露，还是需要delete的，
//因此在把析构设为private的同时要提供接口来删除堆上对象。


//结论：如果想让某一个类的对象只在堆上生成，则把这个类的析构设为
//private的同时提供接口来析构堆上对象。 
