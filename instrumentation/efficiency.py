import os
import numpy as np
import matplotlib.pyplot as plt
from acceptance import acceptance

# acceptance = 1.0

counts_path = os.path.join(os.path.dirname(__file__), 'counts')

single = np.loadtxt(os.path.join(counts_path, 'single_rate.txt'))
coinc = np.loadtxt(os.path.join(counts_path, 'coincidences.txt'))

width = 1.8e-5
time = 100

fake_double = single[:, 1] * single[:, 3] * width / (coinc[:, 2] * time)
fake_triple = single[:, 2] * coinc[:, 2] * width / (coinc[:, 1] * time)

e = (coinc[:, 1] / coinc[:, 2]) / acceptance
de = np.sqrt(coinc[:, 1] * (coinc[:, 2] - coinc[:, 1])
             / coinc[:, 2]**3) / acceptance

X = np.stack([single[:, 0], e, de, fake_double, fake_triple], axis=1)

np.savetxt(os.path.join(counts_path, '..', 'efficiencies',
           'working_efficiencies.txt'), X, fmt='%.3f')

plt.title('Working efficiencies')
plt.errorbar(e, np.arange(1, len(e)+1), xerr=de, ls='', fmt='.', color='black')
plt.grid(which='both', ls='--', alpha=0.5)
plt.xlabel(r'$\epsilon$')
plt.ylabel('PMT')
plt.savefig(os.path.join(counts_path, '..',
            'figures', 'working_efficiencies.png'))
plt.show()
