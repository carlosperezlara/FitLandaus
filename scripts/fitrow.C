void fitrow(char *run, int arm, int lyr, int sen, int row,
	    bool draw,xfit_min,xfit_max,minentries) {
  if(0) {
    gROOT->LoadMacro("scripts/fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("scripts/fit_C.so");
  }
  TString stmp;
  for(int i=0; i!=32; ++i) {
    int mpd = row*32+i;
    fit(run,arm,lyr,sen,mpd,draw,xfit_min,xfit_max,minentries);
  }
}
