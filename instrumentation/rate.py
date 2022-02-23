import argparse
import numpy as np
import os
import ROOT

parser = argparse.ArgumentParser()
parser.add_argument("detector_number")

args = parser.parse_args()

data_name = ''.join([f'{args.detector_number}', '.txt'])
data_path = os.path.join(os.path.dirname(__file__), 'counts', data_name)

v, triplets, doublets = np.loadtxt((data_path), unpack=True)

v = v * 1e-3
dv = np.ones_like(v) * 0.001

d_triple = np.sqrt(triplets)
d_double = np.sqrt(doublets)

w = triplets/doublets
dw = np.sqrt((d_triple/triplets)**2 + (d_double/doublets)**2)

X = np.stack([v, w, dv, dw], axis=1)

eff_name = ''.join(['eff', data_name])
eff_path = os.path.join(os.path.dirname(__file__), 'efficiencies', eff_name)

np.savetxt(eff_path, X, fmt='%.3f')

title = f'Efficienza scintillatore {args.detector_number}; alimentazione [kV]; #epsilon'
g = ROOT.TGraphErrors(eff_path)
g.SetMarkerStyle(8)
g.SetTitle(title)
g.Draw("AP")
input()
