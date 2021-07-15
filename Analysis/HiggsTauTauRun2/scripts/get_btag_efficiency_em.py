import ROOT
import json

btag_categories = {
                   "em":["em_NbtagGt1_DZetaGt30","em_NbtagGt1_DZetam10To30","em_NbtagGt1_DZetam35Tom10"],
                   "et":["et_NbtagGt1_MTLt40","et_NbtagGt1_MT40To70"],
                   "mt":["mt_NbtagGt1_MTLt40","mt_NbtagGt1_MT40To70"],
                   "tt":["tt_NbtagGt1"],
                   }
nobtag_categories = {
                     "em":["em_Nbtag0_DZetaGt30","em_Nbtag0_DZetam10To30","em_Nbtag0_DZetam35Tom10","em_DZetaLtm35"], 
                     "et":["et_Nbtag0_MTLt40", "et_Nbtag0_MT40To70"],
                     "mt":["mt_Nbtag0_MTLt40", "mt_Nbtag0_MT40To70"],
                     "tt":["tt_Nbtag0"],
                     }

categories = {
              "em": btag_categories["em"] + nobtag_categories["em"],
              "et": btag_categories["et"] + nobtag_categories["et"],
              "mt": btag_categories["mt"] + nobtag_categories["mt"],
              "tt": btag_categories["tt"] + nobtag_categories["tt"]
             }

cat_map = {
           "em_NbtagGt1_DZetaGt30":"35",
           "em_NbtagGt1_DZetam10To30":"36",
           "em_NbtagGt1_DZetam35Tom10":"37",
           "em_Nbtag0_DZetaGt30":"32",
           "em_Nbtag0_DZetam10To30":"33",
           "em_Nbtag0_DZetam35Tom10":"34",
           "em_DZetaLtm35":"2",
           "et_NbtagGt1_MTLt40":"35",
           "et_NbtagGt1_MT40To70":"36",
           "et_Nbtag0_MTLt40":"32",
           "et_Nbtag0_MT40To70":"33",
           "mt_NbtagGt1_MTLt40":"35",
           "mt_NbtagGt1_MT40To70":"36",
           "mt_Nbtag0_MTLt40":"32",
           "mt_Nbtag0_MT40To70":"33",
           "tt_NbtagGt1":"32",
           "tt_Nbtag0":"35"
           }

json_dict = {}
for i in ["CMS_htt_eff_b_$ERA","CMS_htt_mistag_b_$ERA"]:
#for i in ["CMS_htt_mistag_b_$ERA"]:
  # Do bkg
  #for k in ["W", "ZL", "TTL", "VVL","ggH125","qqH125"]:
  for k in ["ZTT","TTT","VVT"]:
    print 'cb.cp().process({"%(k)s"}).AddSyst(cb, "%(i)s", "lnN", SystMapAsymm<channel,ch::syst::era,bin_id>::init' % vars()
    if k == "W": channels = ["em"]
    else: channels = ["em","et","mt","tt"]
    for ch in channels:
      for bc in categories[ch]:
        for year in ["2016","2017","2018"]:
          if ch == "em":
            file_input = "/vols/cms/dw515/MSSM/Unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes_mc/%(year)s/%(ch)s/htt_all.inputs-mssm-vs-sm-Run%(year)s-mt_tot_puppi.root" % vars()
          else:
            n = bc[3:]
            file_input = "mssm_dc_ZTT_v4/%(year)s/%(ch)s/htt_all.inputs-mssm-vs-sm-Run%(year)s-mt_tot_puppi.root" % vars()
          f = ROOT.TFile.Open(file_input ,"READ")
          h_nom = f.Get(bc+"/"+k % vars())
          h_up = f.Get(bc+"/"+k+'_'+i.replace("$ERA",year)+"Up")
          h_down = f.Get(bc+"/"+k+'_'+i.replace("$ERA",year)+"Down")
  
          nom = h_nom.Integral(0,h_nom.GetNbinsX()+1)
          up = h_up.Integral(0,h_nom.GetNbinsX()+1)
          down = h_down.Integral(0,h_nom.GetNbinsX()+1)
  
          frac_up = round(up/nom,3)
          frac_down = round(down/nom,3)
          cat_num = cat_map[bc]  

          print '({"%(ch)s"}, {"%(year)s"}, {%(cat_num)s}, %(frac_down)s, %(frac_up)s)' % vars()
    print ");"
    print ""

  ## Do signal
  #json_dict[i] = {}
  #sig_masses = ["60","80","100","120","125","130","140","160","180","200","250","300","350","400","450","500","600","700","800","900","1000","1200","1400","1600","1800","2000","2300","2600","2900","3200","3500"]
  #for proc in ["mssm_bbH_signals","mssm_ggH_signals"]:
  #  json_dict[i][proc] = {}
  #  for mass in sig_masses:
  #    json_dict[i][proc][mass] = {}
  #    print 'cb.cp().process(%(proc)s).AddSyst(cb, "%(i)s", "lnN", SystMapAsymm<channel,ch::syst::era,bin_id,mass>::init' % vars()
  #    for bc in ["btag_catagories","nobtag_catagories"]:
  #      json_dict[i][proc][mass][bc] = {}
  #      for ch in ["em","et","mt","tt"]: 
  #        json_dict[i][proc][mass][bc][ch] = {}
  #        for year in ["2016","2017","2018"]:
  #          nom = 0
  #          up = 0
  #          down = 0
  #          if bc == "btag_catagories": loop = btag_categories[ch]
  #          elif bc == "nobtag_catagories": loop = nobtag_categories[ch]
  #          for j in loop:
  #            if ch == "em":
  #              file_input = "/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/%(year)s/em/htt_all.inputs-mssm-vs-sm-Run%(year)s-mt_tot_puppi.root" % vars()
  #            else:
  #              n = j[3:]
  #              file_input = "output/datacard_m_vis_%(n)s_%(ch)s_%(year)s.root" % vars()

  #            f = ROOT.TFile.Open(file_input ,"READ")
  #            if "bbH" in proc: name = "bbH_"+mass
  #            elif "ggH" in proc: name = "ggh_t_"+mass
 
  #            h_nom = f.Get(j+"/"+name)
  #            h_up = f.Get(j+"/"+name+'_'+i.replace("$ERA",year)+"Up")
  #            h_down = f.Get(j+"/"+name+'_'+i.replace("$ERA",year)+"Down")

  #            if str(h_nom) == "<ROOT.TObject object at 0x(nil)>": 
  #              nom += 0
  #              up += 0
  #              down += 0
  #            else: 
  #              nom += h_nom.Integral(0,h_nom.GetNbinsX()+1)
  #              up += h_up.Integral(0,h_nom.GetNbinsX()+1)
  #              down += h_down.Integral(0,h_nom.GetNbinsX()+1)

  #          if nom != 0: frac_up = round(up/nom,3)
  #          else: frac_up = "!!!"
  #          if nom !=0: frac_down = round(down/nom,3)
  #          else: frac_down = "!!!"

  #          if (frac_up != "!!!" and frac_down != "!!!") and (frac_up > 1.5 or frac_up < 0.5 or frac_down > 1.5 or frac_down < 0.5):
  #            frac_up = "!!!"
  #            frac_down = "!!!"
  #          #if (frac_up == "!!!" or frac_down == "!!!"):
  #            #frac_up = 1.
  #            #frac_down = 1.

  #          json_dict[i][proc][mass][bc][ch][year] = {"Down":frac_down,"Up":frac_up}
  #          print '({"%(ch)s"}, {"%(year)s"}, %(bc)s, {"%(mass)s"}, %(frac_down)s, %(frac_up)s)' % vars()
  #    print ");"
  #    print ""

#with open('signal_b_efficiencies.json', 'w') as outfile:
#    json.dump(json_dict, outfile, indent=1, sort_keys=True)


  


