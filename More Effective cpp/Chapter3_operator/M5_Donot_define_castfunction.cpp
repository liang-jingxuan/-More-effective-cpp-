#include <iostream>
using namespace std;
//类型转换可能导致数据丢失,比如32位的int转为16位的short或char必然会丢失16位数据
//转换的契机:1)单参数的构造函数.2)自己写的类型转换operator

class Rational{//分数
public:
    explicit Rational(int num1 = 0,int num2 = 1)
        :numerator(num1),denominator(num2){}

    //operator double() const{//类型转换operator函数,尽量不要又这样的转换
    //    return static_cast<double>(numerator)/denominator;
    //}
    double asDouble() const{
        return static_cast<double>(numerator)/denominator;
    }
    friend ostream& operator<<(ostream& s,Rational& data);
private:
    int numerator;//分子
    int denominator;//分母
    void print(ostream& s=cout){
        s<<static_cast<double>(numerator)/denominator;
    }
};
ostream &operator<<(ostream &s, Rational &data)
{
    data.print(s);
    return s;
}
template<typename T>
class Array{
    public:
        class ArraySize{ // 这个类是新的
        public:
            ArraySize(int numElements) : theSize(numElements) {}
            int size() const { return theSize; }

        private:
            int theSize;
        };
        Array(int lowBound, int highBound);
        Array(ArraySize _size):size(_size){} // 注意新的声明
        int size()const{return theSize;}
    private:
        ArraySize size;
};

bool operator==( const Array<int>& lhs,
                const Array<int>& rhs){
    if (lhs.size()==rhs.size())
        cout<<"equal size!";     
}
int main(){
    //使用explicit防止隐式转换
        Rational r(1,2);
        //double r2=r;//如果没有operator double() const;
                    //则该行报错,因为没法对一个自定义类Rational转换为已有类double
                    //
        cout<<"r="<<r<<endl;//如果没有定义<<则报错
        //cout<<"r2="<<r2<<endl;
        cout<<r.asDouble()<<endl;
        return 0;
        //但explicit无法 预防 a==static_cast<Array<int> >(b[i])这样逻辑不合理的隐式转换
        //即如果我们设计的类和数组的表现相似的时候 a==b[i]可能会通过编译
        //因为会把b[i]作为参数构造出一个和a相同的对象再调用这个类的==

    //如果我们设计的类和数组的表现相似的时候
    //可以使用一个类Arraysize来控制Array的大小

    Array<int> a(10);
    Array<int> b(10);
    for (int i = 0; i < 10; ++i);
    //if (a == b[i]) // 假设这里打错了,没发现: "a" 应该是 "a[i]";
    // 现在是一个错误。
}