#ifndef __MYSET_HPP__
#define __MYSET_HPP__
//set即数学概念上的集合,集合内的元素是唯一的。set中 实值=键值
//important:之所以使用const iterator是为了防止   "mySet<int>::iterator p=iset.begin();*p=6;"
//    这样的代码改变了节点的值,但没有相对应地改变树的结构,范围了二叉搜索树的性质
//
#include "MY_RBT.h"
using std::pair;
namespace mySTL{
template<class Key, class Compare = less<Key>,class Alloc=pool_alloc>
class mySet{
    public:
        typedef Key key_type;
        typedef Key value_type;

        typedef Compare key_compare;
        typedef Compare value_compare;

    private:
        template<class T>
        struct identity:public unary_function<T,T>{
            const T& operator()(const T& x) const{
                return x;//即实值=键值
            }
        };

        typedef RBT<key_type,value_type,identity<value_type>
                ,key_compare,Alloc> rep_type;
        
        rep_type t;//底层原理是红黑树
    
    public:
        typedef typename rep_type::const_pointer pointer;
                            //const int*类型,表明该指针指向的对象不能被改变
                            //即 p原本指向的对象的值是7,则任何*p=...都是违法的
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;

        //构造函数
        mySet():t(Compare()){}//这是什么???RBT没有这个构造函数啊
        explicit mySet(const Compare& comp):t(comp){}

        template<class InputIterator>
        mySet(InputIterator first,InputIterator last)
            :t(Compare()){t.insert_unique(first,last);}

        template<class InputIterator>
        mySet(InputIterator first,InputIterator last,const Compare& comp)
            :t(comp){t.insert_unique(first,last);}

        //___拷贝构造函数
        mySet(const mySet<Key,Compare,Alloc>& x):t(x.t){}

        mySet<Key,Compare,Alloc>& operator=(const mySet<Key,Compare,Alloc>& x){
            t=x.t;
            return *this;
        }

        key_compare key_comp()const{return t.key_comp();}
        value_compare value_comp()const{return t.key_comp();}
        iterator begin()const{return t.begin();}
        iterator end()const{return t.end();}
        bool empty()const{return t.empty();}
        size_type size()const{return t.size();}
        size_type max_size()const{return t.max_size();}

        typedef typename std::pair<iterator,bool> pair_iterator_bool;

        pair_iterator_bool insert(const value_type& x){
            pair<typename rep_type::iterator,bool> p=t.insert_unique(x);
            return pair<iterator,bool>(p.first,p.second);
        }

        iterator insert(iterator position,const value_type& x){
            typedef typename rep_type::iterator rep_iterator;
            return t.insert_unique((rep_iterator&)position,x);
        }

        template<class InputIterator>
        void insert(InputIterator first,InputIterator last){
            t.insert_unique(first,last);
        }

        void erase(iterator position){
            typedef typename rep_type::iterator rep_iterator;
            t.erase((rep_iterator&)position);
        }

        size_type erase(const key_type& x){//删除所有x
            return t.erase(x);
        }

        void erase(iterator first,iterator last){
            typedef typename rep_type::iterator rep_iterator;
            t.erase((rep_iterator&)first,(rep_iterator)last);
        }

        void clear(){
            t.clear();
        }
        void printset(){
            typedef typename rep_type::iterator rep_iterator;
            for(rep_iterator p=begin();p!=end();++p){
                cout<<*p<<',';
            }
        }
        iterator find(const key_type& x) const{return t.find(x);}
        size_type count(const key_type& x)const {return t.count(x);}


};
}
#endif