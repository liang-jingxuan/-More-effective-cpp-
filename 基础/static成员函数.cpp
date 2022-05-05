#include<iostream> 
using namespace std;
class Text
{
    public:
    	Text():num(5){} 
    static int fun()
    {
    	count++;//正确, 可以运算但不可以返回 
        //return num;//错误,static成员函数不能返回非static数据
		return count;//正确,static成员函数返回static数据 
    }
    
    int fun2(){
    	count++;//正确, 可以运算可以返回 
    	return count;//正确,非static可以返回static 
		return num;//正确,非static当然可以返回非static 
	}
    static int count;
    int num;
};
int Text::count=5;//用static成员变量必须要初始化

int main()
{
    Text t1;
    Text t2;
    t1.num=100;

    //t1.fun();//发生错误，fun函数return的是非static类成员 如果return count就正确
    return 0;
}
//结论:
//如果想调用某一个类的某个函数而不需要对象,则这个函数必须是static的
//想调用static数据成员必须使用static成员函数。 
