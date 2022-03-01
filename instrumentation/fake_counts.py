import numpy as np

single_data = np.zeros((6, 6))

pmt_try, n1, n2, n3, n4, n5, n6 = np.loadtxt('counts/single_rate.txt', unpack=True)

pmt, triple, doppie = np.loadtxt('counts/coincidences.txt', unpack=True)


single_data = np.zeros(36).reshape(6, 6)





for i in range (0, 6):
    single_data[i][0] = n1[i]
    single_data[i][1] = n2[i]
    single_data[i][2] = n3[i]
    single_data[i][3] = n4[i]
    single_data[i][4] = n5[i]
    single_data[i][5] = n6[i]


print(single_data)
#single_data = [n1.T, n2.T, n3.T, n4.T, n5.T, n6.T]
#print(single_data)

Time = 100 #s


n = triple[0]
N = doppie[0]
eff = n/N
d_eff = np.sqrt(n*(N-n)/(N**3))
print ('Efficienza_1 = %f +- %f' % (eff, d_eff))
a = single_data[0][0]
b = single_data[0][1]
c = single_data[0][2]

print ('Perc. doppie casuali (1) = %f' % (b*c*180*(10**-9)*100/(N*Time)) )
print ('Perc. triple casuali (1) = %f' % (a*N*180*(10**-9)*100/(n*Time)) )
print(' ')

for i in range(2, 6):
    n = triple[i-1]
    N = doppie[i-1]
    eff = n/N
    d_eff = np.sqrt(n*(N-n)/(N**3))
    print ('Efficienza_%d = %f +- %f' % (i, eff, d_eff))
    a = single_data[i-1][i-2]
    b = single_data[i-1][i-1]
    c = single_data[i-1][i]
    print ('Perc. doppie casuali (%d) = %f' % (i, a*c*180*(10**-9)*100/(N*Time)) )
    print ('Perc. triple casuali (%d) = %f' % (i, b*N*180*(10**-9)*100/(n*Time)) )
    print(' ')


n = triple[5]
N = doppie[5]
eff = n/N
d_eff = np.sqrt(n*(N-n)/(N**3))
print ('Efficienza_6 = %f +- %f' % (eff, d_eff))
a = single_data[5][3]
b = single_data[5][4]
c = single_data[5][5]
print ('Perc. doppie casuali (6) = %f' % (a*b*180*(10**-9)*100/(N*Time)) )
print ('Perc. triple casuali (6) = %f' % (c*N*180*(10**-9)*100/(n*Time)) )
print(' ')
