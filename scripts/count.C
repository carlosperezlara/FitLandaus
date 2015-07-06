double x[2][8][24][128];
double y[2][8][24][128];
double z[2][8][24][128];

void ReadGeoMap() {
  ifstream fin;
  fin.open("geomap_OFFL_WRLD.txt");
  int arm,lyr,sen,mpd;
  int i=0;
  for(;;++i) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    fin >> x[arm][lyr][sen][mpd];
    fin >> y[arm][lyr][sen][mpd];
    fin >> z[arm][lyr][sen][mpd];
  }
  cout << "ALL " << i << endl;
  fin.close();
}

//void count(char *run = "pABEFDUMP", int ar=0, int la=2, double min=5., double max=100.) {
void count(char *run = "pAAFTDUMP", int ar=0, int la=2, double min=5., double max=100.) {
  gStyle->SetOptStat(0);
  ReadGeoMap();

  TCanvas *main = new TCanvas("main","main",800,800);
  main->SetLogz(1);
  TFile *file;
  TH1D *ghi;
  TH2D *layer;
  if(la%2)
    layer = new TH2D("layer",Form("LAYER %d;cm;cm",la),60,-20,+20,300,-20,+20);
  else
    layer = new TH2D("layer",Form("LAYER %d;cm;cm",la),300,-20,+20,60,-20,+20);
  int color[4] = {kRed-3,kOrange-3,kCyan-3,kBlue-3};
  for(int s=0; s!=24; ++s) {
    for(int m=0; m!=128; ++m) {
      file = new TFile( Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run,ar,la,s,m) );
      ghi = (TH1D*) file->Get( "out" );
      if(!ghi) continue;
      int binMin = ghi->GetXaxis()->FindBin(min);
      int binMax = ghi->GetXaxis()->FindBin(max);
      layer->Fill(x[ar][la][s][m],
		  y[ar][la][s][m],
		  ghi->Integral(binMin,binMax));
      file->Close();
    }
  }
  layer->GetZaxis()->SetRangeUser(0.9,5e5);
  layer->Draw("colz");

  main->SaveAs( Form("COUNT_ARM%dLYR%d.eps",ar,la),"eps" );
}
