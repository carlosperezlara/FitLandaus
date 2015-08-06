#include "MpcExAeroView.h"

int fA[49152];
int fP[49152];
int fL[49152];
int fCN[49152];
int fCP[49152];
double fX[49152];
double fY[49152];


void checkmap() {
  gStyle->SetOptStat(0);
  loadtable();
  AeroView *display = new AeroView();
  display->SetMaxZ(128);
  for(int key=0; key!=49152; ++key) {
    int arm = key/(8*4*12*64);
    int pkt = (key%(8*4*12*64))/(4*12*64);
    int chipmap = key%(4*12*64);
    int sen = chipmap/128;
    int roc = chipmap%128;
    if(arm!=fA[key]) printf("ARM diff for key %d!\n",key);
    if(pkt!=fP[key]) printf("PKT diff for key %d!\n",key);
    display->Fill( key, roc );
  }
  TCanvas *main = display->Draw();
  //main->SaveAs( Form("DISPLAY.eps",run),"eps" );
}


void loadtable() {
  ifstream inf;
  inf.open("map.txt");
  //# KEY ARM PKT LYR CHN CHP LX LY X Y
  int key;
  double tmp;
  for(;;) {
    inf >> key >> fA[key] >> fP[key] >> fL[key] >> fCN[key] >> fCP[key] >> tmp >> tmp >> fX[key] >> fY[key];
    if(!inf.good()) break;
  }
  return;
}
