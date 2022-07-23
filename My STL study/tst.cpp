#include <iostream>
#include"Myvector.h"
#include "Mylist.hpp"
#include"Mydeque.hpp"
#include"deque"
using namespace mySTL;


int Myvectortst(){
    //1.Myallocator 文件测试:OK
    //void *p=mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::allocate(66);
    //mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::deallocate(p,66)

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

int Mylisttest(){
    cout<<"*************************start testing mylist********************"<<endl;;
    //1.构造测试
    mySTL::list<int> mlist;
    //2.pushback,pushfront测试 popback,popfront 测试
    for(int ix=5;ix<10;++ix)
        mlist.push_back(ix);
    for(auto num:mlist){
        cout<<num<<',';
    }
    mlist.print();
    cout<<"\n";
    for(int ix=4;ix>-3;--ix)
        mlist.push_front(ix);
    for(auto num:mlist){
        cout<<num<<',';
    }
    mlist.print();

    mlist.pop_back(); 
    cout<<"\n pop the last number:";
        for(auto num:mlist){
        cout<<num<<',';
    }
    
    mlist.pop_front();
    cout<<"\n pop the first number:";
        for(auto num:mlist){
        cout<<num<<',';
    }

    mlist.print();
    //3.back,front测试
    cout<<"\nThe last number is "<<mlist.back().data<<endl;
    cout<<"The first number is "<<mlist.front().data<<endl;
    //4.iterator测试
    cout<<"\n";
    mySTL::list<int>::iterator pl=mlist.begin();
    for(;pl!=mlist.end();++pl)
        cout<<*pl<<',';
    ++pl;
    if(pl==mlist.begin())
        cout<<"\nTest that if the list is cycled. Res:no"<<endl;
    cout<<"Size of the list:" << mlist.size()<<endl;
    //5.测试splice
    cout<<"\nsplice test:"<<endl;
    cout<<"\n1.splice other list test:"<<endl;
    mySTL::list<int> l1;
    //前面pop_front释放了一些空间还给内存链表，所以l1可能会得到这个空间，由于list内装的是int，没有析构函数
    //，所以得到的空间的值可能是之前pop的值
    l1.push_back(15);
    mySTL::list<int> l2;
    l2.push_back(56);
    mlist.splice(mlist.begin(),l1);
    mlist.splice(mlist.end(),l2);
    for(pl=mlist.begin();pl!=mlist.end();++pl)
        cout<<*pl<<',';
    cout<<"\n2.splice a range test:"<<endl;
    mlist.splice(mlist.begin()+6,l1,mlist.begin()+7,mlist.end());
        for(pl=mlist.begin();pl!=mlist.end();++pl)
        cout<<*pl<<',';
    //6. reverse 测试
    cout<<"\n\nreverse test:"<<endl;
    mlist.reverse();
    for(pl=mlist.begin();pl!=mlist.end();++pl)
        cout<<*pl<<',';
    
    //7.sort测试
    cout<<"\n sort test:"<<endl;
    mlist.sort();
    cout<<"\nthe sorted list:"<<endl;
    for(pl=mlist.begin();pl!=mlist.end();++pl)
        cout<<*pl<<',';
    return 1;
}

int Mydequetest(){
    //1.构造测试
    std::cout<<"1.My deque constructor test:"<<endl;
    deque<int> mydq(5,0);
    std::cout<<"Print the constructed deque:";
    for(deque<int>::iterator pdq=mydq.begin();pdq!=mydq.end();++pdq)
        std::cout<<*pdq<<',';
    //2.push_back,push_front,size测试
    std::cout<<"\n1.Push back and front test:"<<endl;
    for(int ix=0;ix<150;++ix){
        mydq.push_back(ix);
        mydq.push_front(-ix);
        std::cout<<"\nThe "<< ix <<" -th push size:";
        std::cout<<mydq.size()<<endl;
    }
    std::cout<<"Print the deque after push:";
    std::cout<<"Print the size of deque after push:";
    std::cout<<mydq.size()<<endl;//理论上是5+150*2=305
    for(deque<int>::iterator pdq=mydq.begin();pdq!=mydq.end();++pdq)
        std::cout<<*pdq<<',';
}
int main(){
    //Myvectortst();
    //Mylisttest();
    Mydequetest();
    return 1;
}