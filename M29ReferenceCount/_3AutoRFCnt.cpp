//在写了一个引用计数功能的类并集成后,自己写的类仍要在构造、拷贝构造、赋值和写入函数对计数进行操作，这样繁琐的工作
//可以实现自动化，不用再在自己写的类里加入计数操作
#include<iostream>
#include "autoRefCount.h"
using namespace std;

class RCstr{
    public:
        RCstr(const char* val=""):value(new strValue(val)){
            //构造value的时候回自动解决引用计数问题,因为value是一个引用计数智能指针
            cout<<"RCstr:Constructor called"<<endl;
        }

        RCstr(const RCstr& rhs):value(*(new RCPtr<strValue>(rhs.value))){
            //构造value的时候回自动解决引用计数问题,因为value是一个引用计数智能指针
            cout<<"RCstr:Copy constructor called"<<endl;
        }

        const char& operator[](int idx) const{
            return value->data[idx];
        }

        char& operator[](int idx){
            if(value->isShare()){//正在被2个及2个以上引用使用
                //value->rmReference();//**
                value = new strValue(value->data);
                //value->addReference();//因为RFcount默认从0开始,要手动+1,RCSTRING.h不用是因为计数从1开始
            }
            value->setUnshareable();//**
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
            strValue(const strValue&);
            void init(const char*);
            ~strValue();
        };
        RCPtr<strValue> value;//如果使用普通指针则要在构造、赋值、拷贝构造函数进行引用计数操作
                                //这里采用了引用计数指针

};

void RCstr::strValue::init(const char* initVal){
    data = new char[strlen(initVal+1)];
    strcpy(data,initVal);
}

RCstr::strValue::strValue(const char* val){
    cout<<"RCstr::strValue: Constructor called!"<<endl;
    init(val);
}

RCstr::strValue::strValue(const strValue& rhs){
    cout<<"RCstr::strValue: Copy constructor called!"<<endl;
    init(rhs.data);
}

RCstr::strValue::~strValue(){
    delete [] data;
}

ostream& operator<<(ostream& s,const RCstr& dt){
    dt.print(s);
    return s;
}


int main(){
    cout<<"s1 constructing process:"<<endl;
    RCstr s1("string1");//构造顺序:RCstr::strValue->RCPtr->RCstr
    cout<<"\ns2 constructing process:"<<endl;
    RCstr s2("string2");
    cout<<"\ns3 constructing process:"<<endl;
    RCstr s3(s1);//这里没有调用任务构造函数,原因在于系统自动生成了的拷贝构造函数,然后value指针构造的时候自动完成了引用计数
    //具体的,是在RCPtr的拷贝构造函数的init()里完成的
    //s3的构造顺序:RCPtr. 理论上应该是 RCPtr拷贝构造->RCstr拷贝构造.但是RCstr拷贝构造由系统生成,无法人为加入代码
    //除非自己写。

    cout<<"-----------The above is the refrence constructing process---------------"<<endl;
    
    cout<<"Obj s3 has " << s3.ref_count() << " referecens" << endl;//理论:2(s1&s3)。实际：2
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

