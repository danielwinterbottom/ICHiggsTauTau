#!/bin/sh
# set -x

OUTPUT=output/phys14/Dec7

JSON='
{
  "output" : "",
  "x_axis_title" : "",
  "y_axis_title" : "",
  "y_axis_min" : 0.0,
  "y_axis_max" : 1.0,
  "log_y" : false,
  "legend_height" : 0.18,
  "legend_width" : 0.45,
  "title_right": "VBF H(125 GeV)#rightarrow#tau#tau",
  "default": {
    "color": 1,
    "marker": 20,
    "legend": "",
    "rebin": ""
  },
  "elements": [
    {
      "h_num": "'"${OUTPUT}"'/MC_53X_VBF_HToTauTau_M-125.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_53X_VBF_HToTauTau_M-125.root:phys14/",
      "color": 2,
      "marker": 20,
      "legend": "8 TeV, 53X, PU20bx50"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:phys14/",
      "color": 418,
      "marker": 21,
      "legend": "13 TeV, 70X, PU40bx50"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "color": 4,
      "marker": 22,
      "legend": "13 TeV, 70X, PU20bx25"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "color": 797,
      "marker": 23,
      "legend": "13 TeV, 72X, PU40bx25"
    }
  ]
}'


FRAGMENTS=(
  # '{"output":"th_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th0_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th0_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th0_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th1_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th1_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th1_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th10_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th10_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th10_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  # '{"output":"th0_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  # '{"output":"th1_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  # '{"output":"th10_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  # '{"output":"th_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th0_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th1_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:5"}}'
  # '{"output":"th10_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  # '{"output":"th_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  # '{"output":"th0_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  # '{"output":"th1_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  # '{"output":"th10_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  # '{"output":"th_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  # '{"output":"th0_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  # '{"output":"th1_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  # '{"output":"th10_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  # '{"output":"th_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  # '{"output":"th0_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  # '{"output":"th1_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  # '{"output":"th10_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  # Special plots
  # '{"output":"th1_mt_eff_vs_pt_pi", "x_axis_title":"Gen. #pi^{#pm} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"0-200:10"}}'
  # '{"output":"th1_jet_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Jet (p_{T} > 15) Match Eff.", "default":{"rebin":"0-200:10"}}'
  # '{"output":"th1_mt_eff_after_jet_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff. | Jet Match", "default":{"rebin":"0-200:10"}}'
  # '{"output":"th1_mt_eff_after_jet_pi15_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff. | Jet Match & p_{T}^{#pi}>15", "default":{"rebin":"0-200:10"}}'
  # '{"output":"th1_mt_eff_after_jet_pi20_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff. | Jet Match & p_{T}^{#pi}>20", "default":{"rebin":"0-200:10"}}'
  )

for i in "${FRAGMENTS[@]}"
do
./bin/PlotEff "${JSON}" "${i}"
done

# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root", "phys14/th_mode_table",                 "53X_th_mode_table", "VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}")'
# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root", "phys14/th_mode_table_gen_den",         "53X_th_mode_table_gen_den", "VBF H(125 GeV)#rightarrow#tau#tau #color[2]{53X}")'
# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root", "phys14/th_mode_table_gen_den_rec_fid", "53X_th_mode_table_gen_den_rec_fid", "VBF H(125 GeV)#rightarrow#tau#tau #color[2]{53X}")'
# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root", "phys14/th_mode_table",                 "72X_th_mode_table", "VBF H(125 GeV)#rightarrow#tau#tau #color[2]{72X}")'
# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root", "phys14/th_mode_table_gen_den",         "72X_th_mode_table_gen_den", "VBF H(125 GeV)#rightarrow#tau#tau #color[2]{72X}")'
# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root", "phys14/th_mode_table_gen_den_rec_fid", "72X_th_mode_table_gen_den_rec_fid", "VBF H(125 GeV)#rightarrow#tau#tau #color[2]{72X}")'


# ./bin/PlotCompare  \
#  -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:n_vtx:-1:3:2" \
#  -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:n_vtx:-1:3:418" \
#  -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_vtx:-1:3:4" \
#  -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_vtx:-1:3:797" \
#  --x_axis_title="Number of Vertices" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" \
#  --outname="compare_n_vtx.pdf"

# ./bin/PlotCompare  \
#  -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:n_it_pu:-1:3:2" \
#  -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:n_it_pu:-1:3:418" \
#  -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_it_pu:-1:3:4" \
#  -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_it_pu:-1:3:797" \
#  --x_axis_title="Interactions (bx = 0)" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" \
#  --outname="compare_n_it.pdf"

#  ./bin/PlotCompare  \
#   -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:n_ot_pu:-1:3:2" \
#   -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:n_ot_pu:-1:3:418" \
#   -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_ot_pu:-1:3:4" \
#   -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:n_ot_pu:-1:3:797" \
#   --x_axis_title="Interactions (bx = #pm1)" --norm_mode=3  --big_label=" " \
#   --log_y=false --rebin=1 --norm_bins=false \
#   --title_left="CMS Simulation" \
#   --outname="compare_n_ot.pdf"


# ./bin/PlotCompare  \
#  -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th0_pt_resp:-1:3:2" \
#  -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th0_pt_resp:-1:3:418" \
#  -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th0_pt_resp:-1:3:4" \
#  -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th0_pt_resp:-1:3:797" \
#  --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
#  --title_left="CMS Simulation" \
#  --outname="compare_th0_pt_resp.pdf"

# ./bin/PlotCompare  \
#  -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th1_pt_resp:-1:3:2" \
#  -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th1_pt_resp:-1:3:418" \
#  -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th1_pt_resp:-1:3:4" \
#  -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th1_pt_resp:-1:3:797" \
#  --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
#  --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" \
#  --outname="compare_th1_pt_resp.pdf"

# ./bin/PlotCompare  \
#  -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th10_pt_resp:-1:3:2" \
#  -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th10_pt_resp:-1:3:418" \
#  -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th10_pt_resp:-1:3:4" \
#  -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th10_pt_resp:-1:3:797" \
#  --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
#  --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" \
#  --outname="compare_th10_pt_resp.pdf"

# root -l -b -q 'macros/plotDMTable.C("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root", "phys14/th_mode_table",                 "53X_th_mode_table", "VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}")'
FILE72X="output/phys14/Mar4/MC_72X_PU40bx25_VBF_HToTauTau_M-125.root"
# FILE53X="output/phys14/Feb11/MC_53X_VBF_HToTauTau_M-125.root"
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th_pf_match_pt", "th_pf_match_pt_72X", "72X: all modes", "Track p_{T} (GeV)")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th_pf_match_eta", "th_pf_match_eta_72X", "72X: all modes", "Track #eta")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th0_pf_match_pt", "th0_pf_match_pt_72X", "72X: #pi", "Track p_{T} (GeV)")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th0_pf_match_eta", "th0_pf_match_eta_72X", "72X: #pi", "Track #eta")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th1_pf_match_pt", "th1_pf_match_pt_72X", "72X: #pi#pi^{0}s", "Track p_{T} (GeV)")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th1_pf_match_eta", "th1_pf_match_eta_72X", "72X: #pi#pi^{0}s", "Track #eta")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th10_pf_match_pt", "th10_pf_match_pt_72X", "72X: #pi#pi#pi", "Track p_{T} (GeV)")'
root -l -b -q 'macros/plotPFMatch.C+("'${FILE72X}'", "phys14/th10_pf_match_eta", "th10_pf_match_eta_72X", "72X: #pi#pi#pi", "Track #eta")'

# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th_pf_match_pt", "th_pf_match_pt_53X", "53X: all modes", "Track p_{T} (GeV)")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th_pf_match_eta", "th_pf_match_eta_53X", "53X: all modes", "Track #eta")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th0_pf_match_pt", "th0_pf_match_pt_53X", "53X: #pi", "Track p_{T} (GeV)")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th0_pf_match_eta", "th0_pf_match_eta_53X", "53X: #pi", "Track #eta")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th1_pf_match_pt", "th1_pf_match_pt_53X", "53X: #pi#pi^{0}s", "Track p_{T} (GeV)")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th1_pf_match_eta", "th1_pf_match_eta_53X", "53X: #pi#pi^{0}s", "Track #eta")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th10_pf_match_pt", "th10_pf_match_pt_53X", "53X: #pi#pi#pi", "Track p_{T} (GeV)")'
# root -l -b -q 'macros/plotPFMatch.C+("'${FILE53X}'", "phys14/th10_pf_match_eta", "th10_pf_match_eta_53X", "53X: #pi#pi#pi", "Track #eta")'


# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_unmatched/pixel_hits_vs_algo", "53X_unmatched_track_table", "53X, Non-matched #tau_{h} tracks", "Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_matched/pixel_hits_vs_algo", "53X_matched_track_table", "53X, PF-matched #tau_{h} tracks", "Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_ph_matched/pixel_hits_vs_algo", "53X_ph_matched_track_table", "53X, PFGamma-matched #tau_{h} tracks", "Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_unmatched/pixel_hits_vs_algo", "72X_unmatched_track_table", "72X, Non-matched #tau_{h} tracks", "Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_matched/pixel_hits_vs_algo", "72X_matched_track_table", "72X, PF-matched #tau_{h} tracks", "Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_ph_matched/pixel_hits_vs_algo", "72X_ph_matched_track_table", "72X, PFGamma-matched #tau_{h} tracks", "Pixel Hits")'

# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_unmatched/pixel_barrel_vs_algo", "53X_unmatched_track_table_barrel", "53X, Non-matched #tau_{h} tracks", "Barrel Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_matched/pixel_barrel_vs_algo", "53X_matched_track_table_barrel", "53X, PF-matched #tau_{h} tracks", "Barrel Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_ph_matched/pixel_barrel_vs_algo", "53X_ph_matched_track_table_barrel", "53X, PFGamma-matched #tau_{h} tracks", "Barrel Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_unmatched/pixel_barrel_vs_algo", "72X_unmatched_track_table_barrel", "72X, Non-matched #tau_{h} tracks", "Barrel Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_matched/pixel_barrel_vs_algo", "72X_matched_track_table_barrel", "72X, PF-matched #tau_{h} tracks", "Barrel Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_ph_matched/pixel_barrel_vs_algo", "72X_ph_matched_track_table_barrel", "72X, PFGamma-matched #tau_{h} tracks", "Barrel Pixel Hits")'

# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_unmatched/pixel_endcap_vs_algo", "53X_unmatched_track_table_endcap", "53X, Non-matched #tau_{h} tracks", "Endcap Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_matched/pixel_endcap_vs_algo", "53X_matched_track_table_endcap", "53X, PF-matched #tau_{h} tracks", "Endcap Pixel Hits")'
# root -l -b -q 'macros/plotTrackTable.C+("'${FILE53X}'", "phys14/trk_plots_ph_matched/pixel_endcap_vs_algo", "53X_ph_matched_track_table_endcap", "53X, PFGamma-matched #tau_{h} tracks", "Endcap Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_unmatched/pixel_endcap_vs_algo", "72X_unmatched_track_table_endcap", "72X, Non-matched #tau_{h} tracks", "Endcap Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_matched/pixel_endcap_vs_algo", "72X_matched_track_table_endcap", "72X, PF-matched #tau_{h} tracks", "Endcap Pixel Hits")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_ph_matched/pixel_endcap_vs_algo", "72X_ph_matched_track_table_endcap", "72X, PFGamma-matched #tau_{h} tracks", "Endcap Pixel Hits")'

root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_unmatched/nmiss_vs_algo", "72X_unmatched_track_table_nmiss", "72X, Non-matched #tau_{h} tracks", "numberOfHits(MISSING_INNER_HITS)")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_matched/nmiss_vs_algo", "72X_matched_track_table_nmiss", "72X, PF-matched #tau_{h} tracks", "numberOfHits(MISSING_INNER_HITS)")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_ph_matched/nmiss_vs_algo", "72X_ph_matched_track_table_nmiss", "72X, PFGamma-matched #tau_{h} tracks", "numberOfHits(MISSING_INNER_HITS)")'

root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_unmatched/nmisslost_vs_algo", "72X_unmatched_track_table_nmisslost", "72X, Non-matched #tau_{h} tracks", "numberOfLostTrackerHits(MISSING_INNER_HITS)")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_matched/nmisslost_vs_algo", "72X_matched_track_table_nmisslost", "72X, PF-matched #tau_{h} tracks", "numberOfLostTrackerHits(MISSING_INNER_HITS)")'
root -l -b -q 'macros/plotTrackTable.C+("'${FILE72X}'", "phys14/trk_plots_ph_matched/nmisslost_vs_algo", "72X_ph_matched_track_table_nmisslost", "72X, PFGamma-matched #tau_{h} tracks", "numberOfLostTrackerHits(MISSING_INNER_HITS)")'


# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:algo:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:algo:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:algo:-1:2:40" \
#  --x_axis_title="Track Algorithm" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_algo.pdf"

# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:pt:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:pt:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:pt:-1:2:40" \
#  --x_axis_title="Track p_{T} (GeV)" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_pt.pdf"


# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:eta:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:eta:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:eta:-1:2:40" \
#  --x_axis_title="Track #eta" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false --extra_pad=1.4 \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_eta.pdf"

# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:pixel_hits:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:pixel_hits:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:pixel_hits:-1:2:40" \
#  --x_axis_title="Pixel Hits" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false --extra_pad=1.4 \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_pixel_hits.pdf"

# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:barrel_hits:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:barrel_hits:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:barrel_hits:-1:2:40" \
#  --x_axis_title="Pixel Barrel Hits" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false --extra_pad=1.4 \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_pixel_barrel.pdf"

# ./bin/PlotCompare  \
#  -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:endcap_hits:-1:2:12" \
#  -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:endcap_hits:-1:2:46" \
#  -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:endcap_hits:-1:2:40" \
#  --x_axis_title="Pixel Endcap Hits" --norm_mode=3  --big_label=" " \
#  --log_y=false --rebin=1 --norm_bins=false \
#  --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
#  --outname="compare_53X_pfmatch_pixel_endcap.pdf"

#  ./bin/PlotCompare  \
# -p "pfmatch:PFCharged:${FILE53X}:/phys14/trk_plots_matched:pt_err_over_pt:-1:2:12" \
# -p "phmatch:PFGamma:${FILE53X}:/phys14/trk_plots_ph_matched:pt_err_over_pt:-1:2:46" \
# -p "nomatch:No Match:${FILE53X}:/phys14/trk_plots_unmatched:pt_err_over_pt:-1:2:40" \
# --x_axis_title="#delta p_{T} / p_{T}" --norm_mode=3  --big_label=" " \
# --log_y=true --rebin=1 --norm_bins=false \
# --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{53X}" \
# --outname="compare_53X_pfmatch_pt_err_over_pt.pdf"





./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:algo:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:algo:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:algo:-1:2:40" \
 --x_axis_title="Track Algorithm" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_algo.pdf"

./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:pt:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:pt:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:pt:-1:2:40" \
 --x_axis_title="Track p_{T} (GeV)" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_pt.pdf"

./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:eta:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:eta:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:eta:-1:2:40" \
 --x_axis_title="Track #eta" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false --extra_pad=1.4 \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_eta.pdf"

./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:pixel_hits:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:pixel_hits:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:pixel_hits:-1:2:40" \
 --x_axis_title="Pixel Hits" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false  --extra_pad=1.4 \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_pixel_hits.pdf"

./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:barrel_hits:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:barrel_hits:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:barrel_hits:-1:2:40" \
 --x_axis_title="Pixel Barrel Hits" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false  --extra_pad=1.4 \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_pixel_barrel.pdf"

./bin/PlotCompare  \
 -p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:endcap_hits:-1:2:12" \
 -p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:endcap_hits:-1:2:46" \
 -p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:endcap_hits:-1:2:40" \
 --x_axis_title="Pixel Endcap Hits" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false \
 --title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
 --outname="compare_72X_pfmatch_pixel_endcap.pdf"

 ./bin/PlotCompare  \
-p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:pt_err_over_pt:-1:2:12" \
-p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:pt_err_over_pt:-1:2:46" \
-p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:pt_err_over_pt:-1:2:40" \
--x_axis_title="#delta p_{T} / p_{T}" --norm_mode=3  --big_label=" " \
--log_y=true --rebin=1 --norm_bins=false \
--title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
--outname="compare_72X_pfmatch_pt_err_over_pt.pdf"

 ./bin/PlotCompare  \
-p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:is_high_purity:-1:2:12" \
-p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:is_high_purity:-1:2:46" \
-p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:is_high_purity:-1:2:40" \
--x_axis_title="Track highPurity Flag" --norm_mode=3  --big_label=" " \
--log_y=false --rebin=1 --norm_bins=false --extra_pad=5 \
--title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
--outname="compare_72X_pfmatch_is_high_purity.pdf"

 ./bin/PlotCompare  \
-p "pfmatch:PFCharged:${FILE72X}:/phys14/trk_plots_matched:dr_to_pf:-1:2:12" \
-p "phmatch:PFGamma:${FILE72X}:/phys14/trk_plots_ph_matched:dr_to_pf:-1:2:46" \
-p "nomatch:No Match:${FILE72X}:/phys14/trk_plots_unmatched:dr_to_pf:-1:2:40" \
--x_axis_title="#DeltaR to closest PFCandidate" --norm_mode=3  --big_label=" " \
--log_y=true --rebin=1 --norm_bins=false --extra_pad=1.5 \
--title_left="CMS Simulation" --title_right="VBF H(125 GeV)#rightarrow#tau#tau  #color[2]{72X}" \
--outname="compare_72X_pfmatch_dr_to_pf.pdf"

