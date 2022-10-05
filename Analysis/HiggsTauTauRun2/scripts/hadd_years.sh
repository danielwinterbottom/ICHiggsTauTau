dir="vlq_distributions_v14"
#dir="vlq_jpt_1"
for c in mt tt et; do
  mkdir -p $dir/$c/all
done

for x in $dir/*/2018/datacard*.root; do

  y=${x//2018/"2017"} 
  z=${x//2018/"2016"} 
  out=${x//2018/"all"} 

  echo hadding $out

  hadd -f $out $x $y $z

done
