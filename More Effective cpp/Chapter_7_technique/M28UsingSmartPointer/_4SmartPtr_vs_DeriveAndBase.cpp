#include <iostream>
using namespace std;
//在使用多态的情况下,指向子类的基类dump指针调用哪个函数取决于子类是否写了虚函数。
//                  指向子类的子类dump指针调用一个没定义的虚函数时会调用父类的该函数
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

class Fruit{
    public:
    virtual  void fun1(){
        cout<<"Fruit: this is fun1!"<<endl;
    }
    virtual void fun2(){
        cout<<"Fruit: this is fun2!"<<endl;
    }

    void fun3(){
        cout<<"Fruit: this is fun3!"<<endl;
    }
};

class Apple
:public Fruit{
    public:
    virtual void fun2(){
        cout<<"Apple: this is fun2!"<<endl;
    }

    void fun3(){
        cout<<"Apple: this is fun3!"<<endl;
    }
};

class GreenApple
:public Apple{

};

void Globalfun1(const Fruit* f1){
    cout<<"Globalfun1!"<<endl;
}

void Globalfun2(const smartPtr2<Fruit>& f1){//智能指针记得一定要传引用
    cout<<"Globalfun2!"<<endl;
}

void Globalfun2(const smartPtr2<Apple>& f1){
    //重载Globalfun2
    cout<<"Globalfun3!"<<endl;
}

int main(){
    /*简单复习下虚函数调用规则*/
    {
        Apple* p=new Apple();
        p->fun1();// 输出：Fruit: this is fun1!
                    //Apple里没有fun1(),调用了父类的
        p->fun2();//输出:Apple: this is fun2!
                    //Apple里有fun2(),调用子类的

        Fruit*p2 =new Apple;
        p2->fun3();//输出Fruit: this is fun3!
                    //这个函数不是虚函数,调用了父类的,
    }
    //使用智能指针

    smartPtr2<Fruit> pf = new Fruit();
    Globalfun1(pf);//smart指针到dump指针的转换
    
    smartPtr2<Apple> pa= new Apple;//企图子类智能指针转换到父类智能指针
    Globalfun2(pa);//错误！不存在从 "smartPtr2<Apple>" 到 "smartPtr2<Fruit>" 的适当转换函数
    //解决方法：在智能指针类里用模板进行转化(46~49)。然后就OK了
    
    smartPtr2<GreenApple> pga=new GreenApple();
    Globalfun2(pga);//报错，因为Fruit->Apple->GreenApple的继承关系，
    //pga可以转化为Apple也可以是Fruit，因此出现二义性
    //在可能存在二义的转换时用cast来转换最好！
    return 0;
}