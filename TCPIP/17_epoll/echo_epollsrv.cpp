//epoll是改进的I/O复用方法，缺点是只用于linux系统
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "sys/epoll.h"
const u_char BUF_SZ=255;
const u_char EPOLL_FDNum=50;//最多能申请的描述符数量
void error_handling(std::string str){
    std::cout<<str<<std::endl;
    exit(1);
}


int main(int argc,char*argv[]){
    if(argc!=2){
        error_handling("输入一个参数：<端口号>");
    }
    char buf[BUF_SZ];
    /*1.设置服务器IP地址,socket*/
    int srv_sock,clnt_sock;
    sockaddr_in srv_adr,clnt_adr;

    srv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(srv_sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("bind() error!");
    if(listen(srv_sock,5)==-1)
        error_handling("listen() error!");
    

    /*2.设置epoll*/
    epoll_event *ep_events;//存储发生变化的描述符
    epoll_event tmp_event;//用于暂存需要放入例程的事件
    int epfd,event_cnt;//发生变化事件个数

    epfd=epoll_create(EPOLL_FDNum);//第一步创建epoll
    ep_events=(epoll_event*)malloc(sizeof(epoll_event)*EPOLL_FDNum);
    //2.1第一个事件是服务器socket
    tmp_event.events=EPOLLIN;
    tmp_event.data.fd=srv_sock;//第二部注册描述符
    epoll_ctl(epfd,EPOLL_CTL_ADD,srv_sock,&tmp_event);
            //向例程epfd添加（EPOLL_CTL_ADD）一个srv_sock，相关信息在tmp_evect中
    
    //3.服务器逻辑
    while(true){
        event_cnt=epoll_wait(epfd,ep_events,EPOLL_FDNum,-1);
        if(event_cnt==-1){
            std::cout<<"epoll_wait() error!"<<std::endl;
            break;//break while(true)
        }

        for(int ix=0;ix<event_cnt;++ix){
            if(ep_events[ix].data.fd==srv_sock){//新用户接入时，srv_sock发生变化
                socklen_t sock_len;
                clnt_sock=accept(srv_sock,(sockaddr*)&clnt_adr,&sock_len);
                tmp_event.events=EPOLLIN;
                tmp_event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&tmp_event);
                std::cout<<"new client connected"<<std::endl;
            }
            else{//已有用户请求回声服务
                int str_len=read(ep_events[ix].data.fd,buf,BUF_SZ);
                if(str_len==0){//断开链接请求
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[ix].data.fd,NULL);
                    close(ep_events[ix].data.fd);
                    std::cout<<"closed client socket: "
                            <<ep_events[ix].data.fd<<std::endl;
                }
                else{
                    write(ep_events[ix].data.fd,buf,BUF_SZ);
                }
            }
        }
    }
    close(srv_sock);
    close(epfd);
    return 0;
}