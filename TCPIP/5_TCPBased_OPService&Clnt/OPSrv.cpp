#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"
#include "vector"
using std::endl;
using std::cout;
std::string calculate(std::vector<long>& x,const char &fun_operator);
bool valid_operator(char message);
bool is_numc(char c);

const int BUF_SIZE=1024;
void error_handling(std::string msg){
    fputs(msg.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char* argv[]){
    //argc=2;
    //argc[1] is the used port.
    int srv_sock,clnt_sock;
    char msg[BUF_SIZE];
    int str_len;//The actual received string length.

    struct sockaddr_in srv_addr,clnt_addr;
    socklen_t clnt_adr_sz;

    if(argc!=2){
        std::cout<<"Usga : <"<<argv[0]<<">"<<endl;
        exit(1);
    }

    srv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(srv_sock==-1){
        error_handling("Socket() error!");
    }

    memset(&srv_addr,0,sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY is the IP of this host
    srv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(srv_sock,(struct sockaddr*)&srv_addr,sizeof(srv_addr))==-1)
        error_handling("bind() error!");

    if (listen(srv_sock, 1) == -1) // at most 5 request
        error_handling("listen() error!");

    clnt_adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(srv_sock, (sockaddr *)&clnt_addr, &clnt_adr_sz);
    if (clnt_sock == -1) // accept to the client faild!
        error_handling("accept() error!");
    else
    { // accept to the client successfull!
        str_len = write(clnt_sock, "Operand count", sizeof("Operand count"));
    }
    while (str_len != sizeof("Operand count"))
        ; // wait for the write

    memset(msg, 0, BUF_SIZE); // clear the msg to ensure the first char is not 'q'
    int recv_len = 0, recv_cnt = 0;
    while (msg[recv_len-1] != 'q')
    {                                                  // format:"123\n56\n*q"
        recv_cnt = read(clnt_sock, msg, BUF_SIZE - 1); // receive the echo, which lenght is str_len
        if (recv_len == -1)
            error_handling("read() error!");
        recv_len += recv_cnt;
    }
    msg[recv_len] = '\0'; //
    // format:"123\n56\n*q\0"

    std::vector<long> operands;
    const char *p_left = msg, *p_right = msg;
    while (*p_left!='\0')
    { // end while if *p_left is operator or \0
        if (valid_operator(*p_left)&&*(p_left+1)=='q') break;
        else if(*p_left=='-')    ++p_right;
        while (*p_right <= '9' && *p_right >= '0')
            ++p_right;
        operands.push_back(atol(std::string(p_left, p_right).c_str()));
        ++p_right; // ognore \n
        p_left = p_right;
    }
    std::string res = calculate(operands, *p_left);
    res+='q';
    str_len = write(clnt_sock, res.c_str(), res.size());
    close(clnt_sock);
    close(srv_sock);
    return 0;
}

std::string calculate(std::vector<long> &x, const char &fun_operator)
{
    switch (fun_operator)
    {
    case '+':
    {
        long res = 0;
        for (std::vector<long>::iterator p = x.begin(); p != x.end(); ++p)
        {
            res += *p;
        }
        return std::to_string(res);
        break;
    }
    case '-':
    {
        long res = 0;
        for (size_t ix = 0; ix < x.size(); ++ix)
        {
            res -= x[ix];
        }
        return std::to_string(res);
        break;
    }
    case '*':
    {
        long res = 1;
        for (size_t ix = 0; ix < x.size(); ++ix)
        {
            if (x[ix] == 0)
                return 0;
            res *= x[ix];
        }
        return std::to_string(res);
        break;
    }
    case '/':
    {
        if (x.size() == 1 && x[0] == 0)
            return std::to_string(0);
        else if (x.size() == 1 && x[0] != 0)
            return std::to_string(x[0]);
        double res;
        for (int ix = 0; ix < x.size(); ++ix)
        {
            if (ix >= 0 && x[ix] == 0)
            {
                return std::string("Divisor cannot be 0!");
            }
            if (ix == 0)
                res = x[ix];
            else if(x[ix]!=0)
                res /= x[ix];
        }
        return std::to_string(res);
        break;
    }
    } // end of switch
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