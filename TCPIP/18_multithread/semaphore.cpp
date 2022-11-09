//使用信号量决定两个线程的执行顺序：
//决定n个线程的执行顺序需要n个信号量
//先read再accu需要2个信号量  read --(sem_one)--> accu
//                             <--(sem_two)--

#include "iostream"
#include "pthread.h"
#include "semaphore.h"
void* read(void*);
void* accu(void*);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc,char* argv[]){
    pthread_t id_t1,id_t2;//线程id
    sem_init(&sem_one,0,0);//每次read到一个数据就释放sem_one，允许accu进行计算
    sem_init(&sem_two,0,1);//accu算完后可以继续读

    pthread_create(&id_t1,NULL,read,NULL);//创建线程
    pthread_create(&id_t2,NULL,accu,NULL);

    pthread_join(id_t1,NULL);//线程返回
    pthread_join(id_t2,NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

void* read(void* arg){
    for(int ix=0;ix<5;++ix){
        std::cout<<"输入一个数:"<<std::endl;
        sem_wait(&sem_two);//占用sem_two，防止num被accu
        std::cin>>num;
        sem_post(&sem_one);//释放sem_one，允许accu执行
    }
    return NULL;
}

void* accu(void* arg){
    int sum=0;
    for(int ix=0;ix<5;++ix){
        sem_wait(&sem_one);//占用sem_one，说明read释放了sem_one而允许执行
        sum+=num;
        sem_post(&sem_two);//释放sem_two，允许read执行
    }
    std::cout<<"结束："<<sum<<std::endl;
    return NULL;
}