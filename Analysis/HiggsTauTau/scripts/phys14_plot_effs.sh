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
  '{"output":"th_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  '{"output":"th_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  '{"output":"th_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  '{"output":"th0_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  '{"output":"th0_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  '{"output":"th0_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  '{"output":"th1_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  '{"output":"th1_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  '{"output":"th1_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  '{"output":"th10_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"Match Eff.", "default":{"rebin":"20-200:10"}}'
  '{"output":"th10_rf_mt_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"RecAcc Eff. | Match", "default":{"rebin":"20-200:10"}}'
  '{"output":"th10_dm_rf_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Eff | Match & RecAcc", "default":{"rebin":"20-200:10"}}'
  '{"output":"th_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th0_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th1_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th10_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th0_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th1_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:5"}}'
  '{"output":"th10_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th0_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th1_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th10_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  '{"output":"th0_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  '{"output":"th1_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  '{"output":"th10_dm_eff_vs_it_pu", "x_axis_title":"Pileup (BX=0)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-60.5:4"}}'
  '{"output":"th_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  '{"output":"th0_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  '{"output":"th1_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  '{"output":"th10_dm_eff_vs_ot_pu", "x_axis_title":"Pileup (BX#pm1)", "y_axis_title":"DM Efficiency", "default":{"rebin":"0.5-100.5:4"}}'
  )

# for i in "${FRAGMENTS[@]}"
# do
# ./bin/PlotEff "${JSON}" "${i}"
# done

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


./bin/PlotCompare  \
 -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th0_pt_resp:-1:3:2" \
 -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th0_pt_resp:-1:3:418" \
 -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th0_pt_resp:-1:3:4" \
 -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th0_pt_resp:-1:3:797" \
 --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
 --log_y=false --rebin=1 --norm_bins=false \
 --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
 --title_left="CMS Simulation" \
 --outname="compare_th0_pt_resp.pdf"

./bin/PlotCompare  \
 -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th1_pt_resp:-1:3:2" \
 -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th1_pt_resp:-1:3:418" \
 -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th1_pt_resp:-1:3:4" \
 -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th1_pt_resp:-1:3:797" \
 --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
 --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
 --log_y=false --rebin=1 --norm_bins=false \
 --title_left="CMS Simulation" \
 --outname="compare_th1_pt_resp.pdf"

./bin/PlotCompare  \
 -p "53X:53X:${OUTPUT}/MC_53X_VBF_HToTauTau_M-125.root:/phys14/:th10_pt_resp:-1:3:2" \
 -p "70X_PU40bx50:70X_PU40bx50:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:/phys14/:th10_pt_resp:-1:3:418" \
 -p "70X_PU20bx25:70X_PU20bx25:${OUTPUT}/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th10_pt_resp:-1:3:4" \
 -p "72X_PU20bx25:72X_PU40bx25:${OUTPUT}/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:/phys14/:th10_pt_resp:-1:3:797" \
 --x_axis_title="(p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}" --norm_mode=3  --big_label=" " \
 --ratios="70X_PU40bx50/53X/418:70X_PU20bx25/53X/4:72X_PU20bx25/53X/797" --ratio_axis_label="Ratio/53X"  --ratio_y_min=0.5 --ratio_y_max=1.5 \
 --log_y=false --rebin=1 --norm_bins=false \
 --title_left="CMS Simulation" \
 --outname="compare_th10_pt_resp.pdf"
