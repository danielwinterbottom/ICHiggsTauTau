import ROOT

file2 = "vlq_dc_v6/2018/tt/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"
file1 = "vlq_dc_v8/2018/tt/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"

dir = "tt_Nbtag0"

f1 = ROOT.TFile(file1)
f2 = ROOT.TFile(file2)

d1 = f1.Get(dir)
d2 = f2.Get(dir)


keys = d1.GetListOfKeys()


for i in keys:
  k = i.ReadObj()
  j = d2.Get(k.GetName())
  if str(type(j)) != "<class 'ROOT.TObject'>":
    if float(k.Integral())!=float(j.Integral()):
      print k.GetName(),k.Integral(),j.Integral(),k.Integral()/j.Integral()
