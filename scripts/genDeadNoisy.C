#include "MpcExAeroView.h"

void genDeadNoisy(char *run = "430595_431736_3s", double min=3e4, double max=6e6) {
  gStyle->SetOptStat(0);
  ifstream inf;
  ofstream ouf1, ouf2;
  ouf1.open("dead.lst");
  ouf2.open("noisy.lst");
  double a;
  for(int key=0; key!=49152; ++key) {
    a=0;
    if( (key%10000)==0 )
      cout << key << "\n";
    inf.open( Form("%s/fit/HI_KEY%05d_6_82.dat",run,key) );
    inf >> a;
    if(a<min) ouf1 << key << endl;
    if(a>max) ouf2 << key << endl;
    inf.close();
  }
  ouf1.close();
  ouf2.close();
}
