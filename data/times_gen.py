import numpy as np
import ROOT

ch, t = np.loadtxt('labIF/lab-SCE/additional_dir/prova2.dat', unpack=True)

times = np.zeros(len(t))
j=0
st=0


for i in range(0, len(t)-1):
    if ch[i+1] != ch[i]:
        times[j]= t[i+1]-t[i]
        j+=1
    if ch[i] == 1:
        st+=1

if ch[len(t)-1] == 1:
    st+=1


print('Numero eventi di START =', st)
print('Numero eventi di STOP =', j)

histo = ROOT.TH1F("h", "Decay times", 0, 1e-5, 100)

for k in range (0, len(t)-1):
    histo.Fill(times[k])


#func = ROOT.TF1("double_expo", "double_expo", [0]*((1/[1])*TMath::Exp(-x/[2]) + [3]*TMath::Exp(-x/[4])))
#histo.Fit(func)


histo.Draw()
