//区分接口(access)继承和实现(realize)继承
// 1.派生类只继承成员函数的接口（声明）；
// 2.派生类同时继承函数的接口和实现，但允许派生类改写实现；
// 3.派生类同时继承接口和实现，并且不允许派生类改写任何东西。
//接口:函数名, 实现:实现方法
#include <iostream>
using namespace std;
class Shape
{
public:                                    //公有继承使得所有函数接口都被继承
    virtual void draw() const = 0;         //纯虚函数:派生类必须重写这个函数,这就实现了
                                           // 1.只继承成员函数的接口（声明）
    virtual void error(const string &msg); //虚函数:派生类可以自行决定是否重写
                                           //实现了2.继承接口和实现,允许派生类改写实现

    int objectID() const; //条款37:派生类不应该改写非虚函数,否则调用的时候取决于静态类型
                          //即如果是派生指针则调用派生的函数,基类指针调用基类的函数
                          //不会根据实际指向的对象进行动态调整
};
class Rectangle : public Shape
{
};


//如果有3个类,其中2个可以调用基类的实现,而有一个类必须重新实现,可以这样处理:
class Airport{};
class Airplane{
    public:
        virtual void fly(const Airport &destination) = 0;
    protected :
        void defaultFly(const Airport &destination);
};

void Airplane::defaultFly(const Airport &destination)
{
    //飞机飞往某一目的地的缺省代码
}
//ModelA和ModelB使用同一种fly行为,所以封装起来放在基类调用即可
class ModelA: public Airplane {
    public:
        virtual void fly(const Airport& destination)
        { defaultFly(destination); }
};
class ModelB: public Airplane {
    public:
        virtual void fly(const Airport& destination)
        { defaultFly(destination); }

};
//ModelC必须自行实现一种fly行为
class ModelC: public Airplane {
    public:
    virtual void fly(const Airport& destination);
};


void ModelC::fly(const Airport& destination){
    //必须自行实现一种fly行为
//ModelC 飞往某一目的地的代码
}