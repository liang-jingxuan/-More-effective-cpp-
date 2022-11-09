//多进程缺点：1.创建进程将会复制同样大小的内容存空间，开销大.
//          2.进程间数据交换需要特殊IPC技术(Inter process communication)
//线程解决的问题：为了得到多条代码执行流二复制整个内存区域负担太重的问题
//
//int pthread_creadte(pthread_t* restrict thread,//线程ID的变量地址
//                    const pthread_attr_t * restrict attr,//可选参数，NULL为默认属性
//                    void*(*start_routine)(void*),//要执行的线程的名字
//                    void* restrict arg)//线程所需的参数
//返回值：0成功，其他值失败
#include "iostream"
#include "pthread.h"
#include "unistd.h"//sleep
void* thread_main(void *arg){
    int argc=*((int*)arg);
    for(int ix=0;ix<argc;++ix){
        sleep(1);
        std::cout<<"running thread"<<std::endl;
    }
    return NULL;
}

int main(int argc,char*argv[]){
    pthread_t thread_id;
    int thread_param=5;
    if(pthread_create(&thread_id,NULL,thread_main,(void*)&thread_param)!=0){
        std::cout<<"pthread_create() error"<<std::endl;
        return -1;
    }
    std::cout<<"pthread create success! sleep(10) now..."<<std::endl;
    sleep(10);//如果这个小了，就会进程结束，线程也因此结束
    std::cout<<"end of main()"<<std::endl;
    return 0;
}
