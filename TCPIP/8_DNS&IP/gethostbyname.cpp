#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "string.h"
void error_handling(std::string msg){
    std::cerr<<msg<<std::endl;
    exit(1);
}
int main(int argc,char *argv[]){
    hostent *host;
    if(argc!=2){
        std::cerr<<"请输入一个域名作为参数"<<std::endl;
        exit(1);
    }
    host=gethostbyname(argv[1]);
    if(!host)
        error_handling("gethost...error");
    
    std::cout<<"官方域名"<<host->h_name<<std::endl;
    for(int ix=0;host->h_aliases[ix];++ix)
        std::cout<<"其他域名"<<ix+1<<": "<<host->h_aliases[ix]<<std::endl;

    std::cout<<"地址类型";
    if(host->h_addrtype==AF_INET6) std::cout<<"AF_INET6"<<std::endl;
    else std::cout<<"AF_INET"<<std::endl;

    for(int ix=0;host->h_addr_list[ix];++ix)
        std::cout<<"IP地址"<<ix+1<<": "<<inet_ntoa(*(in_addr*)host->h_addr_list[ix])<<std::endl;
    return 0;
}