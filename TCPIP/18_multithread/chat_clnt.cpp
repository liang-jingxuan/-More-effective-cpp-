//聊天室客户端
#include "iostream"//cout
#include "sys/socket.h"//socket,bind,listen,accept
#include "arpa/inet.h"
#include "unistd.h"//sleep等
#include "threads.h"//pthread
#include "string.h"//memset

const u_char BUF_SZ=100;
const u_char NAME_SZ=20;

void* send_msg(void* arg);//线程1
void* recv_msg(void* arg);//线程2
void error_handling(std::string msg){
    std::cout<<msg<<std::endl;
    exit(1);
}

std::string name;

int main(int argc,char* argv[]){
    if(argc!=4)
        error_handling("输入参数：<服务器IP地址><端口号><ID>");
    int clnt_sock;
    sockaddr_in srv_adr;
    pthread_t snd_thread_id,rcv_thread_id;//线程id
    void* thread_return;//线程返回值

    name='['+std::string(argv[3])+']';

    clnt_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(clnt_sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("connect() error!");
    
    pthread_create(&snd_thread_id,NULL,send_msg,(void*)&clnt_sock);
    pthread_create(&rcv_thread_id,NULL,recv_msg,(void*)&clnt_sock);
    pthread_join(snd_thread_id,&thread_return);//阻塞等待线程结束
    pthread_join(rcv_thread_id,&thread_return);//阻塞等待线程结束
    close(clnt_sock);
    return 0;
}

void* send_msg(void* arg){
    int sock=*((int*)arg);
    int name_sz=name.size();

    while (true){
        std::string name_msg(name);
        char msg[BUF_SZ];
        std::cin.getline(msg,BUF_SZ);        
        if(!strcmp(msg,"q")||!strcmp(msg,"Q")){
            close(sock);//这里不调用close(sock)则recv_msg线程则不会退出
            exit(0);//
        }
        name_msg+=msg;
        write(sock,name_msg.c_str(),name_msg.size());
    }
    return NULL;
}

void* recv_msg(void* arg){
    int sock=*((int*)arg);
    char name_msg[NAME_SZ+BUF_SZ];
    int str_len=0;
    while(true){
        str_len=read(sock,name_msg,BUF_SZ);//最后一个接入聊天室的人一直卡在这，why?
        if(str_len==-1){
            std::cout<<"read() error!退出接收线程！"<<std::endl;
            return (void*)-1;
        }
        else if(str_len==0){
            break;
        }
        name_msg[str_len]=0;
        std::cout<<name_msg<<std::endl;
    }
    return NULL;
}