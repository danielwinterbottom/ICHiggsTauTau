import ROOT

inFileName = 'fake_factor_test/mt/2018/fakefactor_fits_mt_medium_2018.root'
#hist_name = 'jet_pt_med_0jet_pt_2_ff_wjets_fit'
hist_name = 'met_0jet_closure_wjets_fit'

inFile = ROOT.TFile.Open(inFileName,"READ")
inFile_Up = ROOT.TFile.Open(inFileName.replace(".root","Up.root"),"READ")
inFile_Down = ROOT.TFile.Open(inFileName.replace(".root","Down.root"),"READ")

inFile_hist = inFile.Get(hist_name)
inFile_Up_hist = inFile_Up.Get(hist_name) 
inFile_Down_hist = inFile_Down.Get(hist_name) 

x_min = -10
x_max = 10

x_min_cap = -2.5 
x_max_cap = 1.5

inFile_hist = ROOT.TF1(inFile_hist.GetName(),"((x<=%(x_min_cap)f)*("%vars()+str(inFile_hist.GetExpFormula('p')).replace("x",str(x_min_cap))+")) + ((x>%(x_min_cap)f && x<%(x_max_cap)f)*("%vars()+str(inFile_hist.GetExpFormula('p'))+")) + ((x>=%(x_max_cap)f)*("%vars()+str(inFile_hist.GetExpFormula('p')).replace("x",str(x_max_cap))+"))",x_min,x_max)

inFile_Up_hist = ROOT.TF1(inFile_Up_hist.GetName(),"((x<=%(x_min_cap)f)*("%vars()+str(inFile_Up_hist.GetExpFormula('p')).replace("x",str(x_min_cap))+")) + ((x>%(x_min_cap)f && x<%(x_max_cap)f)*("%vars()+str(inFile_Up_hist.GetExpFormula('p'))+")) + ((x>=%(x_max_cap)f)*("%vars()+str(inFile_Up_hist.GetExpFormula('p')).replace("x",str(x_max_cap))+"))",x_min,x_max)

inFile_Down_hist = ROOT.TF1(inFile_Down_hist.GetName(),"((x<=%(x_min_cap)f)*("%vars()+str(inFile_Down_hist.GetExpFormula('p')).replace("x",str(x_min_cap))+")) + ((x>%(x_min_cap)f && x<%(x_max_cap)f)*("%vars()+str(inFile_Down_hist.GetExpFormula('p'))+")) + ((x>=%(x_max_cap)f)*("%vars()+str(inFile_Down_hist.GetExpFormula('p')).replace("x",str(x_max_cap))+"))",x_min,x_max)


#inFile_Up_hist = ROOT.TF1(inFile_Up_hist.GetName(),"("+str(inFile_Up_hist.GetExpFormula('p'))+")",x_min,x_max)
#inFile_Down_hist = ROOT.TF1(inFile_Down_hist.GetName(),"("+str(inFile_Down_hist.GetExpFormula('p'))+")",x_min,x_max)


#inFile_Up_hist.Divide(inFile_hist.Clone())
#inFile_Down_hist.Divide(inFile_hist.Clone())
#inFile_hist.Divide(inFile_hist.Clone())

#inFile_hist_ratio = ROOT.TF1("inFile_ratio","("+str(inFile_hist.GetExpFormula('p'))+")/("+str(inFile_hist.GetExpFormula('p'))+")",-10,10)
#inFile_Up_hist_ratio = ROOT.TF1("inFile_ratio","("+str(inFile_Up_hist.GetExpFormula('p'))+")/("+str(inFile_hist.GetExpFormula('p'))+")",-10,10)
#inFile_Down_hist_ratio = ROOT.TF1("inFile_ratio","("+str(inFile_Down_hist.GetExpFormula('p'))+")/("+str(inFile_hist.GetExpFormula('p'))+")",-10,10)

c = ROOT.TCanvas('c','c',700,700)
#c.SetLogx()

inFile_hist.Draw()
inFile_hist.SetLineColor(1)
inFile_Up_hist.Draw("same")
inFile_Up_hist.SetLineColor(2)
inFile_Down_hist.Draw("same")
inFile_Down_hist.SetLineColor(3)

#inFile_hist.GetXaxis().SetMoreLogLabels()
#inFile_hist.GetXaxis().SetNoExponent()
#inFile_hist.SetMaximum(1.2)
#inFile_hist.SetMinimum(0.8)

inFile_hist.SetMaximum(2)
inFile_hist.SetMinimum(0)
inFile_hist.GetXaxis().SetTitle("c_{W}")
inFile_hist.GetYaxis().SetTitle("Correction")

l = ROOT.TLegend(0.55,0.75,0.87,0.88)
l.AddEntry(inFile_hist,'Nominal','l')
l.AddEntry(inFile_Up_hist,'Up','l')
l.AddEntry(inFile_Down_hist,'Down','l')

#inFile_hist_ratio.Draw()
#inFile_hist_ratio.SetLineColor(1)
#inFile_Up_hist_ratio.Draw("same")
#inFile_Up_hist_ratio.SetLineColor(2)
#inFile_Down_hist_ratio.Draw("same")
#inFile_Down_hist_ratio.SetLineColor(3)

#inFile_hist_ratio.GetXaxis().SetMoreLogLabels()
#inFile_hist_ratio.GetXaxis().SetNoExponent()
#inFile_hist_ratio.SetMaximum(1.2)
#inFile_hist_ratio.SetMinimum(0.8)

#l = ROOT.TLegend(0.55,0.75,0.87,0.88)
#l.AddEntry(inFile_hist_ratio,'Nominal','l')
#l.AddEntry(inFile_Up_hist_ratio,'Up','l')
#l.AddEntry(inFile_Down_hist_ratio,'Down','l')


l.Draw()
c.Print(hist_name+'_systs.pdf')
