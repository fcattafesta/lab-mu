import os
import numpy as np
import ROOT

T = 10

counts_path = os.path.join(os.path.dirname(__file__), 'stop_rate.txt')
data_path = fig_path = os.path.join(os.path.dirname(
    __file__), 'acquisitions', 'stop_rate.txt')

fig_path = os.path.join(os.path.dirname(__file__), 'figures', 'stop_rate.eps')
width, _, stop = np.loadtxt(counts_path, unpack=True)

dwidth = np.ones_like(width) * 0.1
stop = stop / T
dstop = np.sqrt(width)

X = np.stack([width, stop, dwidth, dstop], axis=1)
np.savetxt(data_path, X, fmt='%f')

c = ROOT.TCanvas('c')
c.SetGrid()
g = ROOT.TGraphErrors(data_path)
g.SetMarkerStyle(21)
g.SetTitle('Iron; Width [cm]; Rate [Hz]')
g.Draw("AP")
c.Update()
c.SaveAs(fig_path)
input()
