#ifndef __MYLIST_H
#define __MYLIST_H
#include "Mytraint.h"
namespace mySTL{
//**************1.链表的一个节点的定义
template <class T>
struct __listnode{
    typedef __listnode<T>* pointer;
    pointer prev;
    pointer next;
    T data;
};

//*************2.链表的迭代器
template<class T,class Ref,class Ptr>
struct __list_iterator{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,Ref,Ptr> self;

    typedef bidirectional_iterator_tag iterator_catagory;
    typedef T               value_type;//数据的类型     int
    typedef Ptr             Pointer;//数据的指针        int*
    typedef Ref             reference;//数据的引用      int&
    typedef __listnode<T>*  linktype;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
    
    //迭代器类内应该有一个迭代器
    linktype node;
    //构造函数,用以构造迭代器
    __list_iterator(linktype x):node(x){}//list<int>::iterator p(mylist.begin()+2);
    __list_iterator(){}
    __list_iterator(const iterator& x):node(x.node){}

    //迭代器的遍历规则
    bool operator==(iterator self& x) const{//为什么是self?
        return node==x.node;
    }

    bool operator!=(iterator self& x) const{
        return node!=x.node;
    }

    reference operator*()const{return *(node).data;}

    pointer operator->()const{return &(operator*());}//返回迭代器所指向的节点的地址
};


//**************3.链表的迭代


}//namespace mySTL;
#endif//__MYLIST_H