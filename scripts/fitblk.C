void fitblk(char *run="RANDOM", int sen=0,
	    bool draw=false, bool pa=false) {
  if(0) {
    gROOT->LoadMacro("scripts/fit.C++g");
    return;
  } else {
    gROOT->LoadMacro("scripts/fit_C.so");
  }

  int bmin[128] = {10};
  ifstream fin;
  fin.open( Form("%s/SEN%03d/xini.dat",run,sen) );
  int a, b;
  for(int i=0;;++i) {
    fin >> a >> b;
    if(!fin.good()) break;
    bmin[i] = b;
  }

  for(int i=0; i!=128; ++i) {
    int key = 128*sen + i;
    //cout << key << "|" << bmin[i] << endl;
    fit(run,key,bmin[i],draw,pa);
  }
}
