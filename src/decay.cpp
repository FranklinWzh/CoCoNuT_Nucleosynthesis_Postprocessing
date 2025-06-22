#include "isotope.h"
#include "decay.h"
#include <vector>
#include <cmath>
using namespace std;
using Matrix = vector<vector<double>>;
DecayChain::DecayChain(const isotope& s,const isotope& e): startIsotope(s), endIsotope(e) {}

void DecayChain::DecayData(Matrix& NucResult)
{
    for(int i=0;i<size(NucResult);i++)
    {
        vector<double> record = NucResult[i];
        if(record.size()<3)
        {
            continue;
        }
        std::is_eq
    }
}