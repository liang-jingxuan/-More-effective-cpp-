#include<iostream> 
using namespace std;
//允许对象来去自由
 //方法1：如果想有多个对象的话需要重复编写相似的代码thePrinter1,thePrinter2...
 //       这很麻烦
 //方法2：继承和包含会导致意外的对象增加
 //结合方法1和方法2就可以实现多个对象，也不怕被继承和包含导致意外的对象
  //结论:仍然无法避免继承和包含导致的对象增多,但需要特殊的技巧,
  //	比如需要调用伪构造函数。 
  class Printer{
  	public:
  		class TooManyObj{};//异常 
  		static Printer* makePrinter();//伪构造函数
  		//这里必须是静态函数成员,否则需要对象才能调用 
		void PrintJob(const string& job){
			cout<<job<<endl;
		}
  		~Printer();
  	private:
  		Printer();
  		Printer(const Printer& rhs);
  		static const int MaxNumObj;
  		static int NumObj;
  }; 
  
  class T{//包含printer的类 
  	public:
  		T():p(Printer::makePrinter()){
		}
		void printSth(const string& str){
			cout<<str<<endl;
		}
  	private:
  		Printer* p;//正确,可以通过makePrinter获取一个Printer指针 
  		//Printer p;//错误,因为没法调用构造函数 
  };
  
  
  const int Printer::MaxNumObj = 2;
  int Printer::NumObj = 0;
  
  Printer::Printer(){
  	if(NumObj>=MaxNumObj){
  		throw TooManyObj();
	  }
  		
  	++NumObj;
  }
  
  Printer* Printer::makePrinter(){
  	return new Printer;//这里可以调用构造函数是因为这是类本身 
  }
  
  Printer::~Printer(){
  	--NumObj;
  }
  
  int main(){
  	/*1.使用普通构造函数*/
  	//Printer p1;//报错,因为构造函数是私有的 
  	
	/*2.使用伪构造函数*/
	Printer *p2 = Printer::makePrinter();
  	p2->PrintJob("p2!\n");
  	
  	/*3.使用伪构造函数并检查对象个数*/
  	Printer *p3 = Printer::makePrinter();
  	p3->PrintJob("p3!\n");//正常,因为最多可以有2个对象 
  	try{
	  	Printer *p4 = Printer::makePrinter();
  		p3->PrintJob("p4!\n");
	  }
  	catch(Printer::TooManyObj&){//记得加前缀 
  		//异常,因为前面有p2和p3两个对象,达到了最大值 
  		cout<<"捕获异常:TooManyObj"<<endl;
	  }
	
	/*4.删除一个对象后,使用伪构造函数创建对象*/
	delete p2;//删除p2 
	Printer *p4 = Printer::makePrinter();
  	p3->PrintJob("p4!\n");//成功,因为删除了一个对象 
	
	/*5.包含Printer的另一个类*/
	try{
		T tmpobj;
		//此处必然发生异常,因为T内部有一个Printer对象 
	}
	catch(Printer::TooManyObj&)
	{
		cout<<"捕获异常:TooManyObj"<<endl;
	}
	delete p4;
	T tmpobj2;
	tmpobj2.printSth("This must be correct!\n");
	
  	return 0;
  }
