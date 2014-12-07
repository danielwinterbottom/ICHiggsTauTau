#!/bin/sh
# set -x

JSON='
{
  "output" : "",
  "x_axis_title" : "",
  "y_axis_title" : "",
  "y_axis_min" : 0.0,
  "y_axis_max" : 1.0,
  "default": {
    "color": 1,
    "marker": 20,
    "legend": "",
    "rebin": ""
  },
  "elements": [
    {
      "h_num": "output/phys14/MC_53X_VBF_HToTauTau_M-125.root:phys14/",
      "h_den": "output/phys14/MC_53X_VBF_HToTauTau_M-125.root:phys14/",
      "color": 2,
      "marker": 20,
      "legend": "8 TeV, 53X, Summer12 Unweighted"
    },
    {
      "h_num": "output/phys14/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:phys14/",
      "h_den": "output/phys14/MC_70X_VBF_HToTauTau_M-125_PU40bx50.root:phys14/",
      "color": 418,
      "marker": 21,
      "legend": "13 TeV, 70X, PU40bx50 Unweighted"
    },
    {
      "h_num": "output/phys14/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "h_den": "output/phys14/MC_70X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "color": 4,
      "marker": 22,
      "legend": "13 TeV, 70X, PU20bx25 Unweighted"
    },
    {
      "h_num": "output/phys14/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "h_den": "output/phys14/MC_72X_VBF_HToTauTau_M-125_PU20bx25.root:phys14/",
      "color": 797,
      "marker": 23,
      "legend": "13 TeV, 72X, PU20bx25 Unweighted"
    }
  ]
}'

FRAGMENTS=(
  '{"output":"th_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th0_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th1_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th10_dm_eff_vs_nvtx", "x_axis_title":"Number of Vertices", "y_axis_title":"DM Efficiency", "default":{"rebin":"10.5-50.5:4"}}'
  '{"output":"th_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th0_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th1_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th10_dm_eff_vs_pt", "x_axis_title":"Gen. #tau_{h} p_{T} (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":"20-200:10"}}'
  '{"output":"th_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th0_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th1_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  '{"output":"th10_dm_eff_vs_eta", "x_axis_title":"Gen. #tau_{h} #eta (GeV)", "y_axis_title":"DM Efficiency", "default":{"rebin":""}}'
  )

for i in "${FRAGMENTS[@]}"
do
./bin/PlotEff "${JSON}" "${i}"
done

