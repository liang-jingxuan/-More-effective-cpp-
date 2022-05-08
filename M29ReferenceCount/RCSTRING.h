#ifndef __RCSTRING_H
#define __RCSTRING_H
#include "string.h"
#include <iostream>
using namespace std;
class RCString;


class RCString{//ReferenceCount String
    public:
        void print(ostream& output)const{
            output << value->data;
        }
    //part1:一个具有计数功能的类
        RCString(const char *str = ""):value(new StringValue(str)){
            //构造
        }
        //加入shareable之前的拷贝构造函数:浅拷贝+计数+1
        //RCString(const RCString& rhs)
        //:value(rhs.value){//初始化的时候浅拷贝
        //    //拷贝构造,关键技术在这里,当我们使用p1=p2的时候,两个指针指向同一个对象
        //    //,而不是每个指针各自指向一个值相同的对象
        //    //这时候1)要实现浅拷贝,并且2)实现计数,refCount在此++
        //   ++(value->refCount);//计数+1
        //}

        //加入shareable之后的拷贝构造函数:检查是否共享,是则直接拷贝+计数+1,否则深拷贝

        RCString(const RCString& rhs){
            if(rhs.value->shareable){
                value=rhs.value;
                ++value->refCount;
            }
            else{
                value = new StringValue(rhs.value->data);
            }
            //这样写之后 RCString s2(s1)或RCString s2=s1;
            //首先会检查s1是否可共享,即之前有无修改记录,有则不可共享,无则可共享
            //这么做的原因是:我们没法知道修改是使用引用修改(s1[0]='S',这样是可以接受的),
            //              还是指针导致的修改(char *p=&s1[0],*p='S',这样是不可接受的)。
            //所以一旦被修改过，就考虑最坏的情况，即使用了不可接受的修改方式
        }

        int ref_count(){
            return value->refCount;
        }

        ~RCString(){
            //-1后发现没有指针指向对象了就析构
            --(value->refCount);
            if(value->refCount==0)
                delete value;
        }

        RCString& operator=(const RCString& rhs){
            //1.赋值操作符第一步要先检查是不是自身赋值!!!或者已经指向同一个对象
            if(value==rhs.value) return *this;
            //2.如果不是同一个对象,释放自己的对象,指向目标对象
            --(value->refCount);
            if(value->refCount==0){
                //正在指向的对象的引用计数-1
                delete value;
            }
            value=rhs.value;
            ++value->refCount;//相当于++rhs.value->refCount;因为指向的都是同一个string
            return *this;
        }

    //part 2:写时拷贝
    const char& operator[](int idx) const{//const RCString
        //不能修改数据,所以是只读
        cout<<"Here process const RCString";
        return value->data[idx];
    }

    char & operator[](int idx){
        //可以修改数据,可读可写
        //这里有需要注意的地方:例如s1,s2和s3都是同一个对象的引用,如果想修改s1但不影响s2和s3,那么需要分离出s1出来后修改
        
        if(value->refCount>1){//有2个及2个以上指向这个对象,应该分离出一个
            --(value->refCount);
            value = new StringValue(value->data);
        }
        //否则直接原地修改
        //或者分离后返回索引
        value->shareable=false;//经过了修改,不再可共享
        return value->data[idx];
    // part3:指针、引用和写时拷贝
    //RCString* p=&s2;
    //cout<<"Ptr to s2 has " << p->ref_count() << " referecens" << endl;//理论:2。实际：1  为什么?
    //这里形成了这样一个数据结构s2->refCout->"String1"<-p,
    //p跳过了refCount,引用计数没有统计到p,而且p对对象的修改直接影响所有引用,比如下面的代码
    //                                       RCString* p=&s2;
    //                                       (*p)[1] = 'T';//不受欢迎的修改
    //                                       char* p2=&s2[3];
    //                                       *p2='I';//不受欢迎的修改   
    //解决方法有2:1)忽略他,写入文档告诉他人不要这么做  2)在数据成员部分加入shareable标志,
    //一旦被修改就设为false,否则为true
    }
    private:
        struct StringValue{//用stuct把对象和计数绑定在一起。
            int refCount;//引用计数
            char *data;//实际string
            bool shareable;//part3
            StringValue(const char *initValue);
            ~StringValue();
        };
        StringValue *value;//实际数据成员
};

RCString::StringValue::StringValue(const char* initValue)
:refCount(1),shareable(true){
    //浅拷贝
    data = new char[strlen(initValue)+1];
    strcpy(data,initValue);
}

RCString::StringValue::~StringValue(){
    delete [] data;//
}



ostream& operator<<(ostream& s, const RCString& str){
    str.print(s);
    return s;
}
#endif