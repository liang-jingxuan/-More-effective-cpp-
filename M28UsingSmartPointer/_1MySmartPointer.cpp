#include <iostream>
using namespace std;
//需要使用模板，因为指针可能指向任何东西，包括内建类型或自定义类
template<typename T>
class smartPtr{
    public:
        smartPtr(T *data = 0):realPtr(data){};//调用方法SmartPtr<int> p/p(obj);
        
        ~smartPtr(){
            if(realPtr)
                delete realPtr;
        }
        //指针退出生存空间后被析构，同时删除指向的对象

        smartPtr(smartPtr<T>& rhs){
            //这里不设置为const是因为我们有可能改变rhs，防止多指针指向同一个对象
            realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

        smartPtr<T>& operator=(smartPtr<T>& rhs){
            //调用方法SmartPtr<int> p=p2;  或p=p2  放弃现有的对象,将p2控制权转移给p
            if(this==&rhs) return *this;
            //防止有人这样写代码： p=p;
            delete realPtr;//放弃现有的对象
            realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

        T& operator*()const{
            //这里可以判断realPtr是否指向空,并采取自定义行为
            //if(!realPtr) return nullptr;
            return *realPtr;//返回指针所指向的对象
        }

        T* operator->() const{
            //当我们调用p->fun()的时候,实际上是 (p.operator->())->fun(),
            //因此(p.operator->())部分应该是一个指针,可以是realPtr也可以是smartPtr<T>
            return realPtr;
        }

        
    private:
        T* realPtr;
        
        /*
        ------------------------------------------------------------
        一种防止两个指针指向同一个对象，当一个指针退出生存空间会析构掉对象，此时另一个指针将指向一个被删掉的对象，很危险。
        解决方法1：把他们设置为private，禁止用户构造这样的指针
        ------------------------------------------------------------
        smartPtr(const smartPtr& rhs);
        //调用方法SmartPtr<int> p(p2);  但是使用内建指针的时候，我们不会
        //int *p(p2),为了统一，这个函数应该不被调用

        SmartPtr& operator=(const SmartPtr&);
        //调用方法SmartPtr<int> p=p2;会先给p分配空间，然后调用拷贝构造进行赋值，参数为等号右边的指针
        //但是int *p=p2;这样的语法尽量少用
        */
};

void Notice(smartPtr<int> p){
    //这个函数通过传值的方法传递智能指针,这会导致严重的后果:
    //假设有一个指针smartPtr<int> ptn=&a;
    //在调用Notice(ptn)后,为了构造p会使用拷贝构造函数,即相当于调用了smartPtr<int> p(ptn)
    //这时候根据拷贝构造函数,会把ptn的对象的控制权交给p,并且使ptn=0。
    //然后完成了Notice需要的操作后p离开生存空间调用析构函数来析构p，最后变量a被delete了,ptn也不再指向a
    //隐患就出现了。
    
    //结论:智能指针应该传递引用!
    //同时,为了防止传递来的引用指向的对象被修改,应该使用const引用,即
    //void Notice(const smartPtr<int>& p){}
    
    return;
}

class Test{
    public:
        void fun1(){
            cout<<"In Test: this is fun1!" << endl;
        }
        Test(){}

};

int main(){
    smartPtr<Test> p = new Test();
    p->fun1();

    Test t1;
    smartPtr<Test> p2=&t1;
    p2->fun1();

    smartPtr<int> p4=new int(8);
    cout<<*p4;
    return 0;
}