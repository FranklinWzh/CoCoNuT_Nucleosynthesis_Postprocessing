import numpy as np
import matplotlib.pyplot as plt
import os
import phycon_and_nuc_table as pc

modelname = "14m_old"
inner_ejecta = 2.4414134390894383e+32
progenitor = ""
folder_path = f"output/{modelname}/"
result_filename = f"{modelname}_result.txt"
snap_filename = f"{modelname}_snap"
mergedf_fraction = np.zeros(41)
snaps_fraction = np.zeros((401,41))
if os.path.exists(folder_path) and os.path.isdir(folder_path):
    print(f"文件夹 '{folder_path}' 存在，正在遍历文件...")

    # 遍历文件夹中的所有文件
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                with open(file_path, 'r',errors='ignore') as f:

                    if file==result_filename:
                        f.close()
                        final_fraction = np.loadtxt(file_path)
                        for nuclei in final_fraction:
                            nuc_N = int(nuclei[0])
                            nuc_Z = int(nuclei[1])
                            if(nuc_Z>40):
                                continue
                            mergedf_fraction[nuc_Z] += nuclei[2]


                    if snap_filename in file:
                        timestep = int(f.readline().strip())
                        snap = np.loadtxt(file_path,skiprows=1)
                        for nuclei in snap:
                            nuc_N = int(nuclei[0])
                            nuc_Z = int(nuclei[1])
                            if(nuc_Z>40):
                                continue
                            snaps_fraction[timestep][nuc_Z] += nuclei[2]


            except Exception as e:
                print(f"无法读取文件 {file_path}：{e}")
else:
    print(f"文件夹 '{folder_path}' 不存在")

plt.plot(np.arange(0, 41), mergedf_fraction / pc.pc_msol)
for i in range(0, 41):
    plt.text(i, mergedf_fraction[i] * 1.05 / pc.pc_msol, s=pc.pc_solar_abundance[i][0])
plt.xlabel("Proton number")
plt.ylabel(r"Mass/$\mathrm{M}_{\odot}$")
plt.yscale("log")
plt.title(f"Nucleosynthesis result for innermost ejecta(Mass {np.sum(mergedf_fraction) / pc.pc_msol})"+r"$M_{\odot}$")
plt.grid(True)
plt.tight_layout()
plt.savefig(folder_path + modelname + "_synthesis.png")
plt.savefig("figs/" + modelname + "_synthesis.png")
print(f"total:{np.sum(mergedf_fraction) / pc.pc_msol}")
plt.close()


for i in range(0,41):
    plt.plot(np.linspace(0,2,401),snaps_fraction[:,i]/pc.pc_msol)
    plt.xlabel("Time/s")
    plt.ylabel(r"Mass/$\mathrm{M}_{\odot}$")
    plt.yscale("log")
    plt.title(f"Mass Evolution of {pc.pc_solar_abundance[i,0]}")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(folder_path+modelname+"snap_"+pc.pc_solar_abundance[i,0]+".png")
    plt.close()