#include "string.h"
#include "RCSTRING"
#include <iostream>
using namespace std;

class myString { // the standard st
public: // employ the techniques in this
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
    
    /*通过调试可以发现3个地址都不同*/

}