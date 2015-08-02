#!/bin/sh
# set -x

# OUTPUT=output/toytau_test.root
OUTPUT=$1
SET=$2

JSON='
{
  "output" : "",
  //"x_axis_title" : "Gen. #tau_{h} p_{T} (GeV)",
  "x_axis_title" : "Reco. jet p_{T} (GeV)",
  //"y_axis_title" : "Efficiency",
  "y_axis_title" : "Fake-rate",
  "y_axis_min" : 1E-5,
  "y_axis_max" : 1E-1,
  "log_y" : true,
  "legend_height" : 0.10,
  "legend_width" : 0.20,
  "legend_pos" : 6,
  "title_right": "",
  "default": {
    "color": 1,
    "marker": 20,
    "legend": "",
    //"rebin": ""
    "rebin": "20-100:40,100-500:80"
  },
  "elements": [
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_taus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_taus/",
      "color": 2,
      "marker": 20,
      "legend": "Reco #tau_{h}"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "color": 4,
      "marker": 20,
      "legend": "Toy #tau_{h}"
    }
  ]
}'



FRAGMENTS=(
  '{"output":"'${SET}'/rec_eff_vs_pt","title_right":"Rec. matched"}'
  '{"output":"'${SET}'/rec_dm_eff_vs_pt","title_right":"DMF"}'
  '{"output":"'${SET}'/rec_dm_chiso0p8_eff_vs_pt","title_right":"DMF & I_{ch} < 0.8 GeV"}'
  '{"output":"'${SET}'/rec_dm_chiso1p0_eff_vs_pt","title_right":"DMF & I_{ch} < 1.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_chiso2p0_eff_vs_pt","title_right":"DMF & I_{ch} < 2.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso0p8_eff_vs_pt","title_right":"DMF & I_{comb} < 0.8 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso1p0_eff_vs_pt","title_right":"DMF & I_{comb} < 1.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_eff_vs_pt","title_right":"DMF & I_{comb} < 2.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_nh2p0_eff_vs_pt","title_right":"I_{nh} < 2.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_nh5p0_eff_vs_pt","title_right":"I_{nh} < 5.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_nh10p0_eff_vs_pt","title_right":"I_{nh} < 10.0 GeV"}'
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt","title_right":"I_{nh} < max(10.0, 0.1*p_{T}) GeV"}'
  '{"output":"'${SET}'/rec_chiso0p8_gvn_dm_eff_vs_pt","title_right":"I_{ch} < 0.8 GeV | DMF"}'
  '{"output":"'${SET}'/rec_chiso1p0_gvn_dm_eff_vs_pt","title_right":"I_{ch} < 1.0 GeV | DMF"}'
  '{"output":"'${SET}'/rec_chiso2p0_gvn_dm_eff_vs_pt","title_right":"I_{ch} < 2.0 GeV | DMF"}'
  '{"output":"'${SET}'/rec_cmbiso0p8_gvn_dm_eff_vs_pt","title_right":"DMF & I_{comb} < 0.8 GeV | DMF"}'
  '{"output":"'${SET}'/rec_cmbiso1p0_gvn_dm_eff_vs_pt","title_right":"DMF & I_{comb} < 1.0 GeV | DMF"}'
  '{"output":"'${SET}'/rec_cmbiso2p0_gvn_dm_eff_vs_pt","title_right":"DMF & I_{comb} < 2.0 GeV | DMF"}'

  )

for i in "${FRAGMENTS[@]}"
do
./bin/PlotEff "${JSON}" "${i}"
done

JSON='
{
  "output" : "",
  //"x_axis_title" : "Gen. #tau_{h} p_{T} (GeV)",
  "x_axis_title" : "Reco. jet p_{T} (GeV)",  //"y_axis_title" : "Efficiency",
  "y_axis_title" : "Fake-rate",
  "y_axis_min" : 1E-5,
  "y_axis_max" : 1E-0,
  "log_y" : true,
  "legend_height" : 0.27,
  "legend_width" : 0.40,
  "legend_pos" : 3,
  "title_right": "",
  "default": {
    "color": 1,
    "marker": 20,
    "legend": "",
    //"rebin": ""
    "rebin": "20-100:40,100-500:80"
  },
  "elements": [
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_taus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_taus/",
      "color": 2,
      "marker": 20,
      "legend": "Reco #tau_{h}: DMF & I_{comb} < 2 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_taus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_taus/",
      "color": 12,
      "marker": 20,
      "legend": "  & I_{nh} < max(10, 0.1*pt) GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_taus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_taus/",
      "color": 48,
      "marker": 20,
      "legend": "  & I_{nh} < 5 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh5p0_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_taus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_taus/",
      "color": 46,
      "marker": 20,
      "legend": "  & I_{nh} < 2 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh2p0_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "color": 4,
      "marker": 22,
      "legend": "Toy #tau_{h}: DMF & I_{comb} < 2 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "color": 12,
      "marker": 22,
      "legend": "  & I_{nh} < max(10, 0.1*pt) GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "color": 39,
      "marker": 22,
      "legend": "  & I_{nh} < 5 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh5p0_eff_vs_pt"
    },
    {
      "h_num": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "h_den": "'${OUTPUT}':ToyTauStudy_toyTaus/",
      "color": 38,
      "marker": 22,
      "legend": "  & I_{nh} < 2 GeV",
      "fixname": "'${SET}'/rec_dm_cmbiso2p0_nh2p0_eff_vs_pt"
    }
  ]
}'

FRAGMENTS=(
  '{"output":"'${SET}'/rec_dm_cmbiso2p0_scan_nh_eff_vs_pt","title_right":"DMF & I_{comb} < 2.0 GeV"}'
  )

for i in "${FRAGMENTS[@]}"
do
./bin/PlotEff "${JSON}" "${i}"
done


# ./bin/PlotCompare  \
# -p "Gen:Gen #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:gen_pt:-1:0:1" \
# -p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:rec_pt:-1:0:4" \
# -p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:rec_pt:-1:0:8" \
# --x_axis_title="#tau_{h} p_{T} (GeV)" --norm_mode=0  --big_label=" " \
# --log_y=false --rebin=1 --norm_bins=false \
# --title_left="Z'(1 TeV)#rightarrow#tau#tau" --title_right="Reco/Toy #tau_{h}: Matched to gen, no selection" \
# --outname="${SET}/tau_pt.pdf" --extra_pad=1.5

# ./bin/PlotCompare  \
# -p "Gen:Gen #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:gen_eta:-1:0:1" \
# -p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:rec_eta:-1:0:4" \
# -p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:rec_eta:-1:0:8" \
# --x_axis_title="#tau_{h} #eta" --norm_mode=0  --big_label=" " \
# --log_y=false --rebin=1 --norm_bins=false \
# --title_left="Z'(1 TeV)#rightarrow#tau#tau" --title_right="Reco/Toy #tau_{h}: Matched to gen, no selection" \
# --outname="${SET}/tau_eta.pdf" --extra_pad=1.5

# ./bin/PlotCompare  \
# -p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:pt_resp:-1:0:4" \
# -p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:pt_resp:-1:0:8" \
# --x_axis_title="#tau_{h} p_{T} Response" --norm_mode=0  --big_label=" " \
# --log_y=true --rebin=1 --norm_bins=false \
# --title_left="Z'(1 TeV)#rightarrow#tau#tau" --title_right="Reco/Toy #tau_{h}: acc & DMF & I_{cmb} < 2 GeV" \
# --outname="${SET}/tau_pt_resp.pdf" --extra_pad=1.5

./bin/PlotCompare  \
-p "Gen:PF Jets:${OUTPUT}:/ToyTauStudy_taus/${SET}:gen_pt:-1:0:1" \
-p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:rec_pt:-1:0:4" \
-p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:rec_pt:-1:0:8" \
--x_axis_title="Jet/#tau_{h} p_{T} (GeV)" --norm_mode=0  --big_label=" " \
--log_y=true --rebin=1 --norm_bins=false \
--title_left="QCD" --title_right="Reco/Toy #tau_{h}: Matched to gen, no selection" \
--outname="${SET}/tau_pt.pdf" --extra_pad=1.5

./bin/PlotCompare  \
-p "Gen:PF Jets:${OUTPUT}:/ToyTauStudy_taus/${SET}:gen_eta:-1:0:1" \
-p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:rec_eta:-1:0:4" \
-p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:rec_eta:-1:0:8" \
--x_axis_title="Jet/#tau_{h} #eta" --norm_mode=0  --big_label=" " \
--log_y=false --rebin=1 --norm_bins=false \
--title_left="QCD" --title_right="Reco/Toy #tau_{h}: Matched to gen, no selection" \
--outname="${SET}/tau_eta.pdf" --extra_pad=1.5

./bin/PlotCompare  \
-p "Rec:Reco #tau_{h}:${OUTPUT}:/ToyTauStudy_taus/${SET}:pt_resp:-1:0:4" \
-p "Toy:Toy #tau_{h}:${OUTPUT}:/ToyTauStudy_toyTaus/${SET}:pt_resp:-1:0:8" \
--x_axis_title="#tau_{h}/Jet p_{T} Response" --norm_mode=0  --big_label=" " \
--log_y=true --rebin=1 --norm_bins=false \
--title_left="QCD" --title_right="Reco/Toy #tau_{h}: acc & DMF & I_{cmb} < 2 GeV" \
--outname="${SET}/tau_pt_resp.pdf" --extra_pad=1.5


