#include "isotope.h"
#include <iostream>
using namespace std;
isotope::isotope(int N,int Z,const string& name){
    this->N = N;
    this->Z = Z;
    this->name = name;
}
int isotope::A()
{
    return N+Z;
}