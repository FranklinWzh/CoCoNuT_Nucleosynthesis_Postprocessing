import numpy as np
import matplotlib.pyplot as plt
import os
import phycon_and_nuc_table as pc

modelname = "14m_old"
inner_ejecta = 2.4414134390894383e+32
progenitor = ""
folder_path = f"output/{modelname}/"
result_filename = f"{modelname}_decay.txt"
mergedf_fraction = np.zeros(41)
if os.path.exists(folder_path) and os.path.isdir(folder_path):
    print(f"文件夹 '{folder_path}' 存在，正在遍历文件...")

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
plt.savefig("figs/" + modelname + "_decay.png")
plt.savefig(folder_path + modelname + "_decay.png")
print(f"total:{np.sum(mergedf_fraction) / pc.pc_msol}")
plt.close()

pgfile = f"progenitors/{modelname}.data"
pg = np.loadtxt(pgfile,skiprows=6)
radius = pg[:,14]
dm = pg[:,10]
xnu = pg[:,70:92]
mnu = np.zeros_like(xnu[0,:])
i = 0
Rsun = 696340e5
while(radius[i]*Rsun>1e10):
    mnu += xnu[i,:] * dm[i]
    i+=1
mergedf_fraction[1] += mnu[1] #h1
mergedf_fraction[2] += mnu[3] #he3
mergedf_fraction[2] += mnu[4] #he4
mergedf_fraction[6] += mnu[5] #c12
mergedf_fraction[7] += mnu[6] #n14
mergedf_fraction[8] += mnu[7] #o16
mergedf_fraction[10] += mnu[8] #ne20
mergedf_fraction[12] += mnu[9] #mg24
mergedf_fraction[14] += mnu[10] #si28
mergedf_fraction[16] += mnu[11] #s32
mergedf_fraction[18] += mnu[12] #ar36
mergedf_fraction[20] += mnu[13] #ca40
mergedf_fraction[22] += mnu[14] #ti44
mergedf_fraction[24] += mnu[15] #cr48
mergedf_fraction[24] += mnu[16] #cr56
print(mergedf_fraction[26])
mergedf_fraction[26] += mnu[17] #fe52
print(mergedf_fraction[26])
mergedf_fraction[26] += mnu[18] #fe54
print(mergedf_fraction[26])
mergedf_fraction[26] += mnu[19] #fe56
print(mergedf_fraction[26])
mergedf_fraction[27] += mnu[20] #co56
mergedf_fraction[28] += mnu[21] #ni56

plt.plot(np.arange(0, 41), mergedf_fraction / pc.pc_msol)
for i in range(0, 41):
    plt.text(i, mergedf_fraction[i] * 1.05 / pc.pc_msol, s=pc.pc_solar_abundance[i][0])
plt.xlabel("Proton number")
plt.ylabel(r"Mass/$\mathrm{M}_{\odot}$")
plt.yscale("log")
plt.title(f"Total abundance(Mass {np.sum(mergedf_fraction) / pc.pc_msol})"+r"$M_{\odot}$")
plt.grid(True)
plt.tight_layout()
plt.savefig(folder_path + modelname + "_total.png")
plt.savefig("figs/" + modelname + "_total.png")
print(f"total:{np.sum(mergedf_fraction) / pc.pc_msol}")
plt.close()

total_mass = np.sum(mergedf_fraction[1:])
total_fraction = mergedf_fraction[1:] / total_mass


solar_abu = np.load("lo22_abu_noclass.npz",allow_pickle=True)
pr = total_fraction/solar_abu['abux'][:40]

plt.plot(np.arange(0, 40), pr)
for i in range(0, 40):
    plt.text(i, pr[i]*1.05, s=pc.pc_solar_abundance[i][0])
plt.xlabel("Proton number")
plt.ylabel("Production Rate")
plt.yscale("log")
plt.title(f"Production Rate(Mass {np.sum(mergedf_fraction) / pc.pc_msol})"+r"$M_{\odot}$")
plt.grid(True)
plt.tight_layout()
plt.savefig(folder_path + modelname + "_pr.png")
plt.savefig("figs/" + modelname + "_pr.png")
plt.close()