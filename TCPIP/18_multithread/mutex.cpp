//同步的胡吃两，当一个线程进入临界区时其他线程不允许进入临界区
#include "threads.h"
#include "iostream"
//#include "unistd.h"
const u_char NumThread=100;

void* thread_inc(void*);
void* thread_des(void*);
pthread_mutex_t mutex;//临界区
long long num=0;

int main(int argc,char*argv[]){
    pthread_t thread_id[NumThread];

    pthread_mutex_init(&mutex,NULL);//初始化临界区

    for(int ix=0;ix<NumThread;++ix){
        if(ix%2)
            pthread_create(&(thread_id[ix]),NULL,thread_inc,NULL);
        else
            pthread_create(&(thread_id[ix]),NULL,thread_des,NULL);
    }

    for(int ix=0;ix<NumThread;++ix)
        pthread_join(thread_id[ix],NULL);//这个线程没有返回值，所以第二参数NULL
    
    std::cout<<"result:"<<num<<std::endl;
    pthread_mutex_destroy(&mutex);//销毁临界区
    return 0;
}

void* thread_inc(void* arg){
    //缺点：num要加50000000才解锁。优点：上锁、解锁次数少。
    pthread_mutex_lock(&mutex);//上锁
    for(int ix=0;ix<50000000;++ix)
        num+=1;
    pthread_mutex_unlock(&mutex);//解锁
    return NULL;
}

void* thread_des(void* arg){
    //缺点：要上锁、解锁50000000次。优点：num不会被长时间锁住。
    for(int ix=0;ix<50000000;++ix){
        pthread_mutex_lock(&mutex);
        num-=1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}