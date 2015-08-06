TString runs[10] = {"428211_429133_5s", "429351_430594_3s", "430595_431736_3s", "",
		    "", "", "", "", "",""};

TF1* GetFit(const char *run, const char *outname, int lyr, double xinit) {
  const char *bgr = Form("[6]*TMath::Exp([7]*(x-%f))",xinit);
  const char *la1 = "(1-[3]-[4]-[5])*TMath::Landau(x,1.00*[1],1.0*[2],1)";
  const char *la2 = "[3]*TMath::Landau(x,2.14*[1],2.0*[2],1)";
  const char *la3 = "[4]*TMath::Landau(x,3.33*[1],3.0*[2],1)";
  const char *la4 = "[5]*TMath::Landau(x,4.55*[1],4.0*[2],1)";
  TF1 *ret = new TF1("fit_H1",
                     Form("[0]*(%s+%s+%s+%s)+%s",la1,la2,la3,la4,bgr),
		     xinit,82.5);
  ret->SetParName(0,"A");
  ret->SetParName(1,"lambda");
  ret->SetParName(2,"sigma");
  ret->SetParName(3,"f2");
  ret->SetParameter(0,1e4);  ret->SetParLimits(0,1e2,1e7);
  ret->SetParameter(1,20);   ret->SetParLimits(1,11,30.0);
  ret->SetParameter(2,3.0);  ret->SetParLimits(2,1.5,5.0);
  if(lyr>1) {
    ret->SetParameter(1,15);   ret->SetParLimits(1,2,30);
    ret->SetParameter(2,5.0);  ret->SetParLimits(2,0.8,10.0);
  }
  ret->SetParameter(3,0.20); ret->SetParLimits(3,-1e-10,0.40);
  ret->SetParameter(4,0.0);
  ret->SetParameter(5,0.0);
  ret->SetParameter(6,500); ret->SetParLimits(6,1e2,1e7);
  ret->SetParameter(7,-1);  ret->SetParLimits(7,-10,-0.1);

  ret->SetLineColor(kRed-3);

  ifstream infit;
  infit.open( Form("%s/fit/%s.dat",run,outname) );
  double tmp;
  bool found=false;
  for(int n=0; n!=ret->GetNpar(); ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    ret->SetParameter(n,tmp);
    infit >> tmp;
    found = true;
  }
  infit.close();
  if(found) {
    cout << " Previous fit results found" << endl;
    ret->SetParLimits(4,0,0.10);
    ret->SetParLimits(5,0,0.10);
  } else {
    ret->SetParLimits(4,+1,-1);
    ret->SetParLimits(5,+1,-1);
  }
  return ret;
}

void drawManyRuns(int key=0) {
  gStyle->SetOptStat(0);
  if(key>49151) return;
  int arm = key/(8*4*12*64);
  int pkt = (key%(8*4*12*64))/(4*12*64);
  int chipmap = key%(4*12*64);
  int sen = chipmap/128;
  int roc = chipmap%128;
  printf("A%d P%d S%d R%d\n",arm,pkt,sen,roc);
  int state = findstate(key);
  printf("state %d\n",state);
  gSystem->Exec( Form("mkdir ARM%d_PKT%d_SEN%d",arm,pkt,sen) );
  TH1D *out[3];
  TF1 *fitH[3];
  double lda[3];
  int color[3] = {kOrange-3, kBlue-3, kRed-3};
  int fcolor[3] = {kOrange-6, kBlue-6, kRed-6};
  TFile *file;
  bool danger=false;
  for(int i=0; i!=3; ++i) {
    TString target = Form("%s.root",runs[i].Data());
    cout << target << ":" ;
    file = TFile::Open( target.Data() );
    out[i] = ((TH2D*) file->Get("adcHI"))->ProjectionY( Form("key_out%d",key,runs[i]), key+1, key+1 );
    double scale = out[i]->Integral(30,50);
    out[i]->SetLineColor( color[i] );
    out[i]->SetMarkerColor( color[i] );
    out[i]->Sumw2();
    out[i]->SetMarkerStyle(20);
    if(scale<1000) danger=true;
    cout << scale << endl;
    int pkt=0;
    if(false)
      int pkt = (key%(8*4*12*64))/(4*12*64);
    fitH[i] = GetFit( runs[i].Data(), Form("HI_KEY%05d_%d_%d",key,6,82),pkt,6.5);
    fitH[i]->SetLineColor( fcolor[i] );
    lda[i] = fitH[i]->GetParameter(1);
  }
  TCanvas *main = new TCanvas();
  TLegend *leg = new TLegend(0.5,0.5,0.9,0.9,"PP15");
  leg->SetFillColor(kWhite);
  out[2]->SetTitle( Form("key %d  =>  ARM %d  PKT %d  SEN %d  ROC %d",key,arm,pkt,sen,roc) );
  out[2]->GetXaxis()->SetTitle( "ADC_{H} - PED" );
  double max = 1.5*out[2]->GetBinContent( out[2]->FindBin(6.5) );
  if(max<100) max = 100;
  out[2]->GetXaxis()->SetRangeUser(0,50);
  out[2]->GetYaxis()->SetRangeUser(0,max);
  out[2]->Draw("HE");
  for(int i=0; i!=3; ++i)
    fitH[i]->Draw("same");
  for(int i=2; i!=-1; --i) {
    leg->AddEntry(out[i],Form("%s:  #lambda  %.1f",runs[i].Data(),lda[i]));
    out[i]->Draw("HE same");
  }

  leg->Draw();
  TLatex *status = new TLatex();
  status->SetTextColor(kRed-3);
  if(danger) {
    status->DrawLatex(30,0.30*max,"low stats!?");
    ofstream lstat;
    lstat.open( Form("lstat_A%dP%dS%d.lst",arm,pkt,sen),fstream::out | fstream::app );
    lstat << key << endl;
    lstat.close();
  }
  if(state!=0) {
    status->DrawLatex(30,0.40*max,Form("STATE %d",state));
  }

  main->SaveAs(Form("ARM%d_PKT%d_SEN%d/ROC%d.jpg",arm,pkt,sen,roc),"jpg");
}

int findstate(int key) {
  int ret=0;
  ifstream file;
  file.open("status.lst");
  int tmp;
  for(;;) {
    file >> tmp;
    if(!file.good()) break;
    if(tmp==key) ret = 255;
  }
  file.close();
  return ret;
}

int loadlist() {
  ifstream fin;
  fin.open("runspp.lst");
  int ret=0;
  for(;;++ret) {
    fin >> runs[ret];
    if(!fin.good()) break;
  }
  fin.close();
  return ret;
}
