python scripts/plot_cp_angle.py --file=cp_files/gen_vars/hadronic_reweight_1.root --cp_channel=1 --output_name="cp_files/gen_vars/hadronic_reweight_cp_channel_1" --title="hadronic" --reweighted
python scripts/plot_cp_angle.py --file=cp_files/gen_vars/hadronic_reweight_2.root --cp_channel=2 --output_name="cp_files/gen_vars/hadronic_reweight_cp_channel_2" --title="hadronic" --reweighted
python scripts/plot_cp_angle.py --file=cp_files/gen_vars/hadronic_reweight_3.root --cp_channel=3 --output_name="cp_files/gen_vars/hadronic_reweight_cp_channel_3" --title="hadronic" --reweighted

python scripts/plot_cp_angle.py --file=cp_files/gen_vars/semileptonic_reweight_1.root --cp_channel=1 --output_name="cp_files/gen_vars/semileptonic_reweight_cp_channel_1" --title="semi-leptonic" --reweighted
python scripts/plot_cp_angle.py --file=cp_files/gen_vars/semileptonic_reweight_2.root --cp_channel=2 --output_name="cp_files/gen_vars/semileptonic_reweight_cp_channel_2" --title="semi-leptonic" --reweighted

python scripts/plot_cp_angle.py --file=cp_files/gen_vars/leptonic_reweight_1.root --cp_channel=1 --output_name="cp_files/gen_vars/leptonic_reweight_cp_channel_1" --title="leptonic"  --reweighted