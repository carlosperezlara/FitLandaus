int state[2][8][24][128] = { 0 };

void loadState() {
  ifstream fin;
  int arm, lyr, sen, mpd;

  fin.open("discretised.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 4;
  }
  fin.close();

  fin.open("brokenABD.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 3;
  }
  fin.close();

  fin.open("broken.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 1;
  }
  fin.close();

  fin.open("dead.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 2;
  }
  fin.close();


  return;
}

TF1* GetFit(const char *run, const char *outname, int lyr, double &lda) {
  TString lookup = Form("%s/fit/%s.dat",run,outname);
  //cout << "Looking for " << lookup << endl;
  ifstream infit;
  infit.open( lookup.Data() );
  double tmp;
  bool found=false;
  double pars[6] = {0};
  for(int n=0; n!=6; ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    pars[n] = tmp;
    infit >> tmp;
    found = true;
  }
  infit.close();
  if(found) {
    //cout << " Previous fit results found" << endl;
  }
  lda = pars[1];
  const char *la1 = Form("(1-%f-%f-%f)*TMath::Landau(x,1.00*%f,1.0*%f,1)",pars[3],pars[4],pars[5],pars[1],pars[2]);
  const char *la2 = Form("%f*TMath::Landau(x,2.14*%f,2.0*%f,1)",pars[3],pars[1],pars[2]);
  const char *la3 = Form("%f*TMath::Landau(x,3.33*%f,3.0*%f,1)",pars[4],pars[1],pars[2]);
  const char *la4 = Form("%f*TMath::Landau(x,4.55*%f,4.0*%f,1)",pars[5],pars[1],pars[2]);
  TF1 *ret;
  if(lyr<2) {
    ret = new TF1( Form("fit_%s",run),
		   Form("%f*(%s+%s+%s+%s)",pars[0],la1,la2,la3,la4),
		   14.5,82.5);
  } else {
    ret = new TF1( Form("fit_%s",run),
		   Form("%f*(%s+%s+%s+%s)",pars[0],la1,la2,la3,la4),
		   8.5,82.5);
  }
  return ret;
}

void histotime(int arm=0, int lyr=0, int sen=12) {
  loadState();
  gSystem->Exec( Form(" mkdir -p eps/ARM%dLYR%dSEN%02d/",arm,lyr,sen) );

  for(int i=0; i!=128; ++i) {
    histoone(arm,lyr,sen,i);
  }

}

void histoone(int arm=0, int lyr=0, int sen=12, int mpd=1,
	      const char *run1="19020_19035",
	      const char *run2="19037_19049",
	      const char *run3="19052_19057"
	      ) {

  // data
  TString inname1 = Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run1,arm,lyr,sen,mpd);
  TString inname2 = Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run2,arm,lyr,sen,mpd);
  TString inname3 = Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run3,arm,lyr,sen,mpd);

  TFile *file;
  TH1D *out1, *out2, *out3;
  file = new TFile( inname1.Data() );
  out1 = (TH1D*) file->Get("out");
  file = new TFile( inname2.Data() );
  out2 = (TH1D*) file->Get("out");
  file = new TFile( inname3.Data() );
  out3 = (TH1D*) file->Get("out");  

  out1->Sumw2();
  out2->Sumw2();
  out3->Sumw2();
  double int1 = out1->Integral(80,120);
  double int2 = out2->Integral(80,120);
  double int3 = out3->Integral(80,120);

  out1->SetMarkerColor( kBlue-3 );
  out2->SetMarkerColor( kOrange-3 );
  out3->SetMarkerColor( kRed-3 );
  out1->SetLineColor( kBlue-3 );
  out2->SetLineColor( kOrange-3 );
  out3->SetLineColor( kRed-3 );
  out1->SetMarkerStyle( 20 );
  out2->SetMarkerStyle( 20 );
  out3->SetMarkerStyle( 20 );

  out1->SetTitle("");
  out1->GetXaxis()->SetTitle("ADC - PED");
  double maxy = 1000000;
  double miny = 9;
  if(arm) {
    maxy = 10000;
    miny = 0.9;
  }
  out1->GetYaxis()->SetRangeUser( miny, maxy );
  TCanvas *main = new TCanvas("main");
  main->SetLogy(1);
  out1->Draw("EH");
  out2->Draw("EHSAME");
  out3->Draw("EHSAME");

  double lda1, lda2, lda3;
  TString outname;
  if(lyr<2)
    outname = Form("HI_ARM%d_LYR%d_S%d_M%d_%d_%d",arm,lyr,sen,mpd,14,82);
  else
    outname = Form("HI_ARM%d_LYR%d_S%d_M%d_%d_%d",arm,lyr,sen,mpd,8,62);

  bool everythingOK = false;
  bool noisyABD = state[arm][lyr][sen][mpd]==3;
  bool unsmooth = state[arm][lyr][sen][mpd]==4;
  if( state[arm][lyr][sen][mpd]==0 ) everythingOK = true;

  if(everythingOK||noisyABD||unsmooth) {
    // fit
    TF1 *fitH1 = GetFit(run1,outname.Data(),lyr,lda1);
    TF1 *fitH2 = GetFit(run2,outname.Data(),lyr,lda2);
    TF1 *fitH3 = GetFit(run3,outname.Data(),lyr,lda3);
    fitH1->SetLineColor( kBlue-3 );
    fitH2->SetLineColor( kOrange-3 );
    fitH3->SetLineColor( kRed-3 );

    fitH1->Draw("SAME");
    fitH2->Draw("SAME");
    if(!noisyABD) {
      fitH3->Draw("SAME");
    }
  }
  out1->GetXaxis()->SetRangeUser(0,60);

  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  TLegend *leg = new TLegend(0.5,0.55,0.9,0.9, Form("ARM %d LYR %d SEN %d MPD %d",arm,lyr,sen,mpd) );
  if(everythingOK||noisyABD||unsmooth) {
    leg->AddEntry(out1, Form("%s  #lambda_{1} = %.1f",run1,lda1) );
    leg->AddEntry(out2, Form("%s  #lambda_{2} = %.1f",run2,lda2) );
    if(!noisyABD) {
      leg->AddEntry(out3, Form("%s  #lambda_{3} = %.1f",run3,lda3) );
    }
  }
  leg->Draw();

  TLatex *tex = new TLatex();
  double ytext = maxy*0.5;
  if(state[arm][lyr][sen][mpd]==4) tex->DrawLatex( 2, ytext, "UNSMOOTH" );
  if(state[arm][lyr][sen][mpd]==3) tex->DrawLatex( 2, ytext, "BROKEN ABD" );
  if(state[arm][lyr][sen][mpd]==1) tex->DrawLatex( 2, ytext, "BROKEN" );
  if(state[arm][lyr][sen][mpd]==2) tex->DrawLatex( 2, ytext, "IRRESPONSIVE" );

  main->SaveAs( Form("eps/ARM%dLYR%dSEN%02d/MPD%03d.jpg",arm,lyr,sen,mpd), "jpg" );

  return;
}

