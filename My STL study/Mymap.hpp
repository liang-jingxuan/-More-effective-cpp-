#ifndef __MYMAP_HPP
#define __MYMAP_HPP
#include "Myallocator.h"
#include "MY_RBT.h"

namespace mySTL{

template<class Key, class T,class Compare = less<Key>,class Alloc = pool_alloc >
class Mymap{
    public:
        typedef Key 				key_type;
        typedef T 					data_type;
        typedef T 					mapped_type;
        typedef pair<const Key,T >  value_type;
        typedef Compare 			key_compare;
        
		class value_compare//用于提取pair中的键值并比较,因为RBT需要一个KeyOfValue的方法
		:public binary_function<value_type,value_type,bool>{
            friend class Mymap<Key,T,Compare,Alloc>;//value_compare可以调用Mymap任何
											//中任何类型,函数,比如下面的Compare类型
											//value_type类型
    	protected:
            Compare comp;
            value_compare(Compare c):comp(c){}
        public:
			bool operator()(const value_type& x,const value_type& y)const{
				return comp(x.first,y.first);
			}
        };


	private:
		template<typename _T>
		class select1st{//iportant
            friend class Mymap<Key,T,Compare,Alloc>;
			public:
				const key_type& operator()(const value_type& x){
					return x.first;
				}

				key_type operator()(const key_type& x) {
					return x;
				}
				key_type operator()(key_type& x){
					return x;
				}
		};

	//					<键,  值>            从值求键		     键的比较方法   空间分配器
		typedef RBT<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
		rep_type t;//底层实现

	public:
		typedef typename rep_type::pointer 			pointer;
		typedef typename rep_type::const_pointer 	const_pointer;
		typedef typename rep_type::reference 		reference;
		typedef typename rep_type::const_reference 	const_reference;
		typedef typename rep_type::iterator 		iterator;
		typedef typename rep_type::const_iterator 	const_iterator;
		typedef typename rep_type::size_type		size_type;
		typedef typename rep_type::difference_type	difference_type;

	public://构造函数
		Mymap():t(Compare()){}//Compare()传回了了一个临时对象
		explicit Mymap(const Compare& comp):t(comp){}

		template<class InputIterator>
		Mymap(InputIterator first,InputIterator last)
		:t(Compare()){
			t.insert_unique(first,last);//一般是指针
		}

		template<class InputIterator>
		Mymap(InputIterator first,InputIterator last,const Compare& comp)//指定比较方式
		:t(comp){t.insert_unique(first,last);}

		Mymap(const Mymap<Key,T,Compare,Alloc>& x):t(x.t){}
		//拷贝构造函数
		Mymap<Key,T,Compare,Alloc>& operator=(const Mymap<Key,T,Compare,Alloc>&x){
			t=x.t;
			return *this;
		}
		
		key_compare key_comp()const{
			return t.key_comp();//返回临时对象
		}
		
		value_compare value_comp()const{//返回值的比较方式
			return value_compare(t.key_comp());
		}
		
		iterator begin(){return t.begin();}
		const_iterator begin()const {return t.begin();}

		iterator end(){return t.end();}
		const_iterator end()const{return t.end();}

		bool empty()const{return t.empty();}
		size_type size()const{return t.size();}
		size_type max_size()const{return t.max_size();}

		data_type& operator[](const key_type& k){
			pair<iterator,bool> res=insert(value_type(k,T()));//insert(pair<key,val>(key_type,data_type))
			//res是一个pair<iterator,bool>,第一个元素是iterator,指向一个pair<key,val>
			return (*(res).first).second;
		}
		
		pair<iterator,bool> insert(const value_type& x){
			return t.insert_unique(x);
		}
		
		iterator insert(iterator position,const value_type& x){
			return t.insert_unique(position,x);
		}

		template<class InputIterator>
		void insert(InputIterator first,InputIterator last){
			t.insert_unique(first,last);
		}

		iterator find(const key_type& x){return t.find(x);}
		const_iterator find(const key_type& x)const{return t.find(x);}
		size_type	count(const key_type& x)const{
			return t.count(x);
		}






};

}
#endif