
#include <iostream>
using namespace std;
class Base
{
public:
    void Bfun1(){cout << "I'm Bfun1()" << endl;}
protected:
    void Bfun2(){cout << "I'm Bfun2()" << endl;}
private:
    void Bfun3(){cout << "I'm Bfun3()" << endl;}
};
class Derived : private Base
{
public:
    void Dfun1() {cout << "I'm Dfun1()  " ;Bfun1();}//��ȷ!���ﲻ���û�����!���������Ե��ø��ຯ�� 
    void Dfun2() {cout << "I'm Dfun2()  " ;Bfun2();}//��ȷ!���ﲻ���û�����!���������Ե��ø��ຯ�� 
    void Dfun3() {cout << "I'm Dfun3()  " ;Bfun3();}//����!Base��Bfun3��˽�г�Ա 
};
int main()
{
    Derived d;
    d.Dfun1();
    d.Dfun2();
 
    //d.Bfun1();//����!�û�����ʱ,����Derived������ֱ�ӵ��ø����Ա Bfun1
    //d.Bfun2();//����!�û�����ʱ,����Derived������ֱ�ӵ��ø����Ա Bfun2
   // d.Bfun3();//����!�û�����ʱ,����Derived������ֱ�ӵ��ø����Ա Bfun3
    return 0;
}
//����:˽��(private) �̳к�ֻ�������ڶ����ʱ����ܵ��ø����˽��(��protected��public)����(����16~17)
//�û����򲢲��ܵ��ø��ຯ��(����26~28) 
