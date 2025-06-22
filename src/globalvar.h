#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <array>
using namespace std;
namespace GlobalVar {
    extern std::vector<std::string> g_trajectories_folder;
    extern string g_model_folder;
    extern string g_model_prefix;
    extern std::array<float, 17280> g_massArray;
    extern const int N;
    extern double g_massfraction_threshold;
}