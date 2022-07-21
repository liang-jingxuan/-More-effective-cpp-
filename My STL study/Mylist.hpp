#ifndef __MYLIST_H
#define __MYLIST_H
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
#include "iteratorfunc.hpp"
//CTRL+F 搜查 important可以找到重要知识点
//#include "list"
//std::list<T>::iterator  //在文件stl_list.h
using namespace mySTL;
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
//node是__listnode<T>*类型的,也就是说 node是一个指向节点的指针
//important:迭代器是一种对指针进行包装的类
template<class T,class Ref,class Ptr>
struct __list_iterator{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,Ref,Ptr> self;//迭代器本身

    typedef bidirectional_iterator_tag iterator_category;
    typedef T               value_type;//数据的类型     int
    typedef Ptr             pointer;//数据的指针        int*
    typedef Ref             reference;//数据的引用      int&
    typedef __listnode<T>*  linktype;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
    
    //linktype=__listnode<T>*,即linktype是一个指向节点的指针
    linktype node;
    //构造函数,用以构造迭代器
    __list_iterator(linktype x):node(x){}//list<int>::iterator p(mylist.begin()+2);
    __list_iterator(){}
    __list_iterator(const iterator& x):node(x.node){}

    //迭代器的遍历规则
    bool operator==(const self& x) const{//为什么是self?
        return node==x.node;
    }

    bool operator!=(const self& x) const{
        return node!=x.node;
    }

    //reference是指对象本身,由于node是一个指向节点的指针,因此需要先解引用*(node)得到节点,然后
    reference operator*()const{return (*node).data;}//指向指针所指的对象*(node)的数据*(node).data

    //pointer指的是,指向对象的指针,如调用p->的时候,我们是间接引用对象的成员
    pointer operator->()const{return &(*node);}//返回迭代器所指向的对象的指针

    //从下面的代码可以回到self是指迭代器本身
    //  理由:返回值是self&的时候返回了*this,this是一个指向__list_iterator的指针,而*this就是迭代器本身.
    self& operator++(){//++p
        node=(linktype)((*node).next);//等价于node=node->next
        return *this;
    }

    self& operator--(){//--p
        node=(linktype)((*node).prev);//等价于node=node->next
        return *this;//返回*this是因为this是一个指针,*this表示返回指向的对象
    }

    self operator++(int){//p++
        //不返回self&,因为返回的是一个copy,退出函数会被析构,因此返回一个临时对象即可
        self res=*this;
        ++*this;
        return res;
    }

    self operator--(int){//p++
        //不返回self&,因为返回的是一个copy,退出函数会被析构,因此返回一个临时对象即可
        self res=*this;
        --*this;
        return res;
    }

    self operator+(int n){
        advance(*this,n);
        return *this;
    }

    self operator-(int n){
        (*this).operator+(-n);
        return *this;
    }
};


//**************3.链表的迭代
//linktype==__listnode<T>*
//iterator==__list_iterator<T,T&,T*>==struct{__listnode<T>* }  
//*iterator即调用 __list_iterator<T>::operator*()

template<class T,class Alloc=pool_alloc>
class list{
    protected:
        typedef __listnode<T> list_node;//节点,不向外开
    public:
        typedef list_node* linktype;//指向节点的指针 linktype=__listnode<T>*
        typedef Mysimple_alloc<list_node,Alloc> node_allocator;//第一个参数是list_node
                                                                //是因为这个容器装的是一个个链表节点.

        typedef __list_iterator<T,T&,T*>                iterator;//iterator是一个包装着指向节点的指针的结构体(类)
        typedef __list_iterator<T,const T&,const T*>    const_iterator;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef list_node                               reference;
        typedef T                                       value_type;
        typedef value_type*                             pointer;
        typedef const value_type&                       const_reference;
        typedef const value_type*                       const_pointer;

    protected:
        linktype node;//node是一个指向节点的指针  所以*node是节点本身
                        //list的本质是一个指向一个节点的指针
    public://用户接口
        iterator begin(){
            return (linktype)(*node).next;
        }
        iterator end(){
            return node;
        }//左闭右开,因为end必须是一个开区间
        bool empty(){return (*node).next==node;}
        size_type size(){
            size_type res;
            res=mySTL::distance(begin(),end());//迭代器函数
            return res;
        }

        reference front(){
            //链表头结点
            return *(node->next);
        }

        reference back(){
            return *(node->prev);
        }

        //构造和析构
        list():node(node_allocator::allocate()){
            node->prev=node;
            node->next=node;
        }

        void push_back(const T& x){
            insert(end(),x);
        }

        void push_front(const T& x){
            insert(node->next,x);
        }

        void pop_front(){
            erase(begin());
        }

        void pop_back(){
            erase(--end());
        }

        iterator insert(iterator position,const T& x){//插队函数
            //让position成为新节点的下一个节点
            linktype tmp=creatnode(x);
            tmp->next=position.node;
            tmp->prev=position.node->prev;
            tmp->prev->next=tmp;
            position.node->prev=tmp;
            return tmp;
        }

        void clear(){
            linktype cur=begin();
            while(cur!=node){
                linktype tmp=cur;//tmp标记要删除的节点,方便destroy
                cur=tmp->next;//标记下一个要删除的节点
                destroynode(tmp);
            }
            node->prev=node;
            node->next=node;
        }

        void remove(const T& x){
            //移除所有值为x的数据
            iterator first=begin();
            iterator last=end();
            while(first!=last){
                iterator next=first;
                ++next;
                if(*first==x) erase(first);
                first=next;
            }
        }

        void unique(){
            if(empty()) return;
            iterator first=begin(),last=end(),next=first;
            while(++next!=last){
                if(*first==*next){
                    erase(next);
                }else{
                    first=next;
                }
                next=first;
            }
        }

        iterator erase(iterator position){
            linktype next_node=(position.node)->next;//创建一个指针指向下一个节点
            linktype prev_node=(position.node)->prev;
            prev_node->next=next_node;
            next_node->prev=prev_node;
            destroynode(position.node);
            //release_node(position.node);//important:destroynode已经包含了析构+释放内存,不应该再一次释放内存
            //这一次释放一个已经释放的内存导致了内存池出现了循环链表.
            return iterator(next_node);            
        }
    protected:
        linktype get_node(){//配置空间
            return node_allocator::allocate();
        }

        void release_node(void* p){//释放空间
            node_allocator::deallocate((list_node*)p);
        }

        linktype creatnode(const T& x){//配置空间+构造
            linktype p=node_allocator::allocate();
            construct(&p->data,x);//为什么只构造数据?
                                //因为原生指针没有构造和析构一说
            return p;
        }

        void destroynode(linktype p){
            destroy(&p->data);//为什么只destroy数据?
            release_node(p);
        }

        //iterator是有附加功能的*p,因此*iterator相当于p,即迭代器本身
        void transfer(iterator position,iterator __first, iterator __last){
            //将[first,last)的元素插队到position,即
            //输入:[first,last),x,x,x,x,position
            //输出:x,x,x,x,[fist,last-1] position
            //*(__first.node)->prev;//运算顺序:  . -> *
            //(*__first.node).prev;//运算顺序:  . * .

            iterator first_prev=linktype((*__first.node).prev);//important:.(成员选择)的优先级高于*
            iterator position_prev=linktype((*position.node).prev);
            if(__last!=position){//因为要搬到position之前,如果last==position,说明不用搬
                (*position_prev.node).next=__first.node;//连接position_prev-first
                (*__first.node).prev=position_prev.node;
                //(*__first.node).prev=&(*position_prev.node);//也可以这样
                //(*__first.node).prev是一个指针,position_prev.node是一个指针
                //  因此右边应该是一个地址或者一个指针,而*position_prev.node是一个对象,
                //  因此右边需要取地址,或者将右边包装成指针

                (*position.node).prev=(*__last.node).prev;//处理last_prev-position
                (*(*__last.node).prev).next=position.node;
                
                
                (*first_prev.node).next=__last.node;//连接first_prev-last
                (*__last.node).prev=first_prev.node;
            }
        }


    public:
        bool operator==(list<T> &rhs){
            return rhs.node==(*this).node;
        }
        //四个应用函数 splice,merge,reverse,sort
        //splice:链表的拼接,不要求链表有序
        void splice (iterator position,list& x){
            //将链表x连接到position处
            if(x==*this) return;//x不能是本链表
            if(!x.empty()){
                transfer(position,x.begin(),x.end());
            }
        }

        void splice(iterator position,list&,iterator i){
            //在原链表中,将i所指元素结合于position所指位置之前
            iterator j=i;
            ++j;
            if(i==position||j==position) return;//i==position不用接合;如果i已经在position前面,也不用接合
            transfer(position,i,j);
        }

        void splice(iterator position,list<T,Alloc>&,iterator first,iterator last){
            //在原链表中,将[fist,last)所指元素结合于position所指位置之前
            //没有对position位置进行判断,可能position在first和last之间
            if(first!=last){
                transfer(position,first,last);
            }
        }

        //merge-->有序链表的拼接,要求链表有序
        void merge(list<T,Alloc>& x){
            //两个有序链表的融合
            iterator this_begin=begin(),this_end=end();//隐含前提是this链表已经进行了排序
            iterator x_begin=x.begin(),x_end=x.end();
            while(this_begin!=this_end&&x_begin!=x_end){
                if(*this_begin<*x_begin){
                    ++this_begin;
                }
                else{
                    iterator node_to_merge=x_begin;
                    ++x_begin;
                    transfer(this_begin,node_to_merge,x_begin);
                }
            }
            if(x_begin!=x_end)//另一条链表上还有元素
                transfer(this_end,x_begin,x_end);
        }
        
        void reverse(){//有bug
            //链表反转 2,4,6,10->10,6,4,2
            if(node->next==node||node->next->next==node) return;//size=0或1,用size()==0||size()==1比较慢
            //我的方案:每次把最后一个数已到记号数前面
            iterator first=begin();//不断把最后一个元素插入到cur前面,直到cur->next==end
            int i=size()-1;
            while(i--){
                transfer(first,end().node->prev,end());
            }
            //++first;
            //while(first!=end()){
            //    iterator old=first;
            //    ++first;
            //    transfer(begin(),old,first);
            //}
        }

        void sort(){
            if(node->next==node||node->next->next==node) return;//size=0或1,用size()==0||size()==1比较慢
            //很适合用归并排序
            //5个元素以内用BUBLE_Sort
            list<list<T>,Alloc> carry;
            const int block_size=5;//子链表元素<=5的时候用插入排序
            //1.分组
            while(!empty()){
                list<T,Alloc> tmp_list;
                if(size()>=block_size)
                    tmp_list.splice(tmp_list.begin(),tmp_list,begin(),begin()+5);
                else
                    tmp_list.splice(tmp_list.begin(),tmp_list,begin(),end());
                carry.push_back(tmp_list);
            }
            //2.对每个子链表进行插入排序
            int numofsublist=carry.size();
            for(int ix=0;ix<numofsublist;ix++){
                typename list<list<T>,node_allocator>::iterator CarryIter=carry.begin()+ix;
                //int time=(*CarryIter).size();//待排元素数量
                for(iterator first=++(*CarryIter).begin();first!=(*CarryIter).end();++first){
                    //first指向待排元素
                    iterator p=(*CarryIter).begin();
                    while(p!=first&&*first>=*p) ++p;
                    //节点交换:麻烦,要写一个swap函数.  值交换:会使得其他迭代器的值改变
                    if(p.node!=first.node)
                        (*CarryIter).swap(p,first);
                }
                cout<<"\nlist "<<ix<<" result:";
                for(iterator pl=(*CarryIter).begin();pl!=(*CarryIter).end();++pl)
                    cout<<*pl<<',';
            }
            //3.现在每个子链表都是排序好的,调用merge函数注意合并到(*this)
            while(!carry.empty()){
                typename list<list<T>>::iterator mergelist=carry.begin();
                (*this).merge(*mergelist);
                carry.pop_front();
                //手动pop_front
                //(*tmp.node).prev->next=(*tmp.node).next;
                //(*tmp.node).next->prev=(*tmp.node).prev;
                //destroy(tmp.node);
                //node_allocator::deallocate(*tmp.node);
                
            }   

        }
    void print(){
        iterator p=begin();
        int ix=1;
        while(p!=end()){
            cout<<"The "<< ix <<" node's adress="<<&(*p.node)<<endl;
            cout<<"The "<< ix <<" node's next address="<<p.node->next<<endl;
            cout<<"The "<< ix <<" node's previous address="<<p.node->prev<<endl;
            cout<<"The "<< ix <<" node's value="<<*p<<endl;
            cout<<"\n";
            ++ix;
            ++p;
        }
    }

    protected:
        void swap(iterator node1,iterator node2){
            //内部函数,不对用户开放,开发人员要注意这个函数只能用在链表内部的节点交换
            //1.记录node2后面的一个节点node2_next,将来会把node1插入到这个节点之前
            iterator node_next=node2.node;
            ++node_next;
            //2.将node2搬移到node1前
            (*this).transfer(node1,node2,node_next);
            //3.将node1插入到node2_nextqian
            node_next=node1.node;
            ++node_next;
            (*this).transfer(node_next,node1,node_next);
        }
};
}//namespace mySTL;
#endif//__MYLIST_H