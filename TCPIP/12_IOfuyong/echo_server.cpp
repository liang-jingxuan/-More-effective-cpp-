//使用IO复用完成多用户回声服务
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "sys/time.h"
#include "sys/select.h"

const unsigned char BUF_SZ=100;
void error_handling(std::string buf){
    std::cout<<buf<<std::endl;
    exit(1);
}

int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    timeval timeout;
    fd_set reads,cpy_reads;

    socklen_t adr_sz;

    int fd_max,fd_num,str_len;//最大描述符数，描述符总数
    char buf[BUF_SZ];
    if(argc!=2){
        error_handling("输入1个参数:<端口号>");
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);//IPV4=PF_INET;SOCK_STREAM=TCP协议
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//本地地址
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_port=htons(atoi(argv[1]));//htons=host to network short

    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error!");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error!");

    FD_ZERO(&reads);//初始化描述符数组
    FD_SET(serv_sock,&reads);
    fd_max=serv_sock;

    while(1){
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)
            break;
        //select会返回发生事件（接收，传输，异常）的描述个数,赋值给fd_num
        //select是一个阻塞函数，阻塞过程中在给定事件内进行监视描述符
        if(fd_num==0)//无事发生
            continue;

        for(int ix=0;ix<fd_max+1;++ix){//只知道发生了fd_num个事件，不知道是哪个，所以要遍历
            if(FD_ISSET(ix,&cpy_reads)){
                if(ix==serv_sock){
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=accept(serv_sock,(sockaddr*)&clnt_adr,&adr_sz);
                    FD_SET(clnt_sock,&reads);//注册描述clnt_sock
                    if(fd_max<clnt_sock)fd_max=clnt_sock;
                    std::cout<<"new connection!"<<clnt_sock<<std::endl;
                }//end <if(ix==serv_sock)>
                else{
                    str_len=read(ix,buf,BUF_SZ);
                    //如果服务多个用户，则一个个读取用户的消息,所以第一个不填clnt_sock
                    //问题：如果服务多个用户 {1，2，3}，此时3发了消息，ix从1开始遍历，调用read后不就会阻塞吗？
                    //回答：在进入这个判断之前我们调用了FS_ISSET来确定这个描述符是否有变化
                    //      1和2没变化就不会进入判断也不会调用read进入阻塞；3有变化
                    //      就会进入判断并调用read进入阻塞读取用户3的消息
                    if(str_len==0){
                        FD_CLR(ix,&reads);
                        close(ix);
                        std::cout<<"closed client"<<ix<<std::endl;
                    }
                    else{
                        write(ix,buf,str_len);//回声
                    }
                }//end <else>
            }//<end if(FDISSET())>
        }
    }//<end while(1)>
    close(serv_sock);
    return 0;
}