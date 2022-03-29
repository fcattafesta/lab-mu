import os
import numpy as np
import matplotlib.pyplot as plt
from acceptance import acceptance

# acceptance = 1.0


width = 1.8e-7
d_width = 1.5e-8
time = 100

counts_path = os.path.join(os.path.dirname(__file__), 'counts')

#Numero coincidenze doppie e triple
single = np.loadtxt(os.path.join(counts_path, 'single_rate.txt'))/time
coinc = np.loadtxt(os.path.join(counts_path, 'coincidences.txt'))/time


#Frequenza doppie casuali
fake_double = single[:, 1] * single[:, 3] * width
d_fd = (np.sqrt((1/single[:, 1]) + (1/single[:, 3]))
        + (d_width/width))*fake_double

#Frequenza triple casuali
fake_triple = single[:, 2] * coinc[:, 2] * width
d_ft = (np.sqrt((1/single[:, 2]) + (1/coinc[:, 2]))
        + (d_width/width))*fake_triple


e = (coinc[:, 1] / coinc[:, 2]) / acceptance
de = np.sqrt(coinc[:, 1] * (coinc[:, 2] - coinc[:, 1])
             / coinc[:, 2]**3) / acceptance

X = np.stack([single[:, 0]*time, e, de, acceptance], axis=1)
Y = np.stack([single[:, 0]*time, fake_double, d_fd,
             fake_triple, d_ft], axis=1)

np.savetxt(os.path.join(counts_path, '..', 'efficiencies',
           'working_efficiencies.txt'), X, fmt='%.3f')

np.savetxt(os.path.join(counts_path, '..', 'efficiencies',
           'random_coincidences.txt'), Y)

plt.title('Working efficiencies')
plt.errorbar(e, np.arange(1, len(e)+1), xerr=de, ls='', fmt='.', color='black')
plt.grid(which='both', ls='--', alpha=0.5)
plt.xlabel(r'$\epsilon$')
plt.ylabel('PMT')
plt.savefig(os.path.join(counts_path, '..',
            'figures', 'working_efficiencies.png'))
plt.show()
