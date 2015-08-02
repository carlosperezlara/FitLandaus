void fitblk(char *run="RANDOM", int ini=0, int fin=1,
	    bool draw=false,double xfit_min=6.5,double xfit_max=82.5,int minentries=50) {
  if(0) {
    gROOT->LoadMacro("scripts/fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("scripts/fit_C.so");
  }
  //if(lyr>1) {
  //  xfit_min = 4.5;
  //  xfit_max = 62.5;
  //}
  TString stmp;
  for(int i=ini; i!=fin; ++i) {
    fit(run,i,draw,xfit_min,xfit_max,minentries);
  }
}
