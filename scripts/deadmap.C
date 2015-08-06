#include "MpcExAeroView.h"
int state[49152];

void deadmap() {
  loadstate();
  gStyle->SetOptStat(0);
  AeroView *display = new AeroView();
  for(int key=0; key!=49152; ++key) {
    if( (key%10000)==0 )
      cout << key << "\n";
    display->Fill( key, state[key] );
  }
  display->SetMaxZ(8);
  TCanvas *main = display->Draw();
  main->SaveAs( "DEADMAP.eps","eps" );
}

void loadstate() {
  for(int key=0; key!=49152; ++key) state[key]=0;

  ifstream file;
  int tmp;
  //file.open("lowstat.lst");

  file.open("dead.lst");
  for(;;) {
    tmp=-1;
    file >> tmp;
    if(!file.good()) break;
    if(tmp<0) break;
    if(tmp>49151) break;
    state[tmp] += 1;
  }
  file.close();

  file.open("noisy.lst");
  for(;;) {
    tmp=-1;
    file >> tmp;
    if(!file.good()) break;
    if(tmp<0) break;
    if(tmp>49151) break;
    state[tmp] += 4;
  }
  file.close();

  file.open("bigstd.lst");
  for(;;) {
    tmp=-1;
    file >> tmp;
    if(!file.good()) break;
    if(tmp<0) break;
    if(tmp>49151) break;
    state[tmp] += 2;
  }
  file.close();
}
