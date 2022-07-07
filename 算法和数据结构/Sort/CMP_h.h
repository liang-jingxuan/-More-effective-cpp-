#ifndef __CMP_H
#define __CMP_H
//存储了常见的比较方式
#include"vector"
template<typename T>
class CmpLess{
    public:
        CmpLess(){}

        bool operator()(T &num1,T &num2){
            return num1<num2;
    }
};

template<typename T>
class CmpGreater{
    public:
        CmpGreater(){}

        bool operator()(T &num1,T &num2){
            return num1>num2;
    }
};

class CmpSpecial{
    //各个位上的数相加*(正负)
    public:
        CmpSpecial(){}

        bool operator()(int num1,int num2){
            int tmpnum1=0,tmpnum2=0;
            bool num1symbol=num1>0?true:false,num2symbol=num2>0?true:false;
            num1*=num1symbol?1:-1;
            num2*=num2symbol?1:-1;
            while(num1!=0){
                tmpnum1+=num1%10;
                num1/=10;
            }
            tmpnum1*=num1symbol?1:-1;
            while(num2!=0){
                tmpnum2+=num2%10;
                num2/=10;
            }
            tmpnum2*=num2symbol?1:-1;
            return CmpLess<int>()(tmpnum1,tmpnum2);
        }
};


#endif
