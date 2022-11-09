#include "iostream"
#include "unistd.h"
#include "signal.h"

void Mytimeout(int sig){
    if(sig==SIGALRM)
        std::cout<<"Time out!"<<std::endl;
        alarm(2);
}

void Mykeycontrol(int sig){
    if(sig==SIGINT)
        std::cout<<"CTRL+C pressed"<<std::endl;
}

int main(int argc,char *argv[]){
    struct sigaction act;
    act.sa_handler = Mytimeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM,&act,0);

    struct sigaction act2;
    act2.sa_handler = Mykeycontrol;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = 1;
    sigaction(SIGINT,&act2,0);
    
    alarm(2);//如果没有这一句，则永远无法进入Mytimeout

    for(int ix=0;ix<3;++ix){
        std::cout<<"wait..."<<std::endl;
        sleep(100);
        //sleep期间什么也不做，当alarm（2）到点后由操作系统调用Mytimeout，
    }
    return 0;
}