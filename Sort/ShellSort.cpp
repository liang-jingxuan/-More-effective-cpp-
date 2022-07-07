#include <iostream>
#include "vector"
#include "CMP_h.h"
using namespace std;
typedef vector<int>::iterator v_iter;

template<class CmpMethod>
void ShellSort(vector<int> &v,v_iter left,v_iter right,CmpMethod cmp){
    //分组排序,间隔为len/2
    int len=right-left, gap=len/2;
    while(gap>1){
        int group=len/gap;
        if(len%gap) ++group;//不能整除
        for(int ix=0;ix<group;ix++){//总共有group组数,每组各自进行一次排序
            for(int iy=ix;iy<len;iy+=gap){
                v_iter p=left+iy;//指向最小的数
                for (int iz=iy+gap;iz<len;iz+=gap){//指向待排元素的下标
                    if(cmp(*(left+iz),*p))
                        p=left+iz;
                }
                swap(*(p),*(left+iy));
            }
        }
        gap/=2;
    }
    //间隔为1而且是相对有序的,可以使用改进的排序算法进行排序
    bool Hasswap=true;
    for(int ix=0;ix<len&&Hasswap;ix++){
        Hasswap=false;//如果某一次循环发现没有元素进行交换,说明已经有序
        for(int iy=0;iy<len-ix-1;iy++){//需要-1,因为n个数要比较n-1次 
            if(cmp(*(left+iy+1),*(left+iy))){
                swap(*(left+iy),*(left+iy+1));
                Hasswap=true;
            }
        }
    }

}
main(){
    vector<int>v{1,56,79,15,6,4561,-6,-36,-11,9,2,3,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
    ShellSort( v , v.begin(),v.end(),CmpLess<int>());
    cout<<"Insert sort:"<<endl;
    for (auto num:v){
        cout<<num<<',';
    }
}