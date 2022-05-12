#include <iostream>
#include "typeinfo"
#include "map"
#include "memory"
using namespace std;
//继承自同一个基类的子类的场景下，同级别的类两两有相同行为的互动会有不同的结果，这样的情况下可以使用虚函数。
//此时虚函数的输入参数一般的基类，那么如果判断传来的参数的对象实现精确的函数调用呢
//例如：颜色为基类， R G B为三个继承了基类的颜色类，R+G=黄，G+B=青，R+B=紫,相同颜色不变
class ColorBase;
class Red;
class Green;
class Blue;
class BlendingWithUnknowColor{}; //用于抛出异常
class ColorBase{
public:
    virtual void ColorBlending(ColorBase &) = 0;
};

class Red
: public ColorBase{
public:
    virtual void ColorBlending(ColorBase &); // ColorBase可能传入R、G或B其中一种，都会被ColorBase指针指向
                                             //除非枚举所有类，如virtual void ColorBlending(Red&)
                                             //                 virtual void ColorBlending(Green&)
};

class Green
:public ColorBase{
public:
    virtual void ColorBlending(ColorBase &otherColor);

    virtual void ColorBlending(Red &){
        cout << "Class Green: Green + Red = Yellow" << endl;
    }

    virtual void ColorBlending(Green &){
        cout << "Class Green: Green + Green = Green" << endl;
    }

    virtual void ColorBlending(Blue &){
        cout << "Class Green: Blue + Green = Purple" << endl;
    }
};

class Blue
: public ColorBase{
    public:
        virtual void ColorBlending(ColorBase &);
    private:
        virtual void BlendingRed(ColorBase&){
        //虽然知道是Red,但是传参还是要具体对象,毕竟红还可能是粉红,深红等子类
        //比如在36原色中 红255 和 和红125不一样
            cout<<"Class Blue: Blue + Red = Purple"<<endl;
        }

        virtual void BlendingBlue(ColorBase&){
            cout<<"Class Blue: Blue + Blue = Blue"<<endl;
        }

        virtual void BlendingGreen(ColorBase& green){
            //这里传入参数是ColorBase&的原因是
            //虚函数映射表的值的函数指针的传入参数要的是ColorBase&,所以这些函数传入参数改为ColorBase
            cout<<"Class Blue: Blue + Green = Cyan"<<endl;
            //Green& othergreen=dynamic_cast<Green&>(green);//如果需要后续操作,要先对传入参数进行转换
            //...处理方法
        }
    
        typedef void (Blue::*BlindingWhat_Ptr)(ColorBase&);
        typedef map<string,BlindingWhat_Ptr> BlendMap;//这个映射表只有在第一次调用lookup函数时候才初始化
        
        static BlindingWhat_Ptr lookup(const ColorBase& WhatColor);//虚函数表存在lookup中,用于在表中找到正确的函数
        static BlendMap* initBlendMap();//用于初始化虚函数表
};

Blue::BlindingWhat_Ptr 
Blue::lookup(const ColorBase& WhatColor){
    //虚函数表存在lookup中
    static std::unique_ptr<BlendMap> 
            BlendingWhatMap( initBlendMap() );

    BlendMap::iterator MapEntry = 
            (*BlendingWhatMap).find(typeid(WhatColor).name());

    if(MapEntry==(*BlendingWhatMap).end()) return 0;

    return (*MapEntry).second;
}

//初始化列表
Blue::BlendMap* //返回值
Blue::initBlendMap(){
    BlendMap* p = new BlendMap;
    (*p)[typeid(Red).name()] = &BlendingRed;
    //这里使用typeid(Red).name(),因为我不知道他会产生什么样的string,我想产生Red,但是他却产生了3Red!
    (*p)[typeid(Green).name()] = &BlendingGreen;
    (*p)[typeid(Blue).name()] = &BlendingBlue;
    
    return p;
}

void Blue::ColorBlending(ColorBase& otherColor){
    BlindingWhat_Ptr fun_ptr = lookup(otherColor);//查表

    if(fun_ptr){
        (this->*fun_ptr)(otherColor);//调用
    }
    else  
        throw BlendingWithUnknowColor();
}


void Red::ColorBlending(ColorBase &otherColor)
{
    const type_info &objType = typeid(otherColor);
    if (objType == typeid(Red))
    {
        cout << "Class Red: Red + Red = Red" << endl;
    }
    else if (objType == typeid(Green))
    {
        cout << "Class Red: Red + Green = Cyan" << endl;
    }
    else if (objType == typeid(Blue))
    {
        cout << "Class Red: Red + Blue = Purple" << endl;
    }
    else
        throw BlendingWithUnknowColor();
}

void Green::ColorBlending(ColorBase &otherColor)
{
    cout << "Transforming..." << endl;
    const type_info &objType = typeid(*this);
    if(objType==typeid(Green)){
        ColorBlending(static_cast<Green&>(otherColor));
        return;
    }
    otherColor.ColorBlending(*this);
}

int main()
{
    cout<<"testing class Red~----------------------------------------------------------- "<<endl;
//1.使用if-else语句,
        //原理：在对象的成员函数内部使用if-else识别出参数的类型后直接处理
        //    缺点：每增加一个同级类，都要在每个类增加if-else 逻辑
        Red red1;
        red1.ColorBlending(*(new Red()));   //红+红=红
        red1.ColorBlending(*(new Green())); //红+绿=青
        ColorBase* pr=&red1;
        pr->ColorBlending(*(new Red));
        red1.ColorBlending(*pr);
//2.使用虚函数。
cout<<"testing class Green~----------------------------------------------------------- "<<endl;
        //原理:使用虚函数在参数为基类指针的函数中 跳转 到参数的成员函数内
        //   缺点1：每增加一个同级类，该类都要写 已有同级类数量+1 个虚函数，
        //          比如增加一个white,就要在white里写4+1个虚函数ColorBlending，多出来的一个是基类作参数
        //   缺点2：基类指针指向子类的时候，如果传入的参数的类型和指向的子类一样，就会无限循环，(代码119-123)
        //          修正缺点2的方法：加入代码（81~85）
        //__静态类型,即参数的类型很明确
        Green green1, green2;
        //下面调用的是Green::ColorBlending(Red/Green &)
        green1.ColorBlending(*(new Red()));   // OK,红+红=红
        green1.ColorBlending(*(new Green())); // OK,绿+绿=绿
        green1.ColorBlending(green2);         // OK
        green1.ColorBlending(green1);         // OK

        //__动态类型，即参数的类型不明确
        Green *pg = &green1;
        pg->ColorBlending(*(new Green())); // OK
        pg->ColorBlending(green1);         // OK

        ColorBase *p = &green1;
        //下面两个调用的都是ColorBase::ColorBlending(ColorBase &),
        //然后虚函数让其转移到Green::ColorBlending(ColorBase &),Red::ColorBlending(ColorBase&)
        //再根据输入参数再次转移到对应类的ColorBlending(),这里转移到
        p->ColorBlending(red1); // OK,先吧red1当ColorBase&类因此输出transforming，
        //                      然后调用了Red::ColorBlending(*this),*this是Green类
        (*p).ColorBlending(red1); // OK,先吧red1当ColorBase&类因此输出transforming，
        //                      然后调用了Red::ColorBlending(*this),*this是Green类

        //下面都会无线循环
        p->ColorBlending(*(new Green()));   //无限循环.能检测出*this的类型是Green类型，
        //                                          但是传参的时候调Green::ColorBlending(ColorBase &)
        p->ColorBlending(green1);           //无限循环
        p->ColorBlending(green2);         //无限循环
        (*p).ColorBlending(*(new Green())); //无限循环
        
        green1.ColorBlending(*p);//OK
//3.使用自定义虚函数表
cout<<"testing class Blue~----------------------------------------------------------- "<<endl;
        Blue blue1,blue2;
        blue1.ColorBlending(*(new Green));
        blue1.ColorBlending(green1);
        blue1.ColorBlending(*(new Red));
        blue1.ColorBlending(*(new Blue));
        ColorBase *pb=&blue1;
        blue2.ColorBlending(*pb);
        (*pb).ColorBlending(blue2);
        blue2.ColorBlending(blue2);
    return 0;
}
