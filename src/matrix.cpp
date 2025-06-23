#include "matrix.h"
#include "globalvar.h"
using namespace std;
bool tryParseDouble(const std::string& s, double& result) {
    std::istringstream ss(s);
    ss >> std::noskipws >> result;  // noskipws 防止吞掉空白后的非法字符
    return ss.eof() && !ss.fail();
}
std::vector<std::vector<double>> loadtxt(const std::string& filename, int skip_rows=0) {
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
        std::string token;
        std::vector<double> row;

        while (ss >> token) {
            double value;
            if (tryParseDouble(token, value)) {
                row.push_back(value);
            } 
            else {
                std::cerr << "跳过非法值: " << token << std::endl;
            }
        }
        if (!row.empty()) {
            data.push_back(row);
        }
    }

    return data;
}