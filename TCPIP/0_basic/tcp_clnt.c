#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *msg);

int main(int argc,char *argv[]){
    //argv[1] is the IP address
    //argv[2] is the port num
    int sock;
    struct sockaddr_in serv_addr;
    char msg[30];
    int str_len=0;
    int idx=0,read_len=0;
    
    if(argc!=3){
        printf("Usage : %s <port\n>",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);//  use the IPV4 and TPC
    if(sock==-1)
        error_handling("socket() error\n");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect() error");

    while(read_len=read(sock,&msg[idx],3)){//read 2 chars each times
        if(read_len==-1)
            error_handling("read() error");
        
        ++str_len;//times that call for function read()
        idx+=3;
    }

    printf("Message from server:%s \n",msg);
    printf("function read() called count: %d\n",str_len);
    close(sock);
    return 0;

}

void error_handling(char *msg){
    fputs(msg,stderr);
    fputc('\n',stderr);
    exit(1);
}