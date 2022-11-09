//聊天室服务器
#include "iostream"//cout
#include "sys/socket.h"//socket,bind,listen,accept
#include "arpa/inet.h"
#include "unistd.h"//sleep等
#include "threads.h"//pthread
#include "string.h"//memset
void* handle_clnt(void* arg);//线程：让用户加入聊天室并处理实现聊天功能
void send_msg(char*,int);//线程调用的函数
void error_handling(std::string str){
    std::cout<<str<<std::endl;
    exit(1);
}
const u_char MAX_CLNT=50;//最大接入用户数
int clnt_cnt=0;//临界资源1：接入用户个数
int clnt_socks[MAX_CLNT];//临界资源2：接入用户的socket
const int BUF_SZ=100;
pthread_mutex_t mutx;//锁

int main(int argc,char*argv[]){
    if(argc!=2){
        error_handling("输入参数：<端口号>");
    }
    int srv_sock;
    struct sockaddr_in srv_adr,tmpclnt_adr;
    socklen_t clnt_adr_sz;
    
    /*服务器socket设置*/
    srv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_port=htons(atoi(argv[1]));
    
    if(bind(srv_sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("bind() error!");
    
    if(listen(srv_sock,5)==-1)
        error_handling("listen() error!");
    
    /*锁初始化*/
    pthread_mutex_init(&mutx,NULL);
    
    /*服务器服务*/
    while(true){
        int tmp_clnt_sock;
        clnt_adr_sz=sizeof(tmpclnt_adr);
        if((tmp_clnt_sock=accept(srv_sock,(sockaddr*)&tmpclnt_adr,&clnt_adr_sz))==-1)
            continue;

        
        pthread_mutex_lock(&mutx);//上锁，要处理sock_cnt
        clnt_socks[clnt_cnt]=tmp_clnt_sock;//注意顺序！先赋值再++
        clnt_cnt++;
        pthread_mutex_unlock(&mutx);
        
        pthread_t thread_id;
        pthread_create(&thread_id,NULL,handle_clnt,(void*)&tmp_clnt_sock);
        pthread_detach(thread_id);//线程终止的时候会销毁该线程，未终止则继续执行
        std::cout<<"Connected client IP:"
                <<inet_ntoa(tmpclnt_adr.sin_addr)<<std::endl;
    }
    close(srv_sock);
    return 0;
}

void* handle_clnt(void* arg){//参数是int类型的用户socket号
    int clnt_sock=*((int*)arg);
    char msg[BUF_SZ];
    int str_len=0;    
    while((str_len=read(clnt_sock,msg,BUF_SZ))!=0)
        send_msg(msg,str_len);
    //说明=0，用户断开链接
    //用户断开链接，需要操作操作clnt_cnt,clnt_sockets
    pthread_mutex_lock(&mutx);//上锁，
    for(int ix=0;ix<clnt_cnt;++ix){
        if(clnt_socks[ix]==clnt_sock){
            while(ix<clnt_cnt-1){
                clnt_socks[ix]==clnt_socks[ix+1];
                ++ix;
            }
            break;
        }
    }
    --clnt_cnt;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}

void send_msg(char* msg,int len){
    //把消息发给所有人,上锁，防止有人发送过程有人退出
    pthread_mutex_lock(&mutx);
    for(int ix=0;ix<clnt_cnt;++ix){
        int write_len=write(clnt_socks[ix],msg,len);
        std::cout<<"Send "<<write_len<<" number of char to"<<clnt_socks[ix]<<std::endl;
    }
    pthread_mutex_unlock(&mutx);
}