#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"
const int BUF_SIZE=1024;
void error_handling(std::string msg){
    std::cout<<msg<<std::endl;
    exit(1);
}
int main(int argc,char *argv[]){
    int sock;
    char msg[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    sockaddr_in srv_adr,from_adr;

    if(argc!=3)
        error_handling("参数数量应该为2:<IP地址,端口号>");

    sock=socket(PF_INET,SOCK_DGRAM,0);
    if(sock==-1)
        error_handling("socket() error!");

    memset(&srv_adr,0,sizeof(srv_adr));    
    
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    //inet_addr把“x.x.x.x”格式转换为网络所要求的格式(),即大端存储方式
    srv_adr.sin_port=htons(atoi(argv[2]));//注意这里不能用htonl

    //注意！这里不需要connext，因为UDP是面向数据报的，不需要链接
    while(1){
        std::cout<<"输入语句(输入q或Q退出)"<<std::endl;
        std::cin>>msg;
        if(strcmp(msg,"q")==0||strcmp(msg,"Q")==0) break;

        sendto(sock,msg,strlen(msg),0,(sockaddr*)&srv_adr,sizeof(srv_adr));
        adr_sz=sizeof(from_adr);
        str_len=recvfrom(sock,msg,BUF_SIZE,0,(sockaddr*)&from_adr,&adr_sz);
        msg[str_len]='\0';
        std::cout<<"来自服务器的回声："<<msg<<std::endl;
    }
    close(sock);
    return 0;
}




