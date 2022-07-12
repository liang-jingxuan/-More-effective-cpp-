#ifndef MY_TRAINT_H
#define MY_TRAINT_H
//***********************1.提取迭代器所指向对象的类型等

//***********************2.提取某一数据类型的特征
//比如是否有高效的构造函数等,比如直接用malloc,memcpy
namespace mySTL{
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