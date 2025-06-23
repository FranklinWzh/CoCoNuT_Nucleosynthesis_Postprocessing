#include "isotope.h"
#include "decay.h"
#include "matrix.h"
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;
using Matrix = vector<vector<double>>;
DecayChain::DecayChain(const isotope& s,const isotope& e): startIsotope(s), endIsotope(e) {}
void DecayChainInit(Matrix& result)
{
    auto m = loadtxt("decaychain.txt");
    vector<DecayChain> DecayChains;
    for(int i=0;i<m.size();i++)
    {
        isotope s = isotope(m[i][0],m[i][1]," ");
        isotope e = isotope(m[i][2],m[i][3]," ");
        DecayChains.push_back(DecayChain(s,e));
    }
    cout<<"All Decay Chains:"<<endl;
    for(int i=0;i<DecayChains.size();i++)
    {
        cout<<DecayChains[i].startIsotope.N<<" "<<DecayChains[i].startIsotope.Z<<"---->"<<DecayChains[i].endIsotope.N<<" "<<DecayChains[i].endIsotope.Z<<endl;
        DecayChains[i].DecayData(result);
    }
}
void DecayChain::DecayData(Matrix& NucResult)
{
    for(int i=0;i<size(NucResult);i++)
    {
        vector<double>& record = NucResult[i];
        if(record.size()<3)
        {
            continue;
        }
        if(fabs(record[0]-startIsotope.N)<0.1&&fabs(record[1]-startIsotope.Z)<0.1)
        {
            for(int j=0;j<size(NucResult);j++)
            {
                vector<double>& record2 = NucResult[j];
                if(record2.size()<3)
                {
                    continue;
                }
                if(fabs(record2[0]-endIsotope.N)<0.1&&fabs(record2[1]-endIsotope.Z)<0.1)
                {
                    record2[2] += record[2];
                    record[2] = 0;
                }
            }
        }
        
    }
}