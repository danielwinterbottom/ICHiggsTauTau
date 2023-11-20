year=$1
extra=$2

if [[ "$year" == "2022_preEE" ]]; then
  dir="/vols/cms/ia2318/outputs/MSSM/2022-preEE_031023"
  era="22_preEE"
elif [[ "$year" == "2022_postEE" ]]; then
  dir="/vols/cms/ia2318/outputs/MSSM/2022-postEE_031023"
  era="22_postEE"
fi

mkdir -p TAP_Run3_v2/${year}/SingleE
mkdir -p TAP_Run3_v2/${year}/SingleMu

# single electron triggers
python scripts/TnP.py --channel tpzee --outputfolder=TAP_Run3_v2/${year}/SingleE --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra}
python scripts/TnP.py --channel tpzee --outputfolder=TAP_Run3_v2/${year}/SingleE --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra} --tight_tag
python scripts/TnP_SF.py --channel tpzee --input_folder TAP_Run3_v2/${year}/SingleE/

# single muon triggers:
python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra}
python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu --folder=${dir} --era=${era} --tree_name=tagandprobe ${extra} --tight_tag 
python scripts/TnP_SF.py --channel tpzmm --input_folder TAP_Run3_v2/${year}/SingleMu/

# aiso SFs
mkdir -p TAP_Run3_v2/${year}/SingleMu_aiso
python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu_aiso --folder=${dir} --era=${era} --tree_name=tagandprobe --aiso ${extra}
python scripts/TnP.py --outputfolder=TAP_Run3_v2/${year}/SingleMu_aiso --folder=${dir} --era=${era} --tree_name=tagandprobe --aiso ${extra} --tight_tag
python scripts/TnP_SF.py --channel tpzmm --input_folder TAP_Run3_v2/${year}/SingleMu_aiso/
