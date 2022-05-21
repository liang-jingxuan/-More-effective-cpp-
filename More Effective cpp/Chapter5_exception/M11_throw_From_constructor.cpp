//在析构函数抛出异常会导致程序终止.
//如果有以下继承关系A->B,构造顺序是A->B,那么析构顺序是B->A,如果B抛出异常就会递归传递异常
//因此在写析构函数时你必须保守地假设有异常被激活
#include<iostream>
using namespace std;
class A{
    public:
        A(A* ptr=0):pA(ptr){}
        ~A(){
            cout<<"A: ~A()"<<endl;
            try{
                destoypA();//在析构函数里调用函数的时候,函数抛出的异常由析构来捕获,如果没有捕获就会终止程序
            }
            catch(int &w){
            //    cerr    << "Unable to log destruction of Session object " 
            //            << "at address " 
            //            << this 
            //            << ". Exception id="<<w<<endl; 
            //加入try-catch语块捕获异常可以保证异常不从析构函数中抛出,但是catch内也可能抛出异常.
            //为了防止catch内抛出异常,同时避免catch内使用try-catch,最好的办法是忽略异常,即catch内什么也不做
            }
            cout<<"try-catch is over!"<<endl;
    }
    private:
        void destoypA(){
            throw int(98);
            delete pA;
        }
        A* pA;
};

int main(){
    {
        cout<<"Creating a obj A!"<<endl;
        A myA;
        cout<<"obj is contruted with reference : myA"<<endl;
    }//myA离开生存空间,会调用myA的析构函数
    cout<<"myA has left its living space!"<<endl;//不会执行,因为有myA的析构过程抛出了异常
    return 1;
}