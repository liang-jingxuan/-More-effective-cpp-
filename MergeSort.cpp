#include<iostream>
#include "vector"
using namespace std;
typedef vector<int>::iterator v_iter;
void MergeSort(vector<int> &v,v_iter left,v_iter right);
void _MergeSort_Split(vector<int> &v,v_iter left,v_iter right);
void _MergeSort_Merge(vector<int> &v,v_iter left,v_iter mid,v_iter right);

template<class CmpMethod>
void MergeSort(vector<int> &v,v_iter left,v_iter right,CmpMethod){
    if(v.size()<=1) return;//大小为1不用排序
    _MergeSort_Split(v,left,right-1,CmpMethod());//保证left~right是左闭右闭区间
    //如果采用左闭右开的方法,那么在Merge的时候可能mid+1正好是数组的末尾
}

template<class CmpMethod>
void _MergeSort_Split(vector<int> &v,v_iter left,v_iter right,CmpMethod){
    if(left>=right) return;//已经不能再分
    //还可以再分
    v_iter mid=left+(right-left)/2;
    _MergeSort_Split(v , left , mid  ,CmpMethod());
    _MergeSort_Split(v , mid+1, right,CmpMethod());
    _MergeSort_Merge(v , left , mid  ,right,CmpMethod());
}

template<class CmpMethod>
void _MergeSort_Merge(vector<int> &v,v_iter left,v_iter mid,v_iter right,CmpMethod cmpmethod){
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

class CmpSpecial{
    //各个位上的数相加*(正负)
    public:
    CmpSpecial(){}
    public:
    bool operator()(int num1,int num2){
        int tmpnum1=0,tmpnum2=0;
        bool num1symbol=num1>0?true:false,num2symbol=num2>0?true:false;
        num1*=num1symbol?1:-1;
        num2*=num2symbol?1:-1;
        while(num1!=0){
            tmpnum1+=num1%10;
            num1/=10;
        }
        tmpnum1*=num1symbol?1:-1;
        while(num2!=0){
            tmpnum2+=num2%10;
            num2/=10;
        }
        tmpnum2*=num2symbol?1:-1;
        return CmpLess<int>()(tmpnum1,tmpnum2);
    }
};
main(){
    vector<int>v{1,56,79,15,6,4561,-6,-6,-11,9};
    vector<int>v2{6,11,-6,-11};
    MergeSort(v ,v.begin()+6,v.end(),CmpLess<int>());//对数组v中范围内的数组进行排序
    for (auto ix:v){
        cout<<ix<<endl;
    }
    return 0;
}