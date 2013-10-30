#!/bin/bash
echo Datacard Interpolator for VBF Higgs to Invisible Analysis
CARDDIR="../vbfcards/PostPASCards/"
XSDATFILE="../data/lhc-hxswg/sm/xs/8TeV/8TeV-vbfH.txt"
mkdir sourceuncs

#GET INFO AT MASSES WE HAVE CARDS FOR
echo Processing input cards:
for card in ${CARDDIR}vbfhinv_*.txt
do
    echo "   "$card
    #GET MASS FROM CARD NAME
    mass=`echo $card | sed "s:${CARDDIR}vbfhinv_::" | sed "s/_8TeV.txt/.0/"`
    #GET XS INFO FROM LHC-HXSWG FILE
    grep "^$mass" $XSDATFILE > tmp.txt
    xs=`cat tmp.txt | awk '{print $2}'`
    rm tmp.txt

    #GET SIGNAL YIELD INFO FROM CARDS
    yield=`grep "rate" $card | awk '{print $2}'`
    
    echo $mass $xs $yield >>inputinfo.txt

    #GET FIRST ERROR IN CARDS
    grep -A 1 -e "----------" $card > tmp.txt
    firsterr=`tail -1 tmp.txt | awk '{print $1}'`
    
    #GET ERROR INFO FROM CARDS
    for sources in `grep -A 10000 "$firsterr" $card | awk '{print $1}'`
    do
	#CHECK SOURCE TYPE - CURRENTLY lnN and gmN DEALT WITH
	if [ `grep "$sources" $card | awk '{print $2}'` = "lnN" ]
	then
	    rawerror=`grep "$sources" $card | awk '{print $3}'`
	    
	elif [ `grep "$sources" $card | awk '{print $2}'` = "gmN" ]
	then
	    rawerror=`grep "$sources" $card | awk '{print $4}'`
	else
	    echo ERROR: UNRECOGNISED ERROR TYPE
	    exit 1
	fi

	if [ $rawerror = "-" ]
	then
	#ERROR DOESN'T AFFECT SIGNAL
	    echo $mass noeff >> sourceuncs/$sources.txt
	else
	    #CHECK FOR ASYMMETRIC ERROR ON SIGNAL
	    echo $rawerror | grep "/" > /dev/null
	    if [ $? = 0 ]
	    then
		downerror=`echo $rawerror | sed "s/\// /" | awk '{print $1}'`
		uperror=`echo $rawerror | sed "s/\// /" | awk '{print $2}'`
		echo $mass asym $downerror $uperror >> sourceuncs/$sources.txt
	    else
	    #ERROR IS SYMMETRIC FOR SIGNAL
		echo $mass sym $rawerror >> sourceuncs/$sources.txt
	    fi
	fi

    done
done


#GET INFO AT MASSES WE WANT CARDS FOR

rm -r $CARDDIR/InterpolatedCards
mkdir $CARDDIR/InterpolatedCards

echo Making new cards for:
newmasses="115 125 135 145"
for newmass in $newmasses
do
    echo "   mH =" $newmass GeV
    #GET NEW XS 
    grep "^${newmass}.0" $XSDATFILE > tmp.txt
    xs=`cat tmp.txt | awk '{print $2}'`
    echo $newmass $xs > outputxsinfo.txt
    rm tmp.txt

#MAKE NEW CARDS
    #FIRST 12 LINES ARE UNCHANGED
    grep -m 12 "" ${CARDDIR}vbfhinv_125_8TeV.txt >${CARDDIR}/InterpolatedCards/vbfhinv_${newmass}_8TeV.txt 

    #PUT NEW RATE IN
    root -l -q newyield.cpp >tmp.txt
    rate=`cat tmp.txt | grep "rate" | awk '{print $2}'`
    rm tmp.txt
    oldrate=`grep "rate" ${CARDDIR}vbfhinv_125_8TeV.txt | awk '{print $2}'`                                                                                
    grep "rate" ${CARDDIR}vbfhinv_125_8TeV.txt | sed "s:$oldrate:$rate:" >>${CARDDIR}/InterpolatedCards/vbfhinv_${newmass}_8TeV.txt

    #LINE 14 UNCHANGED
    echo ------------ >>${CARDDIR}/InterpolatedCards/vbfhinv_${newmass}_8TeV.txt 

    #PUT NEW ERRORS IN
    for sources in `grep -A 10000 "$firsterr" $CARDDIR/vbfhinv_125_8TeV.txt | awk '{print $1}'`
    do
	if [ `grep -m 1 "" sourceuncs/$sources.txt | awk '{print $2}'` = "noeff" ]
        then
        #ERROR DOESN'T AFFECT SIGNAL                                                                                                                   
	    type=noeff
        elif [ `grep -m 1 "" sourceuncs/$sources.txt | awk '{print $2}'` = "asym" ]
        then
            #ERROR IS ASYMMETRIC FOR SIGNAL                                                                                                            
	    type=asym
	elif [ `grep -m 1 "" sourceuncs/$sources.txt | awk '{print $2}'` = "sym" ]
	then
            #ERROR IS SYMMETRIC FOR SIGNAL                                                                                                              
		type=sym
        fi
        
	fileforrootmacro=sourceuncs/$sources.txt
	root -l -b -q newunc.cpp"("'"'$fileforrootmacro'"','"'$type'"',$newmass")" > tmp.txt
	err=`cat tmp.txt | grep "newerror" |awk '{print $2}'` #SET ERR EQUAL TO APPROPRIATE BIT OF CPP OUTPUT
	rm tmp.txt
	
	if [ `grep "$sources" $CARDDIR/vbfhinv_125_8TeV.txt | awk '{print $2}'` = "lnN" ]
        then
            olderr=`grep "$sources" $CARDDIR/vbfhinv_125_8TeV.txt | awk '{print $3}'`

        elif [ `grep "$sources" $CARDDIR/vbfhinv_125_8TeV.txt | awk '{print $2}'` = "gmN" ]
        then
            olderr=`grep "$sources" $CARDDIR/vbfhinv_125_8TeV.txt | awk '{print $4}'`
        else
            echo ERROR: UNRECOGNISED ERROR TYPE SHOULDN"'"T HAVE GOT THIS FAR
	    exit 1
        fi

	grep "$sources" ${CARDDIR}vbfhinv_125_8TeV.txt | sed "s:$olderr:$err:" >>${CARDDIR}/InterpolatedCards/vbfhinv_${newmass}_8TeV.txt
    done


    rm outputxsinfo.txt
done
rm inputinfo.txt
rm -r sourceuncs
echo Interpolated datacards successfully created at: $CARDDIR/InterpolatedCards/