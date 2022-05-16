#include <iostream>
#include "string"
using namespace std;

void print_refs(string& s){
    cout<<s<<endl;//不需要检查是否为空，因为引用不能为空
}
void print_ps(string* s){
    if(s)//需要检查是否为空
        cout<<*s<<endl;
}
void Pointer_Vs_Reference(){
        //区别1：1.引用不能为空值
        //string &s1; //报错！
                    //1.引用不能为空值，必须被初始化。编译器不允许通过
        string s2("Hello!");
        string& rfs=s2;

        string* ps; //OK！
                    //指针可以指向空对象，编译器可以通过，但不保证不出问题

        print_refs(s2); //传引用，效率更高，因为不用判断传入参数是否为空
        print_refs(*(new string("")));//OK,这是一个对象，值为空，但是不是空对象。
        print_ps(&s2);  //传指针
    //结论：如果对象可能为空，则用指针；对象必定不为空，可以用引用

    //区别2：引用对象一旦确定，不能改变.引用使用“=”操作符是赋值，而不是改变指向的对象
    //      而指针:                      使用“=”操作符则不是赋值，是改变指向的对象。
    string s3("World!");
    rfs=s3;//现在s2变成了World!而不再是Hello!，“=”没有使rfs指向s3，而是把s3的值赋给了rfs所指向的s2
    print_refs(s2);//World!
    print_refs(s3);//World!
    //结论：如果可能改变指向，则用指针；确定不会改变指向则用引用
}

void casts(){
    //static_cast 两个可以转换的类型之间转换，比如int和double，int和long;
        int num1=1,num2=2;
        double res;
        res=num1/num2;
        cout<<res<<endl;//理论结果是0
        res=(double)num1/num2;//理论结果是0.5,C风格的类型转换
        cout<<res<<endl;
        double res2=static_cast<double>(num1)/num2;
        cout<<res2<<endl;//理论结果是0.5，C++风格的类型转换
    //C++的类型转换若转换失败则会输出信息,而C的转换没有任何提示
    //const_cast 将const类型转换为非const

    //dynamic_cast,有继承关系的类间进行转换,父类指针转子类指针，
}


int main(){
// 第一节：引用和指针的区别
    Pointer_Vs_Reference();
// 第二节：类型转换
    casts();
}