#include "assign.h"
#include "globalvar.h"
using namespace GlobalVar;
const int GlobalVar::N=17280;
void ReadMassAssign(const std::string& filename)
{
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Can't open: " << filename << std::endl;
        return;
    }
    std::string line;
    size_t index = 0;
    while (std::getline(infile, line) && index < N) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string mass_str = line.substr(pos + 1);
            g_massArray[index] = std::stof(mass_str);
            cout<<index+1<<" "<<g_massArray[index]<<endl;
            ++index;
        }
    }
}