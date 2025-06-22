#ifndef ISOTOPE_H
#define ISOTOPE_H
using namespace std;
#include <string>
class isotope{
    public:
        int N;
        int Z;
        string name;
        isotope(int N,int Z,const string& name);
        int A();
};
#endif