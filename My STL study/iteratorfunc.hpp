#ifndef __ITERATOR_CUNC_H
#define __ITERATOR_CUNC_H
//全局迭代器函数
#include"Mytraint.h"

namespace mySTL{
//1.求两个迭代器之间的距离
//用来实现 p1-p2,
    template<class InputIterator>
    inline typename Myiterator_traits< InputIterator >::defference_type
    __distance(InputIterator first,InputIterator last,random_access_iterator_tag){
        return last-first;//随机访问迭代器可以尾-首
    }
    
    template<class InputIterator>
    inline typename Myiterator_traits< InputIterator >::defference_type
    __distance(InputIterator first,InputIterator last,input_iterator_tag){
        //逐一累加
        typename Myiterator_traits<InputIterator>::defference_type res=0;
        while(first!=last){
            ++first;
            ++res;
        }
        return res;
    }


    template<class InputIterator>
    inline typename Myiterator_traits< InputIterator >::defference_type
    distance(InputIterator first,InputIterator last){
        typedef typename Myiterator_traits<InputIterator>::iterator_category category;
        return __distance(first,last,category());
    }


//2.指针的快速定位,
//如果不写这个函数,那么迭代器的位置需要多次++p,但是写了这个可以p+6






    template<class InputIterator,class Distance>
    inline void
    advance(InputIterator& i,Distance n){
        __advance(i,n,iterator_category(i));
    }

}//namspace mySTL






#endif