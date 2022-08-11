#ifndef __MY_RBT_H
#define __MY_RBT_H

#include <corecrt.h>
#include "Mytraint.h"
#include "Myallocator.h"
#include "Myconstructor.h"
//标注为"问题"的内容需要我调试的时候进行观察学习
//注意:自己写的erase_Rebalance使用了节点替换,而不是值替换,因此交换过程非常繁琐,要考虑的情况非常多!
using namespace mySTL;
namespace mySTL
{
    /*全局函数声明*/

    typedef bool __rbt_color_type;
    const __rbt_color_type __rbtree_node_red = false;
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
    // 1.基础节点,即三个指针和节点颜色
    struct __rbt_node_base
    { //一个没有数据段的节点
        typedef __rbt_color_type color_type;
        typedef __rbt_node_base *base_ptr;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x)
        {
            while (x->left != nullptr)
                x = x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x)
        {
            while (x->right != nullptr)
                x = x->right;
            return x;
        }
    };

    // 2.完整节点:基础节点+数据
    template <class Value>
    struct __rbt_node : public __rbt_node_base
    {
        typedef __rbt_node<Value> *link_type;
        Value value_field;
        //由于继承了__rbt_node_base
        //因此该类还包含 color parent left right四个数据成员

        // important:关于构造函数,该类使用默认的构造函数,即拷贝构造函数使用的是浅拷贝
        //默认的构造函数类似于:
        //__rbt_node():color(任意),parent(0),left(0),right(0),value_field(0){}//无参数构造函数
        //__rbt_node(__rbt_node& x){color=x.color;right=x.right;...}
    };

    // 3.基础迭代器
    struct rbt_base_iterator
    {
        // important:iterator可能指向树种任意一个非nullptr节点,当iterator为null时,说明指向了header,实现了左闭右开
        /*类型重命名*/
        typedef __rbt_node_base::base_ptr base_ptr;
        //__rbtree_node_base*类型
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        /*数据成员*/
        base_ptr node;
        /*函数成员*/
        void increment()
        { //后继
            if (node->right != nullptr)
            { //有右孩子,则后继在右子树
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
            }
            else
            { //没有右孩子,则该节点必然在某个节点的左子树
                if (node->right == node)
                    return; //说明此时node==header，且是空树

                base_ptr y = node->parent;
                while (node == y->right)
                {
                    node = y;
                    y = y->parent;
                }
                // 1.找的是根节点的后继,且根节点是最大值,此时y停在root,node停在header---------->返回node
                // 2.找的不是根节点的后继,且该节点是最大值,此时y停在header,node停在root,---------->返回y
                // 3.找的不是根节点的后继,该节点不是最大值,此时y停在正确答案,node停在y的左孩子,---------->返回y
                if (y != node->right)
                { //针对情况2,3
                    node = y;
                }
                //情况1由于node已经停在header,因此不用处理
            }
        }

        void decrement()
        { //前驱
            // 1.考虑node是header,说明指向了end(),则前驱即最大的数,header->right即最大的数
            if (node->color == __rbtree_node_red && node->parent->parent == node)
            {
                // header节点的父节点指向header,左孩子指向最小的数,右孩子指向最大的数,header颜色为红色
                node = node->right;
                return;
            }
            // 2.不是header
            if (node->left != nullptr)
            {
                //有左孩子
                node = node->left;
                while (node->right != nullptr)
                    node = node->right;
            }
            else
            {
                //无左孩子
                base_ptr y = node->parent;
                while (y->left == node)
                {
                    node = y;
                    y = y->parent;
                }
                // 1.找的是根节点的前驱,且根节点是最小值,此时y停在root,node停在header--->返回y(左闭右开原因,最小值的前驱应该是它本身)
                // 2.找的不是根节点的前驱,且该节点是最小值,此时y停在header,node停在root,---------->返回header->left
                // 3.找的不是根节点的前驱,该节点不是最小值,此时y停在正确答案,node停在y的右孩子,---------->返回y
                //原创代码
                if (node->left == y || y->right == node)
                    node = y; //情况1,3
                else
                    node = y->left; //情况2
            }
        }
    };

    // 4.完整迭代器
    template <class value, class ref, class ptr>
    struct __rbt_iterator : public rbt_base_iterator
    {
        typedef value value_type;
        typedef ref reference;
        typedef ptr pointer;
        typedef __rbt_iterator<value, value &, value *> iterator;
        typedef __rbt_iterator<value, const value &, const value *> const_iterator;
        typedef __rbt_iterator<value, ref, ptr> self;
        typedef __rbt_node<value> *link_type; //指向节点的指针

        // important:由于继承了__rbt_node_base,因此__rbt_iterator包含一个node,node指向真实的节点
        //即隐含地声明了
        //__rbt_node_base* node; //important2:
        //该指针只有__rbt_node_base的数据成员,即color,parent,right和left
        //如果要value_field则需要转化为__rbt_node类型

        //__rbt_iterator类的数据成员,所以用"."进行成员选择
        // node是指向节点的指针
        __rbt_iterator() {}
        __rbt_iterator(link_type x) { node = x; }
        __rbt_iterator(const iterator &iter) { node = iter.node; }

        reference operator*() { return link_type(node)->value_field; }
        //这里需要构造一个link_type的原因如important2所示

        pointer operator->() { return &(operator*()); } // p-> 应返回指针本身

        self &operator++()
        {
            increment();
            return *this;
        } //直接调用increment是因为this是一个
          //__rbt_iterator类，而该类继承了rbt_base_iterator，
          // rbt_base_iterator类中定义了一increment和decrement
        self operator++(int)
        { // i++
            self tmp = *this;
            increment();
            return tmp;
        }
        /*
            self operator+=(int x){
                self ans=*this;
                if(x>=0)
                    while(x--)  ans.increment();
                else
                    while(x++) ans.decrement();
                return ans;
            }

            self operator-=(int x){
                self ans=*this;
                ans+=-x;
                return ans;
            }

            self& operator+(int x){
                *this+=x;
                return *this;
            }

            self& operator-(int x){
                *this-=x;
                return *this;
            }
        */
        self &operator--()
        {
            decrement();
            return *this;
        } // decrement()相当于this->decrement()

        self operator--(int)
        {
            self tmp = *this;
            decrement();
            return tmp;
        }

        bool operator==(const self &x) const
        {
            return x.node == node; //如果两个迭代器指向的节点都一样
        }

        bool operator!=(const self &x) const
        {
            return !((*this) == x);
        }
        // important3:因为该迭代器属于bidirectional_iterator_tag,即双向移动迭代器
        //           因此只能前移和后移,只需要实现++ -- * ->。
        //           若是randomaccess_iterator_tag则需要实现 += + -= - ++ -- * ->
        //           所以:迭代器要重载哪些操作符取决于如何进行遍历
    };

    // 5.红黑树
    inline void __rbt_left_rotate(__rbt_node_base *, __rbt_node_base *&);
    inline void __rbt_right_rotate(__rbt_node_base *, __rbt_node_base *&);

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc = pool_alloc>
    class RBT
    {
        /*类型名重命名*/
    protected:
        typedef void*                               void_pointer;
        typedef __rbt_node_base*                    base_ptr; //没有数据成员的部分
        typedef __rbt_node<Value>                   rbt_node;
        typedef Mysimple_alloc<rbt_node, Alloc>     node_allocator; //分配一个节点的空间配置器
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
        link_type alloc_node() { return node_allocator::allocate(); }
        void dealloc_node(link_type p) { node_allocator::deallocate(p); }

        link_type alloc_and_construct_node(const value_type &val)
        { //配置并构造一个节点
            link_type tmp = node_allocator::allocate();
            construct(&tmp->value_field, val);
            return tmp;
        }

        link_type clone_node(link_type node)
        { //深拷贝一个节点
            link_type tmp = alloc_and_construct_node(node->value_field);
            tmp->color = node->color;
            tmp->parent = nullptr;
            tmp->right = nullptr;
            tmp->left = nullptr;
            return tmp;
        }

        void destroy_and_dealloc_node(link_type x)
        { //析构并释放一个节点
            destroy(&x->value_field);
            dealloc_node(x);
        }
        /*数据成员*/
    protected:
        size_type node_count;
        link_type header; // parent指向根节点,right指向最大值,left指向最小值
        compare key_compare;
        /*小工具,比如判断颜色等*/
    protected:
        //问题2:这些函数有什么用----->用来为header赋值
        link_type &root() const { return (link_type &)header->parent; } //返回指向根节点的指针
        link_type &left_most() const { return (link_type &)header->left; }
        link_type &right_most() const { return (link_type &)header->right; }
        //问题3:这些函数由什么用
        static link_type &left(link_type x) { return (link_type &)x->left; }
        static link_type &right(link_type x) { return (link_type &)x->right; }
        static link_type &parent(link_type x) { return (link_type &)x->parent; }
        static reference value(link_type x) { return x->value_field; }
        static const Key &key(link_type x) { return KeyOfValue()(value(x)); }
        static color_type &color(link_type x) { return (color_type &)x->color; }

        static link_type &left(base_ptr x) { return (link_type &)x->left; }
        static link_type &right(base_ptr x) { return (link_type &)x->right; }
        static link_type &parent(base_ptr x) { return (link_type &)x->parent; }
        static reference value(base_ptr x) { return (link_type(x))->value_field; }
        static const Key &key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
        static color_type &color(base_ptr x) { return (color_type &)(link_type(x)->color); }

        link_type &brother(link_type x)
        {
            if (x == header || x == header->parent)
                return nullptr; //根节点和header不存在兄弟节点
            if (left(x->parent) == x)
            { //该节点是左孩子
                return right(x->parent);
            }
            return left(x->parent);
        }

        static link_type minimum(link_type x)
        {
            return (link_type)__rbt_node_base::minimum(x); //返回某节点最左节点
        }

        static link_type maximum(link_type x)
        {
            return (link_type)__rbt_node_base::maximum(x); //返回某节点最右节点
        }

    public:
        typedef __rbt_iterator<value_type, reference, pointer> iterator;
        typedef const iterator                                 const_iterator;
    private:
        iterator __insert(base_ptr y, const value_type &v);
        link_type __copy(link_type x, link_type y);
        void __erase(link_type x);
        void init()
        { //初始化一棵红黑树
            header = alloc_node();
            color(header) = __rbtree_node_red;

            root() = 0;
            left_most() = header;
            right_most() = header;
            //父节点为nullptr
        }

    public:
        RBT(const compare &cmp_method = compare()) : node_count(0), key_compare(cmp_method)
        {
            init();
        }

        ~RBT()
        {
            clear(); //后序遍历析构并释放每个节点
            dealloc_node(header);
        }

    protected:
        //用于erase的分函数
        void __rbt_erase_root();
        void __rbt_erase_leaf(link_type);
        void __rbt_erase_aux(link_type);

    public:
        compare key_comp() const{ return key_compare; }
        iterator begin() const { return left_most(); }
        iterator end() const{ return header; }
        size_type size()const { return node_count; }
        bool empty() { return node_count == 0; }
        size_type max_size() const { return size_type(-1); }
        void clear() {
            if(size()==0) return;
            __clear((link_type)header->parent);
        }
        iterator find(const Key &k); // find的时候是key
        bool erase(iterator);        //删除iterator所指向的节点,既然是iterator,那必然是树中的某一节点
        void print(){;
            if(empty()) cout<<"RBT::print():The tree is empty now!"<<endl;
            size_type i=1;
            for(iterator p=begin();p!=end();++p){
                std::cout<<"\nThe "<<i<<"-th node:"
                        <<"\nvalue:" <<*p
                        <<"\nthis node address"<<p.node
                        <<"\nleft child addres:"<<p.node->left
                        <<"\nright child address"<<p.node->right
                        <<"\nparent address"<<p.node->parent
                        <<"\ncolor"<<p.node->color
                        <<endl;
                ++i;
            }
        }
        
        size_type erase(const Key_type &x){
            size_type elem_del=0;
            iterator p = find(x);
            while(p!=end()){//只要仍然存在该元素就删除
                erase(p);
                p=find(x);
                ++elem_del;
            }
            return elem_del;
        }

        size_type count(const Key_type& x) const{
            //中序遍历,每找到一个就+1
            size_type res=0;
            __count((link_type)header->parent, x, res);
            return res;
        }

        void erase(iterator first,iterator last){
            while(first!=last){
                erase(first);
                ++first;
            }
        }


    public:
        pair<iterator, bool> insert_unique(const value_type &x); //插入的时候是value_type
        iterator insert_equal(const value_type &x);
        pair<iterator, bool> insert_unique(pointer &first,pointer &last){
            pair<iterator,bool> res;
            while(first!=last){
                res = insert_unique(*first);
                if(res.second==false)
                    break;
                ++first;
            }
            return res;
        }
        
        iterator insert_unique(pointer &position,const value_type &x){
            pair<iterator,bool> p=insert_unique(x);
            position=p.first;
            return position;
        }
    protected:
        void __clear(link_type cur){
            if(cur==nullptr) return;
            __clear((link_type)cur->left);
            __clear((link_type)cur->right);
            destroy_and_dealloc_node((link_type)cur);
        }

        void __count(link_type cur,const Key_type& x,size_type& res)const{
            if(cur==nullptr) return;//中序遍历
            __count((link_type)cur->left,x,res);
            if(key_compare(key(cur),x));//说明cur<x
            else if(key_compare(x,key(cur)));//如果是true说明x<cur
            else ++res;//只剩下==
            __count((link_type)cur->right,x,res);
        }


    };

    //可以插入重复的值,但是需要保持平衡数关于值的大小关系
    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    typename RBT<Key, Value, KeyOfValue, compare, Alloc>::iterator
    RBT<Key, Value, KeyOfValue, compare, Alloc>::insert_equal(const value_type &val)
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr)
        {
            y = x;
            Key keyofval = KeyOfValue()(val);
            Key keyofx = key(x);
            x = key_compare(keyofval, keyofx) ? left(x) : right(x); //相当于val<x?true:false;
            // key_compare(arg1,arg2)函数中，若arg1<arg2则返回true，arg1>=arg2返回false。
            //要插入的值val小于目前的值x,则val应该插入在x的左子树,x往左走;否则往右走
            //如果在x_equal节点处遇到了相同的值，那么应该插入到该x_equal节点的后继或前驱位置
            // important：在遇到重复值的时候，STL采取了插入到后继位置的策略
        }
        return __insert(y, val); // x必然是nullptr否则无法退出上述while循环,那么传入x节点是无意义的
    }

    //插入一个值，返回值中，iterator指向插入的节点;bool指示是否插入成功,如果有相同的值则为false,
    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    pair<typename RBT<Key, Value, KeyOfValue, compare, Alloc>::iterator, bool>
    RBT<Key, Value, KeyOfValue, compare, Alloc>::insert_unique(const value_type &val)
    {
        // important:要注意是否插入了一个重复的值，如果有重复的值，那么插入点的父节点的
        link_type y = header;
        link_type x = root(); //若是空树,则此时root()==header->parent,而header的parent是nullpte
        bool com_res = true;
        while (x != nullptr)
        {
            y = x;
            com_res = key_compare(KeyOfValue()(val), key(x)); // val>x则x向右走,val<=x则x向左走
            x = com_res ? left(x) : right(x);
        }
        iterator pre_y = iterator(y);
        //   y前驱  y前驱                      |                         y前驱       y前驱
        //     |    |                          |                          |           |
        //     y    y         val<y            |  val>=y                  y           y
        //   x    x  n  y前驱<val<y            |  val>=y>=y的前驱            x       n   x
        // n表示任意节点,y表示插入点的父节点,x是插入的位置,从左到右分别是情况1,2,3,4

        if (com_res == true)
        {                               // com_res==true说明key(val)<key(y)，（可能有重复值）
            if (iterator(y) == begin()) //如果恰巧y==left_most，那么val必然是新的最小值。这是因为若y有右子树，且val==最小值，那么
                                        //      y可以继续往y的后继走；若y没有右子树，且val==最小值，那么val==key（y）必然使com_res==false.
                return pair<iterator, bool>(__insert(y, val), true);
            else
                --pre_y; //找到y的前驱,确定val是否以有重复值在树中。
                         //如果val小于最小值，那么pre_y指向left_most,并且--pre_y仍是它本身
                         //如果val等于最小值,在后续的判断重复中就会得到false而返回插入失败
        }
        //如果val和pre_y的key值相同,则有重复值--->返回false。如果不重复,则必然是返回true
        // important:这一段代码并不是<STL源码剖析>中所说的插入右侧,而是判断是否存在和val相同的值,无则插入,插入点可左可右
        //如果val!=pre_y必然有pre_y<val:因为pre_y是y的前驱,因此pre_y<y,而由于y处在pre_y的右子树,val是y的孩子,因此pre_y<val
        return key_compare(KeyOfValue()(key(pre_y.node)), KeyOfValue()(val)) ? pair<iterator, bool>(__insert(y, val), true) : //未发现重复
                   pair<iterator, bool>(pre_y, false);                                                                        //发现重复

        //总结:关键在于如何判断是否有重复值,如果有重复值,那么插入点的父节点y的前驱必然==val。
        //      但是如果插入的新值val小于最小值，那么此时y的前驱就是其本身（参考decrement（）函数）
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    typename RBT<Key, Value, KeyOfValue, compare, Alloc>::iterator
    RBT<Key, Value, KeyOfValue, compare, Alloc>::find(const Key &val)
    { //若有重复值，则优先返回接近根节点的一个
        link_type y = header;
        link_type x = root(); //探路节点
        while (x != nullptr)
        {
            if (!key_compare(key(x), val)) // x>=val则往左走,注意key_compare函数在arg1==arg2时返回false,因此在这种情况下
                                           //可能存在x==val,所以只有这种情况下y=x
                y = x, x = left(x);
            else // x<val则往右走,注意这里没有令y=x
                x = right(x);
        }
        iterator ans = iterator(y);
        return (ans == end() || key_compare(val, key(ans.node))) ? end() : ans;
        // important:这段代码妙极了!
        //原理:在key_compare(arg1,arg2)函数中,当arg1<arg2的时候返回true,在arg1>=arg2的时候返回false。
        //具体流程：
        // 1.当所寻的val值不存于在于树中:y最终到达某一个节点，该节点必然满足y>val(因为只有arg1>=arg2时,y才移动,又由于
        //   val值不在树中,因此必然是y>val),因此在最终的key_compare(val,y)=true,所以返回了end(),符合预期
        // 2.当所寻的val值在存在于树中:当y经过 y==val值的节点时停下(因为y>=val时,y会移动并停留),此时y的左子树都小于val
        //   y的右子树都>=val,因此x会先向左走一次,然后一直向右走,因为x所遍历的数必然都<val，而由于x一直向右走
        //   因此y一直停留在原地。最终由于val==y所以key_compare(val,y)返回false,false||false==false,返回ans,符合预期。
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    typename RBT<Key, Value, KeyOfValue, compare, Alloc>::iterator
    RBT<Key, Value, KeyOfValue, compare, Alloc>::__insert(base_ptr __y, const Value &val)
    {
        //思路：
        // 1.判断__y是否header，如果是header,说明是空树,配置并构造一个值为val的节点,让该节点成为root
        // 2.__y不是header,根据 __y和val的大小进行插入
        link_type y = link_type(__y);
        link_type z; //指向插入点
        if (y == header || key_compare(KeyOfValue()(val), key(y)))
        {
            //判断条件1:如果是y==header说明是空树,插入到y左右孩子都可以
            //判断条件2:如果val<y,则应该插入到y的左孩子
            // STL中的x!=nullptr我认为是毫无意义的
            // 1.插入到y的左孩子位置最合适
            z = alloc_and_construct_node(val);
            left(y) = z;
            if (y == header)
            { //发现插入点的父节点是header,说明该树是空树,则val应该成为根节点
                root() = z;
                right_most() = z;
                //为什么没有left_most()=z?--->因为已经通过上面的left(y)=z进行了赋值
            }
            else if (y == left_most())
            { // y!=header&&y==left_most()
                left_most() = z;
            }
        }
        else
        {
            // 2.插入到y的右孩子位置最合适
            z = alloc_and_construct_node(val);
            right(y) = z;
            if (y == right_most())
            {
                right_most() = z;
            }
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __rbt_insert_rebalance(z, header->parent); //插入的重平衡主要解决的是双红
        ++node_count;
        return iterator(z);
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    bool RBT<Key, Value, KeyOfValue, compare, Alloc>::erase(iterator iter)
    {
        if (iter == end() || iter.node == nullptr || empty())
            return false;

        if(iter.node==left_most()){
            iterator it=iter;
            ++it;
            header->left=it.node;
        }
        else if(iter.node==right_most()){
            iterator it=iter;
            --it;
            header->right=it.node;
        }

        if (iter.node == root())
            __rbt_erase_root();
        else if (iter.node->left == nullptr && iter.node->right == nullptr)
            __rbt_erase_leaf((link_type)iter.node); // pass
        else
            __rbt_erase_aux((link_type)iter.node);
        --node_count;
        return true;
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    void RBT<Key, Value, KeyOfValue, compare, Alloc>::__rbt_erase_root()
    {
        //   root      root    root          root
        //            l            r       ... ...
        //  情况1     情况2      情况3      情况4
        link_type old_root = root();
        if (old_root->left == nullptr && old_root->right == nullptr)
        { //情况1:根无孩子
            header->parent = nullptr;
            header->right = header;
            header->left = header;
            destroy_and_dealloc_node(old_root);
        }
        else if (old_root->left == nullptr || old_root->right == nullptr)
        { //情况2,3:根有单孩子,则这个单孩子必然是红色的
            link_type new_root;
            if (old_root->left != nullptr)
            {
                new_root = (link_type)old_root->left;
            }
            else if (old_root->right != nullptr)
            {
                new_root = (link_type)old_root->right;
            }
            header->parent = new_root;
            header->left = new_root;
            header->right = new_root;
            new_root->color = __rbtree_node_black; //根节点必须是黑色的
            // root()=(link_type)new_root.node;//这一句完全不需要,因为header->parent就是root
            destroy_and_dealloc_node(old_root);
            //不需要rebalance
        }
        else //情况4:根有双孩子
        {
            // 必然有后继
            iterator tmp_root = root();++tmp_root;
            link_type new_root = (link_type)tmp_root.node; // new_next即root的后继
            if (new_root->right != nullptr)// 4.1、后继有右孩子
            {
                // 1.root后继有孩子,则后继必然是黑色的；后继孩子必然是红色的,不可能是黑。
                //处理方法：用后继的红色孩子替代后继，用后继替代root.
                //后继的孩子必然是红色的原因是：后继节点y的左黑高度是1或2,
                //如果左黑高度为1，说明后继是红色的，加上nullptr后就是0+1=1，则右孩子只能是红色
                //如果左黑高度为2，说明后继是黑色的，加上nullptr后就是1+1=2，则右孩子只能是红色
                //且右孩子是红色节点的时候必然是叶节点。
                link_type replace_node = (link_type)new_root->right; //后继的后继
                //第一步:后继及其子节点互换
                color_swap(replace_node,new_root);
                if(isLeftChild(new_root))
                    new_root->parent->left=replace_node;
                else
                    new_root->parent->right=replace_node;
                replace_node->parent=new_root->parent;
                new_root->parent=replace_node;
                replace_node->right=new_root;
                new_root->right=nullptr;
                //第二步,new_root和old_root互换
                color_swap(new_root,old_root);
                header->parent=new_root;
                new_root->parent=header;
                new_root->left=old_root->left;
                old_root->left->parent=new_root;
                new_root->right=old_root->right;
                old_root->right->parent=new_root;
                old_root->parent=replace_node;
                replace_node->right=nullptr;
                destroy_and_dealloc_node(old_root);
            }
            else // 4.2.后继无孩子
            {
                if (new_root == right_most())
                {
                    header->right = new_root;
                }
                //后继无孩子,只需要考虑后继的父节点
                bool need_rebalance = new_root->color==__rbtree_node_black;
                color_swap(old_root, new_root);
                link_type new_root_par=(link_type)new_root->parent;

                new_root->right = old_root->right;
                old_root->right->parent = new_root;
                new_root->left=old_root->left;
                old_root->left->parent=new_root;

                if(new_root_par==old_root){//两者是父子关系
                    new_root->right=old_root;
                    old_root->parent=new_root;
                }
                else{//两者不是父子关系
                    new_root_par->left=old_root;
                    old_root->parent=new_root_par;
                }
                    
                header->parent=new_root;
                new_root->parent=header;
                old_root->left=nullptr;
                old_root->right=nullptr;
                if (!need_rebalance)
                {
                    destroy_and_dealloc_node(old_root);
                }
                else
                {
                    __rbt_erase_rebalance (old_root, header->parent);
                    destroy_and_dealloc_node(old_root);
                }
            }
        }
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    void RBT<Key, Value, KeyOfValue, compare, Alloc>::__rbt_erase_leaf(link_type erase_node)
    {
        if (erase_node->color == __rbtree_node_red)
        { // pass
            if (erase_node->parent->right == erase_node)
                erase_node->parent->right = nullptr;
            else
                erase_node->parent->left = nullptr;
            destroy_and_dealloc_node(erase_node);
        }
        else
        { //叶节点是黑色的
            __rbt_erase_rebalance (erase_node, header->parent);
            destroy_and_dealloc_node(erase_node);
        }
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    void RBT<Key, Value, KeyOfValue, compare, Alloc>::__rbt_erase_aux(link_type erase_node)
    {
        // erase_node非根非叶。
        /*erase_node无后继，那么erase_node可能有左孩子,若有,则该孩子必为红;
            *    erase_node(Black)        erase_node(Red)
            *      /         \              /          \
            *   Red         nill          nill         nill
         * (replace_node)         
         *  /   \
         *nill  nill
         *   情况1                   情况2[不可能,因为erase_node不是叶节点,
         *                                 叶节点情况有另一个函数解决]
         */
        if (erase_node->right == nullptr) // 1.要删的节点没有后继
        {//情况1,左孩子替代erase_node,不需要Rebalance,删除即可
            //对应于4阶B数中的叶节点,而且是  "红黑"排列的节点
            link_type replace_node = (link_type)erase_node->left;
            replace_node->parent = erase_node->parent;
            if(erase_node->parent->left==erase_node)
                erase_node->parent->left = replace_node;
            else
                erase_node->parent->right = replace_node;
            replace_node->color = __rbtree_node_black;
            replace_node->left = nullptr;
            replace_node->right = nullptr;
            destroy_and_dealloc_node(erase_node);
        }
        //↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑无后继↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
        //↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓有后继↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        else // 2.要删的节点有后继
        {   //后继必黑,除非父子
            /*
             *找到erase_node的后继
             *        |                 |                          |
             *    erase_node        erase_node                 erase_node
             *        |                 |                          \
             *        /                /                          /
             *      /                /                          /
             *    R(erase_next)     B(erase_next)              B(erase_next)
             *  /   \             /   \                      /   \
             * nill nill        nill   R(replace_node )   nill  nill
             *                        / \
             *                     nill nill
             *情况1                 情况2                   情况3:rebalance
             *共性:都用erase_next替代erase_node--->交换erase_node和erasenext
             */
            //                            /---后继有右子节点
            //后继是右子节点+后继不是右子节点---后继武右子节点
            //                             
            iterator tmp = iterator(erase_node);
            ++tmp;
            link_type erase_next = (link_type)tmp.node;
            if (erase_next->right != nullptr)//后继有孩子
            { //后继有孩子
                //情况2
                link_type replace_node=(link_type)erase_next->right;
                link_type erase_next_par=(link_type)erase_next->parent;
                //第一步:erase_next和replace_node互换位置
                bool need_rebalance = erase_next->right->color==__rbtree_node_black;
                color_swap(replace_node,erase_next);
                if(isLeftChild(erase_next))
                    erase_next_par->left=replace_node;
                else
                    erase_next_par->right=replace_node;
                replace_node->parent=erase_next_par;
                replace_node->right=erase_next;
                erase_next->parent=replace_node;
                erase_next->right=nullptr;//不需要设置左节点,因为erase_next作为后继必然没有左孩子

                //第二步:erase_next和erase_node互换位置
                link_type erase_node_par=(link_type)erase_node->parent;
                color_swap(erase_node,erase_next);
                if(isLeftChild(erase_node))
                    erase_node_par->left=erase_next;
                else
                    erase_node_par->right=erase_next;
                erase_next->parent=erase_node_par;

                erase_next->right=erase_node->right;
                if(erase_node->right!=nullptr)
                    erase_node->right->parent=erase_next;
                erase_next->left=erase_node->left;
                if(erase_node->left!=nullptr)
                    erase_node->left->parent=erase_next;
                replace_node->right=erase_node;
                erase_node->parent=replace_node;
                erase_node->left=nullptr;
                erase_node->right=nullptr;
                if (need_rebalance)
                { //后继是红:不需要rebalance,后继是黑:需要rebalance
                    erase_node->parent->left = nullptr;
                    destroy_and_dealloc_node(erase_node);
                }
                else
                {
                    __rbt_erase_rebalance (erase_node, header->parent);
                    destroy_and_dealloc_node(erase_node);
                }
            }
            else if ((link_type)erase_next->right == nullptr) //后继无孩子,那么该后继是个叶节点
            {   //有bug:没有考虑erase_next和erase_node是父子关系的情况
                //情况1&3:用后继erase_next替代erase_node
                link_type erase_next_par = (link_type)erase_next->parent;//辅助进行交换节点
                link_type erase_node_par = (link_type)erase_node->parent;//辅助完成交换节点
                bool need_rebalance = erase_next->color==__rbtree_node_black;
                color_swap(erase_node,erase_next);

                erase_next->left=erase_node->left;
                if(erase_node->left!=nullptr)
                    erase_node->left->parent=erase_next;
                erase_next->right=erase_node->right;
                if(erase_node->right!=nullptr)
                    erase_node->right->parent=erase_next;

                if(isRightChild(erase_node))
                    erase_node_par->right=erase_next;
                else
                    erase_node_par->left=erase_next;
                erase_next->parent=erase_node_par;

                if(erase_next_par==erase_node){
                    erase_next->right=erase_node;
                    erase_node->parent=erase_next;
                }
                else{
                    erase_node->parent=erase_next_par;
                    erase_next_par->left=erase_node;
                }

                erase_node->left=nullptr;
                erase_node->right=nullptr;
                if (!need_rebalance)
                { //后继是红:不需要rebalance,后继是黑:需要rebalance
                    if(erase_next->right==erase_node)
                        erase_next->right=nullptr;
                    else
                        erase_next_par->left = nullptr;
                    destroy_and_dealloc_node(erase_node);
                }
                else
                {
                    __rbt_erase_rebalance (erase_node, header->parent);
                    destroy_and_dealloc_node(erase_node);
                }
            }
            
        }
    }

    template <class Key, class Value, class KeyOfValue, class compare, class Alloc>
    void RBT<Key, Value, KeyOfValue, compare, Alloc>::__erase(link_type iter)
    {
    }

    // x是新插入的节点
    inline void __rbt_insert_rebalance(__rbt_node_base *x, __rbt_node_base *&root)
    {
        // important:"__rbt_node_base*& root"这里必须要&,因为根节点是唯一的,
        //           没有了&就不会修改唯一的根的信息,而这个函数会修改关于根的信息。
        x->color = __rbtree_node_red; //新节点为红
        while (x != root && x->parent->color == __rbtree_node_red)
        {
            // 1.左叔叔
            if (x->parent == x->parent->parent->right)
            {
                __rbt_node_base *uncle = x->parent->parent->left;
                if (uncle != nullptr && uncle->color == __rbtree_node_red)
                { //左叔叔为红,染色即可
                    //          |              |
                    //          b              b
                    //        r   r          r   r
                    //           x                 x
                    uncle->color = __rbtree_node_black;
                    x->parent->color = __rbtree_node_black;
                    x->parent->parent->color = __rbtree_node_red;
                    x = x->parent->parent;
                }
                else
                { //没有叔叔,2次旋转+染色
                    //          |              |
                    //          b              b
                    //            r              r
                    //           x                 x
                    if (x->parent->left == x)
                    { //左边的情况
                        x = x->parent;
                        __rbt_right_rotate(x, root);
                    }
                    __rbt_left_rotate(x->parent->parent, root);
                    x->parent->color = __rbtree_node_black;
                    x->parent->left->color = __rbtree_node_red;
                }
            }
            // 2.右叔叔
            else
            { //右叔叔是红节点,只需要染色
                __rbt_node_base *uncle = x->parent->parent->right;
                if (uncle != nullptr && uncle->color == __rbtree_node_red)
                {
                    //          |              |
                    //          b              b
                    //       r     r         r    r
                    //         x           x
                    uncle->color = __rbtree_node_black;
                    x->parent->color = __rbtree_node_black;
                    x->parent->parent->color = __rbtree_node_red;
                    x = x->parent->parent;
                }
                else
                { //右叔叔不存在(不存在即黑),旋转+染色
                    //          |              |
                    //          b              b
                    //       r               r
                    //         x           x
                    if (x->parent->right == x)
                    {
                        x = x->parent; //旋转点
                        __rbt_left_rotate(x, root);
                    }
                    __rbt_right_rotate(x->parent->parent, root);
                    x->parent->color = __rbtree_node_black;
                    x->parent->right->color = __rbtree_node_red;
                }
            }
        }                                  // endwhile
        root->color = __rbtree_node_black; //根节点必须是黑色的
    }

    inline void __rbt_right_rotate(__rbt_node_base *x, __rbt_node_base *&root)
    {
        // x为旋转点,即左孩子当父节点
        __rbt_node_base *new_parent = x->left;
        if (new_parent->right != nullptr)
            new_parent->right->parent = x;
        x->left = new_parent->right;

        new_parent->parent = x->parent;
        if (x == root)
        {
            __rbt_node_base *header = x->parent;
            header->parent = new_parent;
            root = new_parent;
        }
        else if (x->parent->right == x)
            x->parent->right = new_parent;
        else
            x->parent->left = new_parent;

        x->parent = new_parent;
        new_parent->right = x;
    }

    inline void __rbt_left_rotate(__rbt_node_base *x, __rbt_node_base *&root)
    {
        // x为旋转点,即右孩子当父节点
        // STL似乎没有考虑x->parent是header的问题
        // 1.嫁接子树:即将new_parent的左子树嫁接到x的右子树
        __rbt_node_base *new_parent = x->right;
        if (new_parent->left != nullptr)
            new_parent->left->parent = x;
        x->right = new_parent->left;

        // 2.接上原来的根
        new_parent->parent = x->parent;
        if (x == root)
        {
            // important:<STL源码剖析>中的STL没有考虑x->parent是header的问题
            __rbt_node_base *header = x->parent;
            header->parent = new_parent;
            root = new_parent;
        }
        else if (x->parent->right == x)
            x->parent->right = new_parent;
        else
            x->parent->left = new_parent;

        // 3.交换x和new_parent的辈分
        new_parent->left = x;
        x->parent = new_parent;
        //代码段3必须放在最后,否则就失去了原来的根的信息
    }

    void inline color_swap(__rbt_node_base *node1, __rbt_node_base *node2)
    {
        __rbt_color_type tmp = node1->color;
        node1->color = node2->color;
        node2->color = tmp;
    }


    inline bool isRed(const __rbt_node_base *node)
    {
        if(node==nullptr) return false;//空节点是黑色的,所以不是红色的
        return node->color == __rbtree_node_red;
    }

    inline bool isBlack(const __rbt_node_base *node)
    {
        if(node==nullptr) return true;//空节点是黑色的
        return node->color == __rbtree_node_black;
    }

    inline bool isLeftChild(const __rbt_node_base *node)
    {
        if (node->parent->parent == node)
            return false; //是根或是header
        if (node->parent->left == node)
            return true;
        return false;
    }

    inline bool isRightChild(const __rbt_node_base *node)
    {
        if (node->parent->parent == node)
            return false; //是根或是header
        if (node->parent->right == node)
            return true;
        return false;
    }

    inline bool __rbtree_hasRedNephew(const __rbt_node_base *node)
    {
        //注意:这个函数只有在红黑树删除节点后的重平衡才调用,在红黑树重平衡过程中,node必然有兄弟节点!!!
        //可能的bug:这里隐含的假设了,如果存在侄子,则侄子必然是红色的
        if (node->parent->parent == node )
            return false; //1.根或header
        //↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        //                  排除了根或header,那么
        //2.兄弟节点是红色则不可能有两个红色孩子,因为会双红
        if(isLeftChild(node)&&isRed(node->parent->right))
            return false;
        else if(isRightChild(node)&&isRed(node->parent->left))
            return false;
        
        if(isLeftChild(node)&&node->parent->right==nullptr)
            return false;//没有兄弟节点就没有侄子
        else if(isRightChild(node)&&node->parent->left==nullptr)
            return false;

        if (isLeftChild(node))
        {
            if (node->parent->right->right == nullptr && node->parent->right->left == nullptr)
                return false; //没有侄子---->nill对应黑
            else if(node->parent->right->right != nullptr||node->parent->right->left != nullptr)
            {//只有一个侄子,但这个侄子是黑的
                if(node->parent->right->right != nullptr)//远侄子
                    return isRed(node->parent->right->right);
                else//近侄子
                    return isRed(node->parent->right->left);
            }
            else//两个侄子, 都是黑的
            {
                return isBlack(node->parent->right->right)&&isBlack(node->parent->right->right);
            }
        }
        else if (isRightChild(node))
        {
            if (node->parent->left->right == nullptr && node->parent->left->left == nullptr)
                return false; //没有侄子---->nill对应黑
            else if(node->parent->left->right != nullptr||node->parent->left->left != nullptr)
            {//只有一个侄子
                if(node->parent->left->left != nullptr)//远侄子
                    return isRed(node->parent->left->left);
                else//近侄子
                    return isRed(node->parent->left->right);
            }
            else//两个侄子, 都是黑的
            {
                return isBlack(node->parent->right->right)&&isBlack(node->parent->right->right);
            }
        }
        return true;
    }

    void __rbt_erase_rebalance (__rbt_node_base * erase_node, __rbt_node_base *&root)
    { 
        //      要避免因为变化导致删错了对象
        
        __rbt_node_base *copy_erase_node = erase_node;

        //负责在调整后将arg1剥离出二叉树中
        // 4种情况:1.父节点是4节点,2.父节点是3节点,3.父节点是2节点
        //                                                                ---红:父兄交换颜色后 continue
        //                                                               /
        // 1.检查erase_node父节点是否为黑色的------是(父为黑): 兄弟节点颜色?: ---黑且兄弟节点无孩子:兄弟染红,
        //  (对应3节点上,erase_node连在黑节点上)\                                D换对象为父节点后continue
        //                                     ---否(父为红):判断条件2
        //
        //                                                    /---有:以父节点为旋转点进行旋转后,兄父颜色互换,远侄染黑return(出口)
        // 2.检查erase_node的检查是否有侄子:---有:判断是否有远侄子---无:以近侄子为旋转点进行旋转,侄子及其父亲颜色互换continue
        //                                |
        //                                ---无:判断条件3
        // 3.对应4节点退化为3节点,3节点退化为2节点:兄父颜色交换后return(出口)
        while (copy_erase_node != root)
        {
            if(isLeftChild(copy_erase_node)){
                if(isBlack(copy_erase_node->parent) && 
                        isRed(copy_erase_node->parent->right)){
                    color_swap(copy_erase_node->parent,copy_erase_node->parent->right);
                    __rbt_left_rotate(copy_erase_node->parent,root);
                }
                else if(isBlack(copy_erase_node->parent) && 
                        isBlack(copy_erase_node->parent->right)&&
                        !__rbtree_hasRedNephew(copy_erase_node)){
                    copy_erase_node->parent->right->color=__rbtree_node_red;
                    copy_erase_node=copy_erase_node->parent;
                    continue;
                }
                
                if(__rbtree_hasRedNephew(copy_erase_node)){
                    if(copy_erase_node->parent->right->right!=nullptr&&
                        isRed(copy_erase_node->parent->right->right)){
                            //有红远侄子
                        color_swap(copy_erase_node->parent,copy_erase_node->parent->right);
                        copy_erase_node->parent->right->right->color=__rbtree_node_black;
                        __rbt_left_rotate(copy_erase_node->parent,root);
                        break;
                    }
                    else{
                        //只有红近侄子
                        color_swap(copy_erase_node->parent->right,copy_erase_node->parent->right->left);
                        __rbt_right_rotate(copy_erase_node->parent->right,root);
                    }
                }
                else{
                    //没有红色侄子,退化
                    color_swap(copy_erase_node->parent,copy_erase_node->parent->right);
                    break;
                }
            }
            else{//要删的节点是右孩子
                if(isBlack(copy_erase_node->parent) && 
                        isRed(copy_erase_node->parent->left)){
                    color_swap(copy_erase_node->parent,copy_erase_node->parent->left);
                    __rbt_right_rotate(copy_erase_node->parent,root);
                }
                else if(isBlack(copy_erase_node->parent) && 
                        isBlack(copy_erase_node->parent->left)&&
                        !__rbtree_hasRedNephew(copy_erase_node)){
                    copy_erase_node->parent->left->color=__rbtree_node_red;
                    copy_erase_node=copy_erase_node->parent;
                    continue;
                }

                if(__rbtree_hasRedNephew(copy_erase_node)){
                    if(copy_erase_node->parent->left->left!=nullptr&&
                        isRed(copy_erase_node->parent->left->left)){
                            //有红远侄子
                        color_swap(copy_erase_node->parent,copy_erase_node->parent->left);
                        copy_erase_node->parent->left->left->color=__rbtree_node_black;
                        __rbt_right_rotate(copy_erase_node->parent,root);
                        break;
                    }
                    else{
                        //只有红近侄子
                        color_swap(copy_erase_node->parent->left,copy_erase_node->parent->left->right);
                        __rbt_left_rotate(copy_erase_node->parent->left,root);
                    }
                }
                else{
                    //没有红色侄子,退化
                    color_swap(copy_erase_node->parent,copy_erase_node->parent->left);
                    break;
                }
            }
        }

        root->color = __rbtree_node_black;
        if(isLeftChild(erase_node))
            erase_node->parent->left=nullptr;
        else
            erase_node->parent->right=nullptr;
    }

} // namespace mySTL
#endif
