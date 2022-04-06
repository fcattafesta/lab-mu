import ROOT
import numpy as np
import os
#from array import array

dirpath = os.path.dirname(__file__)

t1, as1 = np.loadtxt(os.path.join(
    dirpath, 'mag', 'Asymmetry_mag.txt'), unpack=True)
t2, as2 = np.loadtxt(os.path.join(
    dirpath, 'iron', 'Asymmetry_iron.txt'), unpack=True)


l1 = len(t1)
l2 = len(t2)

print(l1, l2)


#as1 = np.abs(as1)
#as2 = np.abs(as2)


n1 = as1.sum()
n2 = as2.sum()

l = len(t1)


##  Test di Kolmogorov-Smirnov sui due campioni

D_vec = np.zeros(l)

for i in range(0, l):
    C1 = 0
    C2 = 0
    for idx in range(0, i+1):
        if (as1[idx] != 58):
            C1 = C1 + as1[idx]/n1
        if (as2[idx] != 58):
            C2 = C2 + as2[idx]/n2
        idx += 1
    D_vec[i] = np.abs(C1-C2)


D_N = np.max(D_vec)*np.sqrt((l1*l2)/(l1+l2))

limit = 1.36

print("D_N = ", D_N)
print("q_alpha = ", limit)


##  Test di KS sui singoli campioni, a confronto con una distribuzione uniforme

offset = 10000


D_1 = np.zeros(len(t2))
D_2 = np.zeros(len(t2))

for i in range(0, l):
    C1 = 0
    C2 = 0
    for idx in range(0, i+1):
        if (as1[idx] != 58):
            C1 = C1 + as1[idx]/n1
        if (as2[idx] != 58):
            C2 = C2 + as2[idx]/n2
        idx += 1
    C0 = offset*i/(offset*len(t1))
    D_1[i] = np.abs(C1-C0)
    D_2[i] = np.abs(C2-C0)


print("Critical region: Dn > ", 0.338)
print("D1 = ", max(D_1))
print("D2 = ", max(D_2))
