//计数排序适用于最大值和最小值相差不大的数组,因为计数排序会创建一个MAX-MIN大小的数组,
//  用空间换时间
//计数排序不需要进行大小的比较,直接放在数字所对应的下标位置
#include <iostream>
#include "vector"
#include "CMP_h.h"
using namespace std;
typedef vector<int>::iterator v_iter;

void CountSort(vector<int> &v,v_iter left,v_iter right){
    int MinNum=*left,MaxNum=*left;
    for(v_iter p=left;p!=right;p++){
        MinNum=*p<MinNum?*p:MinNum;
        MaxNum=*p>MaxNum?*p:MaxNum;
    }
    vector<int> CountVector(MaxNum-MinNum+1,0);//创建一个大小为MAX-MIN+1大数组,值都为0.+1是因为8~10共有10-8+1个数
    //数x对应的下标为:x-MinNum
    //下标i对应的数是MinNum+i
    for(v_iter p=left;p!=right;p++){
        ++CountVector[*p-MinNum];
    }
    v_iter p=left;
    for (int ix=0;ix<MaxNum-MinNum+1;ix++){
        for(int iy=0;iy<CountVector[ix];iy++){
            *p=MinNum+ix;
            p++;
        }
    }
}

main(){
    vector<int>v{1,56,79,15,6,61,-6,-36,-11,9,2,3,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
    CountSort( v , v.begin(),v.end());
    cout<<"Insert sort:"<<endl;
    for (auto num:v){
        cout<<num<<',';
    }
}