//若在线程未结束时进程结束则线程也会结束，因此可以使用pthread_join进行阻塞等待
#include "iostream"
#include "pthread.h"
#include "string.h"
#include "unistd.h"
void* thread_main(void* argv){
    int cnt=*((int*)argv);//开发者知道参数的含义，所以这里时int
                        //需要根据实际变化
    char *msg=(char*) malloc(sizeof("Hi! This is Liang Jingxuan"));
    strcpy(msg,"Hi! This is Liang Jingxuan");
    std::cout<<"From thread:"<<(char*) msg<<std::endl;
    for(int ix=0;ix<cnt;++ix){
        sleep(1);
        std::cout<<"running thread"<<std::endl;
    }
    return (void*)msg;
}

int main(int argc,char*argv[]){
    pthread_t thread_id;
    int thread_param=5;//线程输入参数
    void* thr_ret;//线程输出

    if(pthread_create(&thread_id,NULL,thread_main,(void*)&thread_param)!=0){
        //1.创建线程
        std::cout<<"pthread_create() error"<<std::endl;
        return -1;
    }

    if(pthread_join(thread_id,&thr_ret)!=0){
        //2.等待线程ID为thread_id的线程结束，并获取线程返回值
        std::cout<<"pthread_join() error"<<std::endl;
        return -1;
    }

    std::cout<<"Thread return message"<<(char*) thr_ret<<std::endl;
    free(thr_ret);
    return 0;
}