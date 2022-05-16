#include <iostream>
using namespace std;
//<more effective cpp>指出:判断一个灵巧指针是否指向空不是一件简单的事

class Apple{};

class Orange{};

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

        operator void*(){
            if(realPtr) return realPtr;
            return 0;
        }
        
    private:
        T* realPtr;
};

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
    smartPtr2<int> p2=new int(88);//加入了隐式转换
    
    if(p2==0){}//OK
    if(p2==nullptr){}//OK
    if(p2){}//OK
    if(!p2){}//OK

    //使用解决方法1后可以判断指针是否指向空，但是引入问题2：
    //2.比较逻辑出现混乱
    
    smartPtr2<Apple> pa;
    smartPtr2<Orange> po;
    if(pa==po){//竟然正确
        cout<<"Apple == Orange!"<<endl;
    }


    return 0;
}


