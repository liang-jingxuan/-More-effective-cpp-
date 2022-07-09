//在归并排序中对小数组采用插入排序
//比如分到左右数组都大小都是5的时候采用插入排序法,而不是一直分直到L和R数组大小都是1
#include<iostream>
#include "vector"
#include "CMP_h.h"
using namespace std;
typedef vector<int>::iterator v_iter;
void ImprovedMergeSort(vector<int> &v,v_iter left,v_iter right);
void _ImprovedMergeSort_Split(vector<int> &v,v_iter left,v_iter right);
void _ImprovedMergeSort_Merge(vector<int> &v,v_iter left,v_iter mid,v_iter right);

template<class CmpMethod>
void ImprovedMergeSort(vector<int> &v,v_iter left,v_iter right,CmpMethod){
    //调整前是左闭右开
    if(v.size()<=1) return;//大小为1不用排序,范围不对不排序

    if(right-left>v.size()||right-left<=1) return;//避免left和right是指向两个vector的指针
    //调整为左闭右闭区间
    _ImprovedMergeSort_Split(v,left,right-1,CmpMethod());//保证left~right是左闭右闭区间
    //如果采用左闭右开的方法,那么在Merge的时候可能mid+1正好是数组的末尾
}

template<class CmpMethod>
void _ImprovedMergeSort_Split(vector<int> &v,v_iter left,v_iter right,CmpMethod){
    if(right-left+1<=5){
        __InsertSort(left,right,CmpMethod());
        return;//已经不能再分
    } 
    //还可以再分
    v_iter mid=left+(right-left)/2;
    _ImprovedMergeSort_Split(v , left , mid  ,CmpMethod());
    _ImprovedMergeSort_Split(v , mid+1, right,CmpMethod());
    _ImprovedMergeSort_Merge(v , left , mid  ,right,CmpMethod());
}

template<class CmpMethod>
void _ImprovedMergeSort_Merge(vector<int> &v,v_iter left,v_iter mid,v_iter right,CmpMethod cmpmethod){
    vector<int> V_left(left,mid+1),V_right(mid+1,right+1);
    v_iter Vleft_iter=V_left.begin(),Vright_iter=V_right.begin();
    v_iter Vleft_enditer=V_left.end(),Vright_enditer=V_right.end();//空间换时间,避免了多次查询
    int len=right-left+1;
    for(int ix=0;ix<len;ix++){
        if(Vleft_iter!=Vleft_enditer&&Vright_iter!=Vright_enditer){
            if(cmpmethod(*Vleft_iter,*Vright_iter)){
                *(left+ix)=*Vleft_iter;
                Vleft_iter++;
            }
            else{
                *(left+ix)=*Vright_iter;
                Vright_iter++;
            }
        }//左右都还有元素
        else{
            if(Vleft_iter==Vleft_enditer){
                *(left+ix)=*Vright_iter;
                Vright_iter++;                
            }
            else{
                *(left+ix)=*Vleft_iter;
                Vleft_iter++;  
            }
        } //至少一个没有元素了
    }
}


template<class CmpMethod>
void __InsertSort(v_iter left,v_iter right,CmpMethod cmpmethod){
    int len=right-left+1;
    for(int ix=1;ix<len;ix++){
        v_iter tmp_iter=left+ix;
        while(tmp_iter!=left){
            if(cmpmethod(*tmp_iter,*(tmp_iter-1))){
                swap(*tmp_iter,*(tmp_iter-1));
            }
            --tmp_iter;
        }
    }
}

main(){
    vector<int>v{1,56,79,15,6,4561,-6,-36,-11,9};
    ImprovedMergeSort(v ,v.begin(),v.end(),CmpLess<int>());//对数组v中范围内的数组进行排序
    for (auto ix:v){
        cout<<ix<<endl;
    }
    return 0;
}