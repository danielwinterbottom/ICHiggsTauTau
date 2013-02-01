if [ -z $1 ]
then
    echo "Usage: make_output_folder.sh [output/some_folder]"
    exit
fi

mkdir -p -v $1/TSCALE_UP
mkdir -p -v $1/TSCALE_DOWN
