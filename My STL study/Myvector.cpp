#include"Myvector.h"
using namespace std;

//public内容:对用户开放的内容

template<class T,class Alloc>
Myvector<T,Alloc>::iterator//返回值,因为这么iterator是在Myvector类内定义的,只在该类的内部使用
Myvector<T,Alloc>::begin(){return start;}

template<class T,class Alloc>
Myvector<T,Alloc>::iterator
Myvector<T,Alloc>::end(){return finish;}

template<class T,class Alloc>
Myvector<T,Alloc>::size_type
Myvector<T,Alloc>::size() const {return (size_type)(finish-start);}

template<class T,class Alloc>
bool //bool不是Myvector特有的,所以不用指定
Myvector<T,Alloc>::empty()const{ return start==finish;}
/*
template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::front()const{return *start;}

template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::back()const{return *(finish-1);}
*/
template<class T,class Alloc>
Myvector<T,Alloc>::size_type
Myvector<T,Alloc>::capacity() const{return (size_type)(end_of_storage-start);}

template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::operator[](size_type n){return *(start+n);}


//protected内容:不对用户开放的,但可能被public函数调用实现某些功能的函数

template<class T, class Alloc>
void Myvector<T,Alloc>::insert_aux(iterator position,const T& x){

}