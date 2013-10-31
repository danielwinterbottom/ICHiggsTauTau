#!/bin/bash
carddir=../PASallcards/
mass=125
card=invcombinedcard${mass}.txt

thisdir=`pwd`
cd $carddir/$mass

echo Uncertainty effect estimator script

#GET FIRST ERROR IN CARD
grep -A 1 -e "-------" $card >tmp.txt
firsterr=`tail -1 tmp.txt | awk '{print $1}'`

#RUN WITH ALL NUISANCES
echo "   "Getting expected limit with all nuisances
combine -M Asymptotic -m $mass $card > tmpcombresult.txt
allnuismedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
rm tmpcombresult.txt

#GET CARD WITH NO NUISANCES
numofnuis=`grep "number of nuisance parameters" $card | awk '{print $2}'`
newnumofnuis=0
cat $card | sed s:"kmax $numofnuis number of nuisance parameters":"kmax $newnumofnuis number of nuisance parameters": >nonuiscard.txt

for sources in `grep -A 10000 "$firsterr" $card | awk '{print $1}'`
do
    cat nonuiscard.txt > tmpcard1.txt
    cat tmpcard1.txt | sed s:$sources" ":'#'$sources" ": > nonuiscard.txt
    rm tmpcard1.txt
done

#RUN WITH NO NUISANCES
echo "   "Getting expected limit with no nuisances
combine -M Asymptotic -m $mass nonuiscard.txt > tmpcombresult.txt
nonuismedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
rm tmpcombresult.txt

echo Median expected limit with:
echo "   "All Nuisances: $allnuismedexp, No Nuisances: $nonuismedexp

#LOOP OVER NUISANCES
for sources in `grep -A 10000 "$firsterr" $card | awk '{print $1}'`
do
    echo ${sources}":"
    #GET CARD WITHOUT NUISANCE
    numofnuis=`grep "number of nuisance parameters" $card | awk '{print $2}'`
    newnumofnuis=$[$numofnuis-1]
    cat $card | sed s:"kmax $numofnuis number of nuisance parameters":"kmax $newnumofnuis number of nuisance parameters": >tmpcard1.txt
    cat tmpcard1.txt | sed s:$sources" ":'#'$sources" ": > tmpcard.txt

    #RUN ON CARD WITHOUT NUISANCE
    combine -M Asymptotic -m $mass tmpcard.txt > tmpcombresult.txt
    nuissubtmedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
    deltasubt=`echo "$nuissubtmedexp-$allnuismedexp"|bc -l`
    percdiffsubt=`echo "100*$deltasubt/$allnuismedexp"|bc -l`
    rm tmpcard.txt tmpcard1.txt tmpcombresult.txt

    #GET CARD WITH ONLY THIS NUISANCE
    cat nonuiscard.txt | sed s:"kmax 0 number of nuisance parameters":"kmax 1 number of nuisance parameters": >tmpcard1.txt
    cat tmpcard1.txt | sed s:'#'$sources" ":$sources" ": > tmpcard.txt

    #RUN ON CARD WITH ONLY THIS NUISANCE
    combine -M Asymptotic -m $mass tmpcard.txt > tmpcombresult.txt
    nuisonlymedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
    deltaonly=`echo "$nuisonlymedexp-$nonuismedexp"|bc -l`
    percdiffonly=`echo "100*$deltaonly/$nonuismedexp"|bc -l`
    rm tmpcard.txt tmpcard1.txt tmpcombresult.txt

    #echo Result with nuisance subtracted: $nuissubtmedexp, result with only this Nuisance: $nuisonlymedexp
    printf "Nuisance subtracted: %.1f%% Nuisance only: %.1f%% \n" "$percdiffsubt" "$percdiffonly"
done

cd $thisdir