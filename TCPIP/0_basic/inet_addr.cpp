//inet_addr converse the IP address (string) into a big-endian long type
#include "iostream"
#include <arpa/inet.h>
#include "string"
using std::cout;
using std::endl;

int main(int argc,char *argv[]){
    std::string addr1="1.2.3.4";//In this computer(small-endian),this would be stored as "34 12"
    std::string addr2="159.128.3.256";//inavailable IP address!
    //****************1.use inet_addr() to converse string to 32bit data***********
    //converse the IP address addr1
    unsigned long conv_addr = inet_addr(addr1.c_str());//1.IP address(string)->usigned long(big-endian)
    if (conv_addr==INADDR_NONE)
        cout<<"Error occured!\n: inet_addr(addr1) failed"<<endl;
    else
        cout<<"Network ordered interger addr: \n "<<conv_addr<<endl;

    conv_addr=inet_addr(addr2.c_str());
    if (conv_addr==INADDR_NONE)
        cout<<"Error occured!\n: inet_addr(addr2) failed"<<endl;
    else
        cout<<"Network ordered interger addr: \n "<<conv_addr<<endl;
    
    //*************2.use inet_aton() to converse string to 32bit data and store at the seconde par.
    struct sockaddr_in addr_inet;

    if(!inet_aton("1.2.3.4",&addr_inet.sin_addr)){//2.IP address(string)->usigned long(big-endian)
        fputs("Conversion error",stderr);
        fputc('\n',stderr);
        exit(1);
    }
    else{
        cout<<"Network ordered interger addr:"<< addr_inet.sin_addr.s_addr<<endl;
    }
    //*****************3.converse usigned long ->IP address(string)
    cout<<"Use \"inet_ntoa()\" to concerse usigner long into string:"
        <<std::string(inet_ntoa(addr_inet.sin_addr))<<endl;
    
    return 0;
}