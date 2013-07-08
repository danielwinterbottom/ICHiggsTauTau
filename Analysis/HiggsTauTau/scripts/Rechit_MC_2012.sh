cd /vols/cms02/ajg06/CMSSW_5_3_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scramv1 runtime -sh`
source /vols/cms02/ajg06/CMSSW_5_3_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh
eval ./bin/HiggsTauTau --cfg=scripts/Paper_2012.cfg --is_data=0 --input_prefix=/vols/ssd00/cms/ajg06/June6/MC_53X/ --tau_scale_mode=0 --filelist=filelists/June6_MC_53X_RecHit-ETau-TTJets_FullLeptMGDecays_et_skim.dat --channel=et  --is_embedded=true --output_name=RecHit-TTJets_FullLeptMGDecays_et_2012.root

eval ./bin/HiggsTauTau --cfg=scripts/Paper_2012.cfg --is_data=0 --input_prefix=/vols/ssd00/cms/ajg06/June6/MC_53X/ --tau_scale_mode=0 --filelist=filelists/June6_MC_53X_RecHit-MTau-TTJets_FullLeptMGDecays_mt_skim.dat --channel=mt  --is_embedded=true --output_name=RecHit-TTJets_FullLeptMGDecays_mt_2012.root

eval ./bin/HiggsTauTau --cfg=scripts/Paper_2012.cfg --is_data=0 --input_prefix=/vols/ssd00/cms/ajg06/June6/MC_53X/ --tau_scale_mode=0 --filelist=filelists/June6_MC_53X_RecHit-EM-TTJets_FullLeptMGDecays_em_skim.dat --channel=em  --is_embedded=true --output_name=RecHit-TTJets_FullLeptMGDecays_em_2012.root

eval ./bin/HiggsTauTau --cfg=scripts/Paper_2012.cfg --is_data=0 --input_prefix=/vols/ssd00/cms/ajg06/June6/MC_53X/ --tau_scale_mode=0 --filelist=filelists/June6_MC_53X_RecHit-ETau-DYJetsToLL_et_skim.dat --channel=et  --is_embedded=true --output_name=RecHit-DYJetsToLL_et_2012.root

eval ./bin/HiggsTauTau --cfg=scripts/Paper_2012.cfg --is_data=0 --input_prefix=/vols/ssd00/cms/ajg06/June6/MC_53X/ --tau_scale_mode=0 --filelist=filelists/June6_MC_53X_RecHit-MTau-DYJetsToLL_mt_skim.dat --channel=mt  --is_embedded=true --output_name=RecHit-DYJetsToLL_mt_2012.root

