TH1 *pointersHI[8];
TH1 *pointersLO[8];
TH1 *pratioHI[8];
TH1 *pratioLO[8];
int N;
int color[8] = {kMagenta-3, kRed-3, kOrange-3, kYellow-3, kGreen-3, kCyan-3, kBlue-3, kBlack};
TString text[8];

void compareShape(int arm=1, int lyr=1, int sen=9, int mpd=29, char *input="input") {
  //void compareShape(int arm=1, int lyr=0, int sen=9, int mpd=29, char *input="input") {
  //void compareShape(int arm=1, int lyr=1, int sen=9, int mpd=29, char *input="input") {
  gStyle->SetOptStat(0);

  int idx = arm*8*24*128 + lyr*24*128 + sen*128 + mpd;
  //open(input,idx,idx+12*128);
  double ymax = open(input,idx,1);

  pointersHI[0]->GetXaxis()->SetRangeUser(5,35);
  pointersHI[0]->GetYaxis()->SetRangeUser(1,ymax);
  pointersLO[0]->GetXaxis()->SetRangeUser(5,50);
  pointersLO[0]->GetYaxis()->SetRangeUser(1,ymax);
  TLegend *leg = new TLegend(0.75,0.32,0.99,0.99,Form("A%dL%dS%dM%d",arm,lyr,sen,mpd));
  for(int i=0; i!=N; ++i)
    leg->AddEntry(pointersHI[i],text[i].Data());

  for(int i=0; i!=N; ++i) {
    pratioHI[i] = (TH1D*) pointersHI[i]->Clone( Form("r%s",pointersHI[i]->GetName()) );
    pratioLO[i] = (TH1D*) pointersLO[i]->Clone( Form("r%s",pointersLO[i]->GetName()) );
    pratioHI[i]->Divide( pointersHI[3] );
    pratioLO[i]->Divide( pointersLO[3] );
    pratioHI[i]->GetYaxis()->SetTitle("RATIO");
    pratioLO[i]->GetYaxis()->SetTitle("RATIO");
    pratioHI[i]->GetYaxis()->SetRangeUser(0,2.7);
    pratioLO[i]->GetYaxis()->SetRangeUser(0,2.7);
  }


  TCanvas *main = new TCanvas();
  main->SetBottomMargin(0.2);
  main->Divide(1,2,0,0);

  main->cd(1);
  pointersHI[0]->Draw();
  for(int i=0; i!=N; ++i)
    pointersHI[i]->Draw("SAME");

  //main->cd(2);
  //pointersLO[0]->Draw();
  //for(int i=0; i!=N; ++i)
  //  pointersLO[i]->Draw("SAME");
  leg->Draw();

  main->cd(2);
  pratioHI[0]->Draw();
  for(int i=0; i!=N; ++i)
    pratioHI[i]->Draw("SAME");

  //main->cd(4);
  //pratioLO[0]->Draw();
  //for(int i=0; i!=N; ++i)
  //  pratioLO[i]->Draw("SAME");



}

double open(char *input, int idxA, int idxN) {
  cout << "Reading " << input << " file. Remeber to put maximum 8 run numbers." << endl;
  N=0;
  ifstream infile;
  infile.open(input);
  TString run;
  TFile *rfile;
  TH2D *hi2d, *lo2d;
  double max = 10;
  for(;N<8;++N) {
    infile >> run;
    text[N] = run;
    if(!infile.good()) break;
    rfile = TFile::Open( Form("%s.root",run.Data()) );
    hi2d = (TH2D*) rfile->Get("adcHI");
    lo2d = (TH2D*) rfile->Get("adcLO");
    pointersHI[N] = hi2d->ProjectionY( Form("%s_HI_%d_%d",run.Data(),idxA,idxN), idxA+1, idxA+idxN );
    pointersLO[N] = lo2d->ProjectionY( Form("%s_LO_%d_%d",run.Data(),idxA,idxN), idxA+1, idxA+idxN );
    pointersHI[N]->Sumw2();
    pointersLO[N]->Sumw2();
    int events = ((TH1D*) rfile->Get("Events"))->GetBinContent(5);
    //double events = pointersHI[N]->Integral(14+11,26+11);
    pointersHI[N]->Scale(1000000.0/events);
    pointersLO[N]->Scale(1000000.0/events);
    double thisheight = pointersHI[N]->GetBinContent(18);
    if(max<thisheight)  max = thisheight;

    //style
    pointersHI[N]->SetLineColor( color[N] );
    pointersHI[N]->SetMarkerColor( color[N] );
    pointersHI[N]->GetXaxis()->SetTitleOffset(1.0);
    pointersHI[N]->GetXaxis()->SetTitleSize(0.08);
    pointersHI[N]->GetXaxis()->SetLabelSize(0.07);
    pointersHI[N]->GetYaxis()->SetTitleOffset(0.5);
    pointersHI[N]->GetYaxis()->SetTitleSize(0.08);
    pointersHI[N]->GetYaxis()->SetLabelSize(0.07);

    //pointersHI[N]->SetTitle( Form("%s_HI_%d_%d",run.Data(),idxA,idxN) );
    pointersHI[N]->SetTitle( "" );
    pointersHI[N]->GetXaxis()->SetTitle( "ADC_{HI} - PED" );
    pointersHI[N]->GetYaxis()->SetTitle( "ENTRIES / N_{evts}   (x10^{6})" );    
    pointersHI[N]->GetYaxis()->CenterTitle();
    pointersLO[N]->SetLineColor( color[N] );
    pointersLO[N]->SetMarkerColor( color[N] );
    //pointersLO[N]->SetTitle( Form("%s_LO_%d_%d",run.Data(),idxA,idxN) );
    pointersLO[N]->SetTitle( "" );
    pointersLO[N]->GetXaxis()->SetTitle( "ADC_{LOW} - PED" );    
    pointersLO[N]->GetYaxis()->SetTitle( "ENTRIES / N_{evts}   (x10^{6}" );
    pointersLO[N]->GetYaxis()->CenterTitle();
  }
  infile.close();
  return max;
}
