#include <iostream>
#include <fstream>
using namespace std;
int main(int argc,char *argv[]){
    //argv[0] is this file name
    //argc[1] is the first string
    
    ofstream file("/home/jingxuan1/TCPIP_Learn/data.txt",ios_base::app);
    if(!file)
         cerr<<"Oops! Unable to save session data!\n";
    else{
        for(int ix=1;ix<argc;++ix)
        file<<argv[ix]<<endl;
    }

    
}