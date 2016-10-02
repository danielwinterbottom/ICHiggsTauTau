#!/bin/bash

export channel=$1
export dirname=$2

export output_dir=$dirname/1Jet3CatsVBF2Cats
mkdir -p $output_dir
./bin/CombineCategories --inputDirectory $dirname --OneJetCats 3 --VBFCats 2 --MakePlots --channel $channel
cp -R $dirname/Plots $output_dir/.
# cp $dirname/combined* $output_dir/.

export output_dir=$dirname/1Jet3CatsVBF1Cats
mkdir -p $output_dir
./bin/CombineCategories --inputDirectory $dirname --OneJetCats 3 --VBFCats 1 --MakePlots --channel $channel
cp -R $dirname/Plots $output_dir/.
cp $dirname/combined* $output_dir/.

export output_dir=$dirname/1Jet2CatsVBF2Cats
mkdir -p $output_dir
./bin/CombineCategories --inputDirectory $dirname --OneJetCats 2 --VBFCats 2 --MakePlots --channel $channel
cp -R $dirname/Plots $output_dir/.
cp $dirname/combined* $output_dir/.

export output_dir=$dirname/1Jet2CatsVBF1Cats
mkdir -p $output_dir
./bin/CombineCategories --inputDirectory $dirname --OneJetCats 2 --VBFCats 1 --MakePlots --channel $channel
cp -R $dirname/Plots $output_dir/.
cp $dirname/combined* $output_dir/.
