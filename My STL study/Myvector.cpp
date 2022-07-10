#include"Myvector.h"
using namespace std;

//****************************public内容:对用户开放的内容
template<class T,class Alloc>
Myvector<T,Alloc>::~Myvector(){
    destroy(start,finish);//全局函数,用来析构start到finish之间的对象
    deallocate();//调用vector专用的控件释放器,释放所有空间
}

template<class T,class Alloc>
Myvector<T,Alloc>::iterator//返回值,因为这么iterator是在Myvector类内定义的,只在该类的内部使用
Myvector<T,Alloc>::begin(){return start;}

template<class T,class Alloc>
Myvector<T,Alloc>::iterator
Myvector<T,Alloc>::end(){return finish;}

template<class T,class Alloc>
Myvector<T,Alloc>::size_type
Myvector<T,Alloc>::size() const {return (size_type)(finish-start);}

template<class T,class Alloc>
bool //bool不是Myvector特有的,所以不用指定
Myvector<T,Alloc>::empty()const{ return start==finish;}

template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::front()const{return *start;}

template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::back()const{return *(finish-1);}

template<class T,class Alloc>
Myvector<T,Alloc>::size_type
Myvector<T,Alloc>::capacity() const{return (size_type)(end_of_storage-start);}

template<class T,class Alloc>
Myvector<T,Alloc>::reference
Myvector<T,Alloc>::operator[](size_type n){return *(start+n);}

template<class T,class Alloc>
void Myvector<T,Alloc>::push_back(const T&x){
    if(finish!=end_of_storage){
        construct(finish,x);
        ++finish;
    }
    else{
        insert_aux(finish,x);//finish位置插入一个x
    }
}

template<class T,class Alloc>
void Myvector<T,Alloc>::pop_back(){
    if(empty()) return;
    --finish;
    destroy(finish);
}
template<class T,class Alloc>
Myvector<T,Alloc>::iterator
Myvector<T,Alloc>::erase(iterator position){
    if(position+1!=finish){
        copy(position+1,finish,position);//把position+1到finish的元素移动到以position为起点的位置,
    }
    --finish;
    destroy(finish);
    return position;
}

template<class T,class Alloc>
Myvector<T,Alloc>::iterator
Myvector<T,Alloc>::erase(iterator first,iterator last){
    //1.先把last后面的元素复制到first的位置
    iterator i=copy(last,finish,first);//完成后i指向结尾,理论上i==last
    //2.析构last开始的元素
    destroy(i,finish);
    //3.总共消除了last-first个元素,让finish后退last-first个元素
    finish=finish-(last-first);
    //4.返回插入点
    return first;
}

template<class T,class Alloc>
void Myvector<T,Alloc>::resize(size_type new_size,const T& x){
    //将数组重新塑形为一个大小为new_size的数组,元素的值都是x
    if(new_size>size()){//新空间大于旧空间
        insert(finish,new_size-size(),x);
    }
    else{
        //新空间小于旧空间,消除多余的控件
        erase(start+new_size,finish);
    }
}

template<class T,class Alloc>
void Myvector<T,Alloc>::clear(){
    erase(start,finish);
}

template<class T,class Alloc>
void Myvector<T,Alloc>::insert(iterator position,size_type n,T& x){
    //在position位置上插入n个x
    //1.插入后仍然不需要扩充
    if(capacity()>=n){
        //分两种情况,因为要考虑覆盖的问题

    }
    else{
    //2.插入后需要扩充

    }
}
//************************************protected内容:不对用户开放的,但可能被public函数调用实现某些功能的函数
template<class T, class Alloc>
void Myvector<T,Alloc>::deallocate(){//释放掉目前所有的元素
    if(start)
        data_allocator::deallocate(start,finish-1);
}

template<class T, class Alloc>
void Myvector<T,Alloc>::insert_aux(iterator position,const T& x){
    //功能:先把position开始的元素全部后移一个单位,然后在position位置插入一个未知x,
    //在finish位置进行插入的时候调用insert_aux，由insert_aux判断是否用尽所有空间,未用尽就简单插入,
    //  用尽则进行扩充
    if (finish!=end_of_storage){
        construct(finish,*(finish-1));
        ++finish;
        T x_copy=x;
        copy_backward(position,finish-2,finish-1);//把position~finish-2的元素复制到finish-1为结尾的位置
        //比如v={0,0,0},v2={2,4,6},
        //输入:copy_backward(v2.begin(),v2.begin()+1,v.end()-1)
        //输出v={2,4,0}
        *position=x_copy;
    }
    else{
        //finish=end_of_storage,没有位置进行插入,需要进行扩充
        const size_type old_size=size();
        const size_type new_size=old_size==0 ? 1 : 2*old_size;

        iterator new_start=data_allocator::allocate(new_size);
        iterator new_finish = new_start;

        //进行转移
        new_finish=unitiallized_copy(start,finish,new_start);//把旧的只进行转移
        construct(new_finish,x);//在末尾构造新的值
        ++new_finish;
        //new_finish=unitiallized_copy(position,finish,new_finish);//可忽略的一行代码

        destroy(start,end_of_storage);//原文这里是start到finish?原因:此时finish=end_of_storage,无所谓
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

template<class T, class Alloc>
Myvector<T,Alloc>::iterator 
Myvector<T,Alloc>::allocate_and_fill(size_type n,const T& x) {
    //配置大小为n的空间,在空间上赋值为X
    iterator result=data_allocator::allocator(n);
    unitiallized_fill_n(result,n,x);//全局变量,在起始为result的空间上赋值n个x
    return result;
}

template<class T, class Alloc>
void Myvector<T,Alloc>::fill_initialize(size_type n,const T& val){
    start=allocate_and_fill(n,val);
    finish=start + n;
    end_of_storage=finish;
}

