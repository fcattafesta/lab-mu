import os
import numpy as np
import matplotlib.pyplot as plt


counts_path = os.path.join(os.path.dirname(
    __file__), 'iron', 'stop_rate_ir.txt')

w, start, stop = np.loadtxt(counts_path, unpack=True)


r = (stop[:])
dr = np.sqrt(stop[:])

plt.title('STOP/START rate')
plt.errorbar(w,  r, yerr=dr, ls='', fmt='.', color='black')
plt.grid(which='both', ls='--', alpha=0.5)
plt.xlabel('Width [cm]')
plt.ylabel('Stop/Start')
#plt.savefig('iron/stop_rate.png')
plt.show()
