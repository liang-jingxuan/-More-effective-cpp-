//  如果构造函数没有被完整地执行(即构造函数内抛出异常导致构造函数没跑完),
//则该对象无法被delete.
//  解决办法:使用智能指针,即使构造函数内抛出异常也不用担心构造不完全
#include "string"
#include <iostream>
#include "list"
#include "vector"
#include "memory"
using namespace std;
class Image { // 用于图像数据 
    public: 
        Image(const string &value = ""):img(value){}
        ~Image(){
            cout<<"~Image()"<<endl;
        }
    private:
        string img;//图像是二维数组
}; 
 
class AudioClip { // 用于声音数据 
    public: 
        AudioClip(const string &val = ""):voise(val){}
        ~AudioClip(){
            cout<<"~AudioClip()"<<endl;
        }
    private:
        string voise;
}; 
 
class PhoneNumber {  }; // 用于存储电话号码 
class BookEntry { // 通讯录中的条目 
    public: 
    
        BookEntry(  const string& name, 
                    const string& address = "", 
                    const string& imageFileName = "", 
                    const string& audioClipFileName = ""); 
        ~BookEntry(){
            //不需要delete因为使用的是智能指针
            cout<<"~BookEntry()"<<endl;
        }
        // 通过这个函数加入电话号码 
        void addPhoneNumber(const PhoneNumber& number); 

    private: 
        string theName; // 人的姓名 
        string theAddress; // 他们的地址 
        list<PhoneNumber> thePhones; // 他的电话号码 
        const auto_ptr<Image> theImage; // 他们的图像 
        const auto_ptr<AudioClip> theAudioClip; // 他们的一段声音片段 
}; 

BookEntry::BookEntry(   const string& name, 
                        const string& address, 
                        const string& imageFileName, 
                        const string& audioClipFileName) 
    : theName(name), theAddress(address), 
    theImage(imageFileName!=""?new Image(imageFileName):0),
    theAudioClip(audioClipFileName!=""?new AudioClip(audioClipFileName):0){  
        throw int(8);
} 

class A{
    public:
        A():number(true?throw int(100):95){
            //throw int(98);//并不一定是在函数体抛出异常,可能初始化列表也会抛出异常
            //但在初始化列表抛出异常的话没法捕捉
        }
        A(int val):number(val){}

        int check(){return number;}

        ~A(){
            cout<<"~A() called!"<<endl;
        }
    private:
        int number;
};

void Exception_from_Constructor(){
    A* pA = 0;
    cout<<" Exception in the constructor"<<endl;
    try{
        pA  =   new A;//抛异常的构造函数, 如果构造函数抛出异常,赋值语句不会被执行
    }
    catch(int &num){
        cout<<"execption id: "<< num<<endl;
        //cout<<pA->check()<<endl;  //无输出,因为对象没构造完成
                                    //而且因为调用了未构造完成的对象,程序被终止
        delete pA;//不会被执行
    }
    cout<<"No exception constructor"<<endl;
    pA  =   new A(35);//不抛异常的构造函数
    cout<< pA->check() <<endl;//输出35
    delete pA;//OK,会调用构造
}

int main(){
    //实验1:如果构造函数抛出异常
    //如果构造函数抛出异常,会导致:  1.构造出来的对象无法被析构
    //                            2.调用不完整对象的成员函数时候回导致程序终止
    Exception_from_Constructor();
    
    //实验2:将指针数据成员全部换成智能指针
    //使用智能指针进行初始化，即使抛出异常也能析构已初始化的对象
    try{
        BookEntry("Jingxuan","GDUT","IMG","VOISE");
    }
    catch(int &w){
        //因为BookEntry构造每完整运行，所以要析构掉已完成的部分
        //输出：
        //~AudioClip()
        //~Image()
        cout<<"Exception!"<<endl;
    }
    return 1;
}