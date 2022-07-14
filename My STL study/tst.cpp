#include <iostream>
#include"Myvector.h"
using namespace std;
int main(){
    //1.Myallocator 文件测试:OK
    //void *p=mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::allocate(66);
    //mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::deallocate(p,66);

    mySTL::Myvector<int> v;
    cout<<"Capacity of v:"<<v.capacity()<<endl;
    v.push_back(4);
    int* p=&v[0];//等价于mySTL::Myvector<int>::iterator 因为typedef T* iterator,
                //其中T是int  所以int*是iterator原型
    cout<<*p<<endl;
    cout<<v[0]<<endl;
    cout<<"Size of v:"<<v.size()<<endl;
    cout<<"Capacity of v:"<<v.capacity()<<endl;

    v.push_back(15);
    cout<<"Size of v:"<<v.size()<<endl;
    cout<<"Capacity of v:"<<v.capacity()<<endl;

    v.push_back(9);
    cout<<"Size of v:"<<v.size()<<endl;
    cout<<"Capacity of v:"<<v.capacity()<<endl;
    //结论1：如果一个一个地插入元素，则容量的增长是1 2 4 8 16

    mySTL::Myvector<int> v2(10,0);
    cout<<"Size of v2:"<<v2.size()<<endl;//10
    cout<<"Capacity of v2:"<<v2.capacity()<<endl;//10

    v2.push_back(9);
    cout<<"Size of v2:"<<v2.size()<<endl;//11
    cout<<"Capacity of v2:"<<v2.capacity()<<endl;//20

    //结论2:如果使用了批量初始化,那么容量的初始大小是批量数据的数量,之后再一个一个地插入元素,容量以2倍速度增长

    mySTL::Myvector<int> v3(v);
    for (auto num:v3){cout<<num<<',';}
    cout<<"\nSize of v3:"<<v3.size()<<endl;//
    cout<<"Capacity of v3:"<<v3.capacity()<<endl;//

    //结论3:系统提供的拷贝构造函数足矣.
    cout<<"\n\n test 4"<<endl;
    cout<<"Original v=";
    for (auto num:v ){cout<<num<<',';}
    cout<<"\nOriginal v3=";
    for (auto num:v3 ){cout<<num<<',';}
    v[1]=83;
    cout<<"After v=";
    for (auto num:v ){cout<<num<<',';}
    cout<<"\n After v3=";
    for (auto num:v3){cout<<num<<',';}
    v3.clear();
    cout<<"\nSize of v3:"<<v3.size()<<endl;//
    cout<<"Capacity of v3:"<<v3.capacity()<<endl;//

    cout<<"\nSize of v:"<<v.size()<<endl;//
    cout<<"Capacity of v:"<<v.capacity()<<endl;//
    
    
    //结论4:系统提供的拷贝构造函数属于浅拷贝,即只是将this->start=rhs.start...
    //至于为什么v3在clear后v的大小仍然为3,这是因为这个vector里装的是原生数据类型int,
    //      对于原生数据类型,析构的时候什么也不做,因此v仍然可以正常使用。但如果容器里装的是自定义类，
    //      此时操作v必然是危险的事。可以参考lazy coding原则，在调用[]的时候进行深拷贝。


    mySTL::Myvector<int> v4({1,3,4,45});//OK
    //mySTL::Myvector<int>v4{1,5,6};//OK
    cout<<"\nSize of v:"<<v4.size()<<endl;//
    cout<<"Capacity of v:"<<v4.capacity()<<endl;//
    cout<<"\n After v4=";
    for (auto num:v4){cout<<num<<',';}
    return 1;
}