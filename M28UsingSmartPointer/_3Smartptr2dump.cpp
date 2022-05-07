#include <iostream>
using namespace std;
//前言:
//如果有从只能指针到普通指针的转换需求，比如某个库之前不使用智能指针，
//现在想通过使用智能指针进行操作，则需要一些从智能指针到普通指针的转换，以实现兼容
//但是这样做会引入一定的问题：

template<typename T>
class smartPtr;

template<typename T>
class smartPtr2;//加入了类型转换

class intAccessors;

void fun1(int *);
void fun2(smartPtr2<int>&);
void fun3(const intAccessors&);

void fun1(int *v){
    return;
}


void fun2(smartPtr2<int>& ptr){
    int *tmpPtr = ptr;//报错!不能从智能指针转换到dump
    //左边是int* 右边是smartPtr2<int>,所以存在转换,获得了一个dump指针
    //tmpPtr不具备智能指针的功能,可能会使M29的引用计数的数据结构遭到破坏
    cout << *ptr;//报错!原因不明
    return;
}


void fun3(const intAccessors& num1){
    return;
}


template<typename T>
class smartPtr{
    public:
        smartPtr(T *data = 0):realPtr(data){};
        
        ~smartPtr(){
            if(realPtr)
                delete realPtr;
        }

        smartPtr(smartPtr<T>& rhs){
            realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

        smartPtr<T>& operator=(smartPtr<T>& rhs){
            if(this==&rhs) return *this;
            delete realPtr;
            realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

        T& operator*()const{
            return *realPtr;
        }

        T* operator->() const{
            return realPtr;
        }

        
    private:
        T* realPtr;
};

template<typename T>
class smartPtr2{

    public:
        smartPtr2(T *data = 0):realPtr(data){};
        
        ~smartPtr2(){
            if(realPtr)
                delete realPtr;
        }

        smartPtr2(smartPtr2<T>& rhs){
            realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

        smartPtr2<T>& operator=(smartPtr2<T>& rhs){
            if(this==&rhs) return *this;
            delete realPtr;
            realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

        T& operator*()const{
            return *realPtr;
        }

        T* operator->() const{
            return realPtr;
        }

        operator T*(){return realPtr;}//最大区别在此,(类型转换函数不需要返回类型,即开头不用加 T* )
        //fun1需要一个int* 类型,而智能指针main中串到fun1的是smartPtr<int>因此存在从smartPtr<int>到int*的转换
    private:
        T* realPtr;
};

class intAccessors{
    public:
        intAccessors(const int* num1){}
};


int main(){
    //1.智能指针不能转到普通指针
    smartPtr<int> p = new int(53);
    cout<<*p<<endl;
    if(*p==53)
        cout << "59";
    
    fun1(p);//报错!fun1需要的是一个int*类指针,但是p是一个smartPtr<int>类
    //为了解决问题1,可以使用类型转换,当需要从smartPtr<int>转换到另一个类T时转换
    smartPtr2<int> p2 = new int(56);
    fun1(p2);//问题1解决!

    //虽然问题1解决了,但是引入了加入了"隐式转换"的smartPtr2引入了问题2:
    //2.暴露了dump指针,失去了智能指针的功能
    fun2(p2);//没问题,可以通过。但是fun2函数里发生着可怕的事

    //3.隐式的转换智能进行一次
    //例如:smartPtr2->int是可以的,但是smartPtr2->int*->类intAccessors是不行的
    //其实这很明显,intAccerssors类可以有用int*作为参数的构造函数,即intAccessors(const int* num1)
    //但是不一定有intAccessors(const smartPtr<int> num1)
    int *pi1;
    fun3(pi1);//OK,因为有int*作为参数的构造函数

    smartPtr2<int> pi2;
    fun3(pi2);//报错!原因如上

    //4.delete掉智能指针
    //设计的时候我们尽量使智能指针和dump指针使用方法尽量统一,以致于有时候忘了正在使用智能指针
    //而调用了delete。
    delete p2;//可以通过但是很危险!!!p2并不是一个指针,而是一个伪装成指针的类!
    //delete后面应该跟一个指针而不是对象
    
    return 0;
}


