#ifndef MY_TRAINT_H
#define MY_TRAINT_H
//***********************0.迭代器类型

namespace mySTL{
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};
//***********************1.提取迭代器所指向对象的类型等
template < class Category, class T,class Distance = ptrdiff_t,
            class Pointer = T*,class Reference =T& >
struct iterator{
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

template<class I>//针对自定义的指定
struct Myiterator_traits{
    typedef typename  I::iterator_category    iterator_category;
    typedef typename  I::value_type           value_type;
    typedef typename  I::difference_type      defference_type;
    typedef typename  I::pointer              pointer;
    typedef typename  I::reference            reference;
};

template<class T>//针对自定义的指定
struct Myiterator_traits<T*>{
    typedef   random_access_iterator_tag     iterator_category;
    typedef   T                              value_type;
    typedef   ptrdiff_t                      defference_type;
    typedef   T*                             pointer;
    typedef   T&                             reference;
};

template<class T>//针对自定义的指定
struct Myiterator_traits<const T*>{
    typedef   random_access_iterator_tag     iterator_category;
    typedef   T                              value_type;
    typedef   ptrdiff_t                      defference_type;
    typedef   const T*                       pointer;
    typedef   const T&                       reference;
};

template<class I>
inline typename Myiterator_traits<I>::value_type*
value_type(const I&){
    return static_cast<typename Myiterator_traits<I>::value_type*>(0);
}

template<class I>
inline typename Myiterator_traits<I>::iterator_category
iterator_category(const I&){
    typedef typename Myiterator_traits<I>::iterator_category category;
    return category();//返回一个临时对象
}
//***********************2.提取某一数据类型的特征
//比如是否有高效的构造函数等,比如直接用malloc,memcpy

struct __Myfalse_type{};
struct __Mytrue_type{};
template<class type>
struct __Mytype_traits{
    typedef __Mytrue_type  this_dummy_member_must_be_first;

    typedef __Myfalse_type     has_trivial_default_constructor;
    typedef __Myfalse_type     has_trivial_copy_constructor;
    typedef __Myfalse_type     has_trivial_assigment_operator;
    typedef __Myfalse_type     has_trivial_destructor;
    typedef __Myfalse_type     is_POD_type;

};

template<>
struct __Mytype_traits<int>{//因为我的学习过程只用了vector<int>
    typedef __Mytrue_type     has_trivial_default_constructor;
    typedef __Mytrue_type     has_trivial_copy_constructor;
    typedef __Mytrue_type     has_trivial_assigment_operator;
    typedef __Mytrue_type     has_trivial_destructor;
    typedef __Mytrue_type     is_POD_type;
};
}//namespace mySTL
#endif