#include<iostream> 
using namespace std;
//像1,2,和3那样需要针对每个类进行对象计数编程很呆，可以考虑类继承一个计数功能类
//原理: 继承一个对象计数功能基类。 在创建一个对象指针的时候Printer *p=Printer::makePrinter()-> 调用伪构造函数 Printer::makePrinter()
															//->调用构造函数Printer()->先构造Counted再构造Printer对象->完成构造。 

//总结: 不论私有还是共有继承都会先构造父类，因此子类继承父类后并不需要太多修改。 Counted基类负责计算调用了多少次构造函数并在超过最大可
//		接受对象数的时候抛出异常。 
 template<class BeingCounted>//需要用模板,因为不知道每个类都可能会需要 
 class Counted{
 	public:
 		class TooManyObj{};
 		
 		static int ObjCount(){
		 //这个函数适合用static,这样就算没有对象也能查对象个数
		 //根据对象个数决定要不要继续创建对象 
 			cout<<NumObj<<"/"<<MaxNumObj<<endl;
			return NumObj;
		 }
 		
 	protected:
 		Counted();
 		Counted(const Counted&);
 		~Counted(){
 			--NumObj;
		 }
	private:
		static int NumObj;
		static const int MaxNumObj;
		void init();
 };
 
 //int Counted::MaxNumObj=2;//这里应该根据实际填不同的数 
 
 //静态成员初始化 
 template<class BeingCounted>//别忘了全名展开 
 int Counted<BeingCounted>::NumObj=0;//不管是什么类都从0个对象开始 
 
 template<class BeingCounted>
 Counted<BeingCounted>::Counted(){
 	cout<<"In class \"Counted\": creating an counting machine..."<<endl; 
 	init();
 } 
 
  template<class BeingCounted>
 Counted<BeingCounted>::Counted(const Counted<BeingCounted>&){
 	init();
 } 
 
 template<class BeingCounted>
 void Counted<BeingCounted>::init(){
 	if(NumObj>=MaxNumObj)
 		throw TooManyObj();
 	++NumObj;
 }
 
 /*自定义类*/
 class Printer: 
 /*继承列表*/
 private Counted<Printer>{ 
 	public:
 		static Printer* makePrinter();//伪构造 
		static Printer* makePrinter(const Printer&);//伪拷贝构造	
		~Printer(){} 
		void submitJob(const string& job){
			cout<<job<<endl;
		}
		using Counted<Printer>::ObjCount;
		//如果不写这个,那么用户需要这样查对象个数: Counted<Printer>::ObjCount();
		//但如果写了这个,那么只需要 p->ObjCount(); 
		using Counted<Printer>::TooManyObj;
	private:
		Printer(){cout<<"In class Printer: creating a printer object...\n";};//如1和3那样,不允许调用构造函数
		Printer(const Printer&);//这个不写,因为同一个指针指向同一个对象其实只有一个对象 
 };
 
 Printer* Printer::makePrinter(){
 	return new Printer;
 }
 
 
template<>//莫名其妙 
const int Counted<Printer>::MaxNumObj=2;//设定最大可创建对象个数 
int main(){
	//Printer p;//报错,无法调用构造函数 
	//Printer *p2;//正确,但不具备 任何打印功能,因为没有可指向的对象进行操作 
	
	//查看对象个数方法1 :无指定对象的时候 
	Counted<Printer>::ObjCount();//OK,输出0/2  
	
	Printer *p1=Printer::makePrinter();//正确 
	//查看对象个数方法2:无指定对象的时候 
	//Printer::ObjCount(); //OK,输出1/2
						 //如果Printer内没有using Counted<Printer>::ObjCount,则这句会出错 
	
	p1->submitJob("Hello!\n");
	
	//查看对象个数方法3:有指定对象的时候 
	p1->ObjCount();//OK,输出1/2
					//如果Printer内没有using Counted<Printer>::ObjCount,则这句会出错 
	Printer *p2=Printer::makePrinter();
	try{
		Printer *p2=Printer::makePrinter();
	}
	catch(Printer::TooManyObj&){
	//疑问:这里不是Counted<Printer>::是因为Printer内使用了using ...,但实际抛出的异常的是Counted<Printer>::TooManyOBJ 
		cout<<"Too many printer obj!"<<endl;
	}
	return 0;
}
 
 
 
 
