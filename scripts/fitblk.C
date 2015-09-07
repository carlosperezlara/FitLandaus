void fitblk(char *run="RANDOM", int ini=0, int fin=1,
	    bool draw=false, bool pa=false) {
  if(0) {
    gROOT->LoadMacro("scripts/fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("scripts/fit_C.so");
  }
  TString stmp;
  for(int i=ini; i!=fin; ++i) {
    fit(run,i,draw,pa);
  }
}
