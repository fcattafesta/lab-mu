import numpy as np
import os

path = os.path.join(os.path.dirname(__file__),
                    'efficiencies', 'working_efficiencies.txt')

rate = 2.126  # hz
err_rate = np.sqrt(2.126) / 10

s = 1927 * 1e-4  # m**2
err_s = 35 * 1e-4

acceptance = 0.3

_, eff, err_eff, _, _, _ = np.loadtxt(path, unpack=True)

tot_eff = np.prod(eff)
err_tot_eff = np.sqrt(np.sum((err_eff/eff)**2)) * tot_eff

phi = rate / (s * tot_eff * acceptance)
err_phi = np.sqrt((err_rate/rate)**2 + (err_s/s)
                  ** 2 + (err_tot_eff/tot_eff)**2) * phi


print(f'{phi} +- {err_phi}')
