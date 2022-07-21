#ifndef __MY_DEQUE_H
#define __MY_DEQUE_H
#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//notice:"原创代码"是没有按书本写的代码,可能有bug
const size_t BufSz = 512;

using namespace mySTL;
namespace mySTL{
template<class T,class Ref,class Pointer,size_t BufSz>
struct __deque_iterator{
    typedef __deque_iterator<T,T&,T*,BufSz> iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSz> const_iterator;
    static size_t buffer_size(){return __deque_fub_size(BufSz,sizeof(T));}//一个存储区能放的元素个数

    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T&                          reference;
    typedef T*                          pointer;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef T**                         map_pointer;

    typedef __deque_iterator            self;

    T*              cur;        //指向存储区正在遍历的元素
    T*              first;      //指向存储区的开始
    T*              last;       //指向存储区的末尾
    map_pointer     node;       //node指向一个存储区

    inline size_t deque_buf_size(size_t n,size_t sz){
        return  n!=0?n:(sz<512?size_t(512/sz):size_t(1));//如果sz>=256,则一个存储区存一个数据,相当于链表了
    }

    void set_node(map_pointer new_node){//存储区选择,让迭代器指向指定的存储区
    //set_node保证跳到正确的存储区,并让first,last指向正确的范围
        node=new_node;
        first=*new_node;//operator*会返回new_node的cur指向的元素
        last=first + difference_type(buffer_size);
    }

    reference operator*()const{return *cur;}

    pointer operator->()const{return &(operator*());}

    difference_type operator-(const self& rhs){
        return difference_type(buffer_size * (node-rhs.node-1))//被填满的存储器
                +(cur-first)+(rhs.last-rhs.cur);//两边未满的存储区 假设rhs在左,
    }

    self& operator++(){//++i
        ++cur;//右移
        //1.存储区未满
        if(cur!=last){
            return *this;
        }
        //2.存储区已满
        set_node(node+1);//切换存储区
        cur=first;//指向存储区第一个元素
    }

    self operator++(int){//i++;
        self tmp=(*this);
        ++(*tmp);
        return tmp;
    }
    
        self& operator++(){//--i
        //1.cur不在存储区开端
        if(cur!=first){
            --cur;
            return *this;
        }
        //2.cur在存储区开端
        set_node(node-1);//切换存储区
        cur=last;//指向存储区末端元素
    }

    self operator--(int){//i--;
        self tmp=(*this);
        --(*tmp);
        return tmp;
    }

    self& operator+=(difference_type n){
        //要考虑n的正负
        difference_type offset =n +(cur-first);//如果n是正,则offset表示初始存储区的first开始到目标位置要经过的节点数
                                            //如果n为负,则offset表示必然要跨越存储区
                                            //总结:offset表示从初始存储区的first出发,到达目标位置所需的跳数
        if(offset>=0&&offset<difference_type(buffer_size())){
            //>=0说明有可能不需要跨越存储区,因为为负必须跨存储区
            //offset<buffersize说明从该存储区的first移动offset个单位后就是所需的位置
            //等价于cur=first+offset;  ==将offset代入,cur=first+n+cur-first=n+cur,cur+=n
            cur+=n;
        }
        else{
            //需要跨存储区
            difference_type node_offset=offset>0?
                                        offset/difference_type(buffer_size())://offset>0
                                        -difference_type((-offset-1)/buffer_size())-1;//offset<=0
                        //-offset是为了把跳数调整为正数,-offset-1是因为存储区是左闭右闭区间,
                        //末尾-1是因为初始存储区不需要算上
            set_node(node+node_offset);
            cur= first+(offset-node_offset*difference_type(buffer_size()));
        }
        return *this;
    }

    self operator+(difference_type n) const{//p2=p1+5 我们并不希望p1+5,而是想要p1+5之后
        self tmp=*this;//所以要先复制(*this),不对*this进行操作
        return tmp+=n;
    }

    self operator-=(difference_type n){return *this+=-n;}

    self operator-(difference_type n) const{
        self tmp=*this;
        tmp+=-n;
    }

    reference operator[](difference_type n) const{
        return *(*this+n);
    }

    bool operator==(const self& x)const{
        return cur==x.cur;
    }

    bool operator!=(const self& x)const{
        return !((*this)==x);//等价于!(*this==x)因为*的优先级高于==
    }

    bool operator<(const self& x)const{
        return (node==x.node)?(cur<x.cur):(node<x.node);
        //node==x.node说明在同一个存储区,
        //如果不在同一个存储区,那么看看存储区的先后关系
    }
};



//*******************************deque的实现

template<class T,class Alloc= pool_alloc>
class deque {
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type&     reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        typedef __deque_iterator<T,T&,T*,BufSz>    iterator;
    
    protected:
        typedef pointer*        map_pointer;
        typedef Mysimple_alloc<value_type,Alloc>    node_allocate;//配置一个存储区的空间
        typedef Mysimple_alloc<pointer,Alloc>       map_allocate;//配置一个T**空间
    protected:
        iterator start;
        iterator finish;

        map_pointer map;

        size_type   map_size;
    
    public://用户接口
        iterator begin(){return start;}
        iterator end(){return finish;}

        reference front(){return *start;}
        reference back(){return *(finish--);}

        reference operator[](const size_type  n){
            return start[difference_type(n)];
        }

        size_type size(){return finish-start;}

        size_type max_size(){return size_type(-1);}
        bool empty(){return finish==start;}

        //上面是简单的查询
        //下面需要用到构造,内存管理
        deque(size_type num, const value_type &val)
            :start(),finish(),map(0),map_size(0)
        {
            fill_initialize(num,val);
        }

        void push_back(const T&x){
            //1.判断finish是不是存储区最后一个元素
            if(finish.cur != finish.last-1){//在当前存储区即可解决
               //2.不是则在finish位置上构造一个x,然后令finish后移 
               construct(finish.cur,x);
               ++finish.cur;
               return;
            }
            //不能再当前存储区进行插入,需要切换到finish的下一个节点,
            //但有可能finish已经是最后一个存储区,这种情况下要申请新的存储区
            push_back_aux(x);
        }

    protected:
        void fill_initialize(size_type num, const value_type &val){
            //1.创建num个空间,每个空间的大小都是buffer_size()
            create_map_and_nodes(num);
            //2.构造num个val
            map_pointer cur;//遍历每个T**
            for(cur=start.node;cur<finish.node;++cur)
                uninitialized_fill(*cur,*cur+buffer_size(),val);//能填满一个存储区的
            
            uninitialized_fill(finish.first,finish.cur,val);//不能填满一个存储区的
        }

        void create_map_and_nodes(size_type elem_num){
            //1.计算放下elem_num个节点需要的节点数,节点是指存储区
            //buffer_size返回一个存储区能存放的数据个数(注意不是大小)
            size_type num_nodes = elem_num/buffer_size() +1;

            //2.确定map(T**)的个数,最少8个,最多num_nodes+2ge
            max_size = initial_map_size()<num_nodes+2?num_nodes+2:initial_map_size();//有多少个T**
            map = map_allocate::allocate(max_size);//构造足够存放max_size个T**数据空间

            //3.调整start和finish
            map_pointer nstart = map + (max_size - num_nodes) /2;//存放该lem_num个数据将占用nstart~nfinish所指的T**
            map_pointer nfinish = nstart + num_nodes-1;

            map_pointer cur;//用来遍历map的上的每个T**,并让每个T**指向一个存储区

            for(cur=nstart;cur!=nfinish;++cur){
                *cur=allocate_node();//分配存储区,并让T**指向存储区
            }

            start.set_node(nstart);//set_node保证跳到正确的存储区,并让first,last指向正确的范围
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + elem_num % buffer_size();
        }

    void push_back_aux(const T& x){
        value_type val_copy = x;
        reserve_map_at_back();//检查是否要申请新的存储区
        *(finish.node + 1) = node_allocate();

        construct(finish.cur,val_copy);
        finish.set_node(finish.node+1);
        finish.cur= finish.first;
    }

    void reserve_map_at_back(size_type nodes_to_add = 1){
        if(nodes_to_add + 1 > map_size - (finish.node-map))//一次增加至少2个新的存储区,右边表示finish右侧可用的存储区个数
                                            //如果右侧可用存储区个数少于需要增加的存储区数量
            reallocate_map(nodes_to_add,false);
    }

    void reallocate_map(size_type nodes_to_add, bool add_at_front){
        
    }
};
}
#endif