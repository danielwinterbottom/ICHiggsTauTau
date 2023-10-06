year=$1
extra=$2

if [[ "$year" == "2022_preEE" ]]; then
  dir="/vols/cms/dw515/outputs/MSSM/2022-preEE_031023_v2/"
  era="22_preEE"
elif [[ "$year" == "2022_postEE" ]]; then
  dir="/vols/cms/dw515/outputs/MSSM/2022-postEE_031023_v2/"
  era="22_postEE"
fi

mkdir -p TAP_Run3_v2/${year}/SingleE
mkdir -p TAP_Run3_v2/${year}/SingleMu

# single electron triggers
python scripts/TnP.py --channel tpzee --outputfolder=TAP_Run3_v2/${year}/SingleE --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra}
python scripts/TnP.py --channel tpzee --outputfolder=TAP_Run3_v2/${year}/SingleE --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra} --tight_tag

## single muon triggers:
#python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra}
#python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra} --tight_tag


