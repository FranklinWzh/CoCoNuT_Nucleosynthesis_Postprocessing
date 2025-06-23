#include "isotope.h"
#include "decay.h"
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;
using Matrix = vector<vector<double>>;
DecayChain::DecayChain(const isotope& s,const isotope& e): startIsotope(s), endIsotope(e) {}
void DecayChainInit(Matrix& result)
{
    isotope Ni56 = isotope(28,28,"Ni56");
    isotope Co56 = isotope(29,27,"Co56");
    isotope Fe56 = isotope(30,26,"Fe56");
    DecayChain DC1 = DecayChain(Ni56,Fe56);
    DecayChain DC2 = DecayChain(Ni56,Fe56);
    DC1.DecayData(result);
    DC2.DecayData(result);
    
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
                    cout<<record[0]<<" "<<startIsotope.N<<" ";
                    record2[2] += record[2];
                    record[2] = 0;
                    cout<<record[2]<<" "<<NucResult[i][2]<<endl;
                }
            }
        }
        
    }
}