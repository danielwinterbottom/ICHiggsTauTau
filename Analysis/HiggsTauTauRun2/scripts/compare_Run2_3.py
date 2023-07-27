import ROOT as R
from UserCode.ICHiggsTauTau.plotting import *
import numpy as np

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
             outputfolder = None,
             integral_with_lowerlimit = None,
             integral_with_upperlimit = None,
             do_integral_stats=False):
   
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
    legend = PositionedLegend(0.35,0.15,3,0.04)
    legend.SetTextFont(42)
    legend.SetTextSize(0.03)
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
    
    if do_integral_stats:
      legendstats = PositionedLegend(0.2,0.15,2,0.01)
      legendstats.SetTextFont(42)
      legendstats.SetTextSize(0.024)
      legendstats.SetFillColor(0)
      legendstats.SetHeader("Integrals")
      

      for legi,hist in enumerate(legend_hists):
          legendstats.AddEntry(hist,"%i (%.1f%%)"%(hist.Integral(),
                              (hist.Integral()/legend_hists[0].Integral())*100),"l")
      if isinstance(uncert_hist, (list,)):
        count=0
      if uncert_hist is not None and uncert_title: legendstats.AddEntry(uncert_hist,uncert_title,'f')
      legendstats.Draw("same")
    
    if type(integral_with_lowerlimit) is float or type(integral_with_lowerlimit) is int:
      t = R.gPad.GetTopMargin()
      r = R.gPad.GetRightMargin()
      legendstatslim = R.TLegend(1 - r  - 0.2, 1 - t  - 0.2-0.2, 1 - r - 0.05, 1 - t -0.2, '', 'NBNDC')
      legendstatslim.SetTextFont(42)
      legendstatslim.SetTextSize(0.024)
      legendstatslim.SetFillColor(0)
      legendstatslim.SetHeader("Integrals >%.0fGeV"%integral_with_lowerlimit)
      

      for legi,hist in enumerate(legend_hists):
          legendstatslim.AddEntry(hist,"%i (%.1f%%)"%(hist.Integral(hist.FindBin(integral_with_lowerlimit),hist.GetNbinsX()),
                              (hist.Integral(hist.FindBin(integral_with_lowerlimit),
                                hist.GetNbinsX())/legend_hists[0].Integral(hist.FindBin(integral_with_lowerlimit),hist.GetNbinsX()))*100),"l")
      legendstatslim.Draw("same")
    if type(integral_with_upperlimit) is float or type(integral_with_upperlimit) is int:
      t = R.gPad.GetTopMargin()
      r = R.gPad.GetRightMargin()
      legendstatslim = R.TLegend(1 - r  - 0.2, 1 - t  - 0.2-0.2, 1 - r - 0.05, 1 - t -0.2, '', 'NBNDC')
      legendstatslim.SetTextFont(42)
      legendstatslim.SetTextSize(0.024)
      legendstatslim.SetFillColor(0)
      legendstatslim.SetHeader("Integrals <%.0fGeV"%integral_with_upperlimit)
      

      for legi,hist in enumerate(legend_hists):
          legendstatslim.AddEntry(hist,"%i (%.1f%%)"%(hist.Integral(1,hist.FindBin(integral_with_upperlimit)),
                              (hist.Integral(1,hist.FindBin(integral_with_upperlimit))/legend_hists[0].Integral(1,hist.FindBin(integral_with_upperlimit)))*100),"l")
      legendstatslim.Draw("same")          
    
    c1.SaveAs(outputfolder+plot_name+'.png')
    #c1.SaveAs(outputfolder+plot_name+'.pdf')
    if output_file is not None:
        output_file.WriteObject(c1, plot_name)
    for o in objects:
        o.IsA().Destructor(o)
    c1.Close()

USEBYBIN = True
LUMI22 = 38.2
LUMI18 = 59.7
DO_RATIO = True
file18_dirname = "output_2018_all/"
file22_dirname = "output_2022/"

output_folder_name = "output_2018vs2022/"


variables = [
  
  ["n_jets","N_{jets}"],
  # ["pt_1","p_{T}^{#tau_{1}}"],
  # ["pt_2", "p_{T}^{#tau_{2}}"],
  # ["mt_1", "m_{T}(p_{T}^{#tau_{1}},p_{T}^{miss})"],
  # ["mt_2","m_{T}(p_{T}^{#tau_{2}},p_{T}^{miss})"],
  ["pt_tt", "p_{T}^{#tau#tau}"],
  ["m_vis","m_{#tau#tau}"]

]

selname = "medium_vloose_medium_vloose"
types=["OppS","SS"]

for var in variables:
  for chargetype in types:
      
#--------------------------------------------------------------------
#Reading files and creating histograms - 2018
        
    file18names,files18,file18_dirs,hists18QCD,hists18Data,hists18Diff = [],[],[],[],[],[]
    versions= ["v2p1","v2p5"]
    for version in versions:
      if chargetype == "SS":
        file18names.append("datacard_" + var[0] + "_inclusive_" + selname +"_trg_doubletau_"+version+"_tt_2018_ss.root")
      else:
        file18names.append( "datacard_" + var[0] + "_inclusive_" + selname +"_trg_doubletau_"+version+ "_tt_2018.root")
    
    for f in file18names: files18.append( R.TFile.Open(file18_dirname + f, "READ"))
    for f in files18: file18_dirs.append(f.Get("tt_inclusive"))
    for f in file18_dirs: 
      f.cd()
      if USEBYBIN:
        hists18QCD.append( f.Get("QCD_bybin_nosub").Clone())
      else:
        hists18QCD.append( f.Get("QCD_nosub").Clone())
      
      hists18Data.append(f.Get("data_obs").Clone())
      hists18Diff.append(f.Get("data_obs").Clone())
      hists18Diff[-1].SetName("DATA_minus_QCD")
      hists18Diff[-1].Add(hists18Data[-1],hists18QCD[-1],1,-1)
     
    uncertQCD = hists18QCD[0].Clone()
    for bin_i in range(1,uncertQCD.GetNbinsX()+1):
      error = uncertQCD.GetBinContent(bin_i)
      error = error*0.1
      uncertQCD.SetBinError(bin_i,error)
      
    uncertDiff = hists18Diff[0].Clone()
    for bin_i in range(1,uncertDiff.GetNbinsX()+1):
      content = uncertDiff.GetBinContent(bin_i)
      uncertDiff.SetBinContent(bin_i,content)
      error = uncertQCD.GetBinContent(bin_i)*0.1
      uncertDiff.SetBinError(bin_i,error)

    
    SBstatHist18 = []
    for hist_i in range(0,len(hists18Data)):
      SBstatHist18.append(hists18QCD[hist_i].Clone())
      for i_bin in range(1,SBstatHist18[-1].GetNbinsX()+1):
        if SBstatHist18[-1].GetBinContent(i_bin)<=0:
          SBstatHist18[-1].SetBinContent(i_bin, 0)
          SBstatHist18[-1].SetBinError(i_bin,0)
        else:
          SBstatHist18[-1].SetBinContent(i_bin, np.sqrt(SBstatHist18[-1].GetBinContent(i_bin)))
          SBstatHist18[-1].SetBinError(i_bin,SBstatHist18[-1].GetBinError(i_bin)/(2*np.sqrt(SBstatHist18[-1].GetBinContent(i_bin)))) 
          SBstatHist18[-1].Divide(hists18Data[hist_i],SBstatHist18[-1],1,1)    
#--------------------------------------------------------------------
#Reading files and creating histograms - 2022

    file22names,files22,file22_dirs,hists22QCD,hists22Data,hists22Diff = [],[],[],[],[],[]
    trignames = ["trg_doubletau","trg_both"]
    for trig in trignames:
      if chargetype == "SS":
        file22names.append("datacard_" + var[0] + "_inclusive_" + selname +"_"+trig+ "_v2p5_tt_2022_ss.root")
      else:
        file22names.append( "datacard_" + var[0] + "_inclusive_" + selname+"_"+trig + "_v2p5_tt_2022.root")
    
    for f in file22names: files22.append( R.TFile.Open(file22_dirname + f, "READ"))
    for f in files22: file22_dirs.append(f.Get("tt_inclusive"))
    for f in file22_dirs: 
      f.cd()
      if USEBYBIN:
        hists22QCD.append( f.Get("QCD_bybin_nosub").Clone())
      else:
        hists22QCD.append( f.Get("QCD_nosub").Clone())
      
      hists22Data.append(f.Get("data_obs").Clone())
      hists22QCD[-1].Scale(LUMI18/LUMI22)
      hists22Data[-1].Scale(LUMI18/LUMI22)
      
      hists22Diff.append(f.Get("data_obs").Clone())
      hists22Diff[-1].SetName("DATA_minus_QCD")
      hists22Diff[-1].Add(hists22Data[-1],hists22QCD[-1],1,-1)
    
    SBstatHist22 = []
    for hist_i in range(0,len(hists22Data)):
      SBstatHist22.append(hists22QCD[hist_i].Clone())
      for i_bin in range(1,SBstatHist22[-1].GetNbinsX()+1):
        if SBstatHist22[-1].GetBinContent(i_bin)<=0:
          SBstatHist22[-1].SetBinContent(i_bin, 0)
          SBstatHist22[-1].SetBinError(i_bin,0)
        else:
          SBstatHist22[-1].SetBinContent(i_bin, np.sqrt(SBstatHist22[-1].GetBinContent(i_bin)))
          SBstatHist22[-1].SetBinError(i_bin,SBstatHist22[-1].GetBinError(i_bin)/(2*np.sqrt(SBstatHist22[-1].GetBinContent(i_bin)))) 
          SBstatHist22[-1].Divide(hists22Data[hist_i],SBstatHist22[-1],1,1)

#--------------------------------------------------------------------
#Outputfile
    output = R.TFile(output_folder_name+"/"+var[0]+"_"+selname+"_"+chargetype+".root","RECREATE")
    
#--------------------------------------------------------------------    
    for histtype in [[hists18Data,hists22Data,"Data",None],[hists18QCD,hists22QCD,"QCD",uncertQCD],
                 [hists18Diff,hists22Diff,"nonQCD",uncertDiff]]:
      # legend_titles = ["2018 %s (%s)"%(histtype[2],chargetype)] + [
      #   "#splitline{Scaled 2022 %s}{%s (%s)}"%(histtype[2],trig,chargetype) for trig in ["doubletau trig","all trigs"]
      # ]
      legend_titles = ["2018 %s"%(ver) for ver in versions] + [
        "2022 %s"%(trig) for trig in ["doubletau","doubletau+jet"]
      ]      
      if var[0] == "n_jets":
        MyCompareHists(hists = histtype[0]+histtype[1],
                legend_titles = legend_titles,
                title="%s (%s) 59.7 fb^{-1}"%(histtype[2],("Same-Sign" if chargetype=="SS" else "Opposite-Sign")), extra_pad=0,ratio_range="0.5,1.5",
                x_title = var[1], y_title = "Events",plot_name =
                var[0]+"_"+selname+"_"+chargetype+"_"+histtype[2], output_file = output, ratio = DO_RATIO,
                outputfolder=output_folder_name,uncert_hist=histtype[3],
                uncert_title=
                ("Systematic Error" if histtype[3] is not None else ''))
      elif var[0] == "pt_tt":
        MyCompareHists(hists = histtype[0]+histtype[1],
                legend_titles = legend_titles,
                title="%s (%s) 59.7 fb^{-1}"%(histtype[2],("Same-Sign" if chargetype=="SS" else "Opposite-Sign")), extra_pad=0,ratio_range="0.3,1.5",
                x_title = var[1], y_title = "Events",plot_name =
                var[0]+"_"+selname+"_"+chargetype+"_"+histtype[2], output_file = output, ratio = DO_RATIO,
                outputfolder=output_folder_name, integral_with_lowerlimit=None,uncert_hist=histtype[3],uncert_title=
                ("Systematic Error" if histtype[3] is not None else ''))        
      else:
        MyCompareHists(hists = histtype[0]+histtype[1],
                legend_titles = legend_titles,
                title="%s (%s) 59.7 fb^{-1}"%(histtype[2],("Same-Sign" if chargetype=="SS" else "Opposite-Sign")), extra_pad=0,ratio_range="0.5,1.5",
                x_title = var[1]+" (GeV)", y_title = "Events/GeV",plot_name =
                var[0]+"_"+selname+"_"+chargetype+"_"+histtype[2], output_file = output, ratio = DO_RATIO,
                outputfolder=output_folder_name,uncert_hist=histtype[3], uncert_title=
                ("Systematic Error" if histtype[3] is not None else ''),integral_with_upperlimit=None)
#title="59.7 fb^{-1} (13 TeV)"        
#-------------------------------------------------------------------- 
    # legend_titles = ["2018 S/#sqrt{B} (%s)"%chargetype] + [
    #   "#splitline{2022 S/#sqrt{B} (%s)}{%s}"%(chargetype,trig) for trig in ["doubletau trig","all trigs"]
    # ]
    MyCompareHists(hists = SBstatHist18+SBstatHist22,
                legend_titles = legend_titles,
                title="S/#sqrt{B} %s (%s)"%(histtype[2],chargetype), extra_pad=0,ratio_range="0.5,2.5",
                x_title = var[1]+" (GeV)", y_title = "Events/GeV",plot_name =
                var[0]+"_"+selname+"_"+chargetype, output_file = output, ratio = True, outputfolder=output_folder_name)

        
#--------------------------------------------------------------------        
    for f in files18: f.Close()
    for f in files22: f.Close()

