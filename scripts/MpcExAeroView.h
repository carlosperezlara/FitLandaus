#include "TH2D.h"
#include "TCanvas.h"

#include <map>
#include <fstream>

class AeroView {
 public:
  void AeroView();
  void InitGeo();
  void Fill(const int key, const double adc);
  void SetLogZ() {fLogz=true;}
  void SetMaxZ(double val) {fMaxz=val;fRangez=true;}
  TCanvas *Draw();
 protected:
  TH2D *fLayer[2][8];
  int fA[49152];
  int fL[49152];
  double fX[49152];
  double fY[49152];
  bool fLogz;
  bool fRangez;
  double fMaxz;
};

void AeroView::AeroView() {
  const double rg = 20;
  const int nx = 400;
  const int ny = 30;
  for(int arm=0; arm!=2; ++arm) 
    for(int lyr=0; lyr!=8; ++lyr)
      if(lyr%2) {
	fLayer[arm][lyr] = new TH2D( Form("ARM%dLYR%d",arm,lyr), Form("ARM%d  LYR%d",arm,lyr), ny, -rg, +rg, nx, -rg, +rg );
      } else {
	fLayer[arm][lyr] = new TH2D( Form("ARM%dLYR%d",arm,lyr), Form("ARM%d  LYR%d",arm,lyr), nx, -rg, +rg, ny, -rg, +rg );
      }
  InitGeo();
  return;
}

void AeroView::InitGeo() {
  ifstream inf;
  inf.open("map.txt");
  //# KEY ARM PKT LYR CHN CHP LX LY X Y
  int key;
  double tmp;
  for(;;) {
    inf >> key >> fA[key] >> tmp >> fL[key] >> tmp >> tmp >> tmp >> tmp >> fX[key] >> fY[key];
    if(!inf.good()) break;
  }
  fLogz=false;
  fRangez=false;
  return;
}

void AeroView::Fill(const int key, const double val) {
  fLayer[ fA[key] ][ fL[key] ]->Fill( fX[key], fY[key], val );
  return;
}

TCanvas* AeroView::Draw() {
  TCanvas *ret = new TCanvas("AeroView","AeroView",1200,300);
  ret->Divide(8,2);
  for(int arm=0; arm!=2; ++arm) 
    for(int lyr=0; lyr!=8; ++lyr) {
      ret->cd(arm*8+lyr+1)->SetLogz(fLogz);
      fLayer[arm][lyr]->Draw("colz");
      if(fRangez)
	fLayer[arm][lyr]->GetZaxis()->SetRangeUser(0,fMaxz);
    }
  return ret;
}
