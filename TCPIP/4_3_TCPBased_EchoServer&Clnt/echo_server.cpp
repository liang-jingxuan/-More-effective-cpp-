#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"

using std::endl;
using std::cout;

void error_handling(std::string msg);
const int BUF_SIZE=1024;
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    char msg[BUF_SIZE];
    int str_len,i;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2){
        std::cout<<"Usga : <"<<argv[0]<<">"<<endl;
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);//using IPV4 and TCP 
    if(serv_sock==-1)
        error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));


    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error!");
    
    if(listen(serv_sock,5)==-1)//at most 5 request
        error_handling("listen() error!");

    clnt_adr_sz = sizeof(clnt_adr);

    for(i=0;i<5;++i){
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock==-1)
            error_handling("accept() error!");
        else
            cout<<"The "<<i+1<<"-th connect client \n"<<endl;

        while((str_len=read(clnt_sock,msg,BUF_SIZE))!=0){
        //Read the a string with BUF_SIZE lenght sent by clnt_sock, and store at msg.
        // When client call close(), an "EOF" would be read which is equal to -1
            msg[str_len]=0;
            cout<<inet_ntoa(clnt_adr.sin_addr)<<":"<<msg<<endl;
            write(clnt_sock,msg,str_len);
        }
        cout<<"Client"<<i+1<<" has disconnected!"<<endl;
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(std::string msg){
    fputs(msg.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}