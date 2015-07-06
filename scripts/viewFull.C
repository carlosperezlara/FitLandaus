//void viewFull(const char* run="19020_19035") {
//void viewFull(const char* run="19037_19049") {
void viewFull(const char* run="19052_19057") {
  gStyle->SetOptStat(0);
  TFile *file = new TFile( Form("%s.root",run) );
  TH2D *a = (TH2D*) file->Get("adcHI");
  TH2D *b = (TH2D*) file->Get("adcLO");

  a->SetTitle( Form("%s HIGH",run) );
  a->GetYaxis()->SetTitle("ADC-PED");
  a->GetXaxis()->SetTitle("CHANNEL INDEX: 8*24*128*ARM + 24*128*LYR + 128*SENSOR + MINIPAD");

  b->SetTitle( Form("%s LOW",run) );
  b->GetYaxis()->SetTitle("ADC-PED");
  b->GetXaxis()->SetTitle("CHANNEL INDEX: 8*24*128*ARM + 24*128*LYR + 128*SENSOR + MINIPAD");


  TH1D *hist[2][3];
  int bins[3] = {4499,8459,16493};
  for(int i=0; i!=3; ++i) {
    int sel = bins[i];
    hist[0][i] = a->ProjectionY( Form("CHANNEL%d",sel),sel,sel);
    hist[0][i]->SetTitle( Form("ARM%d LYR%d SEN%d MPX%d MPY%d",
			 sel/24576,
			 (sel%24576)/3072,
			 ((sel%24576)%3072)/128,
			 (((sel%24576)%3072)%128)%32,
			 (((sel%24576)%3072)%128)/32 ) );
    hist[0][i]->GetXaxis()->SetRangeUser(-5,60);
  }
  for(int i=0; i!=3; ++i) {
    int sel = bins[i]+24576;
    hist[1][i] = a->ProjectionY( Form("CHANNEL%d",sel),sel,sel);
    hist[1][i]->SetTitle( Form("ARM%d LYR%d SEN%d MPX%d MPY%d",
			 sel/24576,
			 (sel%24576)/3072,
			 ((sel%24576)%3072)/128,
			 (((sel%24576)%3072)%128)%32,
			 (((sel%24576)%3072)%128)/32 ) );
    hist[1][i]->GetXaxis()->SetRangeUser(-5,60);
  }
  cout << "PUFF" << endl;
  TCanvas *little = new TCanvas("little","little");
  little->Divide(3,2);
  little->cd(1); hist[0][0]->Draw("E BAR");
  little->cd(2); hist[0][1]->Draw("E BAR");
  little->cd(3); hist[0][2]->Draw("E BAR");
  little->cd(4); hist[1][0]->Draw("E BAR");
  little->cd(5); hist[1][1]->Draw("E BAR");
  little->cd(6); hist[1][2]->Draw("E BAR");
  little->SaveAs( Form("little_%s.eps",run),"eps");

  TCanvas * main = new TCanvas();
  main->SetLogz(1);
  a->Draw("colz");
  main->SaveAs( Form("over_%s.jpg",run),"jpg");
  a->GetYaxis()->SetRangeUser(0,45);
  a->GetXaxis()->SetRangeUser(3072,6144);
  main->SaveAs( Form("over_zoomin_%s.jpg",run),"jpg");

}
