#include "string.h"
#include "RCSTRING.h"
#include <iostream>
using namespace std;

class myString { 
public:
// Item, but that is not required
    myString(const char *value = ""){
        data=new char[strlen(value)+1];
        strcpy(data,value);
    }

    myString& operator=(const myString& rhs){//拷贝构造
        if(this==&rhs) return *this;
        delete [] data;
        data = new char[strlen(rhs.data)+1];
        strcpy(data,rhs.data);
        return *this;
    }

    //~myString(){delete data;}
    
    void print(ostream& s){
        s<<data;
    }
    
    private:
        char *data;
};


ostream& operator<<(ostream& s, myString& str){
    str.print(s);
    return s;
}

int main(){
    /*
    string a,b,c;
    a=b=c="Hello!";
    cout<<"a=" <<a <<", address="<< &a << endl;
    cout<<"b=" <<b <<", address="<< &b << endl;
    cout<<"c=" <<c <<", address="<< &c << endl;
    //STL的string采用的是深拷贝，通过调试可以看出三个地址都不相同
    
   
    myString a2,b2,c2;
    a2=b2=c2="Hello!";
    
    cout<<"a=" <<a2  << endl;
    cout<<"b=" <<b2  << endl;
    cout<<"c=" <<c2  << endl;
    */
    /*通过调试可以发现3个地址都不同*/
    RCString s1("string1");
    RCString s2("string2");
    RCString s3(s1);
    cout<<"Obj s3 has " << s3.ref_count() << " referecens" << endl;//理论:2。实际：2
    cout<<"Obj s2 has " << s2.ref_count() << " referecens" << endl;//理论:1。实际：1
    cout<<"Obj s1 has " << s1.ref_count() << " referecens" << endl;//理论上s1和s3引用数相同,都是2。实际：2
    RCString* p=&s2;
    (*p)[1] = 'T';//不受欢迎的修改
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:1,实际:1 .字符串:sTring1
    char* p2=&s2[3];
    *p2='I';//不受欢迎的修改
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:1,实际:1 .字符串:sTrIng1
    cout<<"s2="<< s2 <<". Ptr to s2 has " << p->ref_count() << " referecens" << endl;//理论:1。实际：1
    s2=s1;
    cout<<"Obj s2 has " << s2.ref_count() << " referecens" << endl;//理论:3。实际：3
                                                        //,因为s1可以共享,共享前有s1,s3两个引用,共享有s1~s3
    //此时s1,s2和s3都指向同一个字符串
    s1[2]='q';//结果:srqing1,s1修改了结果,s1被分离,s1有1个ref,s2和s3共用一个
    cout<< s1 <<" has " << s1.ref_count() << " referecens" << endl;//理论:1,实际:1.字符串:srqing1
    cout<< s2 <<" has " << s2.ref_count() << " referecens" << endl;//理论:2,实际:2.字符串:string1
    cout<< s3 <<" has " << s3.ref_count() << " referecens" << endl;//理论:2,实际:2
}