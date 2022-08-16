#ifndef __MYHASH_HPP
#define __MYHASH_HPP
#include "Myallocator.h"
#include "Mytraint.h"
#include "Myvector.h"
namespace mySTL{

template<typename value>
struct __hashtable_node{//单链表
    __hashtable_node* next;
    value val;
};

template<class Value,class Key, class HashFcn,class ExtractKey,class EqualKey,class Alloc>
class hashtable;


template<class Value,class Key, class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator{
    typedef hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>
                                        hashtable;
    typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>
                                        iterator;
    //typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc> //不理解这个
    //                                    const_iterator;
    typedef __hashtable_node<Value>     node;
    typedef forward_iterator_tag        iterator_category;//单向移动,只向前
    typedef Value                       value_type;
    typedef ptrdiff_t                   difference_type;
    typedef size_t                      size_type;
    typedef Value&                      reference;
    typedef Value*                      pointer;

    node* cur;//迭代器指向哈希表中某个元素,而这个元素存储在链表中的某个节点
    hashtable* ht;//用来指示该迭代器停留在哈希表中的哪个"桶子"

    __hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab){}
    __hashtable_iterator(){}

    reference operator*()const{return cur->val;}
    pointer operator->()const{return &(operator*());}
    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& it)const{return cur == it.cur;}
    bool operator!=(const iterator& it)const{return cur != it.cur;}
};

template<class V,class K, class HF,class ExK,class EqK,class A>
__hashtable_iterator<V,K,HF,ExK,EqK,A>&
__hashtable_iterator<V,K,HF,ExK,EqK,A>::operator++(){
    const node* old = cur;
    cur = cur->next;
    if(cur->next==nullptr){
        size_type bucket = ht->bkt_num(old->val);//通过求当前迭代器所指的对象反向求出该对象在哪个
                                        //"桶子"
        while(cur==nullptr&&++bucket < ht->bucktes.size())//如果是nullptr一直前进,直到找到下一个有元素的"桶子"
            cur = ht->buckets[bucket];
    }
    return *this;
}

template<class V,class K, class HF,class ExK,class EqK,class A>
inline __hashtable_iterator<V,K,HF,ExK,EqK,A>
__hashtable_iterator<V,K,HF,ExK,EqK,A>::operator++(int){
    iterator tmp=*this;
    ++*this;
    return tmp;
}

static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes]={
    //第i个桶子装数值为i的对象,那负数怎么办?
    53,         97,             193,            389,        769,
    1543,       3079,           6151,           12289,      24593,
    49157,      98317,          196613,         393241,     786433,
    1572869,    3145739,        6291469,        12582917,   25165843,
    50331653,   100663319,      201326611,      402653189,  805306457,
    1610612741, 3221225473ul,   4294967291ul
};

template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc=pool_alloc>
class hashtable{
    public:
        typedef HashFcn                 hasher;
        typedef EqualKey                key_equal;
        typedef size_t                  size_type;
        typedef Value                   value_type;
        typedef Value*                  pointer;
        typedef Value&                  reference;
        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>
                                        iterator;
        typedef Key                     key_type;
    private://数据成员 5个:分别是1.哈希函数,2.键值相等判断函数,3.值求键,4.哈希表,5.表内元素个数
        hasher      hash;
        key_equal   equals;
        ExtractKey  get_key;

        typedef __hashtable_node<Value> node;
        typedef Mysimple_alloc<node,Alloc>  node_alloc;

        Myvector<node*,Alloc>   buckets;//桶子
        size_type               num_elems;//记录有多少个元素

    public:
        size_type   bucket_count()const{return buckets.size();}
        size_type max_bucket_cound() const{
            return __stl_prime_list[__stl_num_primes-1];
        }
    private:

        inline unsigned long __stl_next_prime(unsigned long n)const{
            //找到最接近n的下一个质数,比如90,返回97
            const unsigned long* first = __stl_prime_list;
            const unsigned long* last = __stl_prime_list + __stl_num_primes;
            const unsigned long* pos = lower_bound(first,last,n);//泛型算法,第六章
            return pos == last ? *(last-1) : *pos;
        }



        //1--------->4
        //2--->3---->4

        //1.接受                  实值       "桶子"个数
        size_type bkt_num(const value_type& val,size_type n)const{
            return bkt_num_key(get_key(val),n);//调用4
        }

        //2.接受                实值
        size_type bkt_num(const value_type& val)const{
            return bkt_num_key(get_key(val));//调用3
        }

        //3.接受                    键值
        size_type bkt_num_key(const key_type& key) const{
            return bkt_num_key(key,buckets.size());//调用4
        }

        //4.接受    键值和buckets个数 (最终归宿)
        size_type bkt_num_key(const key_type& key,size_type n) const{
            return hash(key) % n;
        }


    protected://内存管理,
        node* new_node(const value_type& obj){//分配一个链表节点的空间,构造后挂到对应的桶子
            node* n = node_alloc::allocate();
            n->next = nullptr;
            construct(&n->val,obj);//构造对象
            return n;
        }

        void delete_node(node* p){
            destroy(&p->val);
            node_alloc::deallocate(p);
        }

        unsigned long next_size(size_type n)const{return __stl_next_prime(n);}//问题:有点莫名其妙???
        
        void initialize_buckets(size_type n){//初始化n个桶子
            const size_type n_buckets = next_size(n);
            buckets.reserve(n_buckets);//vector开辟存储n个对象的空间
            buckets.insert(buckets.end(), n_buckets, (node*) 0);
            num_elems = 0;
        }

        pair<iterator,bool> insert_unique_noresize(const value_type&);//这个返回一个pair是为了指示是否有重复的键值
        iterator insert_equal_noresize(const value_type&);//因为不考虑是否要重复值,所以不担心插入是否失败

    public://初始化
        hashtable(size_type n,const HashFcn& hf,const EqualKey& eql)
        :hash(hf),equals(eql),get_key(ExtractKey()),num_elems(0){
            initialize_buckets(n);
        }

        pair<iterator,bool> insert_unique(const value_type& obj){
            resize(num_elems + 1);//判断是否需要重建表格,以提高搜索效率。
            return insert_unique_noresize(obj);
        }

        void resize(size_type);

        iterator insert_equal(const value_type& obj){
            resize(num_elems + 1);
            return insert_equal_noresize(obj);
        }
        void clear();//清除桶子里所有节点,但保留表的大小

        void copy_from(const hashtable&);
        size_type size()const{return num_elems;}
};


template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc>
void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::resize(size_type num_elem_hint){
    const size_type old_n = buckets.size();//久的桶子个数
    if(num_elem_hint > old_n){//元素个数>桶子个数
        const size_type n = next_size(num_elem_hint);//找到下一个质数
        if(n > old_n){//下一个质数 > 久的桶子个数
            Myvector<node*,Alloc> tmp(n,(node*)0);
            for(size_type bucket=0; bucket<old_n;++bucket){
                //将久的桶子移到新的桶子,bucket是目前在处理的桶子
                node* first= buckets[bucket];
                while(first){
                    //1.bkt_num可以求出元素放到哪个"桶子"
                    size_type new_bucket = bkt_num(first->val,n);
                    //2.把久桶子中的链表放到新桶子里
                    buckets[bucket] = first->next;
                    first->next = tmp[new_bucket];
                    //3.指向下一个桶子
                    first = buckets[bucket];
                }
            }
            buckets.swap(tmp);
        }
    }
}


template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc>
pair<typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator, bool>
hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
insert_unique_noresize (const value_type& obj){
    const size_type n = bkt_num(obj);//bkt_num为对象找到合适的桶子
    node* first = buckets[n];

    for(node* cur=first;cur!=nullptr;cur=cur->next){
        if(equals(get_key(cur->val),get_key(obj)))
            return pair<iterator,bool>(iterator(cur,this),false);
    }

    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elems;
    return pair<iterator,bool>(iterator(tmp,this),true);
}


template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc>
typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator
hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
insert_equal_noresize(const value_type& obj){
    const size_type n = bkt_num(obj);
    node* first = buckets[n];

    for(node* cur=first;cur!=nullptr;cur=cur->next){
        if(equals(get_key(cur->val),get_key(obj))){
            node* tmp = new_node(obj);
            tmp->next = cur->next;
            cur->next=tmp;
            ++num_elems;
            return iterator(tmp,this);
        }
    }

    //插入到链表结尾
    node* cur = new_node(obj);
    cur->next = cur->next;
    cur->next = cur;
    ++num_elems;
    return iterator(cur,this);
}


template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc>
void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
clear(){
//针对每个桶子,观察是否桶子内有节点
    for(size_type ix=0;ix<buckets.size();++ix){
        node* cur = buckets[ix];
        while(cur!=nullptr){
            node* next= cur->next;
            delete_node(cur);//析构链表,并释放内存
            cur = next;
        }
        buckets[ix] = nullptr;
    }
    num_elems = 0;
}


template<class Value,class Key, class HashFcn,
            class ExtractKey,class EqualKey,class Alloc>
void  hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
copy_from(const hashtable& ht){
    //将另一个哈希表的内容复制到本哈希表
    //保留ht,所以需要一一复制,不能把ht中的链表移到本链表
    buckets.clear();//清空vector中所有元素
    buckets.reserve(ht.buckets.size());
    buckets.insert(buckets.end(),ht.buckets.size(),(node*) 0);//因为clear会析构链表并释放空间
    
    for(size_type ix=0;ix<buckets.size();++ix){
        if(const node* cur=ht.buckets[ix]){
            node* copy = new_node(cur->val);
            buckets[ix] =copy;

            for(node* next= cur->next;next!=nullptr;cur=next,next=cur->next){
                copy->next = new_node(cur->val);
                copy = copy->next;
            }
        }
    }
    num_elems = ht.num_elems;
}






}//namespace mySTL
#endif