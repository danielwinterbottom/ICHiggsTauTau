year=$1
extra=$2

if [[ "$year" == "2016-preVFP" ]]; then
  era="UL_16preVFP"
elif [[ "$year" == "2016-postVFP" ]]; then
  era="UL_16postVFP"
elif [[ "$year" == "2017" ]]; then
  era="UL_17"
elif [[ "$year" == "2018" ]]; then
  era="UL_18"
fi

mkdir -p TAP_UL_v7/${year}/SingleE
mkdir -p TAP_UL_v7/${year}/ETau_ELeg
mkdir -p TAP_UL_v7/${year}/EMu_E12Leg
mkdir -p TAP_UL_v7/${year}/EMu_E23Leg

mkdir -p TAP_UL_v7/${year}/SingleMu
mkdir -p TAP_UL_v7/${year}/MuTau_MuLeg
mkdir -p TAP_UL_v7/${year}/EMu_Mu8Leg
mkdir -p TAP_UL_v7/${year}/EMu_Mu23Leg
if [[ "$year" == "2017" ]]; then
  mkdir -p TAP_UL_v7/${year}/MuTau_MuLeg_RunB
fi
mkdir -p TAP_UL_v7/${year}/Mu17
mkdir -p TAP_UL_v7/${year}/Mu8
mkdir -p TAP_UL_v7/${year}/DZ
mkdir -p TAP_UL_v7/${year}/DZ_binned
mkdir -p TAP_UL_v7/${year}/DZ_dR_binned

# single electron triggers
if [[ "$year" == "2016-preVFP" || "$year" == "2016-postVFP" ]]; then
  python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/SingleE --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_Single --embedded ${extra}
  python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/SingleE --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_Single --embedded ${extra} --tight_tag
else
  python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/SingleE --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe --embedded ${extra}
  python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/SingleE --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe --embedded ${extra} --tight_tag
fi

# electron legs of e+mu cross triggers:
# Low pT leg 
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/EMu_E12Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emLow --embedded ${extra} --em_iso
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/EMu_E12Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emLow --embedded ${extra} --em_iso --tight_tag
#High pT leg:
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/EMu_E23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emHigh --embedded ${extra} --em_iso --trg_only
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/EMu_E23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emHigh --embedded ${extra} --em_iso --tight_tag --trg_only

# electron leg of e+tau cross trigger:
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/ETau_ELeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_ET --embedded ${extra} --em_iso --trg_only
python scripts/TnP.py --channel tpzee --outputfolder=TAP_UL_v7/${year}/ETau_ELeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_ET --embedded ${extra} --em_iso --tight_tag --trg_only


# single muon triggers:
if [[ "$year" == "2016-preVFP" || "$year" == "2016-postVFP" ]]; then
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/SingleMu --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_Single --embedded ${extra}
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/SingleMu --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_Single --embedded ${extra} --tight_tag
else
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/SingleMu --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe --embedded ${extra}
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/SingleMu --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe --embedded ${extra} --tight_tag
fi

# muon leg of mu+tau triggers:
if [[ "$year" == "2016-preVFP" || "$year" == "2016-postVFP" ]]; then
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt --embedded ${extra} --trg_only
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt --embedded ${extra} --tight_tag --trg_only
else
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt_c_to_f --embedded ${extra} --trg_only
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt_c_to_f --embedded ${extra} --tight_tag --trg_only

  # for 2017 we have to use a different monitoring trigger to measure SFs for runB, these will be combined later on using lumi averaging

  if [[ "$year" == "2017" ]]; then
    python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg_RunB --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt_b --embedded ${extra} --trg_only
    python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/MuTau_MuLeg_RunB --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_mt_b --embedded ${extra} --tight_tag --trg_only

  fi
 
fi

# muon legs of e+mu cross triggers:
# Low pT leg - we also run the iso SFs again in this case as we use a different iso cut for the em channel:
if [[ "$year" == "2016-preVFP" || "$year" == "2016-postVFP" ]]; then
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu8Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emLow --embedded ${extra} --em_iso
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu8Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emLow --embedded ${extra} --em_iso --tight_tag
else
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu8Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_em_low --embedded ${extra} --em_iso
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu8Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_em_low --embedded ${extra} --em_iso --tight_tag
fi
#High pT leg:
if [[ "$year" == "2016-preVFP" || "$year" == "2016-postVFP" ]]; then
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emHigh --embedded ${extra} --em_iso --trg_only
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_emHigh --embedded ${extra} --em_iso --tight_tag --trg_only
else
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_em_high --embedded ${extra} --em_iso --trg_only
  python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/EMu_Mu23Leg --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_em_high --embedded ${extra} --em_iso --tight_tag --trg_only
fi

# embed selections
# high pT leg and ID SFs
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/Mu17 --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_high --embed_sel ${extra}
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/Mu17 --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_high --embed_sel ${extra} --tight_tag
# low pT leg
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/Mu8 --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_low --embed_sel ${extra} --trg_only 
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/Mu8 --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_low --embed_sel ${extra} --trg_only --tight_tag
# dz filter
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/DZ --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_dz --embed_dz ${extra} --trg_only
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/DZ --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_dz --embed_dz ${extra} --trg_only --tight_tag

# dz in bins of etas
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/DZ_binned --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_dz --embed_dz ${extra} --trg_only --fine_dz_bins
python scripts/TnP.py --outputfolder=TAP_UL_v7/${year}/DZ_binned --folder=/vols/cms/dw515/outputs/MSSM/UL_Mar31_${year}_v2_TAP_v4/ --era=${era} --tree_name=tagandprobe_dimu_dz --embed_dz ${extra} --trg_only --tight_tag --fine_dz_bins


