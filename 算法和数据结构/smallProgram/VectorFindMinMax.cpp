#include<iostream>
#include"vector"
using namespace std;
int main(){
    vector<int> v{1,94,91,0,489,0,45,-96,02,4,-4,5,4,-89,1,2,3,4,5,6,-5,-4,8,-7,-6,-10,1};
//1.传统找最大最小值方法
    int minNum=v[0],maxNum=v[0];
    int cnt=0;
    for(auto num:v){
        minNum=minNum>num?num:minNum;
        ++cnt;
        maxNum=maxNum<num?num:maxNum;
        ++cnt;
    }
    cout<<"Tradition finding max/min number algorithm's comparition times:"<<cnt<<endl;
    cout<<"The maximum number::"<<minNum<<"\nThe minimum number:"<<maxNum<<endl;
//2.改进找最大最小值方法
    cnt=0;
    int startnum=v.size()%2?1:2;
    //第一步:确定初始值,如果数组大小为奇数,则初始最大最小值都是第一个数;
    if(v.size()%2){//奇数
        minNum=v[0],maxNum=v[0];
    }else{
        if(v[0]<v[1]){
            minNum=v[0];
            maxNum=v[1];
        }else{
            minNum=v[1];
            maxNum=v[0];
        }
        ++cnt;
    }

    for(int ix=startnum;ix<v.size();ix+=2){
        int tmpmin,tmpmax;
        if(v[ix]<v[ix+1]){
            tmpmin=v[ix];
            tmpmax=v[ix+1];
        }else{
            tmpmin=v[ix+1];
            tmpmax=v[ix];
        }
        ++cnt;

        cout<<tmpmin<<','<<tmpmax<<endl;
        minNum=minNum>tmpmin?tmpmin:minNum;
        ++cnt;
        maxNum=maxNum<tmpmax?tmpmax:maxNum;
        ++cnt;
    }

    cout<<"Improved finding max/min number algorithm's comparition times:"<<cnt<<endl;
    cout<<"The maximum number::"<<minNum<<"\nThe minimum number:"<<maxNum<<endl;
    return 0;
}

