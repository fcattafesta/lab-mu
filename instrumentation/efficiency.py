import os
import numpy as np

ACCEPTANCE = 0.03

path = os.path.join(os.path.dirname(__file__), 'counts')

id, triplets, doublets = np.loadtxt(os.path.join(path, 'coincidences.txt'),
                                    unpack=True)

w = triplets / doublets
dw = np.sqrt(triplets * (doublets - triplets) / doublets**3)

X = np.stack([id, w, dw], axis=1)
np.savetxt(os.path.join(path, '..', 'efficiencies',
           'working_efficiencies.txt'), X, fmt='%.3f')
