
#include <iostream>
using namespace std;
class Base
{
public:
    void Bfun1(){cout << "I'm Bfun1()" << endl;}
protected:
    void Bfun2(){cout << "I'm Bfun2()" << endl;}
private:
    void Bfun3(){cout << "I'm Bfun3()" << endl;}
};
class Derived : private Base
{
public:
    void Dfun1() {cout << "I'm Dfun1()  " ;Bfun1();}//正确!这里不是用户操作!因此子类可以调用父类函数 
    void Dfun2() {cout << "I'm Dfun2()  " ;Bfun2();}//正确!这里不是用户操作!因此子类可以调用父类函数 
    void Dfun3() {cout << "I'm Dfun3()  " ;Bfun3();}//错误!Base中Bfun3是私有成员 
};
int main()
{
    Derived d;
    d.Dfun1();
    d.Dfun2();
 
    //d.Bfun1();//错误!用户操作时,子类Derived对象不能直接调用父类成员 Bfun1
    //d.Bfun2();//错误!用户操作时,子类Derived对象不能直接调用父类成员 Bfun2
   // d.Bfun3();//错误!用户操作时,子类Derived对象不能直接调用父类成员 Bfun3
    return 0;
}
//结论:私有(private) 继承后只有在类内定义的时候才能调用父类非私有(即protected和public)函数(代码16~17)
//用户程序并不能调用父类函数(代码26~28) 
