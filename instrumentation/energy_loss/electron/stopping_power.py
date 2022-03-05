import os
import numpy as np
import matplotlib.pyplot as plt

dirpath = os.path.dirname(__file__)
figpath = os.path.join(dirpath, '..', '..', 'figures',
                       'electron_stop_power.png')

label = ['carbon', 'aluminum', 'iron', 'lead']
density = np.array([2.267, 2.7, 7.874, 11.340])

stop_power = np.array([])

for name in label:
    energy, loss = np.loadtxt(
        ''.join([os.path.join(dirpath, name), '.txt']), unpack=True)
    if stop_power.size:
        stop_power = np.column_stack((stop_power, loss))
    else:
        stop_power = np.stack((energy, loss), axis=1)
    del loss, energy


for i, name in enumerate(label):
    plt.plot(stop_power[:, 0], stop_power[:, i+1] * density[i], label=name)

plt.legend()
plt.yscale('log')
plt.ylabel(r'$\frac{dE}{dx}$ [MeV/cm]')
plt.xlabel('Electron Energy [MeV]')
plt.title('Electron total stopping power')
plt.minorticks_on()
plt.grid(which='both', ls='--', alpha=0.5)
plt.savefig(figpath)
plt.show()
