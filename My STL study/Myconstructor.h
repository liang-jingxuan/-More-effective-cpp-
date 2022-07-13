#ifndef __MYCONSTRUCTOR_H
#define __MYCONSTRUCTOR_H
//问：为什么vector<int>在pop_back后虽然被析构了,但是可以从监视中发现数据还在，如果数据还在，那么析构的意义在哪里？
//答：因为int是原生数据，并不需要被析构，所以虽然调用了析构函数，但int是原生类，什么也不用做
//    但是如果vector里放的不是原生类，而是自定义的类，那么这些类就需要被构造和析构。
//用于在已有的空间上构造对象
#include <new.h>
#include "Mytraint.h"
//需要用到型别提取类,
namespace mySTL{

template<class ForwardIterator, class T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*);

template<class ForardIterator>
inline void
__destroy_aux(ForardIterator , ForardIterator , __Mytrue_type);

template<class ForardIterator>
inline void
__destroy_aux(ForardIterator , ForardIterator , __Myfalse_type);
//*******************对于单个对象
template<class T1,class T2>
inline void construct(T1* p,const T2& val){//构造1个
    new (p) T1(val);
}

template<class T>
inline void destroy(T* pointer){//析构1个
    pointer->~T();
}

//***************对于多个对象
template<class ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last){
    __destroy(first,last,value_type(first));
}

template<class ForwardIterator, class T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*){
    typedef typename __Mytype_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first,last,trivial_destructor());
}

template<class ForardIterator>
inline void
__destroy_aux(ForardIterator first, ForardIterator last, __Myfalse_type){
    for(;first<last;++first){
        destroy(&*first);
    }
}

template<class ForardIterator>
inline void
__destroy_aux(ForardIterator , ForardIterator , __Mytrue_type){
//原生类型不需要被构造,所以也不用被析构,这里什么也不用做
}

inline void destroy(char* ,char*){}
inline void destroy(wchar_t * ,wchar_t *){}
}//namespace mySTL
#endif