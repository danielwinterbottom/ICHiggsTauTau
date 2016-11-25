#!/bin/bash
carddir=./
mass=125
card=vbf_hinv_13TeV_2016.txt
nsysts=34
nsystsminus1=33

thisdir=`pwd`
cd $carddir/

echo "Uncertainty effect estimator script"

#extraoption="--freezeNuisances WZ_xsection,QCD_xsection" 
#extraoption="--run expected" #--freezeNuisances Z_xsection,QCD_xsection,W_el_xsection,W_mu_xsection,W_tau_xsection" 
extraoption="--run blind"

#GET FIRST ERROR IN CARD
grep -A 1 -e "-------" $card >tmp.txt
firsterr=`tail -1 tmp.txt | awk '{print $1}'`
echo "First err: $firsterr"
rm tmp.txt

#RUN WITH ALL NUISANCES
echo "   "Getting expected limit with all nuisances
combine -M Asymptotic -m $mass $extraoption $card &> tmpcombresult.txt
allnuismedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
rm tmpcombresult.txt

#GET CARD WITH NO NUISANCES
#numofnuis=`grep "number of nuisance parameters" $card | awk '{print $2}'`
#newnumofnuis=0
#cat $card | sed s:"kmax $numofnuis ":"kmax $newnumofnuis ": >nonuiscard.txt
cat $card | sed s:"kmax $nsysts":"kmax 0": >nonuiscard.txt
#echo "kmax *" >nonuiscard.txt

for sources in `grep -A 10000 "$firsterr" $card | awk '{print $1}'`
do
  if [ "$sources" != "QCD_xsection" -a "$sources" != "WZ_xsection" -a "$sources" != "Z_xsection" -a "$sources" != "W_el_xsection" -a "$sources" != "W_mu_xsection" -a "$sources" != "W_tau_xsection" ]; then
      echo $sources
      cat nonuiscard.txt > tmpcard1.txt
      cat tmpcard1.txt | sed s:$sources:'#'$sources: > nonuiscard.txt
      rm tmpcard1.txt
  fi
done

#RUN WITH NO NUISANCES
echo "   "Getting expected limit with no nuisances
combine -M Asymptotic -m $mass $extraoption nonuiscard.txt &> tmpcombresult.txt
nonuismedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
rm tmpcombresult.txt

allnuislimit=`echo "100*$allnuismedexp"|bc -l`
nonuislimit=`echo "100*$nonuismedexp"|bc -l`

printf "Median expected limit with: & All Nuisances: %.1f\\%% & No Nuisances: %.1f\\%% \\\\\\\\ \n" "$allnuislimit" "$nonuislimit"

printf "Nuisance      &                Removal effect &  Addition effect \\\\\\\\ \n"
#LOOP OVER NUISANCES
for sources in `grep -A 10000 "$firsterr" $card | awk '{print $1}'`
do
  if [ "$sources" != "QCD_xsection" -a "$sources" != "WZ_xsection" ]; then
    sourceout=`echo ${sources}|sed s:_:'\\\\'_:g`":"
    printf "%-30s &" "$sourceout"
    #GET CARD WITHOUT NUISANCE
    #numofnuis=`grep "number of nuisance parameters" $card | awk '{print $2}'`
    #newnumofnuis=$[$numofnuis-1]
    #cat $card | sed s:"kmax $numofnuis ":"kmax $newnumofnuis ": >tmpcard1.txt
    cat $card >tmpcard1.txt
    cat tmpcard1.txt | sed s:$sources:'#'$sources: > tmpcard.txt
    cat tmpcard.txt | sed "s:kmax $nsysts:kmax $nsystsminus1:" > tmpcard1.txt

    #cat tmpcard1.txt

    #RUN ON CARD WITHOUT NUISANCE
    combine -M Asymptotic -m $mass $extraoption tmpcard1.txt &> tmpcombresult.txt

    if (( "$?" != "0" )); then
	echo "Problem with limit command, stopping script..."
	exit;
    fi

    nuissubtmedexp=`grep "Expected 50\.0%" tmpcombresult.txt | awk '{print $5}'`
    deltasubt=`echo "$nuissubtmedexp-$allnuismedexp"|bc -l`
    percdiffsubt=`echo "100*$deltasubt/$allnuismedexp"|bc -l`
    rm tmpcard.txt tmpcard1.txt tmpcombresult.txt

    #GET CARD WITH ONLY THIS NUISANCE
    cat nonuiscard.txt | sed s:"kmax 0 ":"kmax 1 ": >tmpcard1.txt
    cat tmpcard1.txt | sed s:'#'$sources:$sources: > tmpcard.txt

    #RUN ON CARD WITH ONLY THIS NUISANCE
    combine -M Asymptotic -m $mass $extraoption tmpcard.txt &> tmpcombresult.txt
    nuisonlymedexp=`grep "Expected 50.0%" tmpcombresult.txt | awk '{print $5}'`
    deltaonly=`echo "$nuisonlymedexp-$nonuismedexp"|bc -l`
    percdiffonly=`echo "100*$deltaonly/$nonuismedexp"|bc -l`
    rm tmpcard.txt tmpcard1.txt tmpcombresult.txt

    #echo Result with nuisance subtracted: $nuissubtmedexp, result with only this Nuisance: $nuisonlymedexp
    
    outpercdiffsubt=`printf "%.1f%%" "$percdiffsubt"`
    #printf "%-29s %.1f%% \n" "$outpercdiffsubt" "$percdiffonly"
    printf "%5.1f\\%%                &        %5.1f\\%% \\\\\\\\ \n" "$percdiffsubt" "$percdiffonly"
  fi
done

rm nonuiscard.txt

cd $thisdir
