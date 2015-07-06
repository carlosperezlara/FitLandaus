void fitrow(char *run="pAAFTDUMP", int arm=1, int lyr=2, int sen=0, int row=0,
	    bool draw=false) {
  double xfit_min, xfit_max;
  if(lyr<2) {
    xfit_min=14.5;
    xfit_max=82.5;
  } else {
    xfit_min=7.5;
    xfit_max=62.5;
  }
  if(0) {
    gROOT->LoadMacro("fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("fit_C.so");
  }
  TString stmp;
  for(int i=0; i!=32; ++i) {
    int mpd = row*32+i;
    fit(run,arm,lyr,sen,mpd,draw,xfit_min,xfit_max,50);
  }
}
