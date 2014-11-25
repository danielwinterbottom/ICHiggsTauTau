if [ -z $1 ]
then
    echo "Usage: make_output_folder.sh [output/some_folder]"
    exit
fi

mkdir -p -v $1/TSCALE_UP
mkdir -p -v $1/TSCALE_DOWN
mkdir -p -v $1/BTAG_UP
mkdir -p -v $1/BTAG_DOWN
mkdir -p -v $1/BFAKE_UP
mkdir -p -v $1/BFAKE_DOWN
mkdir -p -v $1/JES_UP
mkdir -p -v $1/JES_DOWN
mkdir -p -v $1/MET_UP
mkdir -p -v $1/MET_DOWN
