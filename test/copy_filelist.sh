# command line argument (crab directory followed by CERN user, eg. Feb27_MC_102X adow)
export folder=$1
export user=$2

rm dirs.txt
# get the subdirectories and pipe into dirs.txt
for dir in $(uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls store/user/${user}/${folder}/*" | tail -n +3 | awk '{print $8}'); do 
    echo $dir >> dirs.txt
done

# for each subdirectory, get the files and remove the unwanted prefices
for line in $(cat dirs.txt); do 
    echo "Finding $line"
    # export lastdate=0
    # export lasttime=0
    # export latest=0
    # latest=$(uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls ${line}/" | awk '{print $9}' | tail -n -1)
    # # uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls ${line}/" | awk '{print $9}' | cut -d "_" -f 1 >> dates.txt
    # for full_date in $(cat full_dates.txt); do
    #     only_date=$(echo $full_date | cut -d "_" -f 1)
    #     only_time=$(echo $full_date | cut -d "_" -f 2)
    #     if [[ $only_date > $lastdate ]]; then
    #         latest=$full_date
    #     else 
    #         if [[ $only_date == $lastdate ]]; then
    #             if [[ $only_time > $lasttime ]] ; then
    #                 latest=$full_date
    #             fi
    #         fi
    #     fi
    # done
    # export newline=$(echo ${line}/${latest})
    # uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls ${line}/${latest}/*" | grep -v failed | tail -n +3 | awk '{print $8}' | \
    uberftp sedsk53.grid.hep.ph.ic.ac.uk "ls ${line}/*/*" | grep -v failed | tail -n +3 | awk '{print $8}' | \
        sed "s/store\/user\/${user}\/${folder}\///g" > "${folder}_$(basename $line | sed 's/crab_//g').dat"
done

# remove the temporary directory filelist
rm dirs.txt

# tar files into one
# tar -czvf ${folder}.tgz ${folder}_*.dat
