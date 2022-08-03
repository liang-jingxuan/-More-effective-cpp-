#ifndef __MY_RBT_H
#define __MY_RBT_H

#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//标注为"问题"的内容需要我调试的时候进行观察学习
using namespace mySTL;
namespace mySTL{
    /*全局函数声明*/

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

    bool operator==(const self& x)const{
        return x.node==node;//如果两个迭代器指向的节点都一样
    }

    bool operator!=(const self& x)const{
        return !((*this)==x);
    }
    //important3:因为该迭代器属于bidirectional_iterator_tag,即双向移动迭代器
    //          因此只能前移和后移,只需要实现++ -- * ->。
    //          若是randomaccess_iterator_tag则需要实现 += + -= - ++ -- * ->
    //          所以:迭代器要重载哪些操作符取决于如何进行遍历
};

//5.红黑树
    inline void __rbt_left_rotate(__rbt_node_base*,__rbt_node_base*&);
    inline void __rbt_right_rotate(__rbt_node_base*,__rbt_node_base*&);

template<class Key,class Value,class KeyOfValue,class compare,class Alloc=pool_alloc>
class RBT{
    /*类型名重命名*/
    protected:
        typedef void*                               void_pointer;
        typedef __rbt_node_base*                    base_ptr;//没有数据成员的部分
        typedef __rbt_node<Value>                   rbt_node;
        typedef Mysimple_alloc<rbt_node,Alloc>      node_allocator;//分配一个节点的空间配置器
        typedef __rbt_color_type                    color_type;

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
        link_type& left_most()const {return (link_type&)header->left;}
        link_type& right_most()const{return (link_type&)header->right;}
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
        static color_type& color(base_ptr x){return (color_type&)(link_type(x)->color);}

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
        iterator __insert(base_ptr y,const value_type& v);
        link_type   __copy(link_type x, link_type y);
        void __erase(link_type x);
        void init(){//初始化一棵红黑树
            header = alloc_node();
            color(header) =__rbtree_node_red;

            root()=0;
            left_most()=header;
            right_most()=header;
            //父节点为nullptr
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
        void clear(){}
        iterator find(const Key&k);//find的时候是key
        bool erase(iterator&);//删除iterator所指向的节点,既然是iterator,那必然是树中的某一节点
    public:
        pair<iterator,bool> insert_unique(const value_type& x);//插入的时候是value_type
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
        Key keyofval=KeyOfValue()(val);
        Key keyofx=key(x);
        x = key_compare(keyofval,keyofx)?left(x):right(x);//相当于val<x?true:false;
        //key_compare(arg1,arg2)函数中，若arg1<arg2则返回true，arg1>=arg2返回false。
        //要插入的值val小于目前的值x,则val应该插入在x的左子树,x往左走;否则往右走
        //如果在x_equal节点处遇到了相同的值，那么应该插入到该x_equal节点的后继或前驱位置
        //important：在遇到重复值的时候，STL采取了插入到后继位置的策略
    }
    return __insert(y,val);//x必然是nullptr否则无法退出上述while循环,那么传入x节点是无意义的
}

//插入一个值，返回值中，iterator指向插入的节点;bool指示是否插入成功,如果有相同的值则为false,
template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
pair<typename RBT<Key,Value,KeyOfValue,compare,Alloc>::iterator,bool>
RBT<Key,Value,KeyOfValue,compare,Alloc>::insert_unique(const value_type& val){
    //important:要注意是否插入了一个重复的值，如果有重复的值，那么插入点的父节点的
    link_type y=header;
    link_type x=root();//若是空树,则此时root()==header->parent,而header的parent是nullpte
    bool com_res=true;
    while(x!=nullptr){
        y = x;
        com_res=key_compare(KeyOfValue()(val),key(x));//val>x则x向右走,val<=x则x向左走
        x = com_res?left(x):right(x);
    }
    iterator pre_y=iterator(y);
//   y前驱  y前驱                      |                         y前驱       y前驱
//     |    |                          |                          |           |
//     y    y         val<y            |  val>=y                  y           y
//   x    x  n  y前驱<val<y            |  val>=y>=y的前驱            x       n   x
// n表示任意节点,y表示插入点的父节点,x是插入的位置,从左到右分别是情况1,2,3,4
    
    if(com_res==true){//com_res==true说明key(val)<key(y)，（可能有重复值）
        if(iterator(y)==begin())//如果恰巧y==left_most，那么val必然是新的最小值。这是因为若y有右子树，且val==最小值，那么
                        //      y可以继续往y的后继走；若y没有右子树，且val==最小值，那么val==key（y）必然使com_res==false.
            return pair<iterator,bool>(__insert(y,val),true);
        else
            --pre_y;//找到y的前驱,确定val是否以有重复值在树中。
                    //如果val小于最小值，那么pre_y指向left_most,并且--pre_y仍是它本身
                    //如果val等于最小值,在后续的判断重复中就会得到false而返回插入失败
    }
    //如果val和pre_y的key值相同,则有重复值--->返回false。如果不重复,则必然是返回true
    //important:这一段代码并不是<STL源码剖析>中所说的插入右侧,而是判断是否存在和val相同的值,无则插入,插入点可左可右
    //如果val!=pre_y必然有pre_y<val:因为pre_y是y的前驱,因此pre_y<y,而由于y处在pre_y的右子树,val是y的孩子,因此pre_y<val
    return key_compare(KeyOfValue()(key(pre_y.node)),KeyOfValue()(val))?
            pair<iterator,bool>(__insert(y,val),true)://未发现重复
            pair<iterator,bool>(pre_y,false);//发现重复
    
    //总结:关键在于如何判断是否有重复值,如果有重复值,那么插入点的父节点y的前驱必然==val。
    //      但是如果插入的新值val小于最小值，那么此时y的前驱就是其本身（参考decrement（）函数）
}
template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
typename RBT<Key,Value,KeyOfValue,compare,Alloc>::iterator
RBT<Key,Value,KeyOfValue,compare,Alloc>::find(const Key& val){//若有重复值，则优先返回接近根节点的一个
    link_type y=header;
    link_type x=root();//探路节点
    while(x!=nullptr){
        if(!key_compare(key(x),val))//x>=val则往左走,注意key_compare函数在arg1==arg2时返回false,因此在这种情况下
                                    //可能存在x==val,所以只有这种情况下y=x
            y=x,x=left(x);
        else//x<val则往右走,注意这里没有令y=x
            x=right(x);
    }
    iterator ans=iterator(y);
    return (ans==end()||key_compare(val,key(ans.node)))?end():ans;
    //important:这段代码妙极了!
    //原理:在key_compare(arg1,arg2)函数中,当arg1<arg2的时候返回true,在arg1>=arg2的时候返回false。
    //具体流程：
    //1.当所寻的val值不存于在于树中:y最终到达某一个节点，该节点必然满足y>val(因为只有arg1>=arg2时,y才移动,又由于
    //  val值不在树中,因此必然是y>val),因此在最终的key_compare(val,y)=true,所以返回了end(),符合预期
    //2.当所寻的val值在存在于树中:当y经过 y==val值的节点时停下(因为y>=val时,y会移动并停留),此时y的左子树都小于val
    //  y的右子树都>=val,因此x会先向左走一次,然后一直向右走,因为x所遍历的数必然都<val，而由于x一直向右走
    //  因此y一直停留在原地。最终由于val==y所以key_compare(val,y)返回false,false||false==false,返回ans,符合预期。
}

template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
typename RBT<Key,Value,KeyOfValue,compare,Alloc>::iterator
RBT<Key,Value,KeyOfValue,compare,Alloc>::__insert(base_ptr __y,const Value& val){
    //思路：
    //1.判断__y是否header，如果是header,说明是空树,配置并构造一个值为val的节点,让该节点成为root
    //2.__y不是header,根据 __y和val的大小进行插入
    link_type y = link_type(__y);
    link_type z;//指向插入点
    if(y==header||key_compare(KeyOfValue()(val),key(y))){
        //判断条件1:如果是y==header说明是空树,插入到y左右孩子都可以
        //判断条件2:如果val<y,则应该插入到y的左孩子
        //STL中的x!=nullptr我认为是毫无意义的
        //1.插入到y的左孩子位置最合适
        z = alloc_and_construct_node(val);
        left(y)=z;
        if(y==header){//发现插入点的父节点是header,说明该树是空树,则val应该成为根节点
            root()=z;
            right_most()=z;
            //为什么没有left_most()=z?--->因为已经通过上面的left(y)=z进行了赋值
        }
        else if (y==left_most()){//y!=header&&y==left_most()
            left_most()=z;
        }
    }
    else{
        //2.插入到y的右孩子位置最合适
        z = alloc_and_construct_node(val);     
        right(y)=z;       
        if (y==right_most()){
            right_most()=z;
        }
    }
    parent(z)=y;
    left(z)=nullptr;
    right(z)=nullptr;
    __rbt_insert_rebalance(z,header->parent);//插入的重平衡主要解决的是双红
    ++node_count;
    return iterator(z);
}

template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
bool RBT<Key,Value,KeyOfValue,compare,Alloc>::erase(iterator &iter){
    if(link_type(iter.node)==end()||iter.node==nullptr) return false;
    //iter已经指向了要删除的节点,不用自己寻找该节点在哪个位置
    //思路:分三种情况：1.iter是叶，2.iter是非叶
    iterator it_prev=iter,it_next=iter;
    --it_prev;++it_next;
    //1.iter是叶，且是iter是红色的
    if(iter.node->left==nullptr&&iter.node->left==nullptr){//iter是叶
        iterator iter_par=iter.node->parent;
        if(link_type(iter.node)==right_most()){
            header->right=iter.node->parent;
            iter.node->parent->right=nullptr;
        }else if(link_type(iter.node)==left_most()){
            header->left=iter.node->parent;
            iter.node->parent->left=nullptr;
        }else{//不是最值，只是一个普通的叶节点
            if(iter.node->parent->right==iter.node){//该叶是右叶
                iter.node->parent->right=nullptr;
            }else{//该叶是左叶
                iter.node->parent->left=nullptr;
            }
        }
        iter.node->parent=nullptr;
        if(iter.node->color==__rbtree_node_black)
            __erase_rebalence(iter_par.node);
        destroy_and_dealloc_node(iter.node);
    }



}

template<class Key,class Value,class KeyOfValue,class compare,class Alloc>
void RBT<Key,Value,KeyOfValue,compare,Alloc>::__erase(link_type iter){

}

//x是新插入的节点
inline void __rbt_insert_rebalance(__rbt_node_base* x,__rbt_node_base*& root){
    //important:"__rbt_node_base*& root"这里必须要&,因为根节点是唯一的,
    //          没有了&就不会修改唯一的根的信息,而这个函数会修改关于根的信息。
    x->color=__rbtree_node_red;//新节点为红
    while( x!=root && x->parent->color == __rbtree_node_red){
        //1.左叔叔
        if(x->parent == x->parent->parent->right){
            __rbt_node_base* uncle = x->parent->parent->left;
            if(uncle != nullptr && uncle->color == __rbtree_node_red){//左叔叔为红,染色即可
                //          |              |
                //          b              b
                //        r   r          r   r
                //           x                 x
                uncle->color = __rbtree_node_black;
                x->parent->color = __rbtree_node_black;
                x->parent->parent->color = __rbtree_node_red;
                x = x->parent->parent;
            }
            else{//没有叔叔,2次旋转+染色
                //          |              | 
                //          b              b
                //            r              r
                //           x                 x
                if(x->parent->left==x){//左边的情况
                    x=x->parent;
                    __rbt_right_rotate(x,root);
                }
                __rbt_left_rotate(x->parent->parent,root);
                x->parent->color = __rbtree_node_black;
                x->parent->left->color = __rbtree_node_red;
            }
        }
        //2.右叔叔
        else{//右叔叔是红节点,只需要染色
             __rbt_node_base* uncle = x->parent->parent->right;
             if(uncle != nullptr && uncle->color == __rbtree_node_red){
                //          |              |
                //          b              b
                //       r     r         r    r
                //         x           x        
                uncle->color = __rbtree_node_black;
                x->parent->color = __rbtree_node_black;
                x->parent->parent->color = __rbtree_node_red;
                x = x->parent->parent;
             }
             else{//右叔叔不存在(不存在即黑),旋转+染色
                //          |              |
                //          b              b
                //       r               r    
                //         x           x      
                if(x->parent->right==x){
                    x=x->parent;//旋转点
                    __rbt_left_rotate(x,root);
                }
                __rbt_right_rotate(x->parent->parent,root);
                x->parent->color = __rbtree_node_black;
                x->parent->right->color = __rbtree_node_red;
             }
        }
    }//endwhile
    root->color=__rbtree_node_black;//根节点必须是黑色的
}


inline void __rbt_right_rotate(__rbt_node_base* x,__rbt_node_base*& root){
    //x为旋转点,即左孩子当父节点
        __rbt_node_base* new_parent = x->left;
        if(new_parent->right!=nullptr)
            new_parent->right->parent=x;
        x->left=new_parent->right;
        
        new_parent->parent=x->parent;
        if(x == root){
            __rbt_node_base* header=x->parent;
            header->parent=new_parent;
            root=new_parent;
        }
        else if(x->parent->right==x)
            x->parent->right=new_parent;
        else
            x->parent->left=new_parent;
        
        x->parent=new_parent;
        new_parent->right=x;
}

inline void __rbt_left_rotate(__rbt_node_base* x,__rbt_node_base*& root){
    //x为旋转点,即右孩子当父节点
    //STL似乎没有考虑x->parent是header的问题
    //1.嫁接子树:即将new_parent的左子树嫁接到x的右子树
        __rbt_node_base* new_parent=x->right;
        if(new_parent->left!=nullptr)
            new_parent->left->parent=x;
        x->right=new_parent->left;

    //2.接上原来的根
        new_parent->parent=x->parent;
        if(x==root){
            //important:<STL源码剖析>中的STL没有考虑x->parent是header的问题
            __rbt_node_base* header=x->parent;
            header->parent=new_parent;
            root=new_parent;
        }
        else if(x->parent->right==x)
            x->parent->right = new_parent;
        else
            x->parent->left = new_parent;
        
    //3.交换x和new_parent的辈分
        new_parent->left=x;
        x->parent=new_parent;
    //代码段3必须放在最后,否则就失去了原来的根的信息
}

}//namespace mySTL
#endif