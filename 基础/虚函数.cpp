#include <iostream>
using namespace std;
class T1{
	public:
	T1(){	}
	virtual void fun1(){
		cout<<"In class T1,This is fun1\n";
	}
	virtual void fun2(){
		cout<<"In class T1,This is fun2\n";
	}
	
	void fun3(){
		cout<<"In class T1,This is fun3\n";
	}
}; 

class T2:public T1{
	public:
	T2(){	}
	void fun2(){
		cout<<"In class T2,This is fun2\n";
	}
	void fun3(){
		cout<<"In class T2,This is fun3\n";
	}
};

int main(){
	T1* p1,*p2,*p3;//���ǻ���ָ�� 
	T2 x1,x3;
	T1 x2;
	 
	p1= &x1 ;//����ָ��ָ�� ���� ���� 
	p1->	fun1();//����� In class T1,This is fun1; ���ಢû��fun1����,����ͨ���̳л����fun1 
	
	p2= &x2 ;//����ָ��ָ�� ���� ���� 
	p2->fun1();//����� In class T1,This is fun1
	
	p3 = &x3;//����ָ��ָ�� ���� ���� 
	p3->fun2();//���:In class T2,This is fun2 
	
	p3 = &x2;//����ָ��ָ�� ���� ���� 
	p3->fun2(); //���: In class T1,This is fun2
	
	T1 y1;T2 y2;
	y1.	fun3();//���: In class T1,This is fun3
	y2.fun3();//���: In class T2,This is fun3
	
	T1 *baseptr=&y2;
	baseptr->	fun3();//���:In class T1,This is fun3,����ָ��ָ������,���ǵ����˸��ຯ�� 
	baseptr=&y1;
	baseptr->fun3();//���:In class T1,This is fun3
	
	T2* sonptr=&y2;
	sonptr->	T1::fun3();//���:In class T1,This is fun3
	sonptr->	fun3();//���:In class T2,This is fun3
	return 0;
	
	
}
//����:
//1.���������ĳ��������virtual���û���ָ��->�������øú������� 
//(a)���಻���¶���ú��������  ����   ��ʵ�ַ���(����35,38)
//(b)����  ���¶���ú��������  ����   ��ʵ�ַ��� (����41)
 

//2.���������ĳ����������virtual,�û���ָ��->�������øú�������
//(a)���಻���������¶���ú���,������ø��ຯ��(����51,53)

//�����ȷʹ��virtual�ؼ���,��ôʹ�û���ָ��ָ�����������ȷ��������ĺ�����
//�������͸�����ͬ����������������໹�Ǹ���ȡ����ָ������� (����51,57)
