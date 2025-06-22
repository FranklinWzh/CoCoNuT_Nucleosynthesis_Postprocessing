#ifndef DECAY_H
#define DECAY_H
#include "isotope.h"
#include <vector>
using namespace std;
using Matrix = vector<vector<double>>;
class DecayChain{
    public:
        isotope startIsotope;
        isotope endIsotope;
        DecayChain(const isotope& s,const isotope& e);
        void DecayData(Matrix& NucResult);
};
#endif