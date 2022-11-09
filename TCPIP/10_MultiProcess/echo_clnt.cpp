#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"

using std::cout;
using std::endl;
const int BUF_SIZE=1024;
void error_handling(std::string msg){
    fputs(msg.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[]){
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;
    if(argc!=3){
        std::cout<<"Usage : "<<argv[0]<<" <IP> <port>"<<std::endl;
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error!");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=PF_INET;//using IPV4
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);//The IP address to connect is given in argv[1]
    serv_adr.sin_port = htons(atoi(argv[2]));//The port to connect is given in argv[2]

    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error!");//cannot connect wo the server
    else
        cout<<"Connected.................."<<endl;//connect to server successfully
    
    while(1){
        cout<<"Input message (Q to quit):"<<endl;
        std::cin.getline(message,BUF_SIZE);

        if((message[0]=='q'||message[0]=='Q')&&message[1]==0)//Type "q" or "Q" to quit
            break;

        str_len = write(sock,message,strlen(message));

        int recv_len = 0,recv_cnt=0;
        while(recv_len < str_len){
            recv_cnt = read(sock,message,BUF_SIZE-1);//receive the echo, which lenght is str_len
            if(recv_cnt == -1)
                error_handling("read() error!");
            recv_len+=recv_cnt;
        }
        message[str_len]='\0';
        cout<<"Message from server:" <<message<<endl;
    }
    close(sock);
    return 0;
}