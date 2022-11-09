#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"
const int BUF_SIZE=1024;
void error_handling(std::string msg){
    std::cerr<<msg<<std::endl;
    exit(1);
}

int main(int argc,char* argv[] ){
    int srv_sock;
    char msg[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;

    sockaddr_in srv_adr,clnt_adr;
    if(argc!=2){
        error_handling("Required number of parameter:2");
    }
    //注册一个SOCKET
    srv_sock=socket(PF_INET,SOCK_DGRAM,0);
    if(srv_sock==-1)
        error_handling("UDP socket creation error");

    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_family = AF_INET;
    srv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_adr.sin_port = htons(atoi(argv[1]));

    //绑定SOCK和IP地址
    if(bind(srv_sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("bind() error");

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(srv_sock,msg,BUF_SIZE,0,
                    (sockaddr*)&clnt_adr,&clnt_adr_sz);
        msg[str_len]=0;
        std::cout<<"来自IP"<<inet_ntoa(clnt_adr.sin_addr)<<":"<<msg<<std::endl;
        //inet_ntoa
        sendto(srv_sock,msg,str_len,0,
                    (sockaddr*)&clnt_adr,clnt_adr_sz);
    }
    close(srv_sock);
    return 0;
}