import ROOT
import numpy as np
import os
#from array import array


t1, as1 = np.loadtxt("mag/Asymmetry_mag.txt", unpack=True)
t2, as2 = np.loadtxt("iron/Asymmetry_iron.txt", unpack=True)

print(len(t1), len(t2))

as1 = np.abs(as1)
as2 = np.abs(as2)


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


D_N = np.max(D_vec)*np.sqrt((n1*n2)/(n1+n2))

limit = 1.36

print("D_N = ", D_N)
print("q_alpha = ", limit)

'''
##  Test di KS sul campione non magnetizzato

D_2 = np.zeros(l)

for i in range(0, l):
    C3 = 0
    for idx in range(0, i+1):
        C2 = C2 + as2[idx]/n2
        idx += 1
    C0 = 10*i/(10*len(t1))
    D_2[i] = np.abs(C1-C0)


limit = 1.36

print("D_N_2 = ", max(D_2))
print("q_alpha = ", limit)
'''
