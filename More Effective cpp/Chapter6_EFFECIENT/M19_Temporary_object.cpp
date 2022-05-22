
#include <iostream>
#include "string"
using namespace std;

int fun1(const string& str ){
    return str.size();
}

void fun2( string& str ){
    for(int ix=0;ix<str.size();++ix){
        if(str[ix]<='z'&&str[ix]>='a')
            str[ix]+=26;
    }
}

class Rational { 
    public: 
        Rational(int numerator = 0, int denominator = 1)
            :num(numerator),den(denominator){
                cout<<"Rational(int,int);"<<endl;
            }

        ~Rational(){
            cout<<"~Rational()"<<endl;
        }
        
        Rational(Rational& r){
            cout<<"Rational(Rational& r);"<<endl;
            this->num=r.num;
            this->den=r.den;
        }
        int numerator() const{return num;} 
        int denominator() const{return den;} 
    private:
        int num;
        int den;
}; 
const Rational  operator*(const Rational& lhs, 
                    const Rational& rhs){
    return Rational(lhs.numerator() * rhs.numerator(), 
                    lhs.denominator() * rhs.denominator()); 
    
}



int main(){
    //M19临时变量从何而来
    //临时对象就是在堆上产生的对象,一般都是由：
    //          1.new产生,
    //          2.函数传递过程中产生。如果函数会修改对象，就要注意隐式转换生成临时对象，比如25&29行
    //          3.函数返回的时候。
    char str1[]="Effective c++";
    fun1(str1);//OK,但是中途会生成类型为string的临时对象
    //fun2(str1); //报错:无法用 "char [14]" 类型的值初始化 "std::string &" 类型的引用(非常量限定)
                //因为参数传递过程中fun2接受的其实是临时对象，对临时对象的修改不改变str1，所以C++禁止了

    const char str2[]="More Effective c++";
    //fun2(str2);//就算是const也不行


    //M20，协助完成返回值优化
    //函数返回的时候很多情况下不得不返回对象，但是返回对象要面临构造和析构。
    //所以要为函数做正确的行为为宗旨。即，为了正确，即使要面临构造和析构也只能返回对象。
    //                                  不要为盲目为了效率导致奇怪的语法
    //1.为了效率返回一个指针:
    //const Rational * operator*(const Rational& lhs, 
    //                const Rational& rhs); 
    //Rational a = 10; 
    //Rational b(1, 2); 
    //Rational c = *(a * b); //运算符*返回的是指针,正确,但导致了奇怪的语法

    //2.为了效率返回一个引用:
    //const Rational & operator*(const Rational& lhs, 
    //                const Rational& rhs){
    //Rational result(lhs.numerator() * rhs.numerator(), 
    //                lhs.denominator() * rhs.denominator()); 
    //return result;
    //                }
    //Rational a = 10; 
    //Rational b(1, 2); 
    //Rational c = a * b;//正确,但其实c已经被析构

    //3.为了正确而让步
    Rational a(1,2); 
    Rational b(1, 2); 
    Rational c = a * b;//正确,且没有产生临时对象
    cout<<"c.denominator()="<<c.denominator()<<"c.numerator()="<<c.numerator()<<endl;//无输出
    return 0;

    //M21 隐式转换
    //在文件M6_incrementVsdecrement.cpp中,我实现了一个UPint来模仿int
    //为了实现UPint+int,int+UPint,UPint+UPint,我需要穷举所有可能的情况,那么对于+-*/就需要12个函数进行实现
    //其中int+int是无法实现的,因为重载的时候必须要要有一个自定义类
    //8-2法则指出并不一定所有运算规律都用得上,要根据实际来重载运算符

    //M22赋值操作符=和其单独形式
    //单独形式即+-/*,赋值形式即+=,-=,*=,/=
    //1.赋值形式效率会更高,因为使用+=的时候我们通常会把值赋给左边的对象,因此返回引用即可
    //2.一个符号就有3次重载,如果有一点修改就要全部进行修改.因此我们把实际实现放在+=中
    //  这样我们维护的时候只需要维护+=即可
    //比如下面的UPInt operator-的内部实现是调用operator-=
}
//class UPInt{
    //...
//        UPInt& operator-=(const UPInt& data){
//            *this = UPInt(this->i - data.i);
//            return *this;
//        }
//...
//};
//const UPInt operator-(const UPInt& lhs,
//                        const UPInt& rhs){
//    //调用这个的时候我们并不想改变符号两边的值,即c=a+b中不改变a和b,所以要创建新对象在新对象上进行操作
//    return UPInt(lhs.i)-=rhs.i;
//}
//const UPInt operator-(const UPInt& lhs,
//                        int rhs){
//    return UPInt(lhs.i)-=rhs;
//}




