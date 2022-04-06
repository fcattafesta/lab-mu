import os
import numpy as np


bck = os.path.join(os.path.dirname(__file__),
                   'background', 'acquisitions', '0504_1700_empty.dat')

al1 = os.path.join(os.path.dirname(__file__),
                   'aluminum', 'acquisitions', '1703_1802.dat')

al2 = os.path.join(os.path.dirname(__file__),
                   'aluminum', 'acquisitions', '1703_1845.dat')

ir = os.path.join(os.path.dirname(__file__),
                  'iron', 'acquisitions', 'doublestop.dat')


def time_calculator(datapath):
    time = np.loadtxt(datapath, usecols=(1))

    total = 0
    for i in range(len(time)-1):
        dt = time[i+1] - time[i]
        if dt > 0:
            total += dt
    return total


w1 = (time_calculator(al1) + time_calculator(al2)) / time_calculator(bck)
w2 = time_calculator(ir) / time_calculator(bck)
print(w2)
