//利用类的析构函数防止内存泄露
#include <iostream>
using namespace std;
class A{
    public:
        void doThis(){
            cout<<"HAHA!"<<endl;
        }
        ~A(){
            cout<<"Deconstructor of class A called!"<<endl;
        }
};

template<class T> 
class auto_ptr { 
    public: 
        auto_ptr(T *p = 0): ptr(p) {} // 保存 ptr，指向对象 

        ~auto_ptr() { 
            cout<<"auto_ptr has deconstructed the object!"<<endl;
            delete ptr; 
        } // 删除 ptr 指向的对象 

        T* operator->(){
            return ptr;
        }
    private: 
    T *ptr; // raw ptr to object 
}; 

void fun1(){
    A *pA=new A;//被动:这里可能抛出内存不足异常
    cout<<"In fun1(): Throw an exception here!"<<endl;
    throw A();//主动:抛出一个A类异常
    
    //下面的语句不被执行,退出fun1离开生存空间也没调用析构函数
    pA->doThis();//没有输出"HAHA"
    delete pA;  //没有输出"Deconstructor of class A called!",说明被有执行
                //因此内存泄露
}


void fun2(){
    auto_ptr<A> pA = new A;//可能抛出内存不足异常
    cout<<"In fun2(): If an exception is throwed here!"<<endl;
    throw A();//主动:抛出一个A类异常
    //下面这语句没有被执行
    pA->doThis();//仍然不会被执行
    //delete pA;不需要
}
int main(){
    A* pA= new A;
    delete pA;//有输出:Deconstructor of class A called!
    //没有使用智能指针
    
    try{
        fun1();//主动抛出已知类型的异常
        //1.抛出异常的位置以下的程序不会被执行,所以自己写的delete没用
        //2.即使离开生存空间,即跳出了fun1,析构函数也没有被调用.
    }
    catch(A&){
        cout<<"An exception with type A is catched!"<<endl;
        //输出:
        //In fun1(): Throw an exception here!
        //An exception with type A is catched!
        //A的析构函数没有被调用,内存泄露
    }

    //使用了智能指针
    try{
        fun2();
    }
    catch(A&){
        cout<<"An exception with type A is catched!"<<endl;
        //输出:
        //In fun2(): If an exception is throwed here!
        //auto_ptr has deconstructed the object!//**已经被析构,没有异常!
        //An exception with type A is catched!
    }
return 1;
}
//结论:用智能指针代替所有需要用到指针的地方,这样就不用担心抛出异常无法执行delete
//     也不用时时记着要delete某些东西