#ifndef __MY_RBT_H
#define __MY_RBT_H

#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//标注为"问题"的内容需要我调试的时候进行观察学习
using namespace mySTL;
namespace mySTL{
typedef bool __rbtree_color_type;
const __rbtree_color_type __rbtree_node_red   = false;
const __rbtree_color_type __rbtree_node_black = true;

//基础节点,即三个指针和节点颜色
struct __rbtree_node_base{//一个没有数据段的节点
    typedef __rbtree_color_type color_type;
    typedef __rbtree_node_base* base_ptr;

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

//完整节点:基础节点+数据
template<class Value>
struct __rbt_node :public __rbt_node_base{//一个节点的数据段
    typedef __rbt_node<Value>* link_type;
    Value value_field;
    //由于继承了__rbt_node_base
    //因此该类还包含 color parent left right四个数据成员
};

//基础迭代器
struct rbt_base_iterator{
    //important:iterator可能指向树种任意一个非nullptr节点,当iterator为null时,说明指向了header,实现了左闭右开
    typedef __rbtree_node_base::base_ptr    base_ptr;
                                        //__rbtree_node_base*类型
    typedef bidirectional_iterator_tag      iterator_category;
    typedef ptrdiff_t                       difference_type;

    base_ptr node;

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
        if(node->color = __rbtree_node_red && node->parent->parent == node){
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

//完整迭代器
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
    //__rbt_node_base* node; 
    
    //__rbt_iterator类的数据成员,所以用"."进行成员选择
    //node是指向节点的指针
    __rbt_iterator(){}
    __rbt_iterator(link_type x){node=x;}
    __rbt_iterator(const iterator& iter){node=iter.node;}
    
    reference operator*(){return link_type(node)->value_field;}//问题:link_type是否会跳到__rbt_node类进行构造
                            //Q:为什么把node转化link_type可以引出value_field?
                            //A:link_type(x)调用了link_type构造函数,即__rbt_node(node),得到了一个__rbt_node对象
                            //  __rbt_node对象包含value_field
};




}//namespace mySTL
#endif