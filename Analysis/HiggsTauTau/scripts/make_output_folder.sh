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
mkdir -p -v $1/JES_UP
mkdir -p -v $1/JES_DOWN
mkdir -p -v $1/MET_UP
mkdir -p -v $1/MET_DOWN
