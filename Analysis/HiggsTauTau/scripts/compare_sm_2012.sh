#### First just print our yields:

### Moriond
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_inclusive --output_file=output/dc_sync/yields_moriond_inclusive_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_vbf --output_file=output/dc_sync/yields_moriond_vbf_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_boost_high --output_file=output/dc_sync/yields_moriond_boost_high_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_boost_low --output_file=output/dc_sync/yields_moriond_boost_low_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_0jet_high --output_file=output/dc_sync/yields_moriond_0jet_high_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_mt.inputs-sm-8TeV.root \
--labels MoriondYields  --category muTau_0jet_low --output_file=output/dc_sync/yields_moriond_0jet_low_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_inclusive --output_file=output/dc_sync/yields_moriond_inclusive_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_vbf --output_file=output/dc_sync/yields_moriond_vbf_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_boost_high --output_file=output/dc_sync/yields_moriond_boost_high_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_boost_low --output_file=output/dc_sync/yields_moriond_boost_low_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_0jet_high --output_file=output/dc_sync/yields_moriond_0jet_high_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs Moriond-htt_et.inputs-sm-8TeV.root \
--labels MoriondYields --category eleTau_0jet_low --output_file=output/dc_sync/yields_moriond_0jet_low_et-sm-8TeV.pdf 
./bin/DatacardCompare --mode=2 --inputs Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MoriondYields   --category emu_vbf --output_file=output/dc_sync/yields_moriond_vbf_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MoriondYields  --category emu_boost_high --output_file=output/dc_sync/yields_moriond_boost_high_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MoriondYields  --category emu_boost_low --output_file=output/dc_sync/yields_moriond_boost_low_em-sm-8TeV.pdf      
./bin/DatacardCompare --mode=2 --inputs Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MoriondYields  --category emu_0jet_high --output_file=output/dc_sync/yields_moriond_0jet_high_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MoriondYields  --category emu_0jet_low --output_file=output/dc_sync/yields_moriond_0jet_low_em-sm-8TeV.pdf

# ### HCP
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_inclusive --output_file=output/dc_sync/yields_hcp_inclusive_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_vbf --output_file=output/dc_sync/yields_hcp_vbf_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_boost_high --output_file=output/dc_sync/yields_hcp_boost_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_boost_low --output_file=output/dc_sync/yields_hcp_boost_low_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_0jet_high --output_file=output/dc_sync/yields_hcp_0jet_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_mt.inputs-sm-8TeV.root \
# --labels HCPYields  --category muTau_0jet_low --output_file=output/dc_sync/yields_hcp_0jet_low_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_inclusive --output_file=output/dc_sync/yields_hcp_inclusive_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_vbf --output_file=output/dc_sync/yields_hcp_vbf_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_boost_high --output_file=output/dc_sync/yields_hcp_boost_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_boost_low --output_file=output/dc_sync/yields_hcp_boost_low_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_0jet_high --output_file=output/dc_sync/yields_hcp_0jet_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs HCP-htt_et.inputs-sm-8TeV.root \
# --labels HCPYields --category eleTau_0jet_low --output_file=output/dc_sync/yields_hcp_0jet_low_et-sm-8TeV.pdf 
# ./bin/DatacardCompare --mode=2 --inputs HCP-htt_em.inputs-sm-8TeV.root  \
# --labels HCPYields   --category emu_vbf --output_file=output/dc_sync/yields_hcp_vbf_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs HCP-htt_em.inputs-sm-8TeV.root  \
# --labels HCPYields  --category emu_boost_high --output_file=output/dc_sync/yields_hcp_boost_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs HCP-htt_em.inputs-sm-8TeV.root  \
# --labels HCPYields  --category emu_boost_low --output_file=output/dc_sync/yields_hcp_boost_low_em-sm-8TeV.pdf      
# ./bin/DatacardCompare --mode=2 --inputs HCP-htt_em.inputs-sm-8TeV.root  \
# --labels HCPYields  --category emu_0jet_high --output_file=output/dc_sync/yields_hcp_0jet_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs HCP-htt_em.inputs-sm-8TeV.root  \
# --labels HCPYields  --category emu_0jet_low --output_file=output/dc_sync/yields_hcp_0jet_low_em-sm-8TeV.pdf


# ### DOnly
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_inclusive --output_file=output/dc_sync/yields_donly_inclusive_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_vbf --output_file=output/dc_sync/yields_donly_vbf_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_boost_high --output_file=output/dc_sync/yields_donly_boost_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_boost_low --output_file=output/dc_sync/yields_donly_boost_low_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_0jet_high --output_file=output/dc_sync/yields_donly_0jet_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_mt.inputs-sm-8TeV.root \
# --labels DOnlyYields  --category muTau_0jet_low --output_file=output/dc_sync/yields_donly_0jet_low_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_inclusive --output_file=output/dc_sync/yields_donly_inclusive_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_vbf --output_file=output/dc_sync/yields_donly_vbf_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_boost_high --output_file=output/dc_sync/yields_donly_boost_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_boost_low --output_file=output/dc_sync/yields_donly_boost_low_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_0jet_high --output_file=output/dc_sync/yields_donly_0jet_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs DOnly-htt_et.inputs-sm-8TeV.root \
# --labels DOnlyYields --category eleTau_0jet_low --output_file=output/dc_sync/yields_donly_0jet_low_et-sm-8TeV.pdf 
# ./bin/DatacardCompare --mode=2 --inputs DOnly-htt_em.inputs-sm-8TeV.root  \
# --labels DOnlyYields   --category emu_vbf --output_file=output/dc_sync/yields_donly_vbf_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs DOnly-htt_em.inputs-sm-8TeV.root  \
# --labels DOnlyYields  --category emu_boost_high --output_file=output/dc_sync/yields_donly_boost_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs DOnly-htt_em.inputs-sm-8TeV.root  \
# --labels DOnlyYields  --category emu_boost_low --output_file=output/dc_sync/yields_donly_boost_low_em-sm-8TeV.pdf      
# ./bin/DatacardCompare --mode=2 --inputs DOnly-htt_em.inputs-sm-8TeV.root  \
# --labels DOnlyYields  --category emu_0jet_high --output_file=output/dc_sync/yields_donly_0jet_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs DOnly-htt_em.inputs-sm-8TeV.root  \
# --labels DOnlyYields  --category emu_0jet_low --output_file=output/dc_sync/yields_donly_0jet_low_em-sm-8TeV.pdf




## Then do comparisons

# ## et with Wisconsin (HCP)
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_mt.inputs-sm-8TeV.root HCP-htt_mt.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial  --category muTau_vbf --output_file=output/dc_sync/compare_hcp_vbf_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_mt.inputs-sm-8TeV.root HCP-htt_mt.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial  --category muTau_boost_high --output_file=output/dc_sync/compare_hcp_boost_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_mt.inputs-sm-8TeV.root HCP-htt_mt.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial  --category muTau_boost_low --output_file=output/dc_sync/compare_hcp_boost_low_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_mt.inputs-sm-8TeV.root HCP-htt_mt.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial  --category muTau_0jet_high --output_file=output/dc_sync/compare_hcp_0jet_high_mt-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_mt.inputs-sm-8TeV.root HCP-htt_mt.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial  --category muTau_0jet_low --output_file=output/dc_sync/compare_hcp_0jet_low_mt-sm-8TeV.pdf
# ## mt with Wisconsin (HCP)
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_et.inputs-sm-8TeV.root HCP-htt_et.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial --category eleTau_vbf --output_file=output/dc_sync/compare_hcp_vbf_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_et.inputs-sm-8TeV.root HCP-htt_et.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial --category eleTau_boost_high --output_file=output/dc_sync/compare_hcp_boost_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_et.inputs-sm-8TeV.root HCP-htt_et.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial --category eleTau_boost_low --output_file=output/dc_sync/compare_hcp_boost_low_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_et.inputs-sm-8TeV.root HCP-htt_et.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial --category eleTau_0jet_high --output_file=output/dc_sync/compare_hcp_0jet_high_et-sm-8TeV.pdf
# ./bin/DatacardCompare --inputs output/JoshuaHCP-htt_et.inputs-sm-8TeV.root HCP-htt_et.inputs-sm-8TeV.root \
# --labels Wisconsin Imperial --category eleTau_0jet_low --output_file=output/dc_sync/compare_hcp_0jet_low_et-sm-8TeV.pdf  
# ## em with MIT (HCP) 
# ./bin/DatacardCompare --mode=2 --inputs output/ValentinaHCP-htt_em.inputs-sm-8TeV.root HCP-htt_em.inputs-sm-8TeV.root  \
# --labels MIT Imperial   --category emu_vbf --output_file=output/dc_sync/compare_hcp_vbf_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs output/ValentinaHCP-htt_em.inputs-sm-8TeV.root HCP-htt_em.inputs-sm-8TeV.root  \
# --labels MIT Imperial  --category emu_boost_high --output_file=output/dc_sync/compare_hcp_boost_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs output/ValentinaHCP-htt_em.inputs-sm-8TeV.root HCP-htt_em.inputs-sm-8TeV.root  \
# --labels MIT Imperial  --category emu_boost_low --output_file=output/dc_sync/compare_hcp_boost_low_em-sm-8TeV.pdf      
# ./bin/DatacardCompare --mode=2 --inputs output/ValentinaHCP-htt_em.inputs-sm-8TeV.root HCP-htt_em.inputs-sm-8TeV.root  \
# --labels MIT Imperial  --category emu_0jet_high --output_file=output/dc_sync/compare_hcp_0jet_high_em-sm-8TeV.pdf
# ./bin/DatacardCompare --mode=2 --inputs output/ValentinaHCP-htt_em.inputs-sm-8TeV.root HCP-htt_em.inputs-sm-8TeV.root  \
# --labels MIT Imperial  --category emu_0jet_low --output_file=output/dc_sync/compare_hcp_0jet_low_em-sm-8TeV.pdf


## et with Wisconsin (Moriond)
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_mt.inputs-sm-8TeV.root Moriond-htt_mt.inputs-sm-8TeV.root \
--labels Wisconsin Imperial  --category muTau_vbf --output_file=output/dc_sync/compare_moriond_vbf_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_mt.inputs-sm-8TeV.root Moriond-htt_mt.inputs-sm-8TeV.root \
--labels Wisconsin Imperial  --category muTau_boost_high --output_file=output/dc_sync/compare_moriond_boost_high_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_mt.inputs-sm-8TeV.root Moriond-htt_mt.inputs-sm-8TeV.root \
--labels Wisconsin Imperial  --category muTau_boost_low --output_file=output/dc_sync/compare_moriond_boost_low_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_mt.inputs-sm-8TeV.root Moriond-htt_mt.inputs-sm-8TeV.root \
--labels Wisconsin Imperial  --category muTau_0jet_high --output_file=output/dc_sync/compare_moriond_0jet_high_mt-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_mt.inputs-sm-8TeV.root Moriond-htt_mt.inputs-sm-8TeV.root \
--labels Wisconsin Imperial  --category muTau_0jet_low --output_file=output/dc_sync/compare_moriond_0jet_low_mt-sm-8TeV.pdf
## mt with Wisconsin (Moriond)
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_et.inputs-sm-8TeV.root Moriond-htt_et.inputs-sm-8TeV.root \
--labels Wisconsin Imperial --category eleTau_vbf --output_file=output/dc_sync/compare_moriond_vbf_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_et.inputs-sm-8TeV.root Moriond-htt_et.inputs-sm-8TeV.root \
--labels Wisconsin Imperial --category eleTau_boost_high --output_file=output/dc_sync/compare_moriond_boost_high_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_et.inputs-sm-8TeV.root Moriond-htt_et.inputs-sm-8TeV.root \
--labels Wisconsin Imperial --category eleTau_boost_low --output_file=output/dc_sync/compare_moriond_boost_low_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_et.inputs-sm-8TeV.root Moriond-htt_et.inputs-sm-8TeV.root \
--labels Wisconsin Imperial --category eleTau_0jet_high --output_file=output/dc_sync/compare_moriond_0jet_high_et-sm-8TeV.pdf
./bin/DatacardCompare --inputs output/JoshuaMoriond-htt_et.inputs-sm-8TeV.root Moriond-htt_et.inputs-sm-8TeV.root \
--labels Wisconsin Imperial --category eleTau_0jet_low --output_file=output/dc_sync/compare_moriond_0jet_low_et-sm-8TeV.pdf  
## em with MIT (Moriond) 
./bin/DatacardCompare --mode=2 --inputs output/ValentinaMoriond-htt_em.inputs-sm-8TeV.root Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MIT Imperial   --category emu_vbf --output_file=output/dc_sync/compare_moriond_vbf_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs output/ValentinaMoriond-htt_em.inputs-sm-8TeV.root Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MIT Imperial  --category emu_boost_high --output_file=output/dc_sync/compare_moriond_boost_high_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs output/ValentinaMoriond-htt_em.inputs-sm-8TeV.root Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MIT Imperial  --category emu_boost_low --output_file=output/dc_sync/compare_moriond_boost_low_em-sm-8TeV.pdf      
./bin/DatacardCompare --mode=2 --inputs output/ValentinaMoriond-htt_em.inputs-sm-8TeV.root Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MIT Imperial  --category emu_0jet_high --output_file=output/dc_sync/compare_moriond_0jet_high_em-sm-8TeV.pdf
./bin/DatacardCompare --mode=2 --inputs output/ValentinaMoriond-htt_em.inputs-sm-8TeV.root Moriond-htt_em.inputs-sm-8TeV.root  \
--labels MIT Imperial  --category emu_0jet_low --output_file=output/dc_sync/compare_moriond_0jet_low_em-sm-8TeV.pdf





 

