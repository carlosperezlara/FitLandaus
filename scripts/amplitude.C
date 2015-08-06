#include "MpcExAeroView.h"

void amplitude(char *run = "430595_431736_3s", int min=6, int max=82) {
  gStyle->SetOptStat(0);
  AeroView *display = new AeroView();
  ifstream inf;
  double a;
  for(int key=0; key!=49152; ++key) {
    a=0;
    if( (key%10000)==0 )
      cout << key << "\n";
    inf.open( Form("%s/fit/HI_KEY%05d_%d_%d.dat",run,key,min,max) );
    inf >> a;
    display->Fill( key, a );
    inf.close();
  }
  TCanvas *main = display->Draw();
  main->SaveAs( Form("AMPLITUDE_RUN%s.eps",run),"eps" );
}
