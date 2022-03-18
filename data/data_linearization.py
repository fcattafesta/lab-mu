import numpy as np
import pylab


channel, t = np.loadtxt('iron/acquisitions/doublestop.dat',unpack=True)

counter=0
j=0

for i in range (0, len(t)-1):
    dt = t[i+1] - t[i]
    if dt < 0:
        counter += 1

print('counter = %d' % counter)

idx_change = pylab.array([0]*counter)
                            #array che raccoglie gli indici DOPO I QUALI si verifica un ripple

for i in range (0, len(t)-1):
    dt = t[i+1] - t[i]
    if dt < 0:
        idx_change[j] = i
        j += 1


for k in range (0, counter):
    idx = idx_change[k]
    scarto = t[idx] - t[idx+1]
    for i in range (idx+1, len(t)):
        t[i] += scarto



counter2 = 0

for i in range (0, len(t)-1):
    dt = t[i+1] - t[i]
    if dt < 0:
        counter2 += 1
print('counter 2 = %d' % (counter2))


print(np.max(t))
