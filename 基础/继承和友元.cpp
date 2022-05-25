#include <iostream>
using namespace std;

void doThis(){//可以访问私有成员
    cout<<(*(new A)).num1;
    cout<<(new A)->num1;
}
class A{
    public:
        friend void doThis();
    private:
        int num1;
};

class B:public A{
    public:
        void doThis(){
            cout<<num1;//报错，派生类无法访问基类私有成员
        }
};

//总结：派生类无法访问基类的私有成员，而友元函数可以通过对象任意访问类