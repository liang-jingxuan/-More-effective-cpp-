#include <iostream>
#include "vector"
#include "CMP_h.h"
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
class Tranfer2Heap{
    //该类直接修改原数组,
    public:
        Tranfer2Heap(vector<int> &v,CmpMethod ){
            for(auto num:v) heap.push_back(num);
            if (heap.size()>1)
                swim(heap,CmpMethod());            
        }
        
        int max(){
            return heap[0];
        }

        int Nth(int Nth){//对于大根堆:第N大的数,对于小根堆:第N小的数
            if(Nth<=0||Nth>heap.size()){
                cout<<"Inval number: range should be within 0~"<<heap.size()<<endl;
                return -1;
            }
            Tranfer2Heap tmpHeap(heap,CmpMethod);
            for (int ix=0;ix<Nth;ix++)
                tmpHeap.RemoveRoot();
            return tmpHeap[0];
        }
    private:
        void sink(CmpMethod );
        void swim(CmpMethod );//该函数是有元素插入时用来调整堆,OK,
        Tranfer2Heap();
        vector<int> heap;
        void RemoveRoot();
};
template<class CmpMethod>
void Tranfer2Heap<CmpMethod>::RemoveRoot(){
    
}

template<class CmpMethod>//假设是小根堆->父节点小于左右孩子
void Tranfer2Heap<CmpMethod>::swim(CmpMethod method){
    int len = heap.size();
    for (int ix=1;ix<len;ix++){
        //下标为i的元素,父节点为i/2-1       
        //例如:3和4的父节点都是1 3/2=1,4/2-1=1 所以要判断奇偶,奇为左,偶为右
        int location=ix;
        while(location>0){
            if(1==location%2){//奇,是左孩子,只需要和父节点比较
                if(method(heap[location],heap[location/2]))
                    swap(heap[location],heap[location/2]);
                location/=2;
            }
            else{//偶,是右孩子,需要和父节点and兄弟节点比较
                if(method(heap[location],heap[location/2-1]))
                    swap(heap[location],heap[location/2-1]);
                location=location/2-1;
            }
        }

    }
}


main(){
    vector<int>v{1,56,79,15,6,4561,-6,-36,-11,9};//不变
    Tranfer2Heap Heap( v , CmpGreater<int>());//这个语句将v编程了一个堆
    cout<<"HeapV is small heap?:"<<isHeap(v,CmpLess<int>())<<endl;
    for (auto ix:v){
        cout<<ix<<endl;
    }
}