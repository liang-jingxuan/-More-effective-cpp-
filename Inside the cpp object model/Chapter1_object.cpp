#include <iostream>
using namespace std;
class Point3D{
    public:
        Point3D(float x=0.0,float y=0.0,float z=0.0):_x(x),_y(y),_z(z){}
        float x()const{return _x;}
        float y()const{return _y;}
        float z()const{return _z;}

        void x(float xval){_x=xval;}
        void y(float yval){_y=yval;}
        void z(float zval){_z=zval;}
    private:
        float _x;
        float _y;
        float _z;
};
inline ostream& 
operator<<(ostream&os, const Point3D &pt){
    os   <<"(" << pt.x()
         << "," << pt.y() << "," << pt.z() <<")";
}

class Point {
    public:
        Point(float xval=0):_x(xval){++_point_count;}
        virtual ~Point(){--_point_count;}

        float x() const{return _x;}
        static int PointCount(){return _point_count;}

    protected:
        virtual ostream&
            print(ostream& os) const{}
        
        float _x;
        static int _point_count;
};
int Point::_point_count=0;

int main(){
    Point3D p(10,2.2,3);
    cout<<p<<endl;
    Point sc;
    cout<<"size of an empty class ="<<sizeof(sc)<<endl;//猜测:4(float)+8(64位系统指针大小)=12B,加上数据对其,
    cout<<"size of int ="<<sizeof(int)<<endl;//4
    cout<<"size of char ="<<sizeof(char)<<endl;//1
    cout<<"size of float ="<<sizeof(float)<<endl;//4
    cout<<"size of int*="<<sizeof(int*)<<endl;//8
    return 0;
}
