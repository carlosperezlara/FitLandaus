void fitrowLOW(char *run="pAAFTDUMP", int arm=1, int lyr=2, int sen=0, int row=0,
	       bool draw=false) {
  int xfit_min, xfit_max;
  if(lyr<2) {
    xfit_min=14;
    xfit_max=82;
  } else {
    xfit_min=8;
    xfit_max=62;
  }
  if(0) {
    gROOT->LoadMacro("fitLOW.C++g");
    //return;
  } else {
    gROOT->LoadMacro("fitLOW_C.so");
  }
  TString stmp;
  for(int i=0; i!=32; ++i) {
    int mpd = row*32+i;
    fitLOW(run,arm,lyr,sen,mpd,draw,xfit_min,xfit_max,2.5,18.5);
  }
}
