#include "matrix.h"
#include "globalvar.h"
using namespace std;
std::vector<std::vector<double>> loadtxt(const std::string& filename, int skip_rows) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> data;
    std::string line;
    int current_line = 0;

    if (!file.is_open()) {
        std::cerr << "Can't open: " << filename << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        if (current_line++ < skip_rows)
        {
            if(current_line == 2)
            {
                double time;
                std::vector<double> row;
                std::istringstream ss(line);
                ss>>time;
                row.push_back(time);
                data.push_back(row);
            }
            continue;
        }
        std::istringstream ss(line);
        std::vector<double> row;
        double value;

        while (ss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            data.push_back(row);
        }
    }

    return data;
}