//Data in the network is big-endian, however, CPU of computers is not always big-endian,
//therefore, when we need to transfer data through network, we first need to do endian converse
//first. 
//function htons(),htonl() can converse host compuer endian to nerwork endian.
//                  where 'h' indecates the 'this host', n means 'network', and s means 'short'
#include <arpa/inet.h>
#include "iostream"
using std::cout;
using std::endl;

int main(int argc,char *argv[]){
    unsigned short host_port=0x1234;
    unsigned short net_port;
    unsigned long host_addr=0x12345678;
    unsigned long net_addr;

    net_port=htons(host_port);//converse the host_port to network short.
                            //network endian should be big-endian.
    net_addr=htonl(host_addr);

    cout<<"Host ordered port: "<<std::hex<<host_port<<std::endl;
    cout<<"Network ordered port: "<<std::hex<<net_port<<endl;
                    //if net_port=3412, and net_addr=78563412
                    //then the CPU of this computer is a small-endian CPU
    cout<<"Host ordered address:  "<<std::hex<<host_addr<<endl;
    cout<<"Network ordered address:  "<<std::hex<<net_addr<<endl;
}
