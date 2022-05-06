#include <iostream>
using namespace std;
//<more effective cpp>指出:判断一个灵巧指针是否指向空不是一件简单的事
template<typename T>
class smartPtr;
int main(){
    smartPtr<int> p=new int(88);
    
    if(p==0){}//报错
    if(p==nullptr){}//报错
    if(p){}//报错
    if(!p){}//报错
    /*
    解决方法1:在class smartPtr里进行类型转换:operator void*();如果为null返回0,否则返回非0.
            这样上面4种判断都是对的
    缺点:不同对象之间进行判断是否相等就会返回true  :pa和pb是指向两个不同对象的指针 pa==pb永远为true
    */

    return 0;
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

