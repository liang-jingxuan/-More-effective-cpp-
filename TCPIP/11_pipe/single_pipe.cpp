#include "iostream"
#include "unistd.h"
const int BUF_SZ=30;
int main(int argc,char* argv[]){
    int fds[2];//fds[0]管道出口，fds[1]管道入口
    std::string str("HI!I am Liang Jingxuan");
    char buf[BUF_SZ];
    pid_t pid;

    pipe(fds);
    pid=fork();//创新一个新进程，0是父进程，其他是子进程
    if(pid==0){
        //子进程向管道入口放入数据
        write(fds[1],str.c_str(),str.size()+1);//+1是因为C字符串需要"\0"
    }
    else{
        //子进程从管道出口取出数据
        std::cout<<"这里是子进程："<<std::endl;
        read(fds[0],buf,BUF_SZ);
        std::cout<<buf<<std::endl;
    }
    return 0;
}
//缺点：只有一个管道，两个进程需要设计严密的使用管道的时序
//      否则可能刚把数据放进管道又自己取出了放入的数据