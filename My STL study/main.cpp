#include <iostream>
#include"Myvector.h"
using namespace std;
int main(){
    Myvector<int> v;
    v.push_back(4);
    cout<<v[0];
    return 1;
}