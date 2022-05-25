#include <iostream>
using namespace std;
class A{
    public:
        virtual void fun1(int ix=5){
            cout<<"class A::virtual void fun1(int ix=5),ix="<<ix<<endl;
        }
        virtual void fun2(int ix){
            cout<<"class A::virtual void fun2(int ix),ix="<<ix<<endl;
        }
};
class B:public A{
    public:
        virtual void fun1(int ix){
            cout<<"class B::virtual void fun1(int ix=10),ix="<<ix<<endl;
        }

        virtual void fun2(int ix=20){
            cout<<"class B::virtual void fun2(int ix=10),ix="<<ix<<endl;
        }
};

int main(){
    B objB;
    A* pA=&objB;
    pA->fun1();//5   注意:这里输出了5,而不是classB中设置10!!!
    pA->fun1(100);//100,毫无疑问,调用了B的fun1,传入参数为100
    pA->fun2();//报错,这个函数必须要有参数,因为基类中需要参数
    pA->fun2(100);//100,调用了B的fun2传入参数为100
    //总结:如果基类中某个虚函数有默认值,那么在派生类中是否设置默认值都毫无意义
    //      如果基类中某个虚函数没有默认值,那么在派生类中设置默认值也没用(28行)
    //所以:继承关系中默认值的设置毫无意义
    return 0;
}