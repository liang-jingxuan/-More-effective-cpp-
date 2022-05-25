//如果数据成员是指针，就尽量不要返回这个数据成员
#include <iostream>
#include "string.h"
using namespace std;
class String{
    public:
        String(const char* _str):str(new char[strlen(_str)+1]){
            strcpy(str,_str);
        }
        ~String(){
            cout<<"~String()";
            delete str;//记得删除数据指针成员
        }
        friend ostream& operator<<(ostream& s,String &data);
                    //开放给operator<<，使operator可以访问print
        friend ostream& operator<<(ostream& s,const String &data);

        operator char*()const{//String 转char*
            //实现方法1：直接返回str。该方法会使得私有数据成员暴露，即使是const也能被修改
            //return str;
            //实现方法2：返回一个拷贝
            char* cpy = new char[strlen(str)+1];
            return strcpy(cpy,str);
        }

         operator const char*()const{//String 转char*
            //return str;
            char* cpy = new char[strlen(str)+1];
            return strcpy(cpy,str);
        }

        const char& operator[](int index) const{//读
            //实现方法1：该方法会使得私有数据成员暴露，即使是const也能被修改
            //return str[index]; 
            //实现方法2:使返回值从char& 编程const char&
            cout<<"const char& operator[](int index) const"<<endl;
            return str[index]; 
        }

        char& operator[](int index){//写
            cout<<"char& operator[](int index)"<<endl;
            return str[index]; 
        }
    private:
        void print(ostream& s)const{
            s<<str;
        }
        char* str;
};

ostream& operator<<(ostream& s,String &data){
    data.print(s);
    return s;
}

ostream& operator<<(ostream& s,const String &data){
    data.print(s);
    return s;
}

String getstr(int a){
    switch (a)
    {
        case 1:
            return "AA";
            break;
        case 2:
            return "BB";
            break;
    }
    return "None";
}
int main(){
    //1.string转char*的转换过程暴露私有数据成员
    const String s1("Hello World!");
    cout<<"const String s1="<<s1<<endl;
    char *B = s1;//string转换为char*的过程中如果转换函数返回了private数据成员，那么strcpy就能改变const
    cout<<"char *B="<<B<<endl;
    strcpy(B,"Hi Jingxuan!");
    cout<<"char *B="<<B<<"\n const String s1="<<s1<<endl;
    //2.使用引用暴露私有数据成员，使用[]返回引用导致的暴露
    cout<<s1[0]<<endl;//罪魁祸首->[],调用const char& operator[](int index) const,因为s1是const
    String s2("Hi");
    cout<<s2[0]<<endl;//char& operator[](int index),因为s2是非const
    cout<<endl<<s1<<endl;
    //char* tmp=&s1[0];//现在tmp也能修改s1了
    //*tmp='Z';
    cout<<s1<<endl;
    //3.函数返回导致的string转char*
    const char* p = getstr(4);  //setstr函数返回string,但是右边是const char*
                                //通过String 的operator const char*成员函数将临时String 对象转换为const
                                //char*指针，并用这个指针初始化pc。
                                //临时String 对象被销毁，其析构函数被调用。析构函数中，data 指针被delete
    cout << p << endl;//无输出,因为在调用析构函数时,析构函数中的delete将其释放了
    //p[0]='b';//报错,左边是const char&型,不能修改
    //cout<<p<<endl;
}