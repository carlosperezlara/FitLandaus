void fitrow(char *run, int arm, int lyr, int sen, int row,
	    bool draw,double xfit_min=6.5,double xfit_max=82.5,int minentries=50) {
  if(0) {
    gROOT->LoadMacro("scripts/fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("scripts/fit_C.so");
  }
  if(lyr>1) {
    xfit_min = 4.5;
    xfit_max = 62.5;
  }
  TString stmp;
  for(int i=0; i!=32; ++i) {
    int mpd = row*32+i;
    fit(run,arm,lyr,sen,mpd,draw,xfit_min,xfit_max,minentries);
  }
}
