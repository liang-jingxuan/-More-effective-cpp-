//三个进程：  1.从管道取来自用户的消息并写入文件
//          2.“接客”
//          3.给用户回声服务，并把消息放入管道
#include <iostream>
#include "unistd.h"//pipe,fork
#include "string"//stl::string
#include "stdlib.h"
#include "signal.h"
#include "sys/socket.h"//socket,bind,listen,accept
#include "arpa/inet.h"//htonl
#include "sys/wait.h"//waitpid WHOHANG
#include "string.h"//memset
#include <fstream>
const uint32_t BUF_SZ=65535;
//using namespace std;
void error_handling(std::string str){
    std::cout<<str<<std::endl;
    exit(1);
}

void read_childproc(int sig){
    //子进程结束时结束方法：
    pid_t pid;
    int status;
    pid = waitpid(-1,&status,WNOHANG);//waitpid用于终止进程，-1表示任意检测到达的任意子进程
                            //
    std::cout<<"removed proc id:"<<pid<<std::endl;
}

int main(int argc,char* argv[]){
    if(argc!=2){
        error_handling("输入一个参数：<端口号>");
    }
    int srv_sock,clnt_sock;
    sockaddr_in srv_adr,clnt_adr;
    int fds[2];

    pid_t pid;//记录进程号
    struct sigaction act;//记录子进程结束时处理方法
    socklen_t adr_sz;//填sock地址消息时用上
    char buf[BUF_SZ];//接收来自用户的消息

    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    int state=sigaction(SIGCHLD,&act,0);

    srv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(srv_sock,(struct sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("bind() error!");
    
    if(listen(srv_sock,5)==-1)
        error_handling("listen() error!");

    pipe(fds);
    pid=fork();
    if(pid==0){//父进程，进行文件读写
        std::ofstream file("/home/jx1/TCPIP_Learn/11_pipe/data.txt",std::ios_base::app);
        if(!file){//打开文件失败，阻塞等待fork的进程结束
            std::cerr<<"Oops! Unable to save session data!\n";
            pid_t pid;
            pid = wait(&state);
            if(WIFEXITED(state))
                std::cout<<"return of child proccess:"
                        <<WEXITSTATUS(state)<<std::endl;
            exit(1);//会不会导致僵尸进程？
        }
        //创建成功
        char msgbuf[BUF_SZ];
        int len;
        for(int ix=0;ix<10;++ix){//这样做是因为难以判断什么时候所有用户断开了
            len=read(fds[0],msgbuf,BUF_SZ);
            msgbuf[len]=0;
            file<<msgbuf<<std::endl;
        }
        return 0;
    }
    
    while(true){
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(srv_sock,(sockaddr*)&clnt_adr,&adr_sz);
        if(clnt_sock==-1)
            continue;
        else
            std::cout<<"new client connected..."<<std::endl;

        pid=fork();//每次有新用户就fork一个进程
        if(pid==0){
            close(srv_sock);
            int str_len;
            while((str_len=read(clnt_sock,buf,BUF_SZ))!=0){
                write(clnt_sock,buf,str_len);
                write(fds[1],buf,str_len);
            }
            close(clnt_sock);
            std::cout<<"client disconnected..."<<std::endl;
            return 0;
        }
        else
            close(clnt_sock);//如果这里关闭了srv_sock则出问题
    }
    close(srv_sock);
    return 0;
}


