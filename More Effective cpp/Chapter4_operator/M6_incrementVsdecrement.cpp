//讨论++和--:
//包括前置和后置
#include <iostream>
using namespace std;

class UPInt{
    public:
        UPInt(int num):i(num){};
        UPInt& operator++(){//++前缀,即++i,先加再赋值
            *this+=1;
            return *this;
        }
        
        const UPInt operator++(int){//++后缀,即i++
            UPInt oldVal = *this;
            ++(*this);
            return oldVal;
            //返回const的原因:避免傻逼代码如 i++++
            //i++++会调用i.operator++(0).operator++(0);
        }
    
        UPInt& operator--(){//--前缀
            *this-=1;
            return *this;
        }

        const UPInt operator--(int){//--后缀
            UPInt oldVal = *this;
            --(*this);
            return oldVal;
        }

        //实际实现运算逻辑的地方是-=和+=
        //调用 前置的时候                ++(*this)  ->  (*this)+=
        //调用 后置的时候 (*this)++  ->  ++(*this)  ->  (*this)+=
        UPInt& operator+=(const UPInt& data){
            *this = UPInt(this->i + data.i);
            return *this;
            //因为 a+=6;的时候调用的是a.operator(6),我们希望在左值直接加上右值
        }

        UPInt& operator-=(const UPInt& data){
            *this = UPInt(this->i - data.i);
            return *this;
        }
        friend const UPInt operator+(const UPInt& lhs,
                        const UPInt& rhs);
        friend const UPInt operator-(const UPInt& lhs,
                        const UPInt& rhs);
        friend const UPInt operator-(const UPInt& lhs,
                                int rhs);
        //这样两两搭配需要进行枚举,会很麻烦,因此会忍不住使用转换
        friend ostream& operator<<(ostream& s,UPInt& data);
        friend ostream& operator<<(ostream& s,const UPInt& data);
    private:
        int i;
        void print(ostream& s){
            s<<i;
        }
};

ostream& operator<<(ostream& s,UPInt& data){
    data.print(s);
    return s;
}

ostream& operator<<(ostream& s,const UPInt& data){
    const_cast<UPInt&>(data).print(s);
    return s;
}

const UPInt operator+(const UPInt& lhs,
                        const UPInt& rhs){
    //调用这个的时候我们并不想改变符号两边的值 c=a+b,我们并不希望改变a和b,所以返回另一个对象
    return UPInt(lhs.i)+=rhs.i;
}
const UPInt operator-(const UPInt& lhs,
                        const UPInt& rhs){
    //调用这个的时候我们并不想改变符号两边的值 c=a+b,我们并不希望改变a和b,所以返回另一个对象
    return UPInt(lhs.i)-=rhs.i;
}
const UPInt operator-(const UPInt& lhs,
                        int rhs){
    //调用这个的时候我们并不想改变符号两边的值 c=a+b,我们并不希望改变a和b,所以返回另一个对象
    return UPInt(lhs.i)-=rhs;
}
int main(){
    UPInt i(8);
    cout<<i++<<endl;//返回8,i=9,调用i.operator++(0)
    cout<<++i<<endl;//返回10,i=10,调用i.operator++()
    UPInt a(8),b(9);
    cout<<a+b<<endl;
    //a+b=5;//憨批代码
    return 0;
}