echo The following jobs did not finish:

grep -iEL 'finished' jobs/duplicate*.out

echo The following jobs produced outputs with duplicated events:

#grep -iEl 'duplicated event' jobs/duplicate*.out

for x in $(grep -iEl 'duplicated event' jobs/duplicate*.out); do

  echo $x
  head -1 $x
done

