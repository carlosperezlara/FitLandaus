#include "MpcExAeroView.h"

void count(char *run = "430595_431736_3s", double min=20., double max=82.) {
  gStyle->SetOptStat(0);
  AeroView *display = new AeroView();
  TFile *file;
  TH1D *ghi;
  for(int key=0; key!=49152; ++key) {
    if( (key%10000)==0 )
      cout << key << "\n";
    file = new TFile( Form("%s/adc/HI_KEY%05d.root",run,key) );
    ghi = (TH1D*) file->Get( "out" );
    if(!ghi) continue;
    int binMin = ghi->GetXaxis()->FindBin(min);
    int binMax = ghi->GetXaxis()->FindBin(max);
    display->Fill( key, ghi->Integral(binMin,binMax) );
    delete ghi;
    file->Close();
  }
  TCanvas *main = display->Draw();
  main->SaveAs( Form("COUNT_RUN%s.eps",run),"eps" );
}
