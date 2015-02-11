#!/bin/sh
# set -x

OUTPUT=output/phys14/Feb5

JSON='
{
  "output" : "",
  "x_axis_title" : "",
  "y_axis_title" : "",
  "y_axis_min" : 0.0001,
  "y_axis_max" : 0.1,
  "log_y" : true,
  "title_right": "QCD_Pt-15to3000",
  "default": {
    "color": 1,
    "marker": 20,
    "legend": "",
    "rebin": ""
  },
  "elements": [
    {
      "h_num": "'"${OUTPUT}"'/MC_53X_QCD_Pt-15to3000_NoJEC.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_53X_QCD_Pt-15to3000_NoJEC.root:phys14/",
      "color": 2,
      "marker": 20,
      "legend": "8 TeV, 53X: NO JEC"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_72X_PU20bx25_QCD_Pt-15to3000_NoJEC.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_72X_PU20bx25_QCD_Pt-15to3000_NoJEC.root:phys14/",
      "color": 4,
      "marker": 22,
      "legend": "13 TeV, 72X: NO JEC"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_53X_QCD_Pt-15to3000_WithJEC.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_53X_QCD_Pt-15to3000_WithJEC.root:phys14/",
      "color": 2,
      "marker": 24,
      "legend": "8 TeV, 53X: WITH JEC"
    },
    {
      "h_num": "'"${OUTPUT}"'/MC_72X_PU20bx25_QCD_Pt-15to3000_WithJEC.root:phys14/",
      "h_den": "'"${OUTPUT}"'/MC_72X_PU20bx25_QCD_Pt-15to3000_WithJEC.root:phys14/",
      "color": 4,
      "marker": 26,
      "legend": "13 TeV, 72X: WITH JEC"
    }
  ]
}'


FRAGMENTS=(
  '{"output":"jet_th_fake_dm_vs_pt", "x_axis_title":"Reco Jet p_{T} (GeV)", "y_axis_title":"Fake rate (DM + Charged Iso < 2 GeV)", "default":{"rebin":"20-200:10"}}'
  # '{"output":"jet_th_fake_dm_vs_pt", "x_axis_title":"Reco Jet p_{T} (GeV)", "y_axis_title":"Fake rate (DM + Charged Iso < 2 GeV)", "default":{"rebin":"20-200:10,200-1000:50"}}'
  )

for i in "${FRAGMENTS[@]}"
do
./bin/PlotEff "${JSON}" "${i}"
done

