import os
import numpy as np
import matplotlib.pyplot as plt



counts_path = os.path.join(os.path.dirname(__file__), 'iron')

w, start, stop = np.loadtxt('iron/stop_rate_ir.txt', unpack=True)



r = (stop[:] / start[:])
dr = np.sqrt(stop[:] * (start[:] - stop[:])/ start[:]**3)

'''
X = np.stack(w, r, dr], axis=1)

np.savetxt(os.path.join(counts_path, '..', 'efficiencies',
           'working_efficiencies.txt'), X, fmt='%.3f')
'''

plt.title('STOP/START rate')
plt.errorbar(w,  r, yerr=dr, ls='', fmt='.', color='black')
plt.grid(which='both', ls='--', alpha=0.5)
plt.xlabel('Width [cm]')
plt.ylabel('Stop/Start')
plt.savefig('iron/stop_rate.png')
plt.show()
