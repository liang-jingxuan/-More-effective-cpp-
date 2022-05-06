#include <iostream>
using namespace std;
template<typename T>
class smartPtr;

template<typename T>
class smartPtr2;//加入了类型转换
//如果有从只能指针到普通指针的转换需求，比如某个库之前不使用智能指针，
//现在想通过使用智能指针进行操作，则需要一些从智能指针到普通指针的转换，以实现兼容
//但是这样做会引入一定的问题：
int main(){
    //1.智能指针不能转到普通指针
    smartPtr<int> p=new int(56);
    fun1(p);//fun1需要的是一个int*类指针,但是p是一个smartPtr<int>类
    //为了解决问题1,可以使用类型转换,当需要从smartPtr<int>转换到另一个类T时转换
    smartPtr2<int> p2=new int(56);
    fun1(p2);//问题1解决!

    //虽然问题1解决了,但是引入了问题2:
    //2.
    return 0;
}

void fun1(int *v){
    return;
}

template<typename T>
class smartPtr{
    public:
        smartPtr(T *data = 0):realPtr(data){};
        
        ~smartPtr(){
            if(realPtr)
                delete realPtr;
        }

        smartPtr(smartPtr<T>& rhs){
            realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

        smartPtr<T>& operator=(smartPtr<T>& rhs){
            if(this==&rhs) return *this;
            delete realPtr;
            realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

        T& operator*()const{
            return *realPtr;
        }

        T* operator->() const{
            return realPtr;
        }

        
    private:
        T* realPtr;
};


template<typename T>
class smartPtr2{
    public:
        smartPtr2(T *data = 0):realPtr(data){};
        
        ~smartPtr2(){
            if(realPtr)
                delete realPtr;
        }

        smartPtr2(smartPtr2<T>& rhs){
            realPtr = rhs.realPtr;

            rhs.realPtr = 0;
        }

        smartPtr2<T>& operator=(smartPtr2<T>& rhs){
            if(this==&rhs) return *this;
            delete realPtr;
            realPtr = rhs.realPtr;
            rhs.realPtr=0;
            return *this;
        }

        T& operator*()const{
            return *realPtr;
        }

        T* operator->() const{
            return realPtr;
        }

        T* operator T*(){return realPtr;}
    private:
        T* realPtr;
};