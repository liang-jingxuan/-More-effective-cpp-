#ifndef __MY_RBT_H
#define __MY_RBT_H

#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//标注为"问题"的内容需要我调试的时候进行观察学习
using namespace mySTL;
namespace mySTL{
typedef bool __rbt_color_type;
const __rbt_color_type __rbtree_node_red   = false;
const __rbt_color_type __rbtree_node_black = true;
/*各个类的关系
 ┏━━━━━━━━━━━━━━━━━━━━━━━━┓                ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
 ┃ 类名:__rbt_node_base   ┃                ┃  类名:__rbt_node        ┃
 ┠━━━━━━━━━━━━━━━━━━━━━━━━┨                ┠━━━━━━━━━━━━━━━━━━━━━━━━┨
 ┃数据成员：               ┃                ┃数据成员：               ┃  
 ┃bool color;             ┃   继承          ┃value_type value_field  ┃ 
 ┃__rbt_node_base* parent;┃--------------》 ┗━━━━━━━━━━━━━━━━━━━━━━━━┛
 ┃__rbt_node_base* left;  ┃                
 ┃__rbt_node_base* right; ┃                  
 ┗━━━━━━━━━━━━━━━━━━━━━━━━┛                 

 ┏━━━━━━━━━━━━━━━━━━━━━━━━┓                ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
 ┃类名:__rbt_base_iterator ┃               ┃    类名:__rbt_iterator  ┃
 ┠━━━━━━━━━━━━━━━━━━━━━━━━┨                ┠━━━━━━━━━━━━━━━━━━━━━━━━┨
 ┃数据成员：               ┃     继承       ┃数据成员：               ┃  
 ┃__rbt_node_base* node;  ┃--------------》┗━━━━━━━━━━━━━━━━━━━━━━━━┛
 ┗━━━━━━━━━━━━━━━━━━━━━━━━┛    
*/
//1.基础节点,即三个指针和节点颜色
struct __rbt_node_base{//一个没有数据段的节点
    typedef __rbt_color_type color_type;
    typedef __rbt_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x){
        while(x->left!=nullptr) x=x->left;
        return x;
    }

    static base_ptr maximum(base_ptr x){
        while(x->right!=nullptr) x=x->right;
        return x;
    }
};

//2.完整节点:基础节点+数据
template<class Value>
struct __rbt_node :public __rbt_node_base{
    typedef __rbt_node<Value>* link_type;
    Value value_field;
    //由于继承了__rbt_node_base
    //因此该类还包含 color parent left right四个数据成员

    //important:关于构造函数,该类使用默认的构造函数,即拷贝构造函数使用的是浅拷贝
    //默认的构造函数类似于:
    //__rbt_node():color(任意),parent(0),left(0),right(0),value_field(0){}//无参数构造函数
    //__rbt_node(__rbt_node& x){color=x.color;right=x.right;...}
};

//3.基础迭代器
struct rbt_base_iterator{
    //important:iterator可能指向树种任意一个非nullptr节点,当iterator为null时,说明指向了header,实现了左闭右开
    /*类型重命名*/
    typedef __rbt_node_base::base_ptr    base_ptr;
                                        //__rbtree_node_base*类型
    typedef bidirectional_iterator_tag      iterator_category;
    typedef ptrdiff_t                       difference_type;
    /*数据成员*/
    base_ptr node;
    /*函数成员*/
    void increment(){//后继
        if( node->right != nullptr){//有右孩子,则后继在右子树
            node=node->right;
            while(node->left!=nullptr) node=node->left;
        }
        else{//没有右孩子,则该节点必然在某个节点的左子树
            base_ptr y=node->parent;
            while(node==y->right){
                node = y;
                y=y->parent;
            }
            //1.找的是根节点的后继,且根节点是最大值,此时y停在root,node停在header---------->返回node
            //2.找的不是根节点的后继,且该节点是最大值,此时y停在header,node停在root,---------->返回y
            //3.找的不是根节点的后继,该节点不是最大值,此时y停在正确答案,node停在y的左孩子,---------->返回y
            if(y!=node->right){//针对情况2,3
                node = y;
            }
            //情况1由于node已经停在header,因此不用处理
        }
    }

    void decrement(){//前驱
    //1.考虑node是header,说明指向了end(),则前驱即最大的数,header->right即最大的数
        if(node->color == __rbtree_node_red && node->parent->parent == node){
            //header节点的父节点指向header,左孩子指向最小的数,右孩子指向最大的数,header颜色为红色
            node=node->right;
            return;
        }
    //2.不是header
        if(node ->left != nullptr){
            //有左孩子
            node=node->left;
            while(node->right!=nullptr) node=node->right;
        }
        else{
            //无左孩子
            base_ptr y=node->parent;
            while(y->left==node){
                node=y;
                y=y->parent;
            } 
//1.找的是根节点的前驱,且根节点是最小值,此时y停在root,node停在header--->返回y(左闭右开原因,最小值的前驱应该是它本身)
//2.找的不是根节点的前驱,且该节点是最小值,此时y停在header,node停在root,---------->返回header->left
//3.找的不是根节点的前驱,该节点不是最小值,此时y停在正确答案,node停在y的右孩子,---------->返回y
            //原创代码
            if(node->left==y||y->right==node) node=y;//情况1,3
            else node=y->left;//情况2
        }

    }
};

//4.完整迭代器
template<class value,class ref,class ptr>
struct __rbt_iterator:public rbt_base_iterator{
    typedef value                                               value_type;
    typedef ref                                                 reference;
    typedef ptr                                                 pointer;
    typedef __rbt_iterator<value,value&,value*>                 iterator;
    typedef __rbt_iterator<value,const value&, const value*>    const_iterator;
    typedef __rbt_iterator<value,ref,ptr>                       self;
    typedef __rbt_node<value>*                                  link_type;//指向节点的指针  

    //important:由于继承了__rbt_node_base,因此__rbt_iterator包含一个node,node指向真实的节点
    //即隐含地声明了
    //__rbt_node_base* node; //important2:
                            //该指针只有__rbt_node_base的数据成员,即color,parent,right和left
                            //如果要value_field则需要转化为__rbt_node类型
    
    //__rbt_iterator类的数据成员,所以用"."进行成员选择
    //node是指向节点的指针
    __rbt_iterator(){}
    __rbt_iterator(link_type x){node=x;}
    __rbt_iterator(const iterator& iter){node=iter.node;}
    
    reference operator*(){return link_type(node)->value_field;}
                            //这里需要构造一个link_type的原因如important2所示
    
    pointer operator->(){return &(operator*());}//p-> 应返回指针本身

    self& operator++(){increment();return *this;}//直接调用increment是因为this是一个
                                                //__rbt_iterator类，而该类继承了rbt_base_iterator，
                                                //rbt_base_iterator类中定义了一increment和decrement
    self operator++(int){//i++
        self tmp=*this;
        increment();
        return tmp;
    }

    self& operator--(){decrement();return *this;}//decrement()相当于this->decrement()

    self operator--(int){
        self tmp=*this;
        decrement();
        return tmp;
    }

    //important3:因为该迭代器属于bidirectional_iterator_tag,即双向移动迭代器
    //          因此只能前移和后移,只需要实现++ -- * ->。
    //          若是randomaccess_iterator_tag则需要实现 += + -= - ++ -- * ->
    //          所以:迭代器要重载哪些操作符取决于如何进行遍历
};

//5.红黑树
template<class Key,class Value,class KeyOfValue,class compare,class Alloc=pool_alloc>
class RBT{
    /*类型名重命名*/
    protected:
        typedef void*                               void_pointer;
        typedef __rbt_node_base*                    base_ptr;//没有数据成员的部分
        typedef __rbt_node<Value>                   rbt_node;
        typedef Mysimple_alloc<__rbt_node,Alloc>    node_allocator;//分配一个节点的空间配置器
        typedef __rbt_color_type                    color_type;
        typedef __rbt_node<Value>                   rbt_node;
    public:
        typedef Key                 Key_type;
        typedef Value               value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef rbt_node*           link_type;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;
    
    /*内存管理*/
    protected:
        link_type alloc_node(){return node_allocator::allocate();}
        void     dealloc_node(link_type p){node_allocator::deallocate(p);}

        link_type   alloc_and_construct_node(const value_type& val){//配置并构造一个节点
            link_type tmp=node_allocator::allocate();
            construct(&tmp->value_field,val);
            return tmp;
        }

        link_type   clone_node(link_type node){//深拷贝一个节点
            link_type tmp=alloc_and_construct_node(node->value_field);
            tmp->color  = node->color;
            tmp->parent = nullptr;
            tmp->right  = nullptr;
            tmp->left   = nullptr;
            return tmp;
        }

        void destroy_and_dealloc_node(link_type x){//析构并释放一个节点
            destroy(&x->value_field);
            dealloc_node(x);
        }
    /*数据成员*/
    protected:
        size_type node_count;
        link_type header;//parent指向根节点,right指向最大值,left指向最小值
        compare key_compare;
    /*小工具,比如判断颜色等*/
    protected:
        //问题2:这些函数有什么用----->用来为header赋值
        link_type& root()const {return (link_type&)header->parent;}//返回指向根节点的指针
        link_type& left_most()const {return header->left;}
        link_type& right_most()const{return header->right;}
        //问题3:这些函数由什么用
        static link_type& left(link_type x){return (link_type&)x->left;}
        static link_type& right(link_type x){return (link_type&)x->right;}
        static link_type& parent(link_type x){return (link_type&)x->parent;}
        static reference value(link_type x){return x->value_field;}
        static const Key& key(link_type x){return KeyOfValue()(value(x));}
        static color_type& color(link_type x){return (color_type&)x->color;}

        static link_type& left(base_ptr x)  {return (link_type&)x->left;}
        static link_type& right(base_ptr x) {return (link_type&)x->right;}
        static link_type& parent(base_ptr x){return (link_type&)x->parent;}
        static reference value(base_ptr x)  {return (link_type(x))->value_field;}
        static const Key& key(base_ptr x)   {return KeyOfValue()(value(link_type(x)));}
        static color_type& color(base_ptr x){return (color_type&)(linktype(x)->color);}

        link_type& brother(link_type x){
            if(x==header||x==header->parent) return nullptr;//根节点和header不存在兄弟节点
            if(left(x->parent)==x){//该节点是左孩子
                return right(x->parent);
            }
            return left(x->parent);    
        }

        static link_type minimum(link_type x){
            return (link_type)__rbt_node_base::minimum(x);//返回某节点最左节点
        }

        static link_type maximum(link_type x){
            return (link_type)__rbt_node_base::maximum(x);//返回某节点最右节点
        }

    public:
        typedef __rbt_iterator<value_type,reference,pointer>    iterator;
    
    private:
        iterator __insert(base_ptr x,base_ptr y,const value_type& v);
        link_type   __copy(link_type x, link_type y);
        void __erase(link_type x);
        void init(){//初始化一棵红黑树
            header = alloc_node();
            color(header) =__rbtree_node_red;

            root()=0;
            left_most()=header;
            right_most()=header;
        }

    public:
        RBT(const compare& cmp_method=compare()):node_count(0),key_compare(cmp_method){
            init();
        }

        ~RBT(){
            clear();//后序遍历析构并释放每个节点
            dealloc_node(header);
        }

    public:
        compare key_comp(){return key_compare;}
        iterator begin(){return left_most();}
        iterator end(){return header;}
        size_type size(){return node_count;}
        bool    empty(){return node_count==0;}
        size_type max_size()const{return size_type(-1);}
    
    public:
        pair<iterator,bool> insert_unique(const value_type& x);
        iterator insert_equal(const value_type& x);
        
};

//可以插入重复的值,但是需要保持平衡数关于值的大小关系
template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
typename RBT<Key,Value,KeyOfValue,compare,Alloc>::iterator
RBT<Key,Value,KeyOfValue,compare,Alloc>::insert_equal(const value_type& val){
    link_type y=header;
    link_type x=root();
    while(x!=nullptr){
        y = x;
        x = key_compare(KeyOfValue()(val),Key(x))?left(x):right(x);
        //key_compare(arg1,arg2)函数中，若arg1<arg2则返回true，否则返回false。
        //要插入的值val小于目前的值x,则val应该插入在x的左子树,x往左走;否则往右走
        //如果在x_equal节点处遇到了相同的值，那么应该插入到该x_equal节点的后继或前驱位置
        //STL采取了插入到前驱位置
    }
    
    return __insert(x,y,val);//x必然是nullptr否则无法退出上述while循环,那么传入x节点是无意义的
}



}//namespace mySTL
#endif