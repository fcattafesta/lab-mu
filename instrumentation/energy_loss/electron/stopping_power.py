import os
import numpy as np
import matplotlib.pyplot as plt

dirpath = os.path.dirname(__file__)
figpath = os.path.join(dirpath, '..', '..', 'figures',
                       'electron_stop_power.png')

label = ['carbon', 'aluminum', 'iron', 'lead', 'scintillator']
density = np.array([2.267, 2.7, 7.874, 11.340, 0.9 * 0.997])

range = np.array([])

for name in label:
    energy, _, material_range = np.loadtxt(
        ''.join([os.path.join(dirpath, name), '.txt']), unpack=True)
    if range.size:
        range = np.column_stack((range, material_range))
    else:
        range = np.stack((energy, material_range), axis=1)
    del material_range, energy


for i, name in enumerate(label):
    plt.plot(range[:, 0], range[:, i+1] / density[i], label=name)

plt.legend()
plt.yscale('log')
plt.ylabel('Range [cm]')
plt.xlabel('Electron Energy [MeV]')
plt.title('Electron total range')
plt.minorticks_on()
plt.grid(which='both', ls='--', alpha=0.5)
plt.savefig(figpath)
plt.show()
