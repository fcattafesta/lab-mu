import numpy as np
import matplotlib.pyplot as plt

E1, stop1 = np.loadtxt('carbonio.txt', unpack=True)
E2, stop2 = np.loadtxt('alluminio.txt', unpack=True)
E3, stop3 = np.loadtxt('ferro.txt', unpack=True)
E4, stop4 = np.loadtxt('piombo.txt', unpack=True)

sample = np.ones(len(E1))



dens1 = 2.267 #g/cm**3
stop1 = stop1*dens1
m = np.mean(stop1)
mean1 = sample*m

dens2 = 2.7 #g/cm**3
stop2 = stop2*dens2
m = np.mean(stop2)
mean2 = sample*m

dens3 = 7.874 #g/cm**3
stop3 = stop3*dens3
m = np.mean(stop3)
mean3 = sample*m

dens4 = 11.340 #g/cm**3
stop4 = stop4*dens4
m = np.mean(stop4)
mean4 = sample*m

plt.figure(1)
plt.plot(E1, stop1, color='blue', label='Carbonio')
plt.plot(E2, stop2, color='red', label='Alluminio')
plt.plot(E3, stop3, color='green', label='Ferro')
plt.plot(E4, stop4, color='gray', label='Piombo')

plt.plot(E1, mean1, color='blue', linestyle='--')
plt.plot(E2, mean2, color='red', linestyle='--')
plt.plot(E3, mean3, color='green', linestyle='--')
plt.plot(E4, mean4, color='gray', linestyle='--')

plt.ylabel('Energy loss per cm [MeV/cm]')
plt.yscale('log')
plt.xlabel('Energy [MeV]')
plt.legend()
plt.grid()
plt.show()
