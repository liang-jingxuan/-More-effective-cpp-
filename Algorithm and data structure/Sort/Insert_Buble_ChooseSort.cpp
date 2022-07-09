#include <iostream>
#include "vector"
#include "CMP_h.h"
using namespace std;
typedef vector<int>::iterator v_iter;
template<class CmpMethod>
void InsertSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
    if(right-left<=1||right-left>v.size()) return;
    int times=right-left;//待排序元素数量
    for(int ix=1;ix<times;ix++){
        v_iter p=left+ix;
        while(p!=left&&cmp(*p,*(p-1))){
            swap(*p,*(p-1));
            p--;
        }
    }
}
template<class CmpMethod>
void BubleSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
    if(right-left<=1||right-left>v.size()) return;
    int times=right-left;//待排序元素数量
    for(int ix=0;ix<times;ix++){
        for(int iy=0;iy<times-ix;iy++){
            if(cmp(*(left+iy+1),*(left+iy)))
                swap(*(left+iy),*(left+iy+1));
        }
    }
}
template<class CmpMethod>
void ImproveBubleSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
    if(right-left<=1||right-left>v.size()) return;
    int times=right-left;//待排序元素数量
    bool Hasswap=true;
    for(int ix=0;ix<times&&Hasswap;ix++){
        Hasswap=false;//如果某一次循环发现没有元素进行交换,说明已经有序
        for(int iy=0;iy<times-ix-1;iy++){//需要-1,因为n个数要比较n-1次 
            if(cmp(*(left+iy+1),*(left+iy))){
                swap(*(left+iy),*(left+iy+1));
                Hasswap=true;
            }
        }
    }
    
}

template<class CmpMethod>
void ChooseSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
    if(right-left<=1||right-left>v.size()) return;
    int times=right-left;//待排序元素数量
    for(int ix=0;ix<times;ix++){
        v_iter p=left+ix;
        for(int iy=1;iy<times;iy++){
            if(cmp(*p,*(left+iy)))
                p=left+iy;
        }
        swap (*p,*(left+ix));
    }
}

template<class CmpMethod>
void ImprovedChooseSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
//改进算法思路:若待排序列已经有序,则停止
    if(right-left<=1||right-left>v.size()) return;
    int times=right-left;//待排序元素数量
    bool isSorted=false;
    for(int ix=0;ix<times-1&&!isSorted;ix++){
        //n个数只需要排列n-1次,因为经过n-1次排序,前n-1个数已经有序,第n个数无数可比
        v_iter p=left+ix+1;
        isSorted=true;//假设已经排序完
        for(int iy=ix;iy<times;iy++){
            if(cmp(*(left+iy),*p))
                p=left+iy;
            if(cmp(*(left+iy),*(left+iy-1))&&isSorted)//发现假设是错的,否定假设
                isSorted=false;//一旦发现后一个比前一个不满足cmp关系则不为有序
        }
        swap(*p,*(left+ix));
    }
}

main(){
    vector<int>v{1,56,79,15,6,4561,-6,-36,-11,9};
    InsertSort( v , v.begin(),v.end(),CmpLess<int>());
    cout<<"Insert sort:"<<endl;
    for (auto num:v){
        cout<<num<<',';
    }

    cout<<"\nInsert sort:"<<endl;
    vector<int>v2{1,56,79,15,6,4561,-6,-36,-11,9};
    BubleSort( v2 , v2.begin(),v2.end(),CmpLess<int>());
    for (auto num:v){
        cout<<num<<',';
    }

    vector<int>v3{99,89,1,2,3,4,5,6};
    cout<<"\nImproved insert sort:"<<endl;
    ImproveBubleSort( v3 , v3.begin(),v3.end(),CmpLess<int>());
    for (auto num:v3){
        cout<<num<<',';
    }

    cout<<"\nChoose sort:"<<endl;
    vector<int>v4{1,56,79,15,6,4561,-6,-36,-11,9};
    ChooseSort( v4 , v4.begin(),v4.end(),CmpLess<int>());
    for (auto num:v4){
        cout<<num<<',';
    }

    vector<int>v5{99,89,3,4,5,2,1};
    cout<<"\nImproved Choose sort:"<<endl;
    ImprovedChooseSort( v5 , v5.begin(),v5.end(),CmpLess<int>());
    for (auto num:v5){
        cout<<num<<',';
    }
}