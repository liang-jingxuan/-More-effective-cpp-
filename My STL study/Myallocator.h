#ifndef __MYALLOCATOR_H
#define __MYALLOCATOR_H
using namespace std;
#include<stdlib.h>
#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#   include<iostream>
#   define __THROW_BAD_ALLOC std::cerr<<"oom!"<<std::endl; exit(1);
#endif
namespace mySTL{
//*******************************************1.包装
template<class T,class Alloc>//T是数据类型,Alloc是分配一个T所使用的空间分配算法
class Mysimple_alloc{//这个类只是转调用
    public:
        static T* allocate(size_t n){//分配n个T类型的空间大小
            return 0==n?0:(T*)Alloc::allocate(n*sizeof(T));
        }

        static T* allocate(void){//分配1个T类型数据的空间大小
            return (T*)Alloc::allocate(sizeof(T));
        }

        static void deallocate(T* p,size_t n){//释放n个以p空间开始的T类型数据的空间大小
            if(0!=n) Alloc::deallocate(p,n*sizeof(T));
        }

        static void deallocate(T* p){
            Alloc::deallocate(p,sizeof(T));
        }
};


//*******************************************2.一级空间配置器
//在STL中,一级空间配置叫__malloc_alloc_template
template<int inst>
class LV1_alloc{
    private:
        static void *oom_malloc(size_t);//函数
        static void *oom_realloc(void *,size_t);//函数
        static void (* __malloc_alloc_oom_handler)();//函数指针
    
    public:
        static void* allocate(size_t n){//分配一个大小为n的空间
            void *result=malloc(n);
            if(result==0)   result=oom_malloc(n);//分配不到内存的时候的处理办法
            return result;
        }

        static void deallocate(void *p,size_t){
            free(p);
        }

        static void* reallocate(void *p,size_t,size_t new_size){
            void* result=realloc(p,new_size);
            if(0==result) result=oom_realloc(p,new_size);
            return result;
        }
        
        //这个函数设置oom处理函数,用来腾出一些内存空间
        static void* (* set_malloc_handler(void (*f)()))(){
            void (*old_handler)()=__malloc_alloc_oom_handler;//函数指针,指向旧的处理函数的地址
            __malloc_alloc_oom_handler=f;//更新处理函数
            return (old_handler);//返回旧的处理函数，方便用来再次使用
        }
};
//静态变量初始化
template<int inst>
void (* LV1_alloc<inst>::__malloc_alloc_oom_handler)()=0;//初始化:指向空

template<int inst>
void* LV1_alloc<inst>::oom_malloc(size_t n){
    void (*my_alloc_handler)();//定义一个函数指针
    void *result=0;//定义一个指针

    for(;result==0;){//循环直到分到空间为止
        my_alloc_handler=__malloc_alloc_oom_handler;
        if(0==my_alloc_handler)//还没有决定好怎么处理
            {__THROW_BAD_ALLOC;}//摆烂,因为想好怎么处理oom是开发人员的任务

        (*my_alloc_handler)();//试图弄出一些内存空间

        result=malloc(n);//有了新的空间后尝试要点空间
    }
    return (result);
}

template<int inst>
void* LV1_alloc<inst>::oom_realloc(void *p,size_t n){
    void (*my_alloc_handler)();
    void *result=0;

    for(;result==0;){
        my_alloc_handler=__malloc_alloc_oom_handler;
        if(0==my_alloc_handler)
            {__THROW_BAD_ALLOC;}

        (*my_alloc_handler)();

        result=realloc(p,n);//和oom_malloc的区别之处
    }
    return (result);
    
}

typedef LV1_alloc<0> malloc_alloc;//改个好记的名字

//void (*old)()=(*set_malloc_handler)(钦定的获取空间函数);


//*******************************************3.二级空间配置器



//*******************************************4.内存的基本处理工具
//4.1--uninitialized_fill_n:填充n个值为x的数据在first开始的位置,返回结束位置
template<class ForwardIterator,class Size,class T>
ForwardIterator 
uninitialized_fill_n(ForwardIterator first,Size n,const T& x){//在first位置填充n个x
    return __uninitialized_fill_n(first,n,x,value_type(first));//为什么不直接填T()?
}

template<class ForwardIterator,class Size,class T,class T1>
ForwardIterator 
__uninitialized_fill_n(ForwardIterator first,Size n,const T& x,T1*){
    typedef typename __Mytype_traits<T1>::is_POD_type mySTL::is_POD;
    return __uninitialized_fill_n_aux(first,n,x,mySTL::is_POD());
}

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x,__Myfalse_type){
    ForwardIterator cur=first;
    for(;n>0;--n,++cur){
        construct(&*cur,x);
    }
    return cur;
}

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x,__Mytrue_type){
    return fill_n(first,n,x);//因数据类型是原生类型，有高效的填充算法
}

//4.2**uninitialized_copy  把first到last之间的对赋值到position位置
//输入: 1.起始位置  2.结束为止  3.目标位置
//输出: 复制后的结尾位置
template<class InputIterator,class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result){
    return __uninitialized_copy(first,last,result,value_type(result));
}

template<class InputIterator,class ForwardIterator,class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*){
    typedef typename __Mytype_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first,last,result,is_POD);
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__Myfalse_type){
    ForwardIterator cur=result;
    for(;first!=last;++first,++cur){
        construct(&*cur,*first);
    }
        
    return cur;
}
//4.3 uninitialized_fill
}//namespace mySTL
#endif