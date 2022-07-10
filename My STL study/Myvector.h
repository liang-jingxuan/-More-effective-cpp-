#ifndef MY_VECTOR_H
#define MY_VECTOR_H
//来自其他文件的内容:1.my_allocator类（分配&释放内存）,2.全局的construct、destroy（构造和析构）
template<class T, class Alloc=my_allocator>
class Myvector{
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type&     reference;
        typedef value_type*     iterator;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
    //以上这部分保证了可以使用STL的算法

    protected:
        typedef simple_alloc<value_type,Alloc> data_allocator;

        iterator start;
        iterator finish;
        iterator end_of_storage;
    
        void insert_aux(iterator position,const T& x);//在position位置插入一个x
        void deallocate();//释放掉目前的vector,一般用在push_back后空间用完,换新地址时释放掉旧的vector//√
        void fill_initialize(size_type n,const T& val);//√
    public:
        iterator begin();//√
        iterator end();//√
        size_type size()const;//两个指针相减得到的是ptrdiff_t,因此要转成size_type//√
        size_type capacity()const;//查看当前vector还能存入多少个元素,//√
        bool empty()const ;//√
        reference operator[](size_type n);//√

        reference back()const;//√
        reference front()const;//√

        void push_back(const T& x);//√

        void pop_back();//√

        void insert(iterator position,size_type n,T& x) ;//在position位置插入n个x

        iterator erase(iterator position);//消除position上的元素,后面的补上//√
        iterator erase(iterator first,iterator last);//消除fist到last中间的元素,后面的补上
        void resize(size_type new_size,const T& x);//√
        void resize(size_type new_size);//√
        void clear();//√


    public:
        //构造函数
        Myvector():start(0),finish(0),end_of_storage(0){}//无参数时构成一个0大小的vector//√
        Myvector(size_type n,const T& val){fill_initialize(n,val);}//√
        Myvector(int n,const T&val){fill_initialize(n,val);}//√
        Myvector(long n,const T&val){fill_initialize(n,val);}//√
        explicit Myvector(size_type n){fill_initialize(n,T());}//√

        //析构
        ~Myvector();

    protected: 
        iterator allocate_and_fill(size_type n,const T& x);//√
    
        
};

#endif