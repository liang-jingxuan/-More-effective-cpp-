#include <iostream>
using namespace std;
template<typename T>//雏形
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

template<typename T>//加入了隐式转换
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

        operator void*(){//用来判断指针是否空(_2)
            if(realPtr) return realPtr;
            return 0;
        }

        operator T*(){//用来从智能指针到dump指针的转换(_3)
            return realPtr;
        }

        template<class newType>
        operator smartPtr2<newType>(){ // 为了实现隐式类
            return smartPtr2<newType>(realPtr);
        }

    private:
        T* realPtr;
};

template<typename T>//继承
class cleverPtr2const{//父类，用cleterPtr2const<int>代替const int
    public:
        //cleverPtr2const<int> p=new int(56);
        cleverPtr2const(const T* data=0):constPtr(data){}
        
         ~cleverPtr2const(){
            delete constPtr;
        }
        
        cleverPtr2const(cleverPtr2const<T>& rhs){
            constPtr = rhs.constPtr;

            rhs.constPtr = 0;
        }

         cleverPtr2const<T>& operator=(cleverPtr2const<T>& rhs){
            if(this==&rhs) return *this;
            delete constPtr;
            constPtr = rhs.constPtr;
            rhs.constPtr=0;
            return *this;
        }

         const T& operator*()const{
            return *constPtr;
        }

         const T* operator->() const{
            return constPtr;
        }

    protected:
        union{
            T* realPtr;
            const T* constPtr;
        };
    
};

template<class T>//继承
class cleverPtr://子类，非const，实现更多功能
public cleverPtr2const<T>{
    public:
        cleverPtr(T *data = 0){
            this->realPtr=data;//这是个普通指针，可以进行赋值
        }
        
        ~cleverPtr(){
            if(this->realPtr)
                delete this->realPtr;
        }

        cleverPtr(smartPtr<T>& rhs){
            this->realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

         cleverPtr<T>& operator=(cleverPtr<T>& rhs){
            if(this==&rhs) return *this;
            delete this->realPtr;
            this->realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

         T& operator*()const{

            return *(this->realPtr);
        }

         T* operator->() const{
            return this->realPtr;
        }

        operator cleverPtr2const<T>(){
            return this->realPtr;
        }
};


class Test{};

int main(){
      //简单复习一下const
    int a=5,c=9;//非const对象
    const int b=6;//const对象
    a=9;//OK
    b=9;//报错，b是一个const类型，值不能变

    int* p1=&a;//非const对象，非const指针：可以赋值，可以换对象
    *p1=8;//OK.可以赋值
    p1=&c;//OK.可以换成非const对象
    p1=&b;//报错，不能换成const对象[因为非const对象可以进行赋值，但是b是一个const，不能被赋值]
    //int* 和const int*不是同一种类型 

    int *const p2=&a;//非const对象，const指针：可以赋值，不能换对象
    *p2=25;//OK，可以赋值
    p2=&c;//报错
    p2=&b;//报错

    const int * p3=&a;//const对象，非const指针：不能赋值，可以换对象
    *p3=59;//报错，不能赋值
    p3=&b;//OK，可以换成非const对象
    p3=&c;//OK，可以换成const对象
    *p3=56;//报错，虽然c不是const，但是指针指向const对象，应用场景：
    //[对于一个可以赋值的对象，我们有时候想让他保持不变一段时间，可以用一个const对象指针指向他，让他的值暂时不变]

    const int* const p4=&a;//const对象，const指针：不能赋值，不能换对象
    *p4=25;//报错，不能赋值
    p4=&b;//报错，不能换对象
    p4=&c;//报错，不能换对象

    int *p1 = new int(56);
    const int *p2=p1;//OK，p1和p2指向同一个对象
    *p1=55;//p1可以修改对象的值
    //*p2=85;//p2不能修改对象的值

    //对于智能指针：
    smartPtr<int> p3=new int(35);
    smartPtr<const int> p4=p3;//错误，不能和dump指针一样进行转换
    //说明smartPtr<const int>和smartPtr<int>不同类
    
    //解决方法1：在智能指针类里加入隐式转换(代码83-86)
    smartPtr2<int> p5=new int(35);
    smartPtr2<const int> p6=p5;
    *p5=58;//OK
    *p6=58;//报错
    
    cleverPtr<int> p7=new int(99);
    *p7=989;//OK，因为p7非const
    cleverPtr2const<int> p8 = p7;
    cout<<"*p8="<<*p8<<endl;
    cout<<"*p7="<<*p7<<endl;//p7无输出，因为空值权交给了p8
    *p8=27;//不行，因为p8是一个const指针，不能赋值

    //解决方法2：写一个专门处理const指针的类，让智能指针继承这个const指针类

}