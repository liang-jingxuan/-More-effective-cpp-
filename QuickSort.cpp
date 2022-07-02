#include<iostream>
#include"vector"
using namespace std;
typedef vector<int>::iterator v_iter;
void _QuickSort(vector<int>&,v_iter left,v_iter right);
void QuickSort(vector<int>&v,v_iter left,v_iter right){
    if(right-left<=1) return;
    _QuickSort(v,left,--right);
}

void _QuickSort(vector<int>&v,v_iter left,v_iter right){//加入vector是为了方便调试观察
    if(left>=right) return;

    int pivot = *(left + (right-left) / 2);
    v_iter tmp_left=left,tmp_right=right;
    while(tmp_left<tmp_right){
        while(tmp_left<tmp_right&&*tmp_right>pivot) tmp_right--;
        while(tmp_left<tmp_right&&*tmp_left<pivot) tmp_left++;
        if(tmp_left<tmp_right)  swap(*tmp_left,*tmp_right);
    }   

    _QuickSort(v,left,tmp_left-1);
    _QuickSort(v,tmp_left+1,right);
}

int main(){
    vector<int>v{1,56,79,15,6,4561,-6,-8,-11};
    QuickSort(v,v.begin(),v.end());
    for (auto ix:v){
        cout<<ix<<endl;
    }
    return 0;
}