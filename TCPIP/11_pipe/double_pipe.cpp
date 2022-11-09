#include "iostream"
#include "unistd.h"
const unsigned char BUF_SZ=255;
int main(int agrc,char* agrv[]){
    int fds1[2],fds2[2];//fds1属于父进程，fds2属于子进程
                        //[0]是出口，[1]是入口
    char str1[]="Who are you?";
    char str2[]="Thank you!";
    char buf[BUF_SZ];
    pid_t pid;

    pipe(fds1),pipe(fds2);
    pid=fork();
    if(pid==0){
        //子进程
        write(fds1[1],str1,sizeof(str1));
            //↑给父进程传数据：把数据放入父进程所用的管道的入口,即fds1[1]
        read(fds2[0],buf,BUF_SZ);
            //↑接收来自父进程的数据：从自己的管道的出口读入数据，即fds2[0]
        std::cout<<"子进程："<<std::string(buf)<<std::endl;
    }
    else{
        //父进程
        read(fds1[0],buf,BUF_SZ);
        std::cout<<"父进程："<<std::string(buf)<<std::endl;
        write(fds2[1],str2,sizeof(str2));
        sleep(3);
    }
    return 0;
}