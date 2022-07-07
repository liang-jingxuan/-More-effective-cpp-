#include <iostream>
#include "CMP_h.h"
#include"vector"
using namespace std;
typedef vector<int>::iterator v_iter;
//桶排序,每个桶代表一定范围的数,桶内进行排序,然后按顺序把桶内的元素取出
//桶的使用情况:负数都放在桶0, 0~10放桶1,11~20放桶2...100~110放桶10

void BucketSort( vector<int > &v, v_iter left ,v_iter right){
    //1.找到最大最小值,确定桶的个数
    int minNum=INT_MAX,maxNum=INT_MIN;
    for(v_iter p=left;p!=right;p++){
        minNum=minNum>*p?*p:minNum;
        maxNum=maxNum<*p?*p:maxNum;
    }
    int numBucket=(maxNum-minNum)/20;//每个桶的范围为20,如0~19,20~39
    if((maxNum-minNum)%20) ++numBucket;
    //2.把元素放入对应的桶
    vector<vector<int> > Buckets(numBucket,vector<int>{});
    for(v_iter p=left;p!=right;p++){
        for(int ix=0;ix<numBucket;ix++){
            if(*p<=minNum+20*(ix+1)-1&&minNum+20*ix<=*p)
                Buckets[ix].push_back(*p);
        }
        
    }
    //3.对每个桶进行排序
    for(int ix=0;ix<numBucket;ix++){
        bool Hasswap=true;
        int times=Buckets[ix].size();
        for(int i=0;i<times&&Hasswap;i++){//使用冒泡排序
            Hasswap=false;//如果某一次循环发现没有元素进行交换,说明已经有序
            for(int j=0 ; j < times-i-1 ; j++){//需要-1,因为n个数要比较n-1次 
                if (Buckets[ix][j]<Buckets[ix][j+1]){//把最小的放后面,方便pop_back
                    swap(Buckets[ix][j],Buckets[ix][j+1]);
                    Hasswap=true;
                }
            }
        }
    }
    //4.遍历每个桶
    vector<vector<int>>::iterator _iter=Buckets.begin();
    while(_iter!=Buckets.end()){    
        for(;!(*_iter).empty();){
            *left=(*_iter)[(*_iter).size()-1];
            (*_iter).pop_back();
            ++left;
        }
        ++_iter;
    }

}

main(){
    vector<int>v{1,56,79,15,6,61,-6,-36,-11,9,2,3,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
    BucketSort( v , v.begin(),v.end());
    cout<<"BucketSort sort:"<<endl;
    for (auto num:v){
        cout<<num<<',';
    }
}