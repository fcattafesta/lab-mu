from array import array
import numpy as np
import ROOT


def make_tree(filepath):
    datpath = ''.join([filepath, '.dat'])
    treepath = ''.join([filepath, '.root'])
    ch, t = np.genfromtxt(datpath, unpack=True, dtype=None)

    f = ROOT.TFile.Open(treepath, "RECREATE")
    tree = ROOT.TTree("events", "events")
    var1 = array('i', [0])
    var2 = array('d', [0])
    tree.Branch("channel", var1, "channel/I")
    tree.Branch("times", var2, "times/D")
    for elm1, elm2 in zip(ch, t):
        var1[0] = elm1
        var2[0] = elm2
        tree.Fill()

    tree.Write()
    f.Close()
