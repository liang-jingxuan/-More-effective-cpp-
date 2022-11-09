#include "iostream"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1,&status,WNOHANG);
    if(WIFEXITED(status)){
        std::cout<<"Removec proc id:"<<id<<std::endl;
        std::cout<<"Child send:"<<WEXITSTATUS(status)<<std::endl;
    }
}

int main(int argc,char* argv[]){
    struct sigaction act;
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);//SIGCHLD在有子进程中止时中断

    pid_t pid=fork();
    if(pid == 0){//子进程执行区域
        std::cout<<"Hi! I'm child process 1!"<<std::endl;
        sleep(10);
        return 12;
    }else{//父进程执行区域
        std::cout<<"Child proc id:"<<pid<<std::endl;
        pid = fork();
        if(pid == 0){//另一个子进程
            std::cout<<"Hi! I'm child process 2!"<<std::endl;
            sleep(10);
            exit(24);
        }
        else{
            std::cout<<"Child procid:"<<pid<<std::endl;
            for(int ix=0;ix<10;++ix){
                std::cout<<"wait..."<<std::endl;
                sleep(5);
            }
        }
    }
    return 0;
}
//       主
//       /\
//      主 子1
//     / \
//    主  子2
//区分主和子进程的方法：对主进程来说子进程的pid==0，子进程不知道自己的pid因为子进程
//眼里自己是主进程