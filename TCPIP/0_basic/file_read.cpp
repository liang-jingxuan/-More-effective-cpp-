#include <iostream>
#include <fstream>
#include "string"
using namespace std;
int main(int argc,char *argv[]){
    //argv[0] is this file name
    //argc[1] is the first string
    
    ifstream file(argv[1]);
    if(!file)
         cerr<<"Oops! Unable to save session data!\n";
    else{
        string tmp_s;
        while(std::getline(file,tmp_s)){
            cout<<tmp_s<<endl;
        }
    }
}