#include<iostream> 
using namespace std;
//方法1：把构造函数设定为私有，设置1个或多个static thePrinter静态函数提供静态对象实现限制对象个数 
//优点：对象个数有保障。 
//缺点：1.该类无法被继承 。2.多个对象时需要繁琐地编相同功能的程序 
class Printer{
	public:
		void submitJob(const string& whatToPrint){
			cout<<whatToPrint<<endl;
		}

		friend Printer& thePrinter();

		static Printer& thePrinter2(){
			static Printer PrinterObj2;
			cout<<"PrinterObj2 address:"<<& PrinterObj2<<endl;
			return PrinterObj2;
			
//如果不是friend,那么调用方法会变成Printer::thePrinter().submitJob("...")
		}

	private:
		Printer(){}//构造 
		Printer(const Printer& rhs);//拷贝构造 
		
}; 

Printer& thePrinter(){
	static Printer PrinterObj;//除了第一次调用会创建一个对象,再次进入会被跳过 
	cout<<"PrinterObj address:"<<& PrinterObj<<endl;
	return PrinterObj;
}

int main(){
	thePrinter().submitJob("Print Test!\n");
	//完全展开后是 std::thePrinter().submitJob("Print Test!\n");
	//但由于我using namespace std所以可以忽略前面 std::
	cout<<"打印机的地址:" <<&thePrinter()<<endl;
	cout<<"打印机的地址:" <<&thePrinter()<<endl;
	//两次地址都相同,说明 static Printer PrinterObj;没有创建新的对象,而是每次都使用同一个 
	
	Printer::thePrinter2().submitJob("Print Test2!\n");
//结论:可以通过建立多个static Printer实现多个对象,但是这样n个对象就要n个相同代码但是名字不同的函数 
}
