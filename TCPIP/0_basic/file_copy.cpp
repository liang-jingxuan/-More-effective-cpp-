//this file realise a function that copy a .txt file
#include "iostream"
#include "fstream"
#include "string.h"
#include "string"
using namespace std;
int main(int argc,char *argv[]){
    //argv[0] is the file name
    //argv[1] is the source file to copied
    //argc[2] is the destination file
    if(argc!=3){
        cerr<<"There must be two fine names!"<<endl;
        exit(1);
    }
    string new_file_name=argv[2];
    if(strcmp( argv[1],argv[2])==0){
        new_file_name.clear();
        string tmp=argv[2];
        for(int ix=0;ix<tmp.size()&&tmp[ix]!='.';++ix)
            new_file_name+=tmp[ix];
        new_file_name+="_copy.txt";
    }

    ifstream source(argv[1]);
    if(!source)
         cerr<<"Oops! Unable to save session data!\n";
    else{
        ofstream new_f(new_file_name);
        string tmp_s;
        while(std::getline(source,tmp_s)){
            new_f<<tmp_s<<endl;
        }
    }
}