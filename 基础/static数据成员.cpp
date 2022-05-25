#include<iostream>
using namespace std;
class Text
{
    public:
    static int count;
};

int Text::count=0;//��static��Ա��������Ҫ��ʼ��

int main()
{
    Text t1;
    Text t2;

    t1.count = 100;     //t1�����static��Աcount��Ϊ100
    cout<<t2.count<<endl; //��t2�����ӡstatic��Ա��ʱ����ʾ����100������0
    return 0;
}
