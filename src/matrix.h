#ifndef MARTIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
std::vector<std::vector<double>> loadtxt(const std::string& filename, int skip_rows = 0);
#endif