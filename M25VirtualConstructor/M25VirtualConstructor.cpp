#include<iostream>
#include<vector>
#include<list>
using namespace std;
 //M25.将构造函数和非成员函数虚拟化

 class NLComponent{
 	public:
		NLComponent(){	 }
		virtual ~NLComponent()=0;
 		virtual void WhatIsThis();
 		virtual NLComponent* clone()=0;
 		virtual ostream& print(ostream& s) =0;
 };
 
 	NLComponent::	~NLComponent(){}
 
  	void NLComponent::WhatIsThis(){cout<<"NLComponent!"<<endl;}
 
 class TextBlock :public NLComponent{
	public:
		
		TextBlock(){cout<<"This is a text\n";}
		
		TextBlock(const TextBlock& rhs){cout<<"TextBlock copy constructor called!\n";}
	
		
		virtual TextBlock* clone(){
			cout<<"TextBlock: Clone function called!    ";
	 		return new TextBlock(*this);
		 }
		 
		void WhatIsThis(){cout<<"TextBlock!"<<endl;	}
		virtual ostream& print(ostream& s){
			s<<"TextBlock: Call for operator<<!\n";
			return s;
		}
 };
 
 class Graphic :public NLComponent{
	public:Graphic(){cout<<"This is a graphic!\n";}
	
	Graphic(const Graphic& rhs){cout<<"Graphic copy constructor called!\n";}
	
	void WhatIsThis(){cout<<"Graphic!"<<endl;}
	
	virtual Graphic* clone(){
		cout<<"Graphic: Clone function called!    ";
	 	return new Graphic(*this);
	}
	virtual ostream& print(ostream& s){
			s<<"Graphic: Call for operator<<\n";
			return s;
	}
 };
 
 class NewsLetter{
 	public:
	 	NewsLetter(vector<bool>&);//构造函数 
	 	NewsLetter(NewsLetter& rhs);//拷贝构造函数 
	 	
	 	ostream& print(ostream& s){//把新闻打印出来 
	 		for(list<NLComponent*>::iterator  it=components.begin();it!=components.end();it++){
		 		((*it)->print(cout));
		 		//components.push_back(new ___((*it)))// 问:下划线处填什么? 是TextBlock类还是Graphic类?
		 		//答:不能人为判断。不能用if-else语句，因为目前没有办法进行类型对比，即没有类似 fun(x)==int这样确定x的类型的函数 
			 }
			 return s;
		 }
		 
 	private:
 		list<NLComponent*> components;
 		
 };
 
 NewsLetter::NewsLetter(vector<bool>& elems) {//构造函数 
 	for(int ix=0;ix<elems.size();ix++){
 		if(elems[ix]==true)	components.push_back(new TextBlock());
 		else components.push_back(new Graphic());
	}
	
 }
 
 NewsLetter::NewsLetter(NewsLetter& rhs){
 	for(list<NLComponent*>::iterator  it=rhs.components.begin();it!=rhs.components.end();it++){
 		components.push_back((*it)->clone());
 		//components.push_back(new ___((*it)))// 问:下划线处填什么? 是TextBlock类还是Graphic类?
 		//答:不能人为判断。不能用if-else语句，因为目前没有办法进行类型对比，即没有类似 fun(x)==int这样确定x的类型的函数 
	 }
 }
 //虚拟行为的非成员函数 
  inline
 ostream& operator<<(ostream& lhs, NLComponent* rhs){
 	return rhs->print(lhs);
 }
 inline
  ostream& operator<<(ostream& lhs, NewsLetter& rhs){
 	return rhs.print(lhs);
 }
 int main(){
 	vector<bool> tmp;//用true表示文,false表示图 
 	tmp.push_back(true);
 	tmp.push_back(true);
 	tmp.push_back(false);
 	tmp.push_back(false);
	 
 	//某个新闻的构成:文+文+图+图
	NewsLetter myNL(tmp);//把上面的新闻要素组合成一个新闻叫myNL 
//	  输出: This is a text
//			This is a text
//			This is a graphic!
//			This is a graphic!
//    原因:NewsLetter中调用了各自的构造函数 
 	//复制这个新闻,要求深拷贝 
	NewsLetter copy_myNL=myNL;
//输出: TextBlock: Clone function called!    TextBlock copy constructor called!
//		TextBlock: Clone function called!    TextBlock copy constructor called!
//		Graphic: Clone function called!    Graphic copy constructor called!
//		Graphic: Clone function called!    Graphic copy constructor called! 
	cout<<copy_myNL;
 	return 0;
 }
 //结论:如果有一基类指针组,基类指针可能指向多个子类的其中一个（例如：A*类指针 可以指向子类B或C或D...）， 
 // 	此时如果想遍历基类指针组的成员，对每个成员调用各自实现方法不同的同名函数，则可以通过基类指针调用该同名函数。 
 // 	这个同名函数必须是virtual类型。如果这个同名函数是 拷贝构造函数，则对拷贝构造函数进行包装，得到虚拟构造函数。 
 //ps：虚构造函数没有意义！所以如果是构造函数则必须进行封装，并返回构造出来的对象的指针 
