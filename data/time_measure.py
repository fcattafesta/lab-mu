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

mag1 = os.path.join(os.path.dirname(__file__),
                    'mag', 'acquisitions', '2203_1626_mag.dat')
mag2 = os.path.join(os.path.dirname(__file__),
                    'mag', 'acquisitions', '2203_1746_mag.dat')
mag3 = os.path.join(os.path.dirname(__file__),
                    'mag', 'acquisitions', '2903_1630_mag.dat')

as_i = os.path.join(os.path.dirname(__file__),
                    'nacl', 'acquisitions', '0604_1115_nacl.dat')


def time_calculator(datapath):
    time = np.loadtxt(datapath, usecols=(1))

    total = 0
    for i in range(len(time)-1):
        dt = time[i+1] - time[i]
        if dt > 0:
            total += dt
    return total


'''w1 = (time_calculator(al1) + time_calculator(al2)) / 3600
w2 = time_calculator(ir) / 3600
w3 = (time_calculator(mag1) + time_calculator(mag2) + time_calculator(mag3))/3600'''

w3 = time_calculator(as_i) / 3600
print(w3)
