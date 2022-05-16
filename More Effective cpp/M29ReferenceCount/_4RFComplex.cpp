#include "RefCount.h"
#include <iostream>
#include <math.h>
using namespace std;
//一个已经存在,没有计数功能的类，为这个类加入引用计数功能
//这个类并不一定正确,比如Complex类和int的运算没有解决
//      

class Complex{
    public:
        friend ostream& operator<<(ostream& s,const Complex& data);
        //friend class RCComplex;//将自身开放给RCComplex,RCComplex可以任意访问自己
        Complex(int real=0,int image=0):rl(real),img(image){}

        Complex(const Complex& rhs):rl(rhs.rl),img(rhs.img){}
        
        ~Complex(){}
        //两个参数的运算法必须在类外进行定义
        friend const Complex operator-(const Complex& rhs,
                            const Complex& lhs);
        
        friend const Complex operator+(const Complex& rhs,
                            const Complex& lhs);
        
        Complex& operator=(const Complex& rhs){
            if(*this==rhs) return *this;
            rl=rhs.rl;
            img=rhs.img;
            return *this;
        }

        Complex& operator+=(const Complex& rhs){
            rl=rl+rhs.rl;
            img=img+rhs.img;
            return *this;
        }

        Complex& operator-=(const Complex& rhs){
            rl=rl-rhs.rl;
            img=img-rhs.img;
            return *this;
        }

        bool operator==(const Complex& rhs)const{
            if(rl==rhs.rl&&img==rhs.img)
                return true;
            return false;
        }

        double abs() const{
            return sqrt(rl*rl+img*img);
        }
    private:
        int rl;//实
        int img;//虚
    
        void print(ostream& s) const{
            if(img>=0)
                s<<rl<<"+"<<img<<"i";
            else
                s<<rl<<img<<"i";
        }
        bool operator>(Complex& rhs);
        bool operator<(Complex& rhs);
};

const Complex operator+ (const Complex& rhs,
                        const Complex& lhs){
    Complex result(lhs);
    return result+=rhs;
}

const Complex operator-(const Complex& rhs,
                            const Complex& lhs){
        //return *(new Complex(rl-rhs.rl,img-rhs.img));//不要用new,因为无法预知谁来进行delete
    Complex result(lhs);
    return result-=rhs;
} 

ostream& operator<<(ostream& s,const Complex& data){
    data.print(s);
    return s;
}
//第一步:写一个智能指针
template<class T>
class RCIPtr{
    public:
        int ref_count(){
            return counter->REFCNT();
        }

        RCIPtr(T* p=0):counter(new CountHolder){
            counter->realPtr = p;
            init();
        }

        RCIPtr(const RCIPtr& rhs):counter(rhs.counter){
            init();
        }

        ~RCIPtr(){
            counter->rmReference();
        }

        RCIPtr& operator=(const RCIPtr& rhs){
            if(counter!=rhs.counter){
                counter->rmReference();//即将发生赋值,调用rmReference
                counter=rhs.counter;
                init();
            }
            return *this;
        }


        const T* operator->() const{
            return counter->realPtr;
        }

        T* operator->(){
            //因为有可能改变指针的指向,比如p是一个指向complex的指针p->img=1;这样修改的对象
            makeCopy();
            return counter->realPtr;
        }

        const T& operator*() const{
            return *(counter->realPtr);
        }

        T& operator*(){
            makeCopy();
            return *(counter->realPtr);
        }


    private:
        struct CountHolder
        :public RFcount{
            ~CountHolder(){delete realPtr;}
            T* realPtr;//这里的T是Complex
        };//声明

        CountHolder* counter;//之所以要这个中间层的原因:
                            //需要继承RFcount,但是不能在类里继承,因为我们要计数的是数据
                            //而不是对象。

        void init(){
            if(counter->isShareable()==false){//发现自己的对象不可共享
                T *oldValue = counter->realPtr;//脱离关系
                counter = new CountHolder;//找新欢
                counter->realPtr = new T(*oldValue);//指向新对象
            }
            counter->addReference();
        }

        void makeCopy(){
            if(counter->isShare()){
                T* oldVal = counter->realPtr;
                counter->rmReference();//发生了赋值,调用rmReference
                counter = new CountHolder;
                counter->realPtr = new T(*oldVal);
                counter->addReference();
            }
        }
};
//第二步:对类进行修改 从Complex->RCComplex,之后如果不需要引用计数就定义Complex
//      需要引用计数就定义RCComplex

class RCComplex{
    public:
        friend ostream& operator<<(ostream& s,const RCComplex& data);
        
        RCComplex(int real=0,int image=0)
        :value(new Complex(real,image)){}

        RCComplex(const Complex& rhs):value(new Complex(rhs)){
        }

        double abs() {
            return value->abs();//不需要传参,因为value指向一个Complex类,内部包含了实部和虚部
        }
        
        //赋值运算使用系统生成的即可,会对左右两边的数据成员一一赋值,赋值的时候回调用RCIPtr的operator=
        //RCComplex& operator=(RCComplex& rhs){
        //    value->operator=(*(rhs.value));
        //    return *this;
            //value->operator=(*(rhs.value))将value所指向的Complex赋值为rhs.value所指向的Complex
            //然后返回修改完成的value,返回*this即可.返回value不行,因为value是RCIPtr<Complex>类型而
            //不是RCComplex
        //}

        friend const RCComplex operator+(const RCComplex& lhs,
                                    const RCComplex& rhs);

        friend const RCComplex operator-(const RCComplex& lhs,
                                    const RCComplex& rhs);

        RCComplex& operator+=(RCComplex& rhs){
            value->operator+=(*(rhs.value));//->左边的变量就是调用者,即函数中的*this
            return *this;
        }

        RCComplex& operator-=(RCComplex& rhs){
            value->operator-=(*(rhs.value));
            return *this;
        }

        bool operator==(RCComplex& rhs){
            return value->operator==(*(rhs.value));
        }

        int refcount(){
            return value.ref_count();
        }
    private:
        RCIPtr<Complex> value;
        //RCComplex(const Complex& initVal):value(){}
        //int rl;
        //int img;
        void print(ostream& s) const{
            s<<*value;
        }
};

const RCComplex operator- (const RCComplex& lhs,
                            const RCComplex& rhs){
    //Complex result=*lhs.value;
    //result-=*rhs.value;
    return RCComplex(*lhs.value-*rhs.value);
}

const RCComplex operator+ (const RCComplex& lhs,
                            const RCComplex& rhs){
            //return RCComplex((value->operator+(*(rhs.value))).rl,
            //                 (value->operator+(*(rhs.value))).img);

            //(value->operator+(*(rhs.value)))返回一个Complex对象,但是我们需要的是RCComplex
            //所以使用Complex构造一个RCComplex,但是RCComplex不能访问Complex的私有对象,所以要先在
            //Complex里把RCComplex设置为friend
    return RCComplex(*lhs.value+*rhs.value);
}



ostream& operator<<(ostream& s,const RCComplex& data){
    //这个函数不能去掉,因为cout<<num1;中右边是一个RCComplex
    data.print(s);
    return s;
}

int main(){
    RCComplex num1(5,3);
    cout<<num1<<endl;
    RCComplex num2(3,2);
    RCComplex num4=num2;
    cout<<"num2= "<<num2<<" has "<<num2.refcount()<<" reference(s)"<<endl;//理论:2(√)
    cout<<"num1= "<<num1<<" has "<<num1.refcount()<<" reference(s)"<<endl;//理论:1(√)
    cout<<num1<<" + "<<num2<<"="<< num1+num2 <<endl;//num1+num2里面移除了1个reference,发生的地方在于:
    //                                                num1+num2总共生成了1个临时变量,Complex::operator+返回的值
    cout<<"num2= "<<num2<<" has "<<num2.refcount()<<" reference(s)"<<endl;//理论:2(√).
    num2+=num1;
    cout<<"num2= "<<num2<<" has "<<num2.refcount()<<" reference(s)"<<endl;//理论:1(√).因为上一步num2的值进行了修改
    //因为num2参与了运算,参与运算的时候在内部必然调用->,而调用->就必须深拷贝,防止函数调用改变了对象
    cout<<num1<<"-"<<num2<<"="<<num1-num2<<endl;
    if(num1==num2)
        cout<<num1<< "=="<< num2<<endl;
    else
        cout<<num1<< "!="<< num2<<endl;
    //if(num1>num2)
    //    cout<<"num1>num2"<<endl;
    RCComplex num3=num2;
    cout<<num3<<endl;
    cout<<"num3= "<<num3<<" has "<<num3.refcount()<<"reference(s)"<<endl;//理论值是2(√),分别是num3和num2
    return 0;
}