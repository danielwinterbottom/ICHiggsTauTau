cd ../../Modules
make -j4
cd ../Analysis
make -j4
cd HiggsTauTau

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_inclusive --output_file=output/dc_sync/compare_inclusive_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_vbf --output_file=output/dc_sync/compare_vbf_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_boost_high --output_file=output/dc_sync/compare_boost_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_boost_low --output_file=output/dc_sync/compare_boost_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_0jet_high --output_file=output/dc_sync/compare_0jet_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_0jet_low --output_file=output/dc_sync/compare_0jet_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_btag_high --output_file=output/dc_sync/compare_btag_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category muTau_btag_low --output_file=output/dc_sync/compare_btag_low_mt-sm-7TeV-mvis.pdf


# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root \
# --labels Yields --category eleTau_inclusive --output_file=output/dc_sync/compare_inclusive_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root \
# --labels Yields --category eleTau_vbf --output_file=output/dc_sync/compare_vbf_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root \
# --labels Yields --category eleTau_boost_high --output_file=output/dc_sync/compare_boost_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category eleTau_boost_low --output_file=output/dc_sync/compare_boost_low_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category eleTau_0jet_high --output_file=output/dc_sync/compare_0jet_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category eleTau_0jet_low --output_file=output/dc_sync/compare_0jet_low_et-sm-7TeV-mvis.pdf  

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category eleTau_btag_high --output_file=output/dc_sync/compare_btag_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Yields --category eleTau_btag_low --output_file=output/dc_sync/compare_btag_low_et-sm-7TeV-mvis.pdf  


# echo "SVFIT cards:"

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_mt-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_mt-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_mt-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_mt-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_mt-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV.root \
--labels OLD NEW  --category muTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_mt-sm-7TeV.pdf


./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_et-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_et-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_et-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_et-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_et-sm-7TeV.pdf

./bin/DatacardCompare --inputs output/Andrew-htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV.root \
--labels OLD NEW --category eleTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_et-sm-7TeV.pdf  



# echo "MVIS cards:"


# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_mt.inputs-sm-7TeV-mvis-OLD.root htt_mt.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_mt-sm-7TeV-mvis.pdf


# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_et-sm-7TeV-mvis.pdf  

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs htt_et.inputs-sm-7TeV-mvis-OLD.root htt_et.inputs-sm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_et-sm-7TeV-mvis.pdf  


echo "SVFIT cards:"

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_nobtag --output_file=output/dc_sync/compare_new_old_nobtag_mt-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-OLD.root htt_mt.inputs-mssm-7TeV.root \
# --labels OLD NEW  --category muTau_btag --output_file=output/dc_sync/compare_new_old_btag_mt-mssm-7TeV.pdf


# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_et-mssm-7TeV.pdf  

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_et-mssm-7TeV.pdf  

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_nobtag --output_file=output/dc_sync/compare_new_old_nobtag_et-mssm-7TeV.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-OLD.root htt_et.inputs-mssm-7TeV.root \
# --labels OLD NEW --category eleTau_btag --output_file=output/dc_sync/compare_new_old_btag_et-mssm-7TeV.pdf  


# echo "MVIS cards:"


# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_nobtag --output_file=output/dc_sync/compare_new_old_nobtag_mt-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_mt.inputs-mssm-7TeV-mvis-OLD.root htt_mt.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW  --category muTau_btag --output_file=output/dc_sync/compare_new_old_btag_mt-mssm-7TeV-mvis.pdf


# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_inclusive --output_file=output/dc_sync/compare_new_old_inclusive_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_vbf --output_file=output/dc_sync/compare_new_old_vbf_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_boost_high --output_file=output/dc_sync/compare_new_old_boost_high_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_boost_low --output_file=output/dc_sync/compare_new_old_boost_low_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_0jet_high --output_file=output/dc_sync/compare_new_old_0jet_high_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_0jet_low --output_file=output/dc_sync/compare_new_old_0jet_low_et-mssm-7TeV-mvis.pdf  

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_btag_high --output_file=output/dc_sync/compare_new_old_btag_high_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_btag_low --output_file=output/dc_sync/compare_new_old_btag_low_et-mssm-7TeV-mvis.pdf  

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_nobtag --output_file=output/dc_sync/compare_new_old_nobtag_et-mssm-7TeV-mvis.pdf

# ./bin/DatacardCompare --mssm_mode=1 --inputs htt_et.inputs-mssm-7TeV-mvis-OLD.root htt_et.inputs-mssm-7TeV-mvis.root \
# --labels OLD NEW --category eleTau_btag --output_file=output/dc_sync/compare_new_old_btag_et-mssm-7TeV-mvis.pdf  

























# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-ZTTMC/htt_et.inputs-sm-7TeV-mvis-mvis.root  \
# --labels Embedded ZTT-MC --category eleTau_inclusive --output_file=output/dc_sync/ztt_comp_inclusive_et-sm-7TeV-mvis-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-ZTTMC/htt_mt.inputs-sm-7TeV-mvis-mvis.root  \
# --labels Embedded ZTT-MC --category muTau_inclusive --output_file=output/dc_sync/ztt_comp_inclusive-mt-sm-7TeV-mvis-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category muTau_inclusive --output_file=output/dc_sync/iso_comp_inclusive_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category muTau_boost_high --output_file=output/dc_sync/iso_comp_boost_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category muTau_boost_low --output_file=output/dc_sync/iso_comp_boost_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category muTau_0jet_high --output_file=output/dc_sync/iso_comp_0jet_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category muTau_0jet_low --output_file=output/dc_sync/iso_comp_0jet_low_mt-sm-7TeV-mvis.pdf



# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category eleTau_inclusive --output_file=output/dc_sync/iso_comp_inclusive_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category eleTau_boost_high --output_file=output/dc_sync/iso_comp_boost_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category eleTau_boost_low --output_file=output/dc_sync/iso_comp_boost_low_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category eleTau_0jet_high --output_file=output/dc_sync/iso_comp_0jet_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-SPEC16/htt_mt.inputs-sm-7TeV-mvis.root \
# --labels Anti-Iso Full-Iso Iso-0p3 --category eleTau_0jet_low --output_file=output/dc_sync/iso_comp_0jet_low_et-sm-7TeV-mvis.pdf  




# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category muTau_inclusive --output_file=output/dc_sync/iso_comp_inclusive_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category muTau_boost_high --output_file=output/dc_sync/iso_comp_boost_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category muTau_boost_low --output_file=output/dc_sync/iso_comp_boost_low_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category muTau_0jet_high --output_file=output/dc_sync/iso_comp_0jet_high_mt-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_mt.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_mt.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category muTau_0jet_low --output_file=output/dc_sync/iso_comp_0jet_low_mt-sm-7TeV-mvis.pdf



# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category eleTau_inclusive --output_file=output/dc_sync/iso_comp_inclusive_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category eleTau_boost_high --output_file=output/dc_sync/iso_comp_boost_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category eleTau_boost_low --output_file=output/dc_sync/iso_comp_boost_low_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category eleTau_0jet_high --output_file=output/dc_sync/iso_comp_0jet_high_et-sm-7TeV-mvis.pdf

# ./bin/DatacardCompare --inputs HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15/htt_et.inputs-sm-7TeV-mvis.root HCP_SM_2011_MT-30_1JET-MET-15_0JET-MET-15-NONMASSMETHOD/htt_et.inputs-sm-7TeV-mvis.root  \
# --labels Anti-Iso Full-Iso --category eleTau_0jet_low --output_file=output/dc_sync/iso_comp_0jet_low_et-sm-7TeV-mvis.pdf  











# ./bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin   --category muTau_vbf --output_file=output/dc_sync/ic_wisc_vbf_mt-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category muTau_boost_high --output_file=output/dc_sync/ic_wisc_boost_high_mt-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category muTau_boost_low --output_file=output/dc_sync/ic_wisc_boost_low_mt-sm-8TeV.pdf      

# ./bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category muTau_0jet_high --output_file=output/dc_sync/ic_wisc_0jet_high_mt-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category muTau_0jet_low --output_file=output/dc_sync/ic_wisc_0jet_low_mt-sm-8TeV.pdf


# ./bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin   --category eleTau_vbf --output_file=output/dc_sync/ic_wisc_vbf_et-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_boost_high --output_file=output/dc_sync/ic_wisc_boost_high_et-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_boost_low --output_file=output/dc_sync/ic_wisc_boost_low_et-sm-8TeV.pdf      

# ./bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_0jet_high --output_file=output/dc_sync/ic_wisc_0jet_high_et-sm-8TeV.pdf

# ./bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root output/dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_0jet_low --output_file=output/dc_sync/ic_wisc_0jet_low_et-sm-8TeV.pdf


# ./bin/DatacardCompare --mode=2 --inputs Andrew_htt_em.inputs-sm-8TeV.root output/dc_sync/MIT-htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT   --category emu_vbf --output_file=output/dc_sync/ic_mit_vbf_em-sm-8TeV.pdf

# ./bin/DatacardCompare --mode=2 --inputs Andrew_htt_em.inputs-sm-8TeV.root output/dc_sync/MIT-htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_boost_high --output_file=output/dc_sync/ic_mit_boost_high_em-sm-8TeV.pdf

# ./bin/DatacardCompare --mode=2 --inputs Andrew_htt_em.inputs-sm-8TeV.root output/dc_sync/MIT-htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_boost_low --output_file=output/dc_sync/ic_mit_boost_low_em-sm-8TeV.pdf      

# ./bin/DatacardCompare --mode=2 --inputs Andrew_htt_em.inputs-sm-8TeV.root output/dc_sync/MIT-htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_0jet_high --output_file=output/dc_sync/ic_mit_0jet_high_em-sm-8TeV.pdf

# ./bin/DatacardCompare --mode=2 --inputs Andrew_htt_em.inputs-sm-8TeV.root output/dc_sync/MIT-htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_0jet_low --output_file=output/dc_sync/ic_mit_0jet_low_em-sm-8TeV.pdf
  


 

