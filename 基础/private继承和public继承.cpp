
#include <iostream>
using namespace std;
class Base{
public:
    void Bfun1(){cout << "I'm Bfun1()" << endl;}
protected:
    void Bfun2(){cout << "I'm Bfun2()" << endl;}
    int adata2;
private:
    void Bfun3(){cout << "I'm Bfun3()" << endl;}
    int adata;
};

class Derived : private Base{//私有继承
public:
    void Dfun1() {cout << "I'm Dfun1()  " ;Bfun1();}        //可以访问public
    void Dfun2() {cout << "I'm Dfun2()  "<<adata2 ;Bfun2();}//可以访问protected
    void Dfun3() {cout << "I'm Dfun3()  "<<adata ;Bfun3();} //不能访问private
};

class Derived2 : public Base{//公有继承
public:
    void Dfun1() {cout << "I'm Dfun1()  " ;Bfun1();}        //可以访问public
    void Dfun2() {cout << "I'm Dfun2()  " <<adata2;Bfun2();}//设计者可以访问protected
    void Dfun3() {cout << "I'm Dfun3()  "<<adata ;Bfun3();} //设计者不能访问private
};

void funforBase(Base&){

}

void funforDerived(Derived&){

}

void funforDerived2(Derived2&){

}
int main()
{
    //1.对于成员函数和成员数据的访问：
    Derived d;
    d.Dfun1();
    d.Dfun2();
    //从私有基类继承而来的成员都成为了派生类的私有成员
    d.Bfun1();//错误，因为Bfun1变成了Derived类的私有成员，不可访问
    d.Bfun2();//错误，因为Bfun2变成了Derived类的私有成员，不可访问
    d.Bfun3();//错误，因为Bfun3变成了Derived类的私有成员，不可访问

    Derived2 d2;
    d2.Dfun1();
    d2.Dfun2();
    //从私有基类继承而来的成员都成为了派生类的私有成员
    d2.Bfun1();//正确
    d2.Bfun2();//错误，开发者不可访问
    d2.Bfun3();//错误，开发者不可访问
    return 0;

    //2.对于非成员函数的参数传递
        //静态情况
    Base obj_B;
    funforBase(obj_B);//OK
    funforBase(d);//报错：不允许对不可访问的基类 "Base" 进行转换
    funforBase(d2);//OK
        //接受私有继承Base的类的函数
    funforDerived(obj_B);//报错：无法用 "Base" 类型的值初始化 "Derived &" 类型的引用(非常量限定)
    funforDerived(d);//OK
        //接受公有继承Base的类的函数
    funforDerived2(obj_B);//报错：无法用 "Derived" 类型的值初始化 "Derived2 &" 类型的引用(非常量限定)
    funforDerived2(d2);//OK

        //动态情况
    Base* pBase=&d2;
    funforBase(*pBase);
    funforDerived2(*pBase);//不能向下转换 从Base到Derived2是向下转换

    pBase=&d;
}
//结论：
//1.对于成员函数和成员数据的访问：
//      私有继承：从私有基类继承而来的public和protected成员成员都成为了派生类的私有成员
//      公有继承：从公有基类继承而来的public和protected成员称为了派生类的public成员

//2.对于非成员函数的参数传递
//  静态的情况下:
//      对于接受基类引用的非成员函数:   接受 基类 和 共有继承 的派生类对象,不接收私有继承的派生类对象
//      对于接受派生类引用的非成员函数: 只接受派生类对象,不接收基类对象
//  动态的情况下:
//      指针指向:基类指针不能指向私有继承的派生类,可以指向公有继承的派生类
//      转换:只能向上转换即
//公有继承具有"是一个..."的含义,因此可以调用基类的public和protected成员,和基类行为一致,
//  动态传参的时候,指向公有派生类的基类指针可以将其传递到接受基类对象的非成员函数,执行基类的行为.
//