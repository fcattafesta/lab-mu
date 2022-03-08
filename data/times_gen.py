import os
import numpy as np
import matplotlib.pyplot as plt
import ROOT

path = os.path.dirname(__file__)

ch, t = np.loadtxt(os.path.join(path, 'iron',
                                '0803_1635.dat'), unpack=True)


times = np.zeros(len(t))
j = 0
st = 0

t = t * 1e6

for i in range(0, len(t)-1):
    if ch[i+1] > ch[i]:
        times[j] = t[i+1]-t[i]
        j += 1
    if ch[i] == 1:
        st += 1

if ch[len(t)-1] == 1:
    st += 1

times = times[times != 0]
print('Numero eventi di START =', st)
print('Numero eventi di STOP =', j)


plt.hist(times, bins=30, range=(0, 10))
plt.xlabel(r'$\Delta t$ [$\mu$s]')
plt.show()
