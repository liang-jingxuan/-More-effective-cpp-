#include <iostream>
#include "string"
using namespace std;
// 第一节：引用和指针的区别
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
// 第二节：类型转换
class A{
virtual ~A()=0;
};
A::~A(){}
class B:
public A{

};

void funA(B*){//接受一个非const对象指针
    cout<<"funA(B *)"<<endl;
}
void funA2(B&){//接受一个非const对象引用
    cout<<"funA2(B&)"<<endl;
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
        const B classB;
        funA(classB);//报错,funA接受一个非const B类,这里传的是const类
        
        B classB2;
        const B& pB =classB2;
        funA(pB);//报错,pB是一个const类引用,但funA要的是非const

        funA((B*)&classB);//OK!C风格转换
        funA(const_cast<B*>(&pB));//OK!C++风格转换,转换为一个B类指针,指向pB的地址

    //dynamic_cast,有继承关系的类间进行转换,父类指针转子类指针，
        //这个一般针对指向具有继承关系的指针
        A* pA = new B;//父类指针指向一个子类对象,pA指向B类的A部分
        funA(pA);   //报错信息:"A *" 类型的实参与 "B *" 类型的形参不兼容
                    //pA是一个A*类无异议,但是funA的要是B类引用

        funA(*pA);  //报错信息:不存在从 "A" 到 "B *" 的适当转换函数
                    //说明pA指向了一个A,但是pA指向的是B,说明pA指向了B中包含的A部分
        //接受指针的时候:转为指针
        funA(dynamic_cast<B*>(pA));//OK
        funA(dynamic_cast<B*>(*pA));
        funA(dynamic_cast<B&>(*pA));
        funA(dynamic_cast<B&>(pA));
        //接受引用的时候:转为引用
        funA2(dynamic_cast<B*>(pA));
        funA2(dynamic_cast<B*>(*pA));
        funA2(dynamic_cast<B&>(*pA));//OK
        funA2(dynamic_cast<B&>(pA));
}

//  第四节:避免无用的缺省构造函数（指没有参数的构造函数）
class EquipmentPiece {
public:
    EquipmentPiece(int IDNumber):ID(IDNumber){}
private:
    int ID;
};
int main(){
// 第一节：引用和指针的区别
    Pointer_Vs_Reference();


// 第二节：类型转换
    casts();


// 第三节:不要对数组使用多态
    //使用了继承的类其父类和子类大小不一样,而遍历的时候是根据内存大小进行访问的
    //比如一个全局函数要了一个父类作为参数,要遍历一个类数组
    //但是传入了一个子类,这是合法的,因为会子类继承字父类。
    //然而，对于遍历这个操作是根据内存大小进行访问的，而父类和子类大小不同，
    //其每次跳转的长度为sizeof(父类)而不是实际传上来的对象大小

    //简单来说,如果一个类具有继承关系,就不要定义一个该类的数组
    //      比如B继承了A,就不要 B Barray[10];这样做了

//  第四节:避免无用的缺省构造函数（指没有参数的构造函数）
//  这个很明显,无参数函数 VS 有默认参数的函数
//  funA() 和funA(int a=0);调用的funA()的时候会产生二义性,因为两个函数都满足 
    EquipmentPiece bestPiece[10];   //报错信息:类 "EquipmentPiece" 不存在默认构造函数
                                    //因为没有对应的无参数构造函数,

    EquipmentPiece* pE = 
        new EquipmentPiece[10]; //报错原因和上面一样
    
}