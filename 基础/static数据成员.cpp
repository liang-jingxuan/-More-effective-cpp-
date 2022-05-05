#include<iostream>
using namespace std;
class Text
{
    public:
    static int count;
};

int Text::count=0;//用static成员变量必须要初始化

int main()
{
    Text t1;
    Text t2;

    t1.count = 100;     //t1对象把static成员count改为100
    cout<<t2.count<<endl; //当t2对象打印static成员的时候，显示的是100而不是0
    return 0;
}
