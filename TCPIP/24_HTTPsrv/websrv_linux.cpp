#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "pthread.h"
#include "array"
const int BUF_SZ=1024;
const u_char SMALL_BUF=100;

void* request_handler(void*);
void send_data(FILE* fp,char* ct,char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(std::string str){
    std::cout<<str<<std::endl;
    exit(1);
}

int main(int argc,char* argv[]){
    if(argc!=2){
        error_handling("输入参数：<端口号>");
    }
    int srv_sock,clnt_sock;
    sockaddr_in srv_adr,clnt_adr;
    char buf[BUF_SZ];
    pthread_t t_id;

    srv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_adr.sin_family=AF_INET;
    srv_adr.sin_port=htons(atoi(argv[1]));
    if(bind(srv_sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("bind() error!");
    if(listen(srv_sock,5)==-1)
        error_handling("listen() error!");
    
    while(true){//每次有新用户接入就创建新线程
        in_addr_t clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(srv_sock,(sockaddr*)&clnt_adr,&clnt_adr_sz);
        std::cout<<"Connect Requests:"<<inet_ntoa(clnt_adr.sin_addr)
                <<','<<ntohs(clnt_adr.sin_port)<<std::endl;
        pthread_create(&t_id,NULL,request_handler,(void*)&clnt_sock);
        pthread_detach(t_id);
    }
    close(srv_sock);//由服务器断开链接
    return 0;
}

void* request_handler(void* arg){
    int clnt_sock=*((int*)arg);
    char req_line[SMALL_BUF];
    FILE* clnt_read;
    FILE* clnt_write;

    std::array<char,10> method;
    std::array<char,15> ct;
    std::array<char,30> file_name;

    clnt_read=fdopen(clnt_sock,"r");
    clnt_write=fdopen(dup(clnt_sock),"w");
    fgets(req_line,SMALL_BUF,clnt_read);
    if(strstr(req_line,"HTTP/")==NULL){
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    strcpy(method.begin(),strtok(req_line," /"));
    strcpy(file_name.begin(),strtok(NULL," /"));
    strcpy(ct.begin(),content_type(file_name.begin()));
    if(strcmp(method.begin(),"GET")!=0){
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    fclose(clnt_read);
    send_data(clnt_write,ct.begin(),file_name.begin());
}

void send_data(FILE* fp,char* ct,char* file_name){
    std::array<char,sizeof("HTTP/1.0 200 OK\r\n")> 
                protocol{"HTTP/1.0 200 OK\r\n"};
    std::array<char,sizeof("Server:Linux Web Server \r\n")>
                server{"Server:Linux Web Server \r\n"};
    std::array<char,sizeof("Content-length:2048\r\n")>
                cnt_len{"Content-length:2048\r\n"};
    std::array<char,SMALL_BUF> cnt_type;
    std::array<char,BUF_SZ> buf;
    FILE* send_file;
    sprintf(cnt_type.begin(),"Content-type:%s\r\n\r\n",ct);
    send_file=fopen(file_name,"r");
    if(send_file==NULL){
        send_error(fp);
        return;
    }
    fputs(protocol.begin(),fp);
    fputs(server.begin(),fp);
    fputs(cnt_len.begin(),fp);
    fputs(cnt_type.begin(),fp);

    while(fgets(buf.begin(),BUF_SZ,send_file)!=NULL){
        fputs(protocol.begin(),fp);
        fflush(fp);
    }
    fflush(fp);
    fflush(fp);
}

char* content_type(char* file){
    std::array<char,SMALL_BUF> extension;
    std::array<char,SMALL_BUF> file_name;
    strcpy(file_name.begin(),file);
    strtok(file_name.begin(),".");
    strcpy(extension.begin(),strtok(NULL,"."));

    if(!strcmp(extension.begin(),"html")||!strcmp(extension.begin(),"htm"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE* fp){
    std::array<char,sizeof("HTTP/1.0 400 Bad Request\r\n")> 
                protocol{"HTTP/1.0 400 Bad Request\r\n"};
    std::array<char,sizeof("Server:Linux Web Server \r\n")>
                server{"Server:Linux Web Server \r\n"};
    std::array<char,sizeof("Content-length:2048\r\n")>
                cnt_len{"Content-length:2048\r\n"};
    std::array<char,sizeof("Content-ype:text/html\r\n\r\n")>
                cnt_type{"Content-ype:text/html\r\n\r\n"};

    fputs(protocol.begin(),fp);
    fputs(server.begin(),fp);
    fputs(cnt_len.begin(),fp);
    fputs(cnt_type.begin(),fp);
    fflush(fp);
}