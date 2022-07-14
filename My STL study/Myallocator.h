#ifndef __MYALLOCATOR_H
#define __MYALLOCATOR_H
using namespace std;
#include<stdlib.h>
#include "Mytraint.h"
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
//在STL中,一级空间配置叫__malloc_alloc_template   直接调用malloc和free和realloc
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
//池化分配器
enum    {__ALIGN=8};//上调边界，即底数，比如9~15上调到16=2*8
enum    {__MAX_BYTES=128};//块的最大尺寸，超过这个值使用mlloc_alloc
enum    {__NFREELISTS=__MAX_BYTES/__ALIGN};//free-list的元素个数
template<bool threads,int inst>
class LV2_alloc {
    private:
        static size_t round_up(size_t bytes){
            return (((bytes) + __ALIGN-1)&~(__ALIGN-1));
            //8的倍数的数的二进制特点：1000 ,1 0000 ,1 1000
            //可以发现最后3位都是0,而要加上__ALIGN-1是防止1~7被调到0
            //因为1~7没有第4位

            //1~7分配7,9~15分配16,-∞~0分配0
        }

    private://链表中存储单位
        union obj{
            union obj* free_list_link;//作为指针时名字,4字节
            char    client_date[1];//即1*1个字节,作为数据存储节点时名字
        };
    
    private:
        static size_t freelist_idx(size_t bytes){
            return (((bytes) + __ALIGN-1)/(__ALIGN-1));
        }

        static obj* volatile free_list[__NFREELISTS];
    //返回一个大小为n个对象,把剩余的放在free list中将来使用
        static void *refill(size_t n);
    //配置可以容纳nobj个大小为size的区块,即大小为size*nobj
        static char *chunk_alloc(size_t size,int &nobj);
        
        static char* start;//链表的起始位置
        static char* finish;
        static size_t heap_size;
//++++++++++++++++++++++以上是为了实现二级空间配置器所必须的工具
//++++++++++++用户需要的只是以下三个函数,即分配/释放空间,不管具体如何实现
    public://对外开放的内容只有 释放和分配内存
        static void* allocate(size_t n);
        static void deaalocate(void* p,size_t n);
        static void* reallocate(void* p,size_t old_sz,size_t new_sz);
};

template<bool threads,int inst>
char* LV2_alloc<threads,inst>::start=0;

template<bool threads,int inst>
char* LV2_alloc<threads,inst>::finish=0;

template<bool threads,int inst>
size_t LV2_alloc<threads,inst>::heap_size=0;

template<bool threads,int inst>
LV2_alloc<threads, inst>::obj * volatile 
LV2_alloc<threads, inst>::free_list[__NFREELISTS]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//free_list是一个装载 obj* volatile类型的数组,即free_list里的每个元素都是
//  obj* volatile 类型的

template<bool threads,int inst>
void* LV2_alloc<threads,inst>::allocate(size_t n){
//1.大于128的内存空间由LV1配置器处理
    if(n>__MAX_BYTES)
        return malloc_alloc::allocate(n);
//2.先上调到8的倍数+找到对应的链表数组的下标->(freelist_idx)
    //一个指向obj* volatile的指针,obj* volatile是free_list元素的类型
    //此处 (obj* volatile) *position,相当于 (int) *p
    obj* volatile *position;
    obj* result;
    //相当于p=v+3;
    position = free_list + freelist_idx(n);
    
//3.确保有足够的空间
    result = *position;
    if(0==result){
        void* r=refill(round_up(n));//获取空间,并分配一个给r
        return r;
    }
//4.从该数组对应下标所指的链表中获取一个节点
    //相当于free_list[freelist_idx(n)]=free_list[freelist_idx(n)]->free_list_link
    *position=result->free_list_link;//指向下一个节点
    return result;
}

template<bool threads,int inst>
void LV2_alloc<threads,inst>::deaalocate(void *p,size_t n){
    //1.判断是否>128,是则用free
    if(n>128){
        malloc_alloc::deallocate(p,n);
        return;
    } 
    //2.找到应该放置的位置
    obj* q=(obj*)p;
    obj* volatile *position 
                = free_list + freelist_idx(n);
    //3.放回free_list中(链表节点插入)
    q->free_list_link=*position;//先让插入节点q指向插入点下一个指针
    *position=q;//再让前置节点指向插入节点q
}

template<bool threads,int inst>
void * LV2_alloc<threads,inst>::refill(size_t n){//装填内存大小为n的数组元素
    int nobjs=20;//默认装填20个
    //1.获取20个大小为n的空间
    char* chunk = chunk_alloc(n,objs);//objs是输入参数也是输出参数
                                    //输入:objs=需要数量
                                    //输出:objs=获得的数量
    obj* volatile *position;
    obj* result;//返回给用户的1个空间
    obj* current_obj,*next_obj;
    int i;
    //2.获得的空间的进行分配
    //2.1.只获得了1个,把这1个分给用户
    if(nobjs==1) return chunk;
    //2.2.获得了多个,把1个留给用户,其余的nobjs-1放到free_list中
    position= free_list + freelist_idx(n);
    result =(obj*)chunk;
    *position=next_obj=(obj*)(chunk + 1*n);//+1*n是因为前n个byite给用户的
    //注意:因为调用这个函数以为着列表中没有相应的内存块了,因此不用考虑把获取的
    //      chunk和剩下的内存进行连接,只需要考虑把获取的内存进行连接
    for(i=1;i<nobjs;++i){
        current_obj=next_obj;
        next_obj=(obj*)((char*)next_obj+n);//告诉系统这个大小为obj的内存的起始地址
        current_obj->free_list_link=next_obj;
    }
    current_obj->free_list_link=0;//已经分配完了
    //3.整理完毕,返回用户所请求的空间
    return result;

}
//*******************************************4.内存的基本处理工具
//4.1--uninitialized_fill_n:填充n个值为x的数据在first开始的位置,返回结束位置
template<class ForwardIterator,class Size,class T,class T1>
ForwardIterator 
__uninitialized_fill_n(ForwardIterator ,Size ,const T& ,T1*);//声明

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x, __Myfalse_type);//声明

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x,__Mytrue_type);//声明

template<class ForwardIterator,class Size,class T>
ForwardIterator 
uninitialized_fill_n(ForwardIterator first,Size n,const T& x){//在first位置填充n个x
    return __uninitialized_fill_n(first,n,x,value_type(first));//为什么不直接填T()?
}

template<class ForwardIterator,class Size,class T,class T1>
ForwardIterator 
__uninitialized_fill_n(ForwardIterator first,Size n,const T& x,T1*){
    typedef typename __Mytype_traits< T1 >::is_POD_type  is_POD_type;
    return __uninitialized_fill_n_aux(first,n,x, is_POD_type());
}

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x, __Myfalse_type){
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
__uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__Myfalse_type);

template<class InputIterator,class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__Mytrue_type);

template<class InputIterator,class ForwardIterator,class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*);

template<class InputIterator,class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result){
    return __uninitialized_copy(first,last,result,value_type(result));
}

template<class InputIterator,class ForwardIterator,class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*){
    typedef typename __Mytype_traits<T>::is_POD_type is_POD_type;
    return __uninitialized_copy_aux(first,last,result,is_POD_type());
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
template<class InputIterator,class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__Mytrue_type){
    return copy(first,last,result);
}
//4.3 uninitialized_fill
}//namespace mySTL
#endif