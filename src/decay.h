#ifndef DECAY_H
#define DECAY_H
#include "isotope.h"
#include <vector>
using namespace std;
class DecayChain{
    public:
        isotope startIsotope;
        isotope endIsotope;
        void GetDecayedData();
}
#endif