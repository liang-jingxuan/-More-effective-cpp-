#include <iostream>
using namespace std;
class T1{
	public:
	T1(){	}
	virtual void fun1(){
		cout<<"In class T1,This is fun1\n";
	}
	virtual void fun2(){
		cout<<"In class T1,This is fun2\n";
	}
	
	void fun3(){
		cout<<"In class T1,This is fun3\n";
	}
}; 

class T2:public T1{
	public:
	T2(){	}
	void fun2(){
		cout<<"In class T2,This is fun2\n";
	}
	void fun3(){
		cout<<"In class T2,This is fun3\n";
	}
};

int main(){
	T1* p1,*p2,*p3;//都是基类指针 
	T2 x1,x3;
	T1 x2;
	 
	p1= &x1 ;//基类指针指向 子类 对象 
	p1->	fun1();//输出： In class T1,This is fun1; 子类并没有fun1函数,但是通过继承获得了fun1 
	
	p2= &x2 ;//基类指针指向 基类 对象 
	p2->fun1();//输出： In class T1,This is fun1
	
	p3 = &x3;//基类指针指向 子类 对象 
	p3->fun2();//输出:In class T2,This is fun2 
	
	p3 = &x2;//基类指针指向 基类 对象 
	p3->fun2(); //输出: In class T1,This is fun2
	
	T1 y1;T2 y2;
	y1.	fun3();//输出: In class T1,This is fun3
	y2.fun3();//输出: In class T2,This is fun3
	
	T1 *baseptr=&y2;
	baseptr->	fun3();//输出:In class T1,This is fun3,父类指针指向子类,但是调用了父类函数 
	baseptr=&y1;
	baseptr->fun3();//输出:In class T1,This is fun3
	
	T2* sonptr=&y2;
	sonptr->	T1::fun3();//输出:In class T1,This is fun3
	sonptr->	fun3();//输出:In class T2,This is fun3
	return 0;
	
	
}
//结论:
//1.如果基类中某个函数是virtual，用基类指针->子类后调用该函数，则 
//(a)子类不重新定义该函数则调用  基类   的实现方法(代码35,38)
//(b)子类  重新定义该函数则调用  子类   的实现方法 (代码41)
 

//2.如果基类中某个函数不是virtual,用基类指针->子类后调用该函数，则
//(a)子类不管有无重新定义该函数,都会调用父类函数(代码51,53)

//如果正确使用virtual关键字,那么使用基类指针指向子类可以正确调用子类的函数。
//如果子类和父类有同名函数，则调用子类还是父类取决于指针的类型 (代码51,57)
