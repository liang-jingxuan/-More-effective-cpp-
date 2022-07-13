#include <iostream>
#include"Myvector.h"
using namespace std;
int main(){
    //1.Myallocator 文件测试:OK
    //void *p=mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::allocate(66);
    //mySTL::Mysimple_alloc<void,mySTL::malloc_alloc>::deallocate(p,66);

    mySTL::Myvector<int> v;
    
    v.push_back(4);
    int* p=&v[0];//等价于mySTL::Myvector<int>::iterator 因为typedef T* iterator,
                //其中T是int  所以int*是iterator原型
    cout<<*p<<endl;
    cout<<v[0]<<endl;
    return 1;
}