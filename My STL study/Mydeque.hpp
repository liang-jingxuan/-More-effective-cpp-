#ifndef __MY_DEQUE_H
#define __MY_DEQUE_H
#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//notice:"原创代码"是没有按书本写的代码,可能有bug
//        CTRL+F搜索  "important"查找重要信息
//deque的中控器类似于 
// int a1[8],a2[8],a3[8];
// int* p1=a1,*p2=a2,*p3=a3;                                              (iterator)  
// int**  node=&p1;  ++node则跳到node=&p2, 再次++node则跳到node=&p3         (node)
//但是node不知道总共有多少个指针,因此需要map_size来确保node不会指向p3之后的指针

using namespace mySTL;
namespace mySTL{
    //全局
const size_t BufSz = 512;
const size_t InitialMapSize=8;//默认申请的存储区个数
inline size_t __deque_buf_size(size_t n,size_t sz){
        return  n!=0?n:(sz<BufSz?size_t(BufSz/sz):size_t(1));//如果sz>=256,则一个存储区存一个数据,相当于链表了
}

//先实现迭代器,因为deque内部维护了两个迭代器。
//important：vector，list和deque都先实现了迭代器再写容器，但是不代表写代码的时候应该先写迭代器再写类。
//  STL先实现迭代器再写容器是因为容器内部维护了至少一个迭代器，比如vector中的start和finish，list中的node；
//  deque中的start，finish
template<class T,class Ref,class Pointer,size_t BufSz>
struct __deque_iterator{
    typedef __deque_iterator<T,T&,T*,BufSz> iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSz> const_iterator;

    
    static size_t buffer_size();//一个存储区能放的元素个数

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



    void set_node(map_pointer new_node){//存储区选择,让迭代器指向指定的存储区
    //set_node保证跳到正确的存储区,并让first,last指向正确的范围
        node=new_node;
        first=*new_node;//operator*会返回new_node的cur指向的元素
        last=first + difference_type(buffer_size());//last指向的空间不存储元素
    }

    reference operator*()const{return *cur;}

    pointer operator->()const{return &(operator*());}

    difference_type operator-(const self& rhs){
        return difference_type(buffer_size() * (node-rhs.node-1))//被填满的存储器
                +(cur-first)+(rhs.last-rhs.cur);//两边未满的存储区 假设rhs在左,
    }

    self& operator++(){//++i
        ++cur;//右移
        //1.存储区已满
        if(cur==last){
            set_node(node+1);//切换存储区
            cur=first;//指向存储区第一个元素
        }
        return *this;
    }

    self operator++(int){//i++;
        self tmp=(*this);
        ++(*tmp);
        return tmp;
    }
    
    self& operator--(){//--i
        //1.cur是最左端
        if(cur==first){
            set_node(node-1);//切换存储区
            cur=last;//指向存储区末端元素,开区间
        }
        --cur;//不管是不是最左端元素都要--
        return *this;
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

template<class T,class Ref,class Pointer,size_t BufSz>
size_t __deque_iterator<T,Ref,Pointer,BufSz>::buffer_size()
{return __deque_buf_size(BufSz,sizeof(T));}


//*******************************deque的实现

template<class T,class Alloc= pool_alloc,size_t BufSz = 0>
class deque {
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type&     reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        typedef __deque_iterator<T,T&,T*, BufSz>    iterator;
    protected:
        typedef pointer*        map_pointer;
        typedef Mysimple_alloc<value_type,Alloc>    node_allocate;
                                    //node_allocate配置一个数据类型T的的存储空间
        typedef Mysimple_alloc<pointer,Alloc>       map_allocate;
                                    ////配置一个指向存储区的指针的空间
    protected:
    //important:start和finish只用set_node来操作,因为很多重复的代码,而且每个iterator里有4个指针要设置
    //      分别是node,first,last,cur
        iterator start;//struct{T*,T*,T*,T**}类型
        iterator finish;//注意要左闭右开
        
        map_pointer map;//T**类型

        size_type   map_size;//作用:1.用来确定总共有多少个存储区 2.初始化时使被占用的T**尽量居中
                            //3.和start以及finish进行运算可以知道左右两侧备用空间是否充足(至少两边各有一个备用)
                            //比如:左侧:map_size-(map_size-(start.node-map))=start.node - map
                            //    右侧：map_size - (finish.node-map)
    
    static size_t buffer_size(){return __deque_buf_size(BufSz,sizeof(T));};//一个存储区能放的元素个数

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

        void push_front(const T& val){
            //判断是否需要切换存储区
            if(start.cur!=start.first){
                //不需要切换存储区
                --start.cur;
                construct(start.cur,val);
                return;
            }
            push_front_aux(val);//需要切换存储区
        }

        void pop_back(){
            //无元素时不pop
            if(empty()) return;
            //判断是否需要切换存储区
            if(finish.cur!=finish.first){
                //不需要切换
                --finish.cur;//important:注意finish是开区间,该指针不指向元素,因此先移动再destroy
                destroy(finish.cur);
                
                return;
            }
            node_allocate::deallocate(finish.first);
            finish.set_node(finish.node--);
            finish.cur=finish.last--;
            destroy(finish.cur);//先切换然后-1,再destroy,是因为STLfinish不指向存储区最后一个地址
        }

        void pop_front(){
            if (empty()) return;
            if(start.cur!=(start.last-1)){
                destroy(start.cur);
                --start.cur;
                return;
            }
            //需要切换到下一个存储区
            destroy(start.cur);
            node_allocate::deallocate(start.first);
            start.set_node(start.node+1);
            start.cur=start.first;
        }

        void clear(){
            //析构所有元素,释放所有空间,只留一个
            if(empty()) return;
            //1.先处理必然是被填满了的存储区,即(start,finish)
            map_pointer to_del=start.node+1;//to_del是一个指向存储区的节点,*to_del表示节点
            for(;to_del<finish.node;++to_del){
                destroy(*to_del,*to_del + buffer_size());
                map_allocate::deallocate(*to_del,buffer_size());
            }
            //原创代码,我把所有元素析构,并释放了所有空间,然后让start居中
            //          STL代码:析构所有元素,释放start以外的空间,start保持不变
            //处理start和finish
            if(start!=finish){
                //start和finish不同存储区,析构并释放finish
                destroy(finish.first,finish.cur);
                node_allocate::deallocate(finish.first,buffer_size());
                //析构start,但保留
                destroy(start.cur,start.last);
                node_allocate::deallocate(start.first,buffer_size());
                
            }
            else{
                //start和finish在相同存储区
                destroy(start.cur,finish.cur);
                node_allocate::deallocate(start.first,buffer_size());
            }
            start = map + map_size/2;//让start居中
            finish=start;
        }

        iterator erase(iterator pos){//移除position,返回移除后补上的元素
            if(pos==finish) return pos;
            iterator _destination=pos+1;
            size_type num_befor_pos = pos - start;
            //start * * pos , _des * * *,num_befor_pos =3;
            //1.如果pos右边的元素较少,左边较多,则用copy从前往后复制
            if(num_befor_pos > size()-num_befor_pos){
                // copy [_First, _Last) to [_Dest, ...)
                copy(_destination , finish , pos);
                pop_back();
            }
            else{
            //2.如果pos左边的元素较少,右边较多,则用copy_backward从后往前复制,避免覆盖
                // (copy_backward)copy [_First, _Last) backwards to [..., _Dest)
                copy_backward(start , pos , _destination);
                pop_front();
            }
            return start + num_befor_pos;
        }

        iterator erase(iterator erase_left,iterator erase_right){
            if(erase_left==start&&erase_right==finish){ 
                clear();
                return finish;
            }
            //将[erase_left,erase_right)的元素擦除
            size_type left_sz,right_sz,num_to_erase;//消除区间两边的大小
            num_to_erase = erase_right - erase_left;
            left_sz  = erase_left -  start;
            right_sz = finish     -  erase_right;
            //如果left_sz > right_sz则将消除区右边的移动到erase_left开始的地方
            if(left_sz > right_sz){
                // (copy_backward)copy [_First, _Last) backwards to [..., _Dest)
                copy_backward(start , erase_left , erase_right);
                iterator new_start = start + num_to_erase;//记录新起点,新起点前的元素都要被析构
                destroy(start , new_start);//析构num_to_erase个元素,因为消除了num_to_erase个元素
                //释放new_start左方所有内存
                for(map_pointer cur = start.node;cur<new_start.node;++cur){
                    node_allocate::deallocate(*cur,buffer_size());
                }
                start = new_start;
            }
            else{
                // copy [_First, _Last) to [_Dest, ...)
                copy(erase_right,finish,erase_left);
                iterator new_finish = finish - num_to_erase;
                destroy(new_finish,finish);
                for(map_pointer cur=finish.node;cur>new_finish;--cur){
                    node_allocate::deallocate(*cur,buffer_size());
                }
                finish=new_finish;
            }
            return start + left_sz;
        }

        iterator insert(iterator pos, const T& val){//原创代码
            //作用在pos前面插入一个val
            //思路:使用push_back或push_front插入一个任意的元素,然后复制元素,最后修改pos-1元素为val
            //1.如果使用push_back,说明插入元素后右边需要搬移元素更少
            size_type left_num_after = pos - start;
            size_type right_num_after = finish - pos;
            iterator new_pos;
            if(right_num_after<left_num_after){
                size_type num_to_move = right_num_after;
                push_back(val);
                new_pos = pos;//根据推演,发现push_back后新的插入点刚好是pos
                copy_backward(pos , finish-1 , finish);
                *new_pos=val;
            }
            else{
            //2.如果使用push_front,说明插入元素后左边搬移元素更少
                iterator num_to_move = left_num_after;
                push_front(val);
                new_pos = pos -1;
                copy(start+1,pos,start);
                *new_pos=val;
            }
            return new_pos;
        }

    protected:
        //初始化的时候采用
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
            map_size = initial_map_size()<num_nodes+2?num_nodes+2:initial_map_size();//有多少个T**
            map = map_allocate::allocate(map_size);//构造足够存放map_size个T**数据空间
                                    //important:一般得到8*map_size大小的空间,因为在64位系统中一个指针的大小是8字节
                                    //sizeof(map)=8,注意不是8*map_size.因为map是一个指向指针的指针,我们用map_size来确定T**个数
            //3.调整start和finish
            map_pointer nstart = map + (map_size - num_nodes) /2;//存放该lem_num个数据将占用nstart~nfinish所指的T**
            map_pointer nfinish = nstart + num_nodes-1;
            //4.为每个map_pointer分配存储区
            map_pointer cur=nstart;
            //cur用来遍历map的上的每个T**,并让每个T**指向一个存储区
            for(; cur<=nfinish; ++cur){
                *cur = (T*)allocate_node();//分配存储区,并让T**指向存储区
            }//cur是

            start.set_node(nstart);//set_node保证跳到正确的存储区,并让first,last指向正确的范围
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + elem_num % buffer_size();//important:没有-1，说明是左闭右开区间
        }

    void push_back_aux(const T& x){
        value_type val_copy = x;
        reserve_map_at_back();//检查是否要申请新的存储区
        *(finish.node + 1) = node_allocate();//必须要先申请新空间,因为切换到了一个未被开发的存储区

        construct(finish.cur,val_copy);
        finish.set_node(finish.node+1);//在要切换存储区的情况下先进行了构造再切换
                                        //这说明finish不能指向存储区的最后一个地址
        finish.cur= finish.first;
    }

    void push_front_aux(const T& x){
        value_type val_copy = x;
        reverse_map_at_front();//看看是否要配置新的存储区
        *(start.node-1)=node_allocate();//必须要先申请新空间,因为切换到了一个未被开发的存储区
        start.set_node(start.node-1);
        start.cur=start.last--;//important,不要用--i,可以用i--
        construct(start.cur,val_copy);
    }
        
        
    void reserve_map_at_back(size_type nodes_to_add = 1){//nodes_to_add表示需要增加的节点个数
        if(nodes_to_add + 1 > map_size - (finish.node-map)){
            //一次增加至少2个新的存储区,右边表示finish右侧可用的存储区个数
            //如果右侧可用存储区个数少于需要增加的存储区数量
            //意味着:"--如果不指定nodes_to_add--"那么finish右边始终至少有2个存储区
            //原因：push_back的时候finish在存储区末尾的时候,会检查finish右边剩余存储区
            //      判断语句左边nodes_to_add(默认为1)+1>{0,1}的时候就分配空间，
            //      所以右边始终至少有2个存储区。
            //证明：因为nodes_to_add
            reallocate_map(nodes_to_add,false);
            }
    }

    void reverse_map_at_front(size_type nodes_to_add = 1){
        if(nodes_to_add > start.node - map)
            reallocate_map(nodes_to_add,true);
    }

    void reallocate_map(size_type nodes_to_add, bool add_at_front){
        //增加nodes_to_add个缓存区,
        size_type old_num_nodes = finish.node - start.node + 1;//左闭右闭所以+1,占用的存储区
        size_type new_num_nodes = old_num_nodes + nodes_to_add;//新增存储区后占用的存储区

        map_pointer new_nstart;
        if (map_size > 2 * new_num_nodes) {
            //情况一：现有的存储区个数足以存储2倍于已用的存储区个数,只需要进行指针的重定向，不需要新空间
            //1.让被占用的map尽量靠中间:总共需要占用new_num_nodes个存储区,共有map_size个存储区
            new_nstart= map + (map_size-new_num_nodes)/2//(map_size-new_num_nodes)/2表示让前后未被使用
                            + (add_at_front?nodes_to_add:0);//的存储区数量均匀
                //(add_at_front?nodes_to_add:0)是因为如果是push_front说明左方需要更多空间,所以靠右一些
            if(new_nstart< start.node)//观察是否会被覆盖(类似vector的erase)
                copy(start.node,finish.node+1,new_nstart);//前移,不会被覆盖
            else
                copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
                //...start.node * * * * new_nstart * finish.node ...，如果用copy那么有可能
                // [new_nstart~finish.node]的内容被覆盖
        }
        else{
            //情况二：需要申请新的map,并进行迁移
            //1.申请了新的节点后总大小
            size_type new_map_size = map_size + (map_size<nodes_to_add?nodes_to_add:map_size) + 2;
            //map_size+nodes_to_add就够用了,但是为了避免多次配置新空间,如果发现map_size>=nodes_to_add
            //就多申请一些,所有有了 (map_size<nodes_to_add?nodes_to_add:map_size) + 2
            //important:申请空间的时候考虑多申请一些
            map_pointer new_map= map_allocate::allocate(new_map_size);
            new_nstart = new_map + (new_map_size-new_num_nodes)/2
                                    + (add_at_front?nodes_to_add:0);
            //不用担心覆盖问题
            copy(start.node,finish.node+1,new_nstart);
            map_allocate::deallocate(map,map_size);

            map=new_map;
            map_size=new_map_size;
        }
        //start和finish只用set_node处理
        start.set_node(new_nstart);
        finish.set_node(new_nstart+old_num_nodes-1);
    }

    void* allocate_node(){
        //返回一个存储区
        return node_allocate::allocate(buffer_size());
    }

    size_type initial_map_size(){
        return InitialMapSize;
    }
};
}
#endif