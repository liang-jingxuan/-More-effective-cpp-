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
    //记法:保证tmp_right右边的数都>=pivot,保证tmp_left左边的数都<=pivot,tmp_left和tmp_right中间的数都是还未确定放在哪边的
    int pivot = *left;
    v_iter tmp_left=left,tmp_right=right;
    while(tmp_left!=tmp_right){
        while(tmp_left<tmp_right&&*tmp_right>=pivot) tmp_right--;
        while(tmp_left<tmp_right&&*tmp_left<=pivot) tmp_left++;
        //移动要右边先行的原因:从最后一次循环使得tmp_left==tmp_right开始分析,若先左走,那么tmp_left停在*tmp_left>pivot
        //  的位置,然后tmp_right前来重合,重合后此时tmp_left左边都小于pivot,tmp_right右边都大于pivot;
        //  开始交换*tmp_left和pivot位置上的数,但是*tmp_left>pivot,交换后就违反了"tmp_left左边都小于pivot"
        if(tmp_left<tmp_right)  swap(*tmp_left,*tmp_right);
    }   
    swap(*tmp_left,*left);
    _QuickSort(v,left,tmp_right-1);
    _QuickSort(v,tmp_right+1,right);
}

int main(){
    vector<int>v{1,56,79,15,6,4561,-6,-8,-11,-11};
    QuickSort(v,v.begin(),v.end());
    for (auto ix:v){
        cout<<ix<<endl;
    }
    return 0;
}