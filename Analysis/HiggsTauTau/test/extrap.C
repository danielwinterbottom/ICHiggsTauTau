{

  gSystem->Load("test/PlotExtrap.C");
  PlotExtrap("htt_et.inputs-sm-8TeV-lowsignal-real.root","eleTau_boost_high","ggH","-33.19+x*0.3415");
  PlotExtrap("htt_mt.inputs-sm-8TeV-lowsignal-real.root","muTau_boost_high","ggH","-109.1+x*1.224");
  
  PlotExtrap("htt_et.inputs-sm-8TeV-lowsignal-real.root","eleTau_boost_low","ggH","-7.695+x*0.1492");
  PlotExtrap("htt_mt.inputs-sm-8TeV-lowsignal-real.root","muTau_boost_low","ggH","8.957+x*0.2888");
  
  PlotExtrap("htt_et.inputs-sm-8TeV-lowsignal-real.root","eleTau_inclusive","ggH","-126.1+x*1.795");
  PlotExtrap("htt_mt.inputs-sm-8TeV-lowsignal-real.root","muTau_inclusive","ggH","-190.6+x*3.497");
  PlotExtrap("htt_et.inputs-sm-8TeV-lowsignal-real.root","eleTau_vbf","qqH","-8.21+x*0.3524");
  PlotExtrap("htt_mt.inputs-sm-8TeV-lowsignal-real.root","muTau_vbf","qqH","-16.07+x*0.7221");





}
