#include "iostream"
#include "string"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/wait.h"
#include "string.h"

using std::endl;
using std::cout;
const int BUF_SIZE=1024;
int endProc=0;
void error_handling(std::string msg){
    cout<<msg<<endl;
    exit(1);
}

void read_childproc(int sig){
    //子进程结束时结束方法：
    pid_t pid;
    int status;
    pid = waitpid(-1,&status,WNOHANG);//waitpid用于终止进程，-1表示任意检测到达的任意子进程
                            //
    std::cout<<"removed proc id:"<<pid<<std::endl;
    endProc++;
}

int main(int argc,char* argv[]){
    if(argc!=2){
        error_handling("输入<IP><PORT>");
    }
    int accept_clnt_num=0;
    int serv_sock,clnt_sock;
    sockaddr_in serv_adr,clnt_adr;

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len,state;
    char buf[BUF_SIZE];
    //1.子程序退出时处理函数
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state = sigaction(SIGCHLD,&act,0);

    //2.设置服务器socket
    serv_sock=socket(PF_INET,SOCK_STREAM,0);//SOCK_STREAM是TCP协议 PF_INET是IPV4
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//服务器的地址为本机
    serv_adr.sin_port = htons(atoi(argv[1]));//先把字符串转为int，然后转为网络所使用的大头段
    //3.绑定+监听
    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");//绑定一个端口
    
    if(listen(serv_sock,5)==-1)//第二个参数并不是说最多有5个用户链接！！！注意
        error_handling("listen() error");
    //设置服务器socket->bind->listen->accept
    
    while(true){
        adr_sz=sizeof(clnt_adr);//用于传给accept，因为不能传&sizeof(clnt_adr)
        clnt_sock=accept(serv_sock,(sockaddr*)&clnt_adr,&adr_sz);
        if(clnt_sock==-1)//接入失败
            continue;
        else{
            ++accept_clnt_num;
            cout<<"new client connected..."<<accept_clnt_num<<endl;
        }
        pid_t pid=fork();
        if(pid==-1){//接入成功但fork失败
            close(clnt_sock);
            continue;
        }
        //fork成功
        if(pid==0){//子进程运行区域
            close(serv_sock);
            while((str_len=read(clnt_sock,buf,BUF_SIZE))!=0)
                write(clnt_sock , buf , str_len);

            close(clnt_sock);
            std::cout<<"client disconnect..."<<endl;
            return 0;
        }
        else{//主进程运行区域
            cout<<"client sock num:"<<clnt_sock<<endl;
            close(clnt_sock);
            if(accept_clnt_num>=5){
                int endflag=false;
                while(endflag==false){
                    sleep(10);
                    if(endProc>=5)
                    endflag=true;
                }
                if(endflag==true) break;
            }
        }
    }
    close(serv_sock);//由主进程执行，因为子进程在最后都会return 0;
    return 0;
}