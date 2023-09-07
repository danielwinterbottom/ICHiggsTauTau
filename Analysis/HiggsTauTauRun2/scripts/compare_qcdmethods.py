import ROOT as R
from UserCode.ICHiggsTauTau.plotting import *
import numpy as np

PLOTBYBIN = True
DOCHI2 = False #Chi2 comparison of QCD fits, produces a text file in the outputfolder named "chi2test.txt"
infiles_dirname = "output_2018/"
output_folder_name = "output_2018qcdcompare/"

def MyCompareHists(hists=[],
             legend_titles=[],
             title="",
             ratio=True,
             log_y=False,
             log_x=False,
             ratio_range="0.7,1.3",
             custom_x_range=False,
             x_axis_max=4000,
             x_axis_min=0,
             custom_y_range=False,
             y_axis_max=4000,
             y_axis_min=0,
             x_title="",
             y_title="",
             extra_pad=0,
             norm_hists=False,
             plot_name="plot",
             label="",
             norm_bins=True,
             uncert_hist=None,
             uncert_title='',
             ReweightPlot=False,
             output_file = None,
             outputfolder = None):
   
    objects=[]
    R.gROOT.SetBatch(R.kTRUE)
    R.TH1.AddDirectory(False)
    ModTDRStyle(r=0.04, l=0.14)

    colourlist=[R.kBlue,R.kRed,R.kGreen+3,R.kBlack,R.kYellow+2,R.kOrange,R.kCyan+3,R.kMagenta+2,R.kViolet-5,R.kGray]
    if ReweightPlot:
      colourlist=[R.kBlack,R.kBlue,R.kRed,R.kGreen+3,R.kYellow+2,R.kOrange,R.kCyan+3,R.kMagenta+2,R.kViolet-5,R.kGray]

    hs = R.THStack("hs","")
    hist_count=0
    legend_hists=[]
    if isinstance(uncert_hist, (list,)):
     for i in uncert_hist:
       if i is None: continue 
       if norm_bins and i is not None: i.Scale(1.0,"width")
    else:
      if norm_bins and uncert_hist is not None: uncert_hist.Scale(1.0,"width")

    for hist in hists:
        # print hist.GetName()
        # for bin_ in range(1,hist.GetNbinsX()):
        #     print hist.GetBinContent(bin_)
        #     print np.sqrt(hist.GetBinContent(bin_))
        if norm_hists: hist.Scale(1.0/hist.Integral(0, hist.GetNbinsX()+1))
        if norm_bins: hist.Scale(1.0,"width")
        h = hist.Clone()
        objects.append(h)
        h.SetFillColor(0)
        h.SetLineWidth(3)
        h.SetLineColor(colourlist[hist_count])
        h.SetMarkerColor(colourlist[hist_count])
        h.SetMarkerSize(0)
        hs.Add(h)
        hist_count+=1
        o=h.Clone()
        objects.append(o)
        legend_hists.append(o)
   # hs.Draw("nostack")
        
    c1 = R.TCanvas()
    c1.cd()
    
    if ratio:
        if ReweightPlot: pads=TwoPadSplit(0.39,0.01,0.01)
        else: pads=TwoPadSplit(0.29,0.01,0.01)
    else:
        pads=OnePad()
    pads[0].cd()
    
    if(log_y): pads[0].SetLogy(1)
    if(log_x): pads[0].SetLogx(1)
    if custom_x_range:
        if x_axis_max > hists[0].GetXaxis().GetXmax(): x_axis_max = hists[0].GetXaxis().GetXmax()
    if ratio:
        if(log_x): pads[1].SetLogx(1)
        axish = createAxisHists(2,hists[0],hists[0].GetXaxis().GetXmin(),hists[0].GetXaxis().GetXmax()-0.01)
        axish[1].GetXaxis().SetTitle(x_title)
        axish[1].GetXaxis().SetLabelSize(0.03)
        axish[1].GetYaxis().SetNdivisions(4)
        axish[1].GetYaxis().SetTitle("Ratio")
        #if ReweightPlot:
        #  axish[1].GetYaxis().SetTitle("Correction")
        axish[1].GetYaxis().SetTitleOffset(1.6)
        axish[1].GetYaxis().SetTitleSize(0.04)
        axish[1].GetYaxis().SetLabelSize(0.03)
    
        axish[0].GetXaxis().SetTitleSize(0)
        axish[0].GetXaxis().SetLabelSize(0)
        if custom_x_range:
          axish[0].GetXaxis().SetRangeUser(x_axis_min,x_axis_max-0.01)
          axish[1].GetXaxis().SetRangeUser(x_axis_min,x_axis_max-0.01)
        if custom_y_range:
          axish[0].GetYaxis().SetRangeUser(y_axis_min,y_axis_max)
          axish[1].GetYaxis().SetRangeUser(y_axis_min,y_axis_max)
    else:
        axish = createAxisHists(1,hists[0],hists[0].GetXaxis().GetXmin(),hists[0].GetXaxis().GetXmax()-0.005)
        axish[0].GetXaxis().SetLabelSize(0.03)
        axish[0].GetXaxis().SetTitle(x_title)
        axish[0].GetXaxis().SetTitleSize(0.04)
        if custom_x_range:
          axish[0].GetXaxis().SetRangeUser(x_axis_min,x_axis_max-0.01)
        if custom_y_range:                                                                
          axish[0].GetYaxis().SetRangeUser(y_axis_min,y_axis_max)
    axish[0].GetYaxis().SetTitle(y_title)
    axish[0].GetYaxis().SetTitleOffset(1.6)
    axish[0].GetYaxis().SetTitleSize(0.04)
    axish[0].GetYaxis().SetLabelSize(0.03)

    hs.Draw("nostack same")

    uncert_hs = R.THStack()
    if uncert_hist is not None:
      if isinstance(uncert_hist, (list,)):
         #col_list = [12,6,4,2,3,4]
         col_list=colourlist
         count = 0
         for i in uncert_hist:
           if i is not None:
             i.SetFillColor(CreateTransparentColor(col_list[count],0.4))
             i.SetLineColor(CreateTransparentColor(col_list[count],0.4))
             i.SetMarkerSize(0)
             i.SetMarkerColor(CreateTransparentColor(col_list[count],0.4))
             i.SetFillStyle(1111)
             uncert_hs.Add(i)
           count+=1
         uncert_hs.Draw("nostack e2same")  
      else: 
        uncert_hist.SetFillColor(CreateTransparentColor(12,0.4))
        uncert_hist.SetLineColor(CreateTransparentColor(12,0.4))
        uncert_hist.SetMarkerSize(0)
        uncert_hist.SetMarkerColor(CreateTransparentColor(12,0.4))
        uncert_hist.SetFillStyle(1111)
        uncert_hs.Add(uncert_hist)
        uncert_hs.Draw("e2same")

      uncert_hs.Draw("nostack e2same")  
    if not custom_y_range:
        if(log_y):
            if hs.GetMinimum("nostack") >0: axish[0].SetMinimum(hs.GetMinimum("nostack"))
            else: axish[0].SetMinimum(0.0009)
            axish[0].SetMaximum(10**((1+extra_pad)*(math.log10(1.1*hs.GetMaximum("nostack") - math.log10(axish[0].GetMinimum())))))
        else:
            maxi=1.1*(1+extra_pad)*max(hs.GetMaximum("nostack"),uncert_hs.GetMaximum("nostack"))
            if not ReweightPlot: axish[0].SetMinimum(0)
            else:
              mini = None
              maxi = None
              for h in hists+uncert_hist:
                if h is None: continue
                for i in range(1,h.GetNbinsX()+1): 
                  lo = h.GetBinContent(i)-h.GetBinError(i) 
                  hi = h.GetBinContent(i)+h.GetBinError(i) 
                  if mini is None:
                    mini = min(lo,hi) 
                    maxi = max(lo,hi) 
                  else:
                    mini = min(mini,lo,hi) 
                    maxi = max(maxi,lo,hi) 
              #mini = min(hs.GetMinimum("nostack"),uncert_hs.GetMinimum("nostack"))
              mini-= abs(mini)*extra_pad
              axish[0].SetMinimum(mini)
              maxi*=(1.+extra_pad)
            axish[0].SetMaximum(maxi)
    axish[0].Draw()
    uncert_hs.Draw("nostack e2same")  

    hs.Draw("nostack hist same")
    axish[0].Draw("axissame")
    
    
    #Setup legend
    tot = len(hists)
    if isinstance(uncert_hist,list): tot+=len(uncert_hist)
    legend = PositionedLegend(0.3,0.1,2,0.04)
    legend.SetTextFont(42)
    legend.SetTextSize(0.035)
    legend.SetFillColor(0)
    

    for legi,hist in enumerate(legend_hists):
        legend.AddEntry(hist,legend_titles[legi],"l")
    if isinstance(uncert_hist, (list,)):
     count=0
     for i in uncert_hist:
       if i is not None: legend.AddEntry(i,uncert_title[count],'f') 
       count+=1
    else:
      if uncert_hist is not None and uncert_title: legend.AddEntry(uncert_hist,uncert_title,'f')
    legend.Draw("same")
    
    #CMS label and title
    FixTopRange(pads[0], axish[0].GetMaximum(), extra_pad if extra_pad>0 else 0.30)
    DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 1.0)
    #DrawCMSLogo(pads[0], 'CMS', 'Simulation', 11, 0.045, 0.05, 1.0, '', 1.0)
    DrawTitle(pads[0], title, 3)
    
    latex2 = R.TLatex()
    latex2.SetNDC()
    latex2.SetTextAngle(0)
    latex2.SetTextColor(R.kBlack)
    latex2.SetTextSize(0.028)
    latex2.DrawLatex(0.145,0.955,label)
    
    #Add ratio plot if required
    if ratio:
        ratio_hs = R.THStack("ratio_hs","")
        hist_count=0
        pads[1].cd()
        pads[1].SetGrid(0,1)
        axish[1].Draw("axis")
        axish[1].SetMinimum(float(ratio_range.split(',')[0]))
        axish[1].SetMaximum(float(ratio_range.split(',')[1]))
        div_hist = hists[0].Clone()
        objects.append(div_hist)
        
        for i in range(0,div_hist.GetNbinsX()+2): div_hist.SetBinError(i,0)
        first_hist=True
        for hist in hists:
            h = hist.Clone()
            objects.append(h)

            h.SetFillColor(0)
            h.SetLineWidth(3)
            h.SetLineColor(colourlist[hist_count])
            h.SetMarkerColor(colourlist[hist_count])
            h.SetMarkerSize(0)

            h.Divide(div_hist)
            if first_hist:
               for i in range(1,h.GetNbinsX()+1): h.SetBinError(i,0.00001)
               first_hist=False
            o = h.Clone()
            objects.append(o)
            ratio_hs.Add(o)
            hist_count+=1
        if uncert_hist is not None:
           if isinstance(uncert_hist, (list,)):
             ratio_err_hs = R.THStack("ratio_err_hs","")
             count=0
             for i in uncert_hist:
               if i is not None:
                 h = i.Clone()
                 objects.append(h)
                 h.Divide(div_hist)
                 ratio_err_hs.Add(h)
                 h.Draw("e2same")
               count+=1
             #ratio_err_hs.Draw("nostack e2same")
           else:
             h = uncert_hist.Clone()
             objects.append(h)
             h.Divide(div_hist)
             h.Draw("e2same") 
        ratio_hs.Draw("nostack e same")  
        pads[1].RedrawAxis("G")
    pads[0].cd()
    pads[0].GetFrame().Draw()
    pads[0].RedrawAxis()
    
    
    legendstats = PositionedLegend(0.3,0.25,3,0.04)
    legendstats.SetTextFont(42)
    legendstats.SetTextSize(0.044)
    legendstats.SetFillColor(0)
    legendstats.SetHeader("         Integrals")
    

    for legi,hist in enumerate(legend_hists):
        legendstats.AddEntry(hist,"%i (%.1f%%)"%(hist.Integral(),
                            (hist.Integral()/legend_hists[0].Integral())*100),"l")
    if isinstance(uncert_hist, (list,)):
     count=0
     for i in uncert_hist:
       if i is not None: legendstats.AddEntry(i,uncert_title[count],'f') 
       count+=1
    else:
      if uncert_hist is not None and uncert_title: legendstats.AddEntry(uncert_hist,uncert_title,'f')
    legendstats.Draw("same")
    
    c1.SaveAs(outputfolder+plot_name+'.png')
    c1.SaveAs(outputfolder+plot_name+'.pdf')
    if output_file is not None:
        output_file.WriteObject(c1, plot_name)
    for o in objects:
        o.IsA().Destructor(o)
    c1.Close()

variables = [
  
  ["n_jets","N_{jets}"],
  ["pt_1","p_{T}^{#tau_{1}}"],
  ["pt_2", "p_{T}^{#tau_{2}}"],
  ["mt_1", "m_{T}(p_{T}^{#tau_{1}},p_{T}^{miss})"],
  ["mt_2","m_{T}(p_{T}^{#tau_{2}},p_{T}^{miss})"],
  ["m_vis","m_{#tau#tau}"],
  ["pt_tt", "p_{T}^{#tau#tau}"]

]

selnames = [
            "medium_vloose_medium_vloose",
            "medium_vloose_medium",
            # "medium_vvvloose_medium",
            # "vloose_vvvloose_medium",
            "medium_vvvloose_medium_vvvloose"
            # "vloose_vvvloose_vloose_vvvloose"
            ]

types=["SS","OppS"]

version = "v2p5"

if DOCHI2: chi2file = open(output_folder_name+'chi2test.txt','w')

selnames_array = np.expand_dims(np.array(selnames,dtype = object),axis=1)

for var in variables:
  if DOCHI2:
    chi2file.write("----------- %s ------------ \n"% var[0])
    ChiList = []
    ChiList_SS = []
  for sel in selnames:
    for chargetype in types:
#--------------------------------------------------------------------
      filename = "datacard_"+var[0]+"_inclusive_"+sel+"_trg_doubletau_"+version+"_tt_2018"
      if chargetype == "SS":
        file= R.TFile.Open(infiles_dirname+filename+"_ss.root", "READ")
      else:
        file= R.TFile.Open(infiles_dirname+filename+".root", "READ")
      file_dir = file.Get("tt_inclusive")
      (file_dir).cd()
      if PLOTBYBIN:
        hist_w_qcdsub_bybin = file_dir.Get("QCD_bybin")
        hist_wo_qcdsub_bybin = file_dir.Get("QCD_bybin_nosub")
        sub_hist_w_qcdsub_bybin = file_dir.Get("data_obs").Clone()
        tot_bkg_hist_w = file_dir.Get("total_bkg")
        sub_hist_w_qcdsub_bybin.Add(tot_bkg_hist_w,-1)
        sub_hist_w_qcdsub_bybin.Add(hist_w_qcdsub_bybin)
        
      hist_w_qcdsub = file_dir.Get("QCD")
      hist_wo_qcdsub = file_dir.Get("QCD_nosub")
      
      sub_hist_w_qcdsub = file_dir.Get("data_obs").Clone()
      tot_bkg_hist_w = file_dir.Get("total_bkg")
      sub_hist_w_qcdsub.Add(tot_bkg_hist_w,-1)
      sub_hist_w_qcdsub.Add(hist_w_qcdsub)
#--------------------------------------------------------------------
      if DOCHI2:
        p = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW")
        chi2 = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW CHI2")
        chi2NDF = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW CHI2/NDF")
        p_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW")
        chi2_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW CHI2")
        chi2NDF_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW CHI2/NDF")      
        
        if chargetype == "SS":
          ChiList_SS.append([p,chi2,chi2NDF,p_obs,chi2_obs,chi2NDF_obs])
        else:
          ChiList.append([p,chi2,chi2NDF,p_obs,chi2_obs,chi2NDF_obs])
#--------------------------------------------------------------------
      if PLOTBYBIN:     
        output_file = R.TFile(output_folder_name+"/"+var[0]+"_"+sel+"_bybin_"+chargetype+".root","RECREATE")
        plot_name = var[0]+"_"+sel+"_bybin_"+chargetype
        if var[0] == "n_jets":
          MyCompareHists(hists = [hist_w_qcdsub_bybin,hist_wo_qcdsub_bybin],
                legend_titles = ["QCD - MC","Data Only"],
                title="Run 2 Data (%s) 59.7 fb^{-1} (13 TeV)"%chargetype, extra_pad=0.2,ratio_range="0.8,1.2",
                x_title = var[1], y_title = "Events",plot_name =plot_name, output_file = output_file, outputfolder=output_folder_name)
        else:
          MyCompareHists(hists = [hist_w_qcdsub_bybin,hist_wo_qcdsub_bybin],
                legend_titles = ["QCD - MC","Data Only"],
                title="Run 2 Data (%s) 59.7 fb^{-1} (13 TeV)"%chargetype, extra_pad=0.2,ratio_range="0.8,1.2",
                x_title = var[1]+" (GeV)", y_title = "Events/GeV",plot_name =plot_name, output_file = output_file, outputfolder=output_folder_name)        
      output_file = R.TFile(output_folder_name+"/"+var[0]+"_"+sel+"_"+chargetype+".root","RECREATE")
      plot_name = var[0]+"_"+sel+"_"+chargetype
      if var[0] == "n_jets":
        MyCompareHists(hists = [hist_w_qcdsub,hist_wo_qcdsub],
              legend_titles = ["QCD - MC","Data Only"],
              title="Run 2 Data (%s) 59.7 fb^{-1} (13 TeV)"%chargetype, extra_pad=0.2,ratio_range="0.8,1.2",
              x_title = var[1], y_title = "Events",plot_name =plot_name, output_file = output_file, outputfolder=output_folder_name)
      else:
        MyCompareHists(hists = [hist_w_qcdsub,hist_wo_qcdsub],
              legend_titles = ["QCD - MC","Data Only"],
              title="Run 2 Data (%s) 59.7 fb^{-1} (13 TeV)"%chargetype, extra_pad=0.2,ratio_range="0.8,1.2",
              x_title = var[1]+" (GeV)", y_title = "Events/GeV",plot_name =plot_name, output_file = output_file, outputfolder=output_folder_name)

#--------------------------------------------------------------------  
      output_file.Close()
      file.Close()
#--------------------------------------------------------------------
  if DOCHI2:      
    ChiList_SS = np.array(ChiList_SS,dtype = object)
    ChiList = np.array(ChiList,dtype = object)
    ChiList = np.concatenate((selnames_array,ChiList), axis=1)
    ChiList_SS = np.concatenate((selnames_array,ChiList_SS), axis=1)
    ChiList = ChiList[ChiList[:,3].argsort()]
    ChiList_SS = ChiList_SS[ChiList_SS[:,3].argsort()]
    chi2file.write("------------ OppS ------------ \n")
    order = 1
    for ChiVar in ChiList:
      chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[1],
                                                              ChiVar[2],ChiVar[3]))
      order+=1
    chi2file.write("------------- SS ------------ \n")
    order = 1
    for ChiVar in ChiList_SS:
      chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[1],
                                                              ChiVar[2],ChiVar[3]))    
      order+=1
      
    chi2file.write("------------ Compared to BKG------------ \n")
    ChiList = ChiList[ChiList[:,-1].argsort()]
    ChiList_SS = ChiList_SS[ChiList_SS[:,-1].argsort()]
    chi2file.write("------------ OppS ------------ \n")
    order = 1
    for ChiVar in ChiList:
      chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[4],
                                                              ChiVar[5],ChiVar[6]))  
      order+=1
    chi2file.write("------------- SS ------------ \n")
    order = 1
    for ChiVar in ChiList_SS:
      chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[4],
                                                              ChiVar[5],ChiVar[6]))    
      order+=1
