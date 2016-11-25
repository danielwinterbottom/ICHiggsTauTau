{
  double sigmanunuewk=1380;
  double sigmanunuqcd=6600000;
  double sigmamumuewk=303;
  double sigmamumuqcd=1168000;

  double nsmcewk=7.9;
  double nsmcqcd=29.5;
  double ncmcewk=6;
  double ncmcqcd=20.5;

  double nsmcewkerr=0.2;
  double nsmcqcderr=3;
  double ncmcewkerr=0.2;
  double ncmcqcderr=2.5;


  double ngenzmassewk=4226.5;
  double ngenzmassqcd=20334900;
  double ngenewk=5781.9;
  double ngenqcd=22789300;

  double epss=((sigmanunuewk*(nsmcewk/ngenzmassewk))+(sigmanunuqcd*(nsmcqcd/ngenzmassqcd)))/(sigmanunuewk+sigmanunuqcd);
  double epsserr=sqrt(pow((sigmanunuewk*(nsmcewkerr/ngenzmassewk))/((sigmanunuewk*(nsmcewk/ngenzmassewk))+(sigmanunuqcd*(nsmcqcd/ngenzmassqcd))),2)+pow((sigmanunuqcd*(nsmcqcderr/ngenzmassqcd))/((sigmanunuewk*(nsmcewk/ngenzmassewk))+(sigmanunuqcd*(nsmcqcd/ngenzmassqcd))),2))*epss;

  double epsc=((sigmamumuewk*(ncmcewk/ngenewk))+(sigmamumuqcd*(ncmcqcd/ngenqcd)))/(sigmamumuewk+sigmamumuqcd);
  double epscerr=sqrt(pow((sigmamumuewk*(ncmcewkerr/ngenewk))/((sigmamumuewk*(ncmcewk/ngenewk))+(sigmamumuqcd*(ncmcqcd/ngenqcd))),2)+pow((sigmamumuqcd*(ncmcqcderr/ngenqcd))/((sigmamumuewk*(ncmcewk/ngenewk))+(sigmamumuqcd*(ncmcqcd/ngenqcd))),2))*epsc;
  std::cout<<"epss: "<<epss<<" \\pm "<<epsserr<<", epsc: "<<epsc<<" \\pm "<<epscerr<<std::endl;
}
