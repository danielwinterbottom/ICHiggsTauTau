if [ -z $1 ]
then
    echo "Usage: make_output_folder.sh [output/some_folder]"
    exit
fi

mkdir -p -v $1/TSCALE_UP
mkdir -p -v $1/TSCALE_DOWN
mkdir -p -v $1/TSCALE_UP_1
mkdir -p -v $1/TSCALE_DOWN_1
mkdir -p -v $1/TSCALE_UP_2
mkdir -p -v $1/TSCALE_DOWN_2
mkdir -p -v $1/TSCALE_UP_3
mkdir -p -v $1/TSCALE_DOWN_3
mkdir -p -v $1/TSCALE_UP_0.5
mkdir -p -v $1/TSCALE_DOWN_0.5
mkdir -p -v $1/TSCALE_UP_1.5
mkdir -p -v $1/TSCALE_DOWN_1.5
mkdir -p -v $1/TSCALE_UP_2.5
mkdir -p -v $1/TSCALE_DOWN_2.5
mkdir -p -v $1/BTAG_UP
mkdir -p -v $1/BTAG_DOWN
mkdir -p -v $1/BFAKE_UP
mkdir -p -v $1/BFAKE_DOWN
mkdir -p -v $1/HF_UP
mkdir -p -v $1/HF_DOWN
mkdir -p -v $1/HFSTATS1_UP
mkdir -p -v $1/HFSTATS1_DOWN
mkdir -p -v $1/HFSTATS2_UP
mkdir -p -v $1/HFSTATS2_DOWN
mkdir -p -v $1/CFERR1_UP
mkdir -p -v $1/CFERR1_DOWN
mkdir -p -v $1/CFERR2_UP
mkdir -p -v $1/CFERR2_DOWN
mkdir -p -v $1/LF_UP
mkdir -p -v $1/LF_DOWN
mkdir -p -v $1/LFSTATS1_UP
mkdir -p -v $1/LFSTATS1_DOWN
mkdir -p -v $1/LFSTATS2_UP
mkdir -p -v $1/LFSTATS2_DOWN
mkdir -p -v $1/JES_UP
mkdir -p -v $1/JES_DOWN
mkdir -p -v $1/MET_SCALE_UP
mkdir -p -v $1/MET_SCALE_DOWN
mkdir -p -v $1/MET_RES_UP
mkdir -p -v $1/MET_RES_DOWN
mkdir -p -v $1/EFAKE0PI_DOWN
mkdir -p -v $1/EFAKE0PI_UP
mkdir -p -v $1/EFAKE1PI_DOWN
mkdir -p -v $1/EFAKE1PI_UP
