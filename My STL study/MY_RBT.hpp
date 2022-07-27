#ifndef __MY_RBT_H
#define __MY_RBT_H

#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"

typedef bool __rbtree_color_type;
const __rbtree_color_type __rbtree_node_red   = false;
const __rbtree_color_type __rbtree_node_black = true;

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

template<class Value>
struct __rbt_node :public __rbtree_node_base{//一个节点的数据段
    typedef __rbt_node<Value>* link_type;
    Value value_field;
};

struct rbt_base_iterator{
    typedef __rbtree_node_base::base_ptr    base_ptr;
                                        //__rbtree_node_base*类型
    typedef bidirectional_iterator_tag      iterator_category;
    typedef ptrdiff_t                       difference_type;

    base_ptr node;

    void increment(){//用来找到后继
        if( node->right != nullptr){//有右孩子,则后继在右子树
            node=node->right;
            while(node->left!=nullptr) node=node->left;
        }
        else{//没有右孩子,则该节点必然在某个节点的左子树
            while(node->parent != nullptr && node->parent->right == node) 
                node = node->parent;
            if(node->parent != nullptr) node = node->parent;
        }
    }
};

#endif