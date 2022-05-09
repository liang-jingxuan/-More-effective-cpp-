#include <iostream>
#include "RefCount.h"
using namespace std;
//前提:每个需要引用计数功能的类都加入相应的代码很繁琐,直接写一个引用计数类,需要引用计数的类都继承这个类即可
//我们把这个计数的类写入了"RefCount.h".然后在这个文件写一个string演示如何在一个自己写的类里引入这个计数功能
class RCstr{
    public:
        RCstr(const char* val):value(new strValue(val)){
            //由类的设计者负责调用addReference
            value->addReference();//**引用+1
        }

        RCstr(const RCstr& rhs){//拷贝构造 rhs是已有的，this是新创建的
            //可分享
            if(rhs.value->isShareable()){//**
                value=rhs.value;
            }
            //不可分享
            else{
                value = new strValue(rhs.value->data);
            }
            value->addReference();//**因为引用计数初始值都是0，所以无论怎样都要+1
        }

        ~RCstr(){
            if(value){//**有2个以上引用
                value->rmReference();//**
                *this=nullptr;//因为还有其他引用，所以不能直接delete,要先和对象脱离关系
            }
        }

        RCstr& operator=(const RCstr& rhs){//rhs是已有的，this也是已有的，
            //this可能已经指向rhs也可能指向另一个对象
            if(value==rhs.value) return *this;
            //处理自己的现有的对象
            if(value)//**
                value->rmReference();//**
            //}else{//<=1
            //    delete value;//不必要的,因为rmReference里面会检查
            //}
            //可共享
            if(rhs.value->isShareable()){//**
                value=rhs.value;
            }
            else{
                value = new strValue(rhs.value->data);
            }
            value->addReference();//**
            return *this;
        }

        const char& operator[](int idx) const{
            return value->data[idx];
        }

        char& operator[](int idx){
            if(value->isShare()){//正在被2个及2个以上引用使用
                value->rmReference();//
                value = new strValue(value->data);
                value->addReference();//因为RFcount默认从0开始,要手动+1,RCSTRING.h不用是因为计数从1开始
            }
            value->setUnshareable();//
            return value->data[idx];
        }

        int ref_count(){
            return value->REFCNT();
        }

        void print(ostream& s)const{
            s<<value->data;
        }
    private:
        struct strValue //strValue 定义在类内，说明只有这个类能使用。在全局定义则开放
        :public RFcount{
            char* data;
            strValue(const char*);
            ~strValue();
        };
        strValue *value;

};

RCstr::strValue::strValue(const char* val){
    data=new char[strlen(val+1)];
    strcpy(data,val);
}

RCstr::strValue::~strValue(){
    delete [] data;
}

ostream& operator<<(ostream& s,const RCstr& dt){
    dt.print(s);
    return s;
}


int main(){
    RCstr s1("string1");
    RCstr s2("string2");
    RCstr s3(s1);
    cout<<"Obj s3 has " << s3.ref_count() << " referecens" << endl;//理论:2。实际：2
    cout<<"Obj s2 has " << s2.ref_count() << " referecens" << endl;//理论:1。实际：1
    cout<<"Obj s1 has " << s1.ref_count() << " referecens" << endl;//理论上s1和s3引用数相同,都是2。实际：2
    RCstr* p=&s2;
    (*p)[1] = 'T';//不受欢迎的修改
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:1,实际:1 .字符串:sTring1
    char* p2=&s2[3];
    *p2='I';//不受欢迎的修改
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:1,实际:1 .字符串:sTrIng1
    cout<<"s2="<< s2 <<". Ptr to s2 has " << p->ref_count() << " referecens" << endl;//理论:1。实际：1
    s2=s1;
    cout<<"Obj s2 has " << s2.ref_count() << " referecens" << endl;//理论:3。实际：3
                                                        //,因为s1可以共享,共享前有s1,s3两个引用,共享有s1~s3
    //此时s1,s2和s3都指向同一个字符串
    s1[2]='q';//结果:srqing1,s1修改了结果,s1被分离,s1有1个ref,s2和s3共用一个
    cout<< s1 <<" has " << s1.ref_count() << " referecens" << endl;//理论:1,实际:1.字符串:srqing1
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:2,实际:2.字符串:string1
    cout<< s3 <<" has " << s3.ref_count() << " referecens" << endl;//理论:2,实际:2
}
