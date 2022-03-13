import os
import numpy as np
import matplotlib.pyplot as plt

dirpath = os.path.dirname(__file__)
figpath = os.path.join(dirpath, '..', '..', 'figures',
                       'muon_stop_power.png')

label = ['carbon', 'aluminum', 'iron', 'lead']
density = np.array([2.267, 2.7, 7.874, 11.340])

range = np.array([])

for name in label:
    kin_energy, material_range = np.loadtxt(
        ''.join([os.path.join(dirpath, name), '.txt']), unpack=True,
        usecols=(0, 8))
    if range.size:
        range = np.column_stack((range, material_range))
    else:
        range = np.stack((kin_energy, material_range), axis=1)
    del material_range, kin_energy


for i, name in enumerate(label):
    plt.plot(range[:, 0], range[:, i+1] / density[i], label=name)

plt.legend()
plt.yscale('log')
plt.ylabel('Range [cm]')
plt.xlabel('Muon Kinetic Energy [MeV]')
plt.xscale('log')
plt.title('Muon total range')
plt.minorticks_on()
plt.grid(which='both', ls='--', alpha=0.5)
plt.savefig(figpath)
plt.show()
