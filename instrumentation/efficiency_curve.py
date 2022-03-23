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

w = triplets/doublets
dw = np.sqrt(triplets * (doublets - triplets) / doublets**3)

X = np.stack([v, w, dv, dw], axis=1)

eff_name = ''.join(['eff', data_name])
fig_name = ''.join(['PMT', f'{args.detector_number}', '.png'])
eff_path = os.path.join(os.path.dirname(__file__), 'efficiencies', eff_name)
fig_path = os.path.join(os.path.dirname(__file__), 'figures', fig_name)
np.savetxt(eff_path, X, fmt='%.3f')

title = f'Scintillatore {args.detector_number};alimentazione [kV]; triple/doppie'
c = ROOT.TCanvas('c1')
c.SetGrid()
g = ROOT.TGraphErrors(eff_path)
g.SetMarkerStyle(8)
g.SetTitle(title)
ROOT.gPad.SetGrid(1, 1)
g.Draw("AP")
c.Update()
c.SaveAs(fig_path)
input()
