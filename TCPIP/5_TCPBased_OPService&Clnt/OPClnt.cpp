#include "iostream"
#include "string"
#include "unistd.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "string.h"
bool valid_operator(char message);
bool is_numc(char c);
using std::cout;using std::cin;using std::endl;
const int BUF_SIZE=1024;
void error_handling(std::string msg){
    fputs(msg.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}
int main(int argc,char* argv[]){
    //argc=3
    //argv[1] is the IP address of the target server.
    //argv[2] is the port number we want.
    sockaddr_in srv_adr;//The server we want to connect.
    int sock;//Indecates that if the socket is created.
    char message[BUF_SIZE];//To receive the result.
    int str_len;//The size of the received message.

    if(argc!=3){
        cout<<"You should type 2 parameter! The first is the <IP> and the second is the <port>"<<endl;
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error!");

    memset(&srv_adr,0,sizeof(srv_adr));
    srv_adr.sin_family=PF_INET;
    srv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    srv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(sockaddr*)&srv_adr,sizeof(srv_adr))==-1)
        error_handling("Connect() error!");
    else
        cout<<"####################Connected#################"<<endl;

    
    //1.receive "Operand count" from the server, then input the total number of Operand and send to server
    //I do not let this occurrs in the client,
    //in order to make sure that the server is well-ready.
    int recv_len = 0, recv_cnt = 0;
    while (recv_len < 14)
    {                                                 // sizeof("Operand count\0")
        recv_cnt = read(sock, message, BUF_SIZE - 1); // receive the echo, which lenght is str_len
        if (recv_len == -1)
            error_handling("read() error!");
        recv_len += recv_cnt;
    }
    message[14] = 0;//message = "Operand count:\0"
    cout << message << ": ";

    std::cin.getline(message, BUF_SIZE); // input the operand number
    bool valid = true;
    char *p = message;
    while (*p != '\0')
    {
        if (*p > '9' || *p < '0')
        {
            valid = false;
            break;
        }
        ++p;
    }

    if (valid != true)
    { // input '0', a number less than 0 or there are character
        close(sock);
        error_handling("Invalid input!");
    }
    int NumOfOperand = atoi(message);
    // 2.receive "Operand x" from the server, then input the total number of Operand and send to server
    std::string send_message;
    for (int ix = 0; ix < NumOfOperand; ix++)
    {
        cout << "Operand " << ix + 1 << ": ";
        std::cin.getline(message, 1024); // the Number of operand
        for (int ix = 0; ix < strlen(message); ++ix)//check the input number
        {   
            if(message[0]=='-') continue;
            if (is_numc(message[ix])==false)
            {
                if (message[ix] == '-')
                    continue;
                std::cout << "Invalid input!" << endl;
                exit(1);
            }
        }
        send_message += message;
        send_message += '\n';
    } // format:"123\n56\n"
    cout << "Operator: ";
    std::cin.getline(message, BUF_SIZE); // the Number of operand
    if (valid_operator(message[0])==false)
    {
        cout << "Invalid operator!" << endl;
        close(sock);
        exit(1);
    }
    send_message += message;
    send_message += 'q'; // terminator
    // format:"123\n56\n*q"

    // 3.Send it to the server
    str_len = write(sock, send_message.c_str(), strlen(send_message.c_str()));
    while (str_len != send_message.size())
        ; // wait for the client sent the whole string

    // 4.receive the result from the server
    // The resule end up with q, for example 324q
    recv_len=0;
    memset(message,0,str_len);
    while (message[recv_len-1] != 'q')
    {
        recv_cnt = read(sock, message, BUF_SIZE - 1); // receive the echo, which lenght is str_len
        if (recv_cnt == -1)
            error_handling("read() error!");
        recv_len += recv_cnt;
    } //<end of while(message[recv_len] != 'q')>
    message[recv_len-1] = '\0';
    cout << "Operation result: " << message << endl;

    //5.close the sock
    close(sock);
    return 0;
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