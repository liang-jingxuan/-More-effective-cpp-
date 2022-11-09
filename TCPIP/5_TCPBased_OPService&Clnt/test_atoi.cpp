#include "iostream"
#include "string"
#include "vector"
#include "string.h"
using namespace std;
std::string calculate(std::vector<long>& x,const char &fun_operator);
void fun(const char *msg);
bool valid_operator(char message);
bool is_numc(char c);



int main(){
    char message[1024];
    std::cin.getline(message,1024);//the Number of operand

    bool valid=true;
    char *p=message;
    while(*p!='\0'){
        if(!is_numc(*p)){
            valid=false;
            break;
        }
        ++p;
    }

    int NumOfOperand = atoi(message);
    if(NumOfOperand==0) valid = false;
    if(valid != true){//input '0', a number less than 0 or there are character
        cout<<"Invalid input! exit(1)!"<<endl;
        exit(1);
    }
    std::string send_message;
    for(int ix=0;ix<NumOfOperand;ix++){
        cout<<"Operand "<<ix+1<<": ";
        std::cin.getline(message,1024);//the Number of operand
        for(int ix=0;ix<strlen(message);++ix){
            if(message[ix]>'9'||message[ix]<'0'){
                if(message[ix]=='-') continue;
                std::cout<<"Invalid input!"<<endl;
                exit(1);
            }
        }
        send_message+=message;
        send_message+='\n';
    }//format:"123\n56\n"
    cout<<"Operator: ";
    std::cin.getline(message,BUFSIZ);//get the operator
    if(valid_operator(message[0])==false){
        std::cout<<"Invalid operator!"<<endl;
        exit(1);
    }
    send_message += message;// + - * /
    cout<<'\n'<<send_message.c_str();
    fun(send_message.c_str());
}

void fun(const char *msg){//format:"123\n56\n+"
    std::vector<long> operands;
    const char* p_left = msg,*p_right = msg;
    while(*p_left!='\0'){//end while if *p_left is operator or \0
        if(*(p_left+1)=='\0') break;
        else if(*p_left=='-') ++p_right;
        while(*p_right <='9'&&*p_right>='0')++p_right;
            operands.push_back(atol(std::string(p_left,p_right).c_str()));
            ++p_right;//ognore \n
            p_left = p_right;
    }
    std::string res = calculate(operands,*p_left);
    std::cout<<res;
}



std::string calculate(std::vector<long>& x,const char &fun_operator){
    switch (fun_operator){
    case '+':{
        long res=0;
        for(std::vector<long>::iterator p=x.begin();p!=x.end();++p){
            res+=*p;
        }
        return std::to_string(res);
        break;
    }
    case '-':{
        long res=0;
        for(int ix=0;ix<x.size();++ix){
            res-=x[ix];
        }
        return std::to_string(res);
        break;
    }
    case '*':{
        long res=1;
        for(int ix=0;ix<x.size();++ix){
            if(x[ix]==0)
                return 0;
            res*=x[ix];
        }
        return std::to_string(res);
        break;
    }
    case '/':{
        if(x.size()==1&&x[0]==0) return std::to_string(0);
        else if(x.size()==1&&x[0]!=0) return std::to_string(x[0]);
        double res; 
        for(int ix=0;ix<x.size();++ix){
            if(ix>=0&&x[ix]==0){
                return std::string("Divisor cannot be 0!");
            }
            if(ix==0) res=x[ix];
            else res/=x[ix];
            return std::to_string(res);
        }
        break;
    }
    }//end of switch
    return "invalid result";
}

bool valid_operator(char message){
    if(message=='+'||message=='*'||
            message=='-'||message=='/')
    return true;
    
    return false;
}

bool is_numc(char c){
    if(c<='9'&&c>='0') return true;
    return false;
}