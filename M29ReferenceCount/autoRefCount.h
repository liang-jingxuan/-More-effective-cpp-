#ifndef __AUTO_REF_COUNT_H
#define __AUTO_REF_COUNT_H
//RefCount的引用计数仍需要用户在类中加入相关引用计数操作,这里使用智能指针实现自动引用计数操作
#include "string.h"
#include <iostream>
#include "RefCount.h"//这个类没有变动,直接包含即可
using namespace std;
//功能:1)引用计数. 2)对象不再被需要时摧毁对象,即引用计数为0 .3)跟踪对象是否可共享
//-----------------------引用计数智能指针--------------------
template<class T>
class RCPtr
{
    public:
        RCPtr(T *initVal=0):realPtr(initVal){
            cout<<"RCPtr: Constructor called!"<<endl;
            init();
        }
        
        RCPtr(const RCPtr &rhs):realPtr(rhs.realPtr){
            cout<<"RCPtr: Copy constructor called!"<<endl;
            init();
        }

        ~RCPtr(){
            if(realPtr)
                realPtr->rmReference();//离开生存空间后检查引用数,
        }

        RCPtr& operator=(const RCPtr& rhs){
            if(realPtr==rhs.realPtr) return *this;
            if(realPtr) {
                //有对象,不为0
                //不用检查原来的对象是不是引用数<=1是因为rmreference里面有检查
                realPtr->rmReference();
            }
            realPtr=rhs.realPtr;
            init();//检查现在的对象是不是可共享的
            return *this;
        }

        T& operator*()const{
            return *realPtr;
        }

        T* operator->() const{
            return realPtr;
        }
    private:
        T *realPtr;
        void init();
};
template<class T>
void RCPtr<T>::init(){//检查是否可共享,不可共享则深拷贝,可共享则不拷贝
    if(realPtr==0) return;
    if(realPtr->isShareable()==false){
    //指针指向的对象不能被共享,就深拷贝
        realPtr = new T(*realPtr);
    }
    realPtr->addReference();
    //这里不需要继承RFcount就可以调用的原因:
    //一般使用引用计数智能指针会这样声明RCPtr<classValue>,而classValue类要实现计数就必须
    //继承RFcount,因此这里的T一般情况下是一个共有继承了RFcount的类,
}


#endif