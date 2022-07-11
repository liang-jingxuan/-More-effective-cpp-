#ifndef __MYCONSTRUCTOR_H
#define __MYCONSTRUCTOR_H
//用于在已有的空间上构造对象
#include <new.h>
//需要用到型别提取类,
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
__destroy_aux(ForardIterator first, ForardIterator last, _false_type){
    for(;first<last;++first){
        destroy(&*first);
    }
}

template<class ForardIterator>
inline void
__destroy_aux(ForardIterator first, ForardIterator last, _true_type){

}

inline void destroy(char* ,char*){}
inline void destroy(wchar_t * ,wchar_t *){}

#endif