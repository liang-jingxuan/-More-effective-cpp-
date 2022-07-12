#include <iostream>
#include"Myvector.cpp"
using namespace std;
int main(){
    mySTL::Myvector<int> v;
    v.push_back(4);
    cout<<v[0];
    return 1;
}