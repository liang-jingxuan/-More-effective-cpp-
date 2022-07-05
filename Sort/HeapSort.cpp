#include <iostream>
#include "vector"
using namespace std;
typename vector<int>::iterator v_iter;
//堆排序可以用来求出第N大的数,前N大的数。为了避免空间浪费，一般将原数组进行一定的修改；如果不对原数组进行修改
//  那么复制工作必然导致空间和速度上的下降
            //调用方法:
            //HeapSort A({...},CmpLess<int>); 
            //A.max();调出最大的值
            // A.Nthmax(6);//调出第6大的值
            //A.sort(a,b);//a到b范围内元素进行排列
template<class CmpMethod>//假设是小根堆
bool isHeap(vector<int> &v,CmpMethod method){
    for(int ix=0;ix<v.size();ix++){
        if(ix*2+1>=v.size()-1){//节点没有孩子
            break;
        }
        if(ix*2+1<=v.size()-1&&method(v[ix*2+1],v[ix])) return false;//发现左孩子小于父
        if(ix*2+2<=v.size()-1&&method(v[ix*2+2],v[ix])) return false;//发现左孩子小于父
    }
    return true;
}//判断是不是堆,isHeap(V,CmpLess)


template<class CmpMethod>
class HeapSort{
    public:
        HeapSort(vector<int> &v,CmpMethod ){
            sink(v,CmpMethod());
            
        }
    private:
        void sink(vector<int> &,CmpMethod );
        HeapSort(); 
};


template<class CmpMethod>//假设是小根堆->父节点小于左右孩子
void HeapSort<CmpMethod>::sink(vector<int> &v,CmpMethod method){
    int len = v.size();
    for (int ix=0;ix<len;ix++){
        //下标为i的元素,左右孩子下标分别为2i+1,和2i+2 比如0,1,2;2,5,6
        if(ix*2+2<=len-1){//有双孩子
            if(method(v[ix*2+1],v[ix])&&method(v[2*ix+2],v[ix])){//左右孩子都小于父节点
                method(v[ix*2+1],v[ix*2+2])?swap(v[ix],v[ix*2+1]):swap(v[ix],v[ix*2+2]);
            }
            else if(method(v[ix*2+1],v[ix])){//做孩子小于父节点,和左孩子换
                swap(v[ix],v[ix*2+1]);
            }
            else if(method(v[ix*2+2],v[ix])){//和右孩子换
                swap(v[ix],v[ix*2+2]);
            }
        }
        else if(ix*2+1<=len-1){//只有单孩子
            if(method(v[ix*2+1],v[ix]))//孩子大于或小于父节点的时候才交换
                swap(v[ix],v[2*ix+1]);
        }
        else{//没有孩子
            break;
        }
    }
}
template<typename T>
class CmpLess{
    public:
        CmpLess(){}
    public:
    bool operator()(T &num1,T &num2){
        return num1<num2;
    }
};

template<typename T>
class CmpGreater{
    public:
        CmpGreater(){}
    public:
    bool operator()(T &num1,T &num2){
        return num1>num2;
    }
};

main(){
    vector<int>v{1,56,79,15,6,4561,-6,-36,-11,9};
    HeapSort HeapV(v,CmpLess<int>());//这个语句将v编程了一个堆
    cout<<"HeapV is small heap?:"<<isHeap(v,CmpLess<int>())<<endl;
    cout<<"HeapV is great heap?:"<<isHeap(v,CmpLess<int>())<<endl;
    for (auto ix:v){
        cout<<ix<<endl;
    }
}