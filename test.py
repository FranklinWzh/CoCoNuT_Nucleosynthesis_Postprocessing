import numpy as np
data = np.load("lo22_abu_noclass.npz", allow_pickle=True)
out = list(zip(data['names'],data['abux']))