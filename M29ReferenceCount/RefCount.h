#ifndef __REF_COUNT_H
#define __REF_COUNT_H
//每个需要引用计数的类都加入相应的代码很繁琐,直接写一个引用计数类,需要引用计数的类都继承这个类即可
#include "string.h"
#include <iostream>
using namespace std;
//功能:1)引用计数. 2)对象不再被需要时摧毁对象,即引用计数为0 .3)跟踪对象是否可共享
class RFcount{
    public:
        RFcount():shareable(true),RefCnt(0){}

        RFcount(const RFcount&):shareable(true),RefCnt(0){
            //拷贝构造函数,调用方法 T s2(s1);或T s2=s1; T是包含了引用计数的类
            //这样的代码是先构造s2,再以s1作为参数调用拷贝构造函数,所以
        }
        RFcount& operator=(const RFcount&){return *this;}

        virtual ~RFcount()=0;//纯虚函数,这个类不能被实例化
        
        void addReference(){++RefCnt;}
        void rmReference(){
            --RefCnt;
            if(RefCnt==0)
                delete this;
        }
        void setUnshareable(){shareable=false;}
        bool isShareable(){
            //查看目前的对象是不是可以共享的
            return shareable;
        }
        bool isShare(){
            //对象是不是正在被多个引用共享,多个的意思是2个及以上
            return RefCnt>1;
        }

        int REFCNT() const{
            return RefCnt;
        }

    private://继承者无法访问,只能通过函数来设置
        bool shareable;
        int RefCnt;
};
RFcount::~RFcount(){}

#endif