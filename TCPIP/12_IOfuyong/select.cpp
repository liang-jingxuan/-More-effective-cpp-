#include "iostream"
#include "unistd.h"
#include "sys/time.h"
#include "sys/select.h"
const unsigned char BUF_SIZE=30;//max=255

int main(int argc,char *argv[]){
    
    fd_set reads,temps;
    int result, str_len;
    char buf[BUF_SIZE];
    timeval timeout;

    FD_ZERO(&reads);//把reads清零,初始化
    FD_SET(0,&reads);//把第0个描述符设为1，监视reads






    while(true){

        temps=reads;//27行的select会初始化没发生变化的对应位，因此在select钱用temps记录
        timeout.tv_sec=5;
        timeout.tv_usec=0;//5秒后取消阻塞
        result=select(1,&temps,0,0,&timeout);//调用后阻塞，并一直监视描述符1，超时返回0，变化返回>0
        if(result==-1){
            //描述符
            std::cout<<"select() error!"<<std::endl;
            break;
        }
        else if(result==0){
            //描述符在给定时间内没有变化
            std::cout<<"Time-out!"<<std::endl;
        }
        else{
            //select返回大于0则说明有变化
            if(FD_ISSET(0,&temps)){
                
                str_len=read(0,buf,BUF_SIZE);
                buf[str_len]=0;
                std::cout<<"message from console:"<<buf<<std::endl;
            }
        }
    }
    return 0;
}