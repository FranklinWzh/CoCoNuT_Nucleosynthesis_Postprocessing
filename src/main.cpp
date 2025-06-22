#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "matrix.h"
#include <filesystem>
#include "globalvar.h"
#include "assign.h"
namespace fs = std::filesystem;
using namespace std;
using namespace GlobalVar;
using Matrix = std::vector<std::vector<double>>;
using NSnap = std::vector<Matrix>;
extern const int N = 17280;
namespace GlobalVar {
    std::vector<std::string> g_trajectories_folder;
    string g_model_folder;
    string g_model_prefix;
    std::array<float, 17280> g_massArray;
    double g_massfraction_threshold;
}
void sortByTime(std::vector<Matrix>& data) {
    std::sort(data.begin(), data.end(),
        [](const Matrix& a, const Matrix& b) {
            return a[0][0] < b[0][0];
        });
}
int main(int argc, char* argv[])
{
    string modelname = "";
    std::ios::sync_with_stdio(false);
    double g_massfraction_threshold = 1e-30;
    std::cin.tie(nullptr);

    if(argc<2)
    {
        cout<<"Model Name not assigned"<<endl;
        return 0;
    }
    for(int i=1;i<argc;i++)
    {
        modelname = argv[i];
        cout<<"Reading Model:"<<modelname<<endl;
    }
    string target_dir = "./models/"+modelname+"/";
    string prefix = "winnet_"+modelname+"_";
    string massAssign_file = "./massassign/"+modelname+"_mass_assign.txt";
    string errstat_file = "./massassign/"+modelname+"_errstat.txt";
    ReadMassAssign(massAssign_file);
    std::vector<std::string> extracted_names;
    try {
        for (const auto& entry : fs::directory_iterator(target_dir)) {
            if (entry.is_directory()) {
                std::string folder_name = entry.path().filename().string();

                if (folder_name.rfind(prefix, 0) == 0 && folder_name.length() > prefix.length()) {
                    std::string suffix = folder_name.substr(prefix.length());
                    extracted_names.push_back(suffix);
                    std::cout << "找到文件夹: " << folder_name << "，提取部分: " << suffix << std::endl;
                }
            }
        }

        std::cout << "\n共找到 " << extracted_names.size() << " 个以 \"" << prefix << "\" 开头的文件夹。\n";

    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "File System Error" << e.what() << std::endl;
    }
    std::vector<std::vector<double>> SynthesisResult;
    NSnap Evolution; // 401 timesteps of matrix like SynthesisResult
    g_trajectories_folder = extracted_names;
    g_model_folder = target_dir;
    g_model_prefix = prefix;
    int trajectory_nums = g_trajectories_folder.size();
    int err_count =0;
    bool first = true;
    ofstream ferr_stat(errstat_file);
    double err_total_mass = 0;
    for(int i=0;i<trajectory_nums;i++)
    {
        string traj_folder = target_dir+g_model_prefix+g_trajectories_folder[i];
        fs::path traj_folder_fs = traj_folder;
        fs::path traj_snap = traj_folder_fs / "snaps";
        bool empty = fs::directory_iterator(traj_snap) == fs::directory_iterator();
        cout<<i+1<<"/"<<trajectory_nums<<endl;
        if (empty) {
            std::cout <<"Empty Snaps" << std::endl;
            err_count++;
            cout<<traj_folder<<endl;
            int TracerID = stoi(g_trajectories_folder[i]);
            double mass;
            mass = g_massArray[TracerID-1];
            err_total_mass += mass;
            ferr_stat<<TracerID<<endl;
        }
        else
        {
            try 
            {
                NSnap snaps;
                double maxtime = 0.0;
                Matrix Finalsnap;
                int TracerID = stoi(g_trajectories_folder[i]);
                double mass;
                mass = g_massArray[TracerID-1];
                cout<<"Reading Trajectory:"<<TracerID<<" "<<"Mass:"<<mass<<endl;
                for (const auto& entry : fs::directory_iterator(traj_snap)) {
                    if (entry.is_regular_file()) {

                        Matrix a = loadtxt(entry.path(),3);
                        //cout<<"time:"<<a[0][0]<<endl;
                        if(a[0][0]>maxtime)
                        {
                            maxtime = a[0][0];
                            Finalsnap = a;
                        }
                        snaps.push_back(a);
                    }
                }
                sortByTime(snaps);
                if(first)
                {
                    first = false;
                    SynthesisResult.resize(Finalsnap.size()-1,vector<double>(3,0.0));
                    Evolution.resize(401,vector<vector<double>>(Finalsnap.size()-1,vector<double>(3,0.0)));
                    for(int i=1;i<Finalsnap.size();i++)
                    {
                        SynthesisResult[i-1][0] = Finalsnap[i][0];
                        SynthesisResult[i-1][1] = Finalsnap[i][1];
                        for(int j=0;j<Evolution.size();j++)
                        {
                            Evolution[j][i-1][0] = Finalsnap[i][0];
                            Evolution[j][i-1][1] = Finalsnap[i][1];
                        }
                        double fraction = Finalsnap[i][3]>g_massfraction_threshold?Finalsnap[i][3]:0;
                        SynthesisResult[i-1][2] = mass*fraction;
                    }
                }
                else
                {
                    for(int i=1;i<Finalsnap.size();i++)
                    {
                        double fraction = Finalsnap[i][3]>g_massfraction_threshold?Finalsnap[i][3]:0;
                        SynthesisResult[i-1][2] += mass*fraction;
                    }
                }
                //Dealing with snaps right now
                if(snaps.size()>0)
                {
                    if(snaps.size()>1)
                    {
                        double first_timestamp = snaps[0][0][0];
                        double last_timestamp = snaps[snaps.size()-1][0][0];
                        double t=0;
                        int timestep = 0;
                        int n = snaps.size()-1;
                        //snaps is a 3d tensor for every tracer, the first dimension is the number of the snap(ordered by time)
                        // second dimension decides the nuclei, notice that the first one(snaps[k][0]) is the timestamp of this snap[k], which has only one element(snaps[k][0][0]), a double-type timestamp
                        // third dimension is fixed for 0,1,2,3, 0 stands for the neturons, 1 stand for the protons, 2 stand for Y, 3 stand for X(except the first one as is mentioned above)
                        for(int i=0;i<Evolution.size();i++)
                        {
                            t = 0.005*i;
                            if(t<=first_timestamp)
                            {
                                for(int j=0;j<Evolution[i].size();j++)
                                {
                                    double fraction = snaps[0][j+1][3]>g_massfraction_threshold?snaps[0][j+1][3]:0;
                                    Evolution[i][j][2] += mass * fraction;
                                }
                            }
                            else
                            {
                                if(t>=last_timestamp)
                                {
                                    for(int j=0;j<Evolution[i].size();j++)
                                    {
                                        double fraction = snaps[n][j+1][3]>g_massfraction_threshold?snaps[n][j+1][3]:0;
                                        Evolution[i][j][2] += mass * fraction;
                                    }
                                }
                                else
                                {   //Interpolation
                                    for(int k=0;k<snaps.size()-1;k++)
                                    {
                                        double dt = t - snaps[k][0][0];
                                        if(t>=snaps[k][0][0]&&t<snaps[k+1][0][0])
                                        {
                                            for(int j=0;j<Evolution[i].size();j++)
                                            {
                                                double fraction_1 = snaps[k][j][3]>g_massfraction_threshold?snaps[k][j][3]:0;
                                                double fraction_2 = snaps[k+1][j][3]>g_massfraction_threshold?snaps[k+1][j][3]:0; 
                                                double dfdt = (fraction_2-fraction_1)/(snaps[k+1][0][0]-snaps[k][0][0]);
                                                Evolution[i][j][2] += mass * (fraction_1+dfdt*dt);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int i=0;i<Evolution.size();i++)
                        {
                            for(int j=0;j<Evolution[i].size();j++)
                            {
                                Evolution[i][j][2] += mass * snaps[0][j+1][3];
                            }
                        }
                    }
                }
            } 
            catch (fs::filesystem_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    std::string folder_path = "output/"+modelname+"/";

    if (fs::exists(folder_path)) {
        if (fs::is_directory(folder_path)) {
            std::cout << "文件夹已存在: " << folder_path << std::endl;
        } else {
            std::cout << "存在同名文件，但不是文件夹！" << std::endl;
        }
    } else {
        // 创建文件夹
        try {
            if (fs::create_directories(folder_path)) {
                std::cout << "成功创建文件夹: " << folder_path << std::endl;
            } else {
                std::cout << "无法创建文件夹。" << std::endl;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "错误: " << e.what() << std::endl;
        }
    }
    ofstream fout("output/"+modelname+"/"+modelname+"_result.txt");
    for(int i=0;i<SynthesisResult.size();i++)
    {
        fout<<int(SynthesisResult[i][0])<<" "<<int(SynthesisResult[i][1])<<" "<<SynthesisResult[i][2]<<endl;
    }
    fout.close();
    for(int i=0;i<Evolution.size();i++)
    {
        fout.open("output/"+modelname+"/"+modelname+"_snap"+std::to_string(i)+".txt");
        fout<<to_string(i)<<endl;
        for(int j=0;j<Evolution[i].size();j++)
        {
            fout<<int(Evolution[i][j][0])<<" "<<int(Evolution[i][j][1])<<" "<<Evolution[i][j][2]<<endl;
        }
        fout.close();
    }
    ferr_stat << err_count << "tracers have error. Total error mass:"<<err_total_mass<<"("<<err_total_mass/1.989e33<<")"<<endl;
}
