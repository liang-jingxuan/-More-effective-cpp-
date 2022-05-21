#include <iostream>
using namespace std;

//一旦抛出异常就会就近找一个try语块否则
//就会一直退出，直到遇见try语块,然后执行下面的catch。如果退出栈
//的时候遇见的不是try语块，不会继续向下执行，而是直接退出
void fun1();
void fun2();
void fun3();
void fun1(){
    try{
        fun2();
    }
    catch(char &c){
        cout<<"fun1() has catch a char exception!"<<endl;
    }
    
    cout<<"fun1() has no exception!"<<endl;//这句没有被执行，
    //因为fun2()里抛出了一个int异常，上面捕获的是char异常，捕获和抛出的异常类型不同
    //跳过try-catch语句下面所有程序!!
}

void fun2(){
    try{
        fun3();
    }
    catch(int &w){
        cout<<"In fun2():An exception is catch!"<<endl;
        cout<<"int =" <<w<<endl;
    }

    fun3();//从这里出去的int异常会被main的try-catch捕获
}

void fun3(){
    try{
        throw double(3.14);
    }
    catch(double &d){
        cout<<"In fun3():A double exception is catch!"<<endl;
        cout<<"double =" <<d<<endl;
    }
    throw int(9);
}
int main(){
    try{
        fun1();//调用顺序fun1()->fun2(){catch}->fun3(){throw}
    }
    catch(double &d){
        cout<<"In main():An double exception is catch!"<<endl;
        cout<<"int =" <<d<<endl;//这句不会被执行，因为fun1()里出来的是int类型
    }
    catch(int &w){
        cout<<"In main():An int exception is catch!"<<endl;
        cout<<"int =" <<w<<endl;
    }
return 0;
}
//In fun3():A double exception is catch!//fun3里自己抛出自己捕获的语块
//double =3.14
//In fun2():An exception is catch!//在fun3抛出int异常后再fun2被捕获
//int =9
//In fun3():A double exception is catch!//fun2里再次调用了fun3，再次出发fun3里自己抛出自己捕获的语块
//double =3.14
//In main():An int exception is catch!//由于fun1没有异常捕获语块，所以被main捕获
//int =9