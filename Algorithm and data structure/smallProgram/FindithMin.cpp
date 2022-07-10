// 从数组中找到第i小的元素。(找中位数!!!)
//一般思路：进行排序，找到第i个小的数
//1.可以用最小堆。2.随机选择算法
//随机选择算法需要集合内的元素各不相同
#include <iostream>
#include"vector"
using namespace std;
typedef vector<int>::iterator v_iter;

v_iter RandSelected(vector<int>& v,
                            v_iter first,v_iter last,int nth){
    if(nth<=0||last-first<nth) 
        return v.end();
    if(first==last) return first;
    
    int pivot=*first;
    v_iter left=first,right=last-1;//last-1是因为vector是左闭右开的
    while(left<right){
        while(left<right&&*right>=pivot) right--;
        while(left<right&&*left<=pivot) left++;
        swap(*left,*right);
    }
    swap(*right,*first);
    //[left,right]
    //现在right==left,*right=pivot,左边是小于pivot的数,右边是大于pivot的数
    //1.如果right-first==nth,说明现在指向的数正好是数组的第n小的数
    //  证明:[first,right]共有right-first+1个数,而*right是其中最大的数
    //       所以共有right-first个数小于*right,如果nth==right-first+1即right
    if(right - first +1== nth) {return right;}
    else if(right-first + 1 < nth){ 
        //right左边元素个数不足nth个,返回右半边
        return RandSelected(v,right+1,last,nth-(right-first+1));
    }
    else{
        //right左边的元素比nth大,返回左半边
        return RandSelected(v,first,right,nth);
    }
}

float getMedian(vector<int> &v){
    float mid;
    if(v.size()%2){//奇数
        mid = *RandSelected(v,v.begin(),v.end(),(int)(v.size()/2)+1);
    } 
    else{
        float num1,num2;
        num1 = (float)(*RandSelected(v,v.begin(),v.end(),(int)(v.size()/2)+1));
        num2 = (float)(*RandSelected(v,v.begin(),v.end(),(int)(v.size()/2)));
        mid = (num1+num2)/2;
    }
    return mid;
}


int main(){
    vector<int> v{5,9,3,55,88,11,22,1,33,44,66,77,99,100,101};
    for(int ix=1;ix<=v.size();ix++){
        cout<<"The"<<ix<<"-th min number is "<<*RandSelected(v,v.begin(),v.end(),ix)<<endl;
    }

    return 0;
    
}