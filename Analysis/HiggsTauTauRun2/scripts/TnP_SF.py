import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
from optparse import OptionParser
import os

parser = OptionParser()
parser.add_option('--channel',help= 'Name of channel', default='tpzee')
parser.add_option("--year", dest="year", type='string', default='2018',help="Year input")
parser.add_option("--input_folder", help = 'Name of the input folder', default='plots/TnP/')
(options, args) = parser.parse_args()

channel = options.channel
year = options.year
input_folder = options.input_folder
file_name = "muon"
if channel == "tpzee": file_name = "electron"

# Open the source and destination root files
f_nom = ROOT.TFile.Open("{}/{}/{}/{}_nom.root".format(input_folder,channel,year,file_name))
f_sig = ROOT.TFile.Open("{}/{}/{}/{}_sig.root".format(input_folder,channel,year,file_name))
f_bkg = ROOT.TFile.Open("{}/{}/{}/{}_bkg.root".format(input_folder,channel,year,file_name))
f_tag = ROOT.TFile.Open("{}/{}/{}/{}_tightTag.root".format(input_folder,channel,year,file_name))

output_folder = "{}/{}/{}/correction/".format(input_folder,channel,year)

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

destination_file = ROOT.TFile.Open("{}/{}_SF.root".format(output_folder,file_name), "RECREATE")

types = ["id","iso","trg"]

names = ["data_id_eff","ZLL_id_eff","data_iso_eff","ZLL_iso_eff","data_trg_eff","ZLL_trg_eff"]
# Get the histogram from the source root file
for name in types:
  h_nom_data = f_nom.Get("data_{}_eff".format(name))
  h_sig_data = f_sig.Get("data_{}_eff".format(name))
  h_bkg_data = f_bkg.Get("data_{}_eff".format(name))
  h_tag_data = f_tag.Get("data_{}_eff".format(name))

  h_nom_ZLL = f_nom.Get("ZLL_{}_eff".format(name))
  h_sig_ZLL = f_sig.Get("ZLL_{}_eff".format(name))
  h_bkg_ZLL = f_bkg.Get("ZLL_{}_eff".format(name))
  h_tag_ZLL = f_tag.Get("ZLL_{}_eff".format(name))

  # Create a copy of the histogram in the destination root file
  h_SF = ROOT.TH2D('ScaleFactor_{}'.format(name), '', h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXmin(), h_nom_data.GetXaxis().GetXmax(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXmin(), h_nom_data.GetYaxis().GetXmax())
  h_SF.SetBins(h_nom_data.GetNbinsX(), h_nom_data.GetXaxis().GetXbins().GetArray(),h_nom_data.GetNbinsY(), h_nom_data.GetYaxis().GetXbins().GetArray())
  h_SF.SetDirectory(destination_file)

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
           
          stat_error = (((data_y_err_high**2 * ZLL_y_val) + (ZLL_y_err_high**2 * data_y_val))/ZLL_y_val**4)**0.5

          content_nom_data = h_nom_data.GetBinContent(xbin,ybin)
          content_sig_data = h_sig_data.GetBinContent(xbin, ybin)
          content_bkg_data = h_bkg_data.GetBinContent(xbin,ybin)
          content_tag_data = h_tag_data.GetBinContent(xbin,ybin)

          content_nom_ZLL = h_nom_ZLL.GetBinContent(xbin,ybin)
          content_sig_ZLL = h_sig_ZLL.GetBinContent(xbin, ybin)
          content_bkg_ZLL = h_bkg_ZLL.GetBinContent(xbin,ybin)
          content_tag_ZLL = h_tag_ZLL.GetBinContent(xbin,ybin)

          error = (stat_error**2 + abs((content_sig_data/content_sig_ZLL)-(content_nom_data/content_nom_ZLL))**2 + abs((content_bkg_data/content_bkg_ZLL)-(content_nom_data/content_nom_ZLL))**2 + abs((content_tag_data/content_tag_ZLL)-(content_nom_data/content_nom_ZLL))**2)**0.5

          errorbkg= abs((content_bkg_data/content_bkg_ZLL)-(content_nom_data/content_nom_ZLL))
          errorww = (abs((content_sig_data/content_sig_ZLL)-(content_nom_data/content_nom_ZLL))) 
          errortt = (abs((content_tag_data/content_tag_ZLL)-(content_nom_data/content_nom_ZLL)))
           
          ratio = content_nom_data/content_nom_ZLL

          if name == "trg" and ((h_SF.GetXaxis().GetBinUpEdge(xbin) <= 33 and year=="2018") or (h_SF.GetXaxis().GetBinUpEdge(xbin) <= 28 and year=="2017") or (h_SF.GetXaxis().GetBinUpEdge(xbin) <= 26 and year=="2016post") or (h_SF.GetXaxis().GetBinUpEdge(xbin) <= 26 and year=="2016pre")):
             ratio = 0
             error = 0

          if ratio!=0:
             if (error) > 0.05: print(name,xbin,h_SF.GetXaxis().GetBinLowEdge(xbin),ybin,"ERROR: ",error,"STAT: ",stat_error/error,"BKG: ",errorbkg/error,"SIGNAL: ",errorww/error,"TAG: ",errortt/error)

          h_SF.SetBinContent(xbin,ybin, ratio)
          h_SF.SetBinError(xbin,ybin, error)
          
          del h_data_eff, h_ZLL_eff

  # Write the copy of the histogram to the destination root file
  h_SF.Write()
  
  # Close the root files
  del h_nom_data
  del h_sig_data
  del h_bkg_data
  del h_tag_data
  del h_nom_ZLL
  del h_sig_ZLL
  del h_bkg_ZLL
  del h_tag_ZLL
  del h_SF

f_nom.Close()
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

   for j in range(1, h_SF.GetNbinsY()+1):
      histo1 = ROOT.TH1D('ScaleFactor_{}_1D'.format(name), '', h_SF.GetNbinsX(), h_SF.GetXaxis().GetXbins().GetArray())
      bin_contents = []
      for i in range(1, h_SF.GetNbinsX()+1):
         bin_content = h_SF.GetBinContent(i,j)
         if bin_content!=0: bin_contents.append(bin_content)
         bin_error = h_SF.GetBinError(i,j)
         histo1.SetBinContent(i,bin_content)
         histo1.SetBinError(i, bin_error)

         # Set the titles and axis labels for the new histograms
         if name == "iso": histo1.SetTitle('Isolation Scale Factor {} < |#eta| < {}'.format(h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
         if name == "trg": histo1.SetTitle('Trigger Scale Factor {} < |#eta| < {}'.format(h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
         histo1.GetXaxis().SetTitle('pT [GeV]')
         histo1.GetYaxis().SetTitle('Correction')
         if i == h_SF.GetNbinsX(): 
            # Draw the new histograms
            canvas = ROOT.TCanvas('canvas', 'canvas', 800, 600)
            histo1.GetYaxis().SetRangeUser(0.9*min(bin_contents),1.1*histo1.GetMaximum())
            histo1.SetStats(0)
            histo1.Draw()
            canvas.SaveAs("{}/{}_SF_{}_{}_{}.png".format(plots_folder,file_name,name,h_SF.GetYaxis().GetBinLowEdge(j),h_SF.GetYaxis().GetBinUpEdge(j)))
            canvas.Update()
            del canvas
            del histo1

   del h_SF 


