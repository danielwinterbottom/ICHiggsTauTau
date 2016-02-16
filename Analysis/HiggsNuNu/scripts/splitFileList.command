for file in `ls long/*.dat | sed "s/\.dat//" | sed "s/long\///"`; do split -l 100 -d long/$file.dat medium/${file}_split; done
