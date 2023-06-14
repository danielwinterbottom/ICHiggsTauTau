import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = {
    "channel":"tpzmm" ,
    "year":"2018",
    "input_folder":"",
    "embed": False,
    "run_systs" : False,
    }

if options.cfg:
    config = ConfigParser.SafeConfigParser()
    config.read([options.cfg])
    defaults.update(dict(config.items("Defaults")))

parser = argparse.ArgumentParser(
    parents=[conf_parser]
    )
parser.add_argument('--channel',type = str, help= 'Name of channel')
parser.add_argument("--year", dest="year", type=str, help="Year input")
parser.add_argument("--input_folder", help = 'Name of the input folder')
parser.add_argument("--embed", dest="embed", action='store_true')
parser.add_argument("--run_systs", dest="run_systs", action='store_true')
options = parser.parse_args(remaining_argv)

channel = options.channel
year = options.year
input_folder = options.input_folder
file_name = "muon"
if channel == "tpzee": file_name = "electron"

# Open the source and destination root files
f_nom = ROOT.TFile.Open("{}/{}/{}/{}_nom.root".format(input_folder,year,channel,file_name))
if options.run_systs:
   f_sig = ROOT.TFile.Open("{}/{}/{}/{}_sig.root".format(input_folder,year,channel,file_name))
   f_bkg = ROOT.TFile.Open("{}/{}/{}/{}_bkg.root".format(input_folder,year,channel,file_name))
   f_tag = ROOT.TFile.Open("{}/{}/{}/{}_tightTag.root".format(input_folder,year,channel,file_name))

output_folder = "{}/{}/{}/correction/".format(input_folder,year,channel)

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

destination_file = ROOT.TFile.Open("{}/{}_SF.root".format(output_folder,file_name), "RECREATE")

types = ["id","iso","trg"]

# Get the histogram from the source root file
for name in types:
  h_nom_data = f_nom.Get("data_{}_eff".format(name))
  h_nom_ZLL = f_nom.Get("ZLL_{}_eff".format(name))
  if options.embed:
    h_nom_EMB = f_nom.Get("embed_{}_eff".format(name))
  if options.run_systs:
     h_sig_data = f_sig.Get("data_{}_eff".format(name))
     h_bkg_data = f_bkg.Get("data_{}_eff".format(name))
     h_tag_data = f_tag.Get("data_{}_eff".format(name))
     h_sig_ZLL = f_sig.Get("ZLL_{}_eff".format(name))
     h_bkg_ZLL = f_bkg.Get("ZLL_{}_eff".format(name))
     h_tag_ZLL = f_tag.Get("ZLL_{}_eff".format(name))
     if options.embed:
       h_sig_EMB = f_sig.Get("embed_{}_eff".format(name))
       h_bkg_EMB = f_bkg.Get("embed_{}_eff".format(name))
       h_tag_EMB = f_tag.Get("embed_{}_eff".format(name))

  # Create a copy of the histogram in the destination root file
  h_SF = ROOT.TH2D('ScaleFactor_{}'.format(name), '', h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXmin(), h_nom_data.GetXaxis().GetXmax(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXmin(), h_nom_data.GetYaxis().GetXmax())
  h_SF.SetBins(h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXbins().GetArray(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXbins().GetArray())
  h_SF.SetDirectory(destination_file)
  if options.embed:
    h_SF_EMB = ROOT.TH2D('ScaleFactor_EMB_{}'.format(name), '', h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXmin(), h_nom_data.GetXaxis().GetXmax(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXmin(), h_nom_data.GetYaxis().GetXmax())
    h_SF_EMB.SetBins(h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXbins().GetArray(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXbins().GetArray())
    h_SF_EMB.SetDirectory(destination_file)

  for xbin in range(1, h_SF.GetNbinsX()+1):
      for ybin in range(1, h_SF.GetNbinsY()+1):

          bin_low_edge = str(round(h_nom_data.GetYaxis().GetBinLowEdge(ybin),1)).replace(".","p")
          bin_up_edge  = str(round(h_nom_data.GetYaxis().GetBinUpEdge(ybin),1)).replace(".","p")

          h_data_eff = f_nom.Get("gr_data_{}_eff_eta_{}_to_{}".format(name,bin_low_edge,bin_up_edge))
          h_ZLL_eff = f_nom.Get("gr_ZLL_{}_eff_eta_{}_to_{}".format(name,bin_low_edge,bin_up_edge))

          data_y_val = h_data_eff.GetY()[xbin-1]
          data_y_err_low = h_data_eff.GetErrorYlow(xbin-1)
          data_y_err_high = h_data_eff.GetErrorYhigh(xbin-1)
          ZLL_y_val = h_data_eff.GetY()[xbin-1]
          ZLL_y_err_low = h_ZLL_eff.GetErrorYlow(xbin-1)
          ZLL_y_err_high = h_ZLL_eff.GetErrorYhigh(xbin-1)

          if options.embed:
            h_EMB_eff = f_nom.Get("gr_embed_{}_eff_eta_{}_to_{}".format(name,bin_low_edge,bin_up_edge))
            EMB_y_val = h_EMB_eff.GetY()[xbin-1]
            EMB_y_err_low = h_EMB_eff.GetErrorYlow(xbin-1)
            EMB_y_err_high = h_EMB_eff.GetErrorYhigh(xbin-1)
         
          #### added this for embedded 
          ###if ZLL_y_val != 0:
          ###  stat_error = (((data_y_err_high**2 * ZLL_y_val) + (ZLL_y_err_high**2 * data_y_val))/ZLL_y_val**4)**0.5
          ###else:
          ###  stat_error = 0
          stat_error_data = max(data_y_err_high,data_y_err_low)
          stat_error_ZLL = max(ZLL_y_err_high,ZLL_y_err_low)
          stat_error_EMB = 0 if not options.embed else max(EMB_y_err_high,EMB_y_err_low)
          if ZLL_y_val != 0:
            # stat error on the overall MC SF
            stat_error = ((stat_error_data/data_y_val)**2+(stat_error_ZLL/ZLL_y_val)**2)**.5*data_y_val/ZLL_y_val 
          else: 
            stat_error = 0
          if options.embed and EMB_y_val != 0:
            stat_error_embed_sf = ((stat_error_data/data_y_val)**2+(stat_error_EMB/EMB_y_val)**2)**.5*data_y_val/EMB_y_val

          if name == 'trg' and (data_y_val<0.1 or ZLL_y_val<0.1 or (options.embed and EMB_y_val<0.1)):
             # for the trigger SF we have to remove the low pT SFs where the SFs don;t make sense as the efficiencies are very small. 
             # at the moment to avoid having to use seperate pT cuts depending on the era/channel/trigger we just zero all SF where the efficiencies are < 10% 
             # need to be careful with this bit method incase efficiencies are low for another reason!
  
             ratio = 0
             error = 0
             error_data = 0
             error_mc = 0
             error_embed = 0
             error_embed_sf = 0
             ratio_emb = 0
          else:
            content_nom_data = h_nom_data.GetBinContent(xbin,ybin)
            content_nom_ZLL = h_nom_ZLL.GetBinContent(xbin,ybin)
            if options.embed: 
              content_nom_EMB = h_nom_EMB.GetBinContent(xbin,ybin)
            if options.run_systs:
              content_sig_data = h_sig_data.GetBinContent(xbin, ybin)
              content_sig_ZLL = h_sig_ZLL.GetBinContent(xbin, ybin)
              content_bkg_data = h_bkg_data.GetBinContent(xbin,ybin)
              content_bkg_ZLL = h_bkg_ZLL.GetBinContent(xbin,ybin)
              content_tag_data = h_tag_data.GetBinContent(xbin,ybin)
              content_tag_ZLL = h_tag_ZLL.GetBinContent(xbin,ybin)
              if options.embed:
                content_sig_EMB = h_sig_EMB.GetBinContent(xbin, ybin)
                content_bkg_EMB = h_bkg_EMB.GetBinContent(xbin,ybin)
                content_tag_EMB = h_tag_EMB.GetBinContent(xbin,ybin) 
              if content_nom_ZLL == 0:
                sig_error = 0
                bkg_error = 0
                tag_error = 0
                ratio = 0
              else:
                sig_error = 0. if content_sig_ZLL<=0. else abs((content_sig_data/content_sig_ZLL)-(content_nom_data/content_nom_ZLL))**2 
                bkg_error = 0. if content_bkg_ZLL<=0. else abs((content_bkg_data/content_bkg_ZLL)-(content_nom_data/content_nom_ZLL))**2
                tag_error = 0. if content_tag_ZLL<=0. else abs((content_tag_data/content_tag_ZLL)-(content_nom_data/content_nom_ZLL))**2
                ratio = content_nom_data/content_nom_ZLL
              if options.embed:
                if content_nom_EMB == 0:
                  sig_error_emb = 0
                  bkg_error_emb = 0
                  tag_error_emb = 0
                  ratio_emb = 0
                else:
                  sig_error_emb = 0. if content_sig_EMB<=0. else abs((content_sig_data/content_sig_EMB)-(content_nom_data/content_nom_EMB))**2
                  bkg_error_emb = 0. if content_bkg_EMB<=0. else abs((content_bkg_data/content_bkg_EMB)-(content_nom_data/content_nom_EMB))**2
                  tag_error_emb = 0. if content_tag_EMB<=0. else abs((content_tag_data/content_tag_EMB)-(content_nom_data/content_nom_EMB))**2 
                  ratio_emb = content_nom_data/content_nom_EMB
              error = (stat_error**2 + sig_error + bkg_error + tag_error)**0.5
              error_data = (stat_error_data**2+(content_sig_data-content_nom_data)**2 + (content_bkg_data-content_nom_data)**2 + (content_tag_data-content_nom_data)**2)**.5 
              error_mc = (stat_error_ZLL**2+(content_sig_ZLL-content_nom_ZLL)**2 + (content_bkg_ZLL-content_nom_ZLL)**2 + (content_tag_ZLL-content_nom_ZLL)**2)**.5
              if options.embed:
                error_embed_sf = (stat_error_embed_sf**2 + sig_error_emb + bkg_error_emb + tag_error_emb)**0.5  
                error_embed = (stat_error_EMB**2+(content_sig_EMB-content_nom_EMB)**2 + (content_bkg_EMB-content_nom_EMB)**2 + (content_tag_EMB-content_nom_EMB)**2)**.5
          h_SF.SetBinContent(xbin,ybin, ratio)
          h_SF.SetBinError(xbin,ybin, error)
          h_nom_data.SetBinError(xbin,ybin, error_data)
          h_nom_ZLL.SetBinError(xbin,ybin, error_mc)
          if options.embed:
            h_SF_EMB.SetBinContent(xbin,ybin, ratio_emb)
            h_SF_EMB.SetBinError(xbin,ybin, error_embed_sf)
            h_nom_EMB.SetBinError(xbin,ybin, error_embed)          

  # Write the copy of the histogram to the destination root file
  h_SF.Write()
  h_nom_data.Write()
  h_nom_ZLL.Write()
  if options.embed:
    h_nom_EMB.Write()
    h_SF_EMB.Write()


  # Close the root files
  del h_nom_data
  del h_nom_ZLL
  if options.run_systs:
     del h_sig_data
     del h_sig_ZLL
     del h_bkg_data
     del h_bkg_ZLL
     del h_tag_data
     del h_tag_ZLL
  del h_SF
  del h_SF_EMB

f_nom.Close()
if options.run_systs:
   f_sig.Close()
   f_bkg.Close()
   f_tag.Close()
destination_file.Close()

destination_file = ROOT.TFile.Open("{}/{}_SF.root".format(output_folder,file_name), "READ")

plots_folder = "{}/plots/".format(output_folder)

if not os.path.exists(plots_folder):
    os.makedirs(plots_folder)


types = ["id","iso","trg"]
for name in types:
   h_SF = destination_file.Get("ScaleFactor_{}".format(name))
   h_SF_EMB = destination_file.Get("ScaleFactor_EMB_{}".format(name))

   for j in range(1, h_SF.GetNbinsY()+1):
     histo1 = ROOT.TH1D('ScaleFactor_{}_1D'.format(name), '', h_SF.GetNbinsX(), h_SF.GetXaxis().GetXbins().GetArray())
     if options.embed: 
       histo2 = ROOT.TH1D('ScaleFactor_EMB_{}_1D'.format(name), '', h_SF_EMB.GetNbinsX(), h_SF_EMB.GetXaxis().GetXbins().GetArray())
     bin_contents = []
     bin_contents_emb = []
     for i in range(1, h_SF.GetNbinsX()+1):
        bin_content = h_SF.GetBinContent(i,j)
        if bin_content!=0: bin_contents.append(bin_content)
        bin_error = h_SF.GetBinError(i,j)
        histo1.SetBinContent(i,bin_content)
        histo1.SetBinError(i, bin_error)
        if options.embed:
          bin_content_emb = h_SF_EMB.GetBinContent(i,j)
          if bin_content_emb!=0: bin_contents_emb.append(bin_content)
          bin_error_emb = h_SF_EMB.GetBinError(i,j)
          histo2.SetBinContent(i,bin_content_emb)
          histo2.SetBinError(i, bin_error_emb)

        # Set the titles and axis labels for the new histograms
        if name == "id": histo1.SetTitle('ID Scale Factor {} < |#eta| < {}'.format(h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j))) 
        if name == "iso": histo1.SetTitle('Isolation Scale Factor {} < |#eta| < {}'.format(h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
        if name == "trg": histo1.SetTitle('Trigger Scale Factor {} < |#eta| < {}'.format(h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
        histo1.GetXaxis().SetTitle('pT [GeV]')
        histo1.GetYaxis().SetTitle('Correction')
     if i == h_SF.GetNbinsX(): 
        # Draw the new histograms
        canvas = ROOT.TCanvas('canvas', 'canvas', 800, 600)
        histo1.GetYaxis().SetRangeUser(0.9*min(bin_contents+bin_contents_emb),1.1*histo1.GetMaximum())
        histo1.SetStats(0)
        histo1.Draw()  
        if options.embed:
          histo2.SetLineColor(ROOT.kRed)
          histo2.SetStats(0)
          histo2.Draw('same')
        canvas.SaveAs("{}/{}_SF_{}_{}_{}.pdf".format(plots_folder,file_name,name,h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
        canvas.Update()
        del canvas
        del histo1
        del histo2
   del h_SF
   del h_SF_EMB 


