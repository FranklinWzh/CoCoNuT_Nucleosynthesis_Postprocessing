import numpy as np
import matplotlib.pyplot as plt
import os
import phycon_and_nuc_table as pc
import matplotlib.colors as mcolors

modelname = "14m_old"
inner_ejecta = 2.4414134390894383e+32
progenitor = ""
file_path = f"output/{modelname}/{modelname}_result.txt"

abux = np.loadtxt(file_path)

norm = mcolors.LogNorm(vmin=1, vmax=abux[:,2].max())

# 画散点图
sc = plt.scatter(
    abux[:,0], abux[:,1],
    c=abux[:,2], cmap='magma', norm=norm,
    s=100, edgecolors='k'
)

# 添加色条
cbar = plt.colorbar(sc)
cbar.set_label('Abundance')

# 设置轴标签和标题
plt.xlabel('Proton Number (Z)')
plt.ylabel('Neutron Number (N)')
plt.title('Nuclide Abundance Chart (Z vs N)')

plt.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.show()