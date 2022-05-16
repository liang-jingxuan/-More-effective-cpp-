#include<iostream> 
using namespace std;
//亮点：静态变量限制对象个数，超出数量时候抛出异常。异常的try-catch运用 
//方法2：利用异常。设置一个静态数据成员记录创建对象个数，一旦大于指针对象个数就抛出异常 
//优点：编程方法已读。
//缺点：派生、被其他类包含都会产生对象。 
class Printer;
ostream& operator<<(Printer& rhs,const string& whatToPrinte){
	cout<<whatToPrinte;
	return cout;
}

class Printer{
	public:
		class TooManyObj{};
		Printer();
		~Printer();
		void PrinteJob(const string& job){
			cout<<job<<endl;
		}
	private:
		static size_t numofPrinter;
		static int MaxNumObj;
	private:
		Printer(const Printer& rhs);
};

size_t Printer::numofPrinter=0;
//错误初始化方法：Printer::numofPrinter=0;
int Printer::MaxNumObj=2;
//错误初始化方法：Printer::MaxNumObj=2;
Printer::Printer(){
	if(numofPrinter>=MaxNumObj)
		throw TooManyObj();
	++numofPrinter;	
}

Printer::~Printer(){--numofPrinter;	}

int main(){
	Printer p1;
	p1<<"Printer test!\n";
	p1.PrinteJob("Printer test2!\n");
	try{//没问题，因为此时只有p1和p2 两个对象 
		Printer p2;
		p2<<"This is Jingxuan Liang!\n";
		//P2在此被析构 
	}
	catch(Printer::TooManyObj&){
		cout<<"Too many objects! Constructing a Printer failed!";
	}
	
	try{//没问题，因为p2已经被析构了所以只有1个对象即P1 
		Printer p3;
		p3<<"This is Jingxuan Liang2!\n";
		//p3在此被析构 
	}
	catch(Printer::TooManyObj&){
		cout<<"Too many objects! Constructing a Printer failed!";
	}
	
	Printer p4;
	try{//此时有两个对象，一个是p1，另一个是p4，所以构造p5时抛出异常 
		Printer p5;
	}
	catch(Printer::TooManyObj&){
		cout<<"Too many objects! Constructing a Printer failed!";
	}
	
	
	return 0;
}
