# command line argument (crab directory, eg. Feb27_MC_102X)
export folder=$1

# get the subdirectories and pipe into dirs.txt
for dir in $(uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls store/user/adow/${folder}/*" | tail -n +3 | awk '{print $8}'); do 
    echo $dir >> dirs.txt
done

# for each subdirectory, get the files and remove the unwanted prefices
for line in $(cat dirs.txt); do 
    uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls ${line}/*/*" | grep -v failed | tail -n +3 | awk '{print $8}' | \
        sed "s/store\/user\/adow\/${folder}\///g" > "${folder}_$(basename $line | sed 's/crab_//g').dat"
done

# remove the temporary directory filelist
rm dirs.txt

# tar files into one
# tar -czvf ${folder}.tgz ${folder}_*.dat
