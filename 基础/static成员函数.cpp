#include<iostream> 
using namespace std;
class Text
{
    public:
    	Text():num(5){} 
    static int fun()
    {
    	count++;//��ȷ, �������㵫�����Է��� 
        //return num;//����,static��Ա�������ܷ��ط�static����
		return count;//��ȷ,static��Ա��������static���� 
    }
    
    int fun2(){
    	count++;//��ȷ, ����������Է��� 
    	return count;//��ȷ,��static���Է���static 
		return num;//��ȷ,��static��Ȼ���Է��ط�static 
	}
    static int count;
    int num;
};
int Text::count=5;//��static��Ա��������Ҫ��ʼ��

int main()
{
    Text t1;
    Text t2;
    t1.num=100;

    //t1.fun();//��������fun����return���Ƿ�static���Ա ���return count����ȷ
    return 0;
}
//����:
//��������ĳһ�����ĳ������������Ҫ����,���������������static��
//�����static���ݳ�Ա����ʹ��static��Ա������ 
