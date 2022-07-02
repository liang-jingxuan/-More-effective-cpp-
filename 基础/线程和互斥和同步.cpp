#include <iostream>
#include <thread>
using namespace std;
int i=0;//共享变量

void test(){
    int num=10000;
    for (int ix=0;ix<num;ix++){
        i+=1;
    }
}
void test2(){
    int num=10000;
    for (int ix=0;ix<num;ix++){
        i+=2;
    }
}
int main(){
    cout<<"Start all threads,"<<endl;

    thread thread_tesst1(test);//线程1
    thread thread_tesst2(test2);//线程2
    thread thread_tesst3(test);//线程3
//  线程1~3都会改变共享变量,由于上下文切换,可能导致有的步骤由于切换而出错
//  由于test和test2都对共享变量i进行了操作，这些代码段因此都被称为“临界区”，临界区是
//访问共享资源的代码片段，共享变量一定不能给多线程同时执行。我们希望这样的代码段“互斥”，
//即当有一个线程在执行共享变量的操作的时候其他线程都不要进入临界区。
//即当线程1执行的时候，线程2不要执行；线程2执行的时候，线程1不要执行。
// 
//  在这段代码中，线程1~3没有先后关系，可以同时运行。
//  
    thread_tesst1.join();
    thread_tesst2.join();
    thread_tesst3.join();

    cout << "All threads joined." << endl;
    cout << "now i is " << i << endl;
    //输出不确定,正确答案是10000+20000+10000=40000，但有时候不是
    return 0;
}
//1.线程与互斥
//  在文件"线程和互斥.cpp"中,由于test和test2都对共享变量i进行了操作，因而被称为临界区，
//三个线程需要解决互斥问题，即三个线程不能同时进行。三个线程没有先后关系
//2.线程与同步
//  有时候线程可以有先后关系，比如“读->写”，读的过程不能写，读完才能写，这样
//需要互相等待和互通消息被称为“同步”。
//3.总结：
//  同步和互斥都有不能同时执行的意思，由于了线程之间的先后关系的引入而有了“同步”。
//  同步⊊互斥

