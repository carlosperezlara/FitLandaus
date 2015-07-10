TString spar[7] = {"AMP","LDA","SG1","FR2","FR3","FR4","NCS"};
double gmean[7][2][8] = {0};
double grms[7][2][8] = {0};

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
  int ndead=0;
  for(;;++ndead) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 2;
  }
  fin.close();
  cout << "numnber of deads = " << ndead << endl;

  return;
}

void fillmeansandrms() {
  ifstream infi;
  for(int par=0; par!=7; ++par) {
    infi.open( Form("cPAR%d.rms",par) );
    int lyr, arm;
    for(;;) {
      infi >> arm;
      if(!infi.good()) break;
      infi >> lyr;
      infi >> gmean[par][arm][lyr];
      infi >> grms[par][arm][lyr];
    }
    infi.close();
  }
}

void plotFits(char *run = "19020_19035", int par=1) {
  loadState();
  fillmeansandrms();
  TH1D *hi_a[7][2][8];
  TH1D *hi_b[7][2][8];
  HistoFromFit(hi_a,hi_b,run,0);
  HistoFromFit(hi_a,hi_b,run,1);
  HistoFromFit(hi_a,hi_b,run,2);
  HistoFromFit(hi_a,hi_b,run,3);
  HistoFromFit(hi_a,hi_b,run,4);
  HistoFromFit(hi_a,hi_b,run,5);
  HistoFromFit(hi_a,hi_b,run,6);
  plotNice(hi_a,hi_b,0,run);
  plotNice(hi_a,hi_b,1,run);
  plotNice(hi_a,hi_b,2,run);
  plotNice(hi_a,hi_b,3,run);
  plotNice(hi_a,hi_b,4,run);
  plotNice(hi_a,hi_b,5,run);
  plotNice(hi_a,hi_b,6,run);
}

void HistoFromFit(TH1D *gr[7][2][8], TH1D *gr2[7][2][8], char *run, int par) {
  ofstream fout1, fout2;
  ifstream fin;
  TString filename;
  double peep;
  double amp[2][8][24][128] = {0};
  double eamp[2][8][24][128] = {0};
  double lda[2][8][24][128] = {0};
  double elda[2][8][24][128] = {0};
  double sg1[2][8][24][128] = {0};
  double esg1[2][8][24][128] = {0};
  double fr2[2][8][24][128] = {0};
  double efr2[2][8][24][128] = {0};
  double fr3[2][8][24][128] = {0};
  double efr3[2][8][24][128] = {0};
  double fr4[2][8][24][128] = {0};
  double efr4[2][8][24][128] = {0};
  double nc2[2][8][24][128] = {0};
  double x[128];
  if(par==1) {
    fout1.open("PROBLEM1.lst");
    fout2.open("PROBLEMS1.lst");
  }
  for(int i=0; i!=128; ++i) x[i] = i;
  double ex[128] = {0.0};
  double tmp;
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr) {
      int min=6;
      int max=82;
      if(lyr>1) {
	min=4;
	max=62;
      }
      gr[par][arm][lyr] = NewHis(par,arm,lyr,0);
      gr2[par][arm][lyr] = NewHis(par,arm,lyr,1);
      for(int sen=0; sen!=24; ++sen)
	for(int mpd=0; mpd!=128; ++mpd) {
	  filename = Form("%s/fit/HI_ARM%d_LYR%d_S%d_M%d_%d_%d.dat",run,arm,lyr,sen,mpd,min,max);
	  if( state[arm][lyr][sen][mpd]==1 ) { // broken
	    if(par==1) {
	      fout1 << filename << endl;
	      fout2 << Form("%d  %d  %d  %d\n", arm, lyr, sen, mpd);
	    }
	    continue;
	  }
	  if( state[arm][lyr][sen][mpd]==2 ) { // dead
	    if(par==1) {
	      fout1 << filename << endl;
	      fout2 << Form("%d  %d  %d  %d\n", arm, lyr, sen, mpd);
	    }
	    continue;
	  }
	  fin.open( filename.Data() );
	  fin >> peep;
	  if(!fin.good()) {
	    //cout << "error reading file " << filename.Data() << endl;
	  } else {
	    //cout << "file " << filename.Data() << " [OK]" << endl;
	    amp[arm][lyr][sen][mpd] = peep;
	    fin >> eamp[arm][lyr][sen][mpd];
	    fin >> lda[arm][lyr][sen][mpd] >> elda[arm][lyr][sen][mpd];
	    fin >> sg1[arm][lyr][sen][mpd] >> esg1[arm][lyr][sen][mpd];
	    fin >> fr2[arm][lyr][sen][mpd] >> efr2[arm][lyr][sen][mpd];
	    fin >> fr3[arm][lyr][sen][mpd] >> efr3[arm][lyr][sen][mpd];
	    fin >> fr4[arm][lyr][sen][mpd] >> efr4[arm][lyr][sen][mpd];
	    fin >> nc2[arm][lyr][sen][mpd];
	    //printf("%f\n",amp[arm][lyr][sen][mpd]);
	    bool pass = true;
	    //if( nc2[arm][lyr][sen][mpd] > gmean[6][arm][lyr]+3*grms[6][arm][lyr] ) pass = false;
	    //if( nc2[arm][lyr][sen][mpd] > 4.0 ) pass = false;
	    //if( TMath::Abs(fr2[arm][lyr][sen][mpd] - gmean[3][arm][lyr]) > 5*grms[3][arm][lyr] ) pass = false;
	    if( TMath::Abs(lda[arm][lyr][sen][mpd] - gmean[1][arm][lyr]) > 3*grms[1][arm][lyr] ) pass = false;
	    if( lda[arm][lyr][sen][mpd] < 5.0 ) pass = false;
	    //if( TMath::Abs(sg1[arm][lyr][sen][mpd] - gmean[2][arm][lyr]) > 3*grms[2][arm][lyr] ) pass = false;
	    double msigma = 2.5;
	    if(lyr<2) msigma = 3.5;
	    if( TMath::Abs(sg1[arm][lyr][sen][mpd] - msigma) > 2.0 ) pass = false;
		
	    if(par==1)
	      if(!pass) {
		fout1 << filename << endl;
		fout2 << Form("%d  %d  %d  %d\n", arm, lyr, sen, mpd);
	      }
	    
	    switch(par) {
	    case(0):
	      gr[par][arm][lyr]->Fill( TMath::Log( amp[arm][lyr][sen][mpd] ) );
	      if(pass)
		gr2[par][arm][lyr]->Fill( TMath::Log( amp[arm][lyr][sen][mpd] ) );
	      break;
	    case(1):
	      gr[par][arm][lyr]->Fill( lda[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( lda[arm][lyr][sen][mpd] );
	      break;
	    case(2):
	      gr[par][arm][lyr]->Fill( sg1[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( sg1[arm][lyr][sen][mpd] );
	      break;
	    case(3):
	      gr[par][arm][lyr]->Fill( fr2[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( fr2[arm][lyr][sen][mpd] );
	      break;
	    case(4):
	      gr[par][arm][lyr]->Fill( fr3[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( fr3[arm][lyr][sen][mpd] );
	      break;
	    case(5):
	      gr[par][arm][lyr]->Fill( fr4[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( fr4[arm][lyr][sen][mpd] );
	      break;
	    case(6):
	      gr[par][arm][lyr]->Fill( nc2[arm][lyr][sen][mpd] );
	      if(pass)
		gr2[par][arm][lyr]->Fill( nc2[arm][lyr][sen][mpd] );
	      break;
	    }
	  }
	  fin.close();
	}
    }
  if(par==1) {
    fout1.close();
    fout2.close();
  }
  return;
}

TH2D* GetAxis(int par ) {
  TH2D *ret;
  switch(par) {
  case(0):
    ret = new TH2D("aamp",";;#Alpha",10,-0.5,127.5,10,1e2,1e6);
    break;
  case(1):
    ret = new TH2D("alda",";;#lambda",10,-0.5,127.5,10,-1.9,44.1);
    break;
  case(2):
    ret = new TH2D("asg1",";;#sigma_{1}",10,-0.5,127.5,10,-1,11);
    break;
  case(3):
    ret = new TH2D("afr2",";;f_{2}",10,-0.5,127.5,10,-0.05,0.45);
    break;
  case(4):
    ret = new TH2D("afr3",";;f_{3}",10,-0.5,127.5,10,-0.05,0.45);
    break;
  case(5):
    ret = new TH2D("afr4",";;f_{4}",10,-0.5,127.5,10,-0.05,0.45);
    break;
  case(6):
    ret = new TH2D("anc2",";;N#chi^{2}",10,-0.5,127.5,10,1e-3,9e+4);
    //ret = new TH2D("anc2",";;N#chi^{2}",10,-0.5,127.5,10,-1.5,+11.5);
    break;
  }
  styleAXIS( ret );
  return ret;
}

TH1D* NewHis(int par, int arm, int lyr, int pas) {
  TH1D *ret;
  switch(par) {
  case(0):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";#Alpha",100,4,15);
    break;
  case(1):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";#lambda",100,0,50);
    break;
  case(2):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";#sigma_{1}",100,0,10);
    break;
  case(3):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";f_{2}",100,0.0,0.5);
    break;
  case(4):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";f_{3}",100,0.0,0.5);
    break;
  case(5):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";f_{4}",100,0.0,0.5);
    break;
  case(6):
    ret = new TH1D(Form("PAR%d_ARM%d_LYR%d_PAS%d",par,arm,lyr,pas),";N#chi^{2}",100,0.0,30);
    break;
  }
  ret->GetYaxis()->SetLabelSize(0.10);
  ret->GetXaxis()->SetLabelSize(0.10);
  //ret->GetXaxis()->SetNdivisions(008);
  //ret->GetYaxis()->SetNdivisions(005);
  //ret->GetYaxis()->SetNdivisions(003);
  ret->SetStats(0);
  return ret;
}

void styleMPD(TGraphErrors *a, int mpd) {
  a->SetLineWidth(1);
  a->SetMarkerStyle(20);
  a->SetMarkerSize(0.4);
  a->SetMarkerColor(mpd);
  a->SetLineColor(mpd);
  a->SetFillColor(kWhite);
}

void styleAXIS(TH2D *axis) {
  axis->GetYaxis()->SetLabelSize(0.26);
  axis->GetXaxis()->SetLabelSize(0.20);
  //axis->GetYaxis()->SetTitleSize(0.4);
  //axis->GetYaxis()->SetTitleOffset(0.4);
  //axis->GetXaxis()->SetTitleSize(0.2);
  axis->GetXaxis()->SetNdivisions(008);
  axis->GetYaxis()->SetNdivisions(005);
  axis->GetYaxis()->SetNdivisions(003);
  //axis->GetYaxis()->SetNoExponent();
  axis->SetStats(0);
}

char* ParName( int par ) {
  if(par==0) return "A";
  if(par==1) return "LAMBDA";
  if(par==2) return "SIGMA";
  if(par==3) return "FR2";
  if(par==4) return "FR3";
  if(par==5) return "FR4";
  if(par==6) return "CHI2/NDF";
}

void plotNice(TH1D *gr[7][2][8], TH1D *gr2[7][2][8], int par, const char *run) {
  TString sname = spar[par];
  TCanvas *cAMP;
  TLatex *text = new TLatex();
  text->SetTextSize(0.1);
  text->SetTextColor(kOrange-3);
  cAMP = new TCanvas( Form("c%s",sname.Data()), ParName(par), 1200, 600 );
  cAMP->SetLeftMargin(0.10);
  cAMP->SetBottomMargin(0.20);
  cAMP->Divide(4,4,0,0);
  TLine *line = new TLine();
  line->SetLineColor(kRed-3);
  line->SetLineWidth(2);
  ofstream outf;
  outf.open( Form("cPAR%d.rms",par) );
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr) {
      cAMP->cd(1+lyr+arm*8)->SetLogy(1);
      gr[par][arm][lyr]->Draw();
      gr2[par][arm][lyr]->Draw("SAME");
      gr[par][arm][lyr]->SetFillColor(kRed-3);
      gr2[par][arm][lyr]->SetFillColor(kGreen-3);
      double mean = gr[par][arm][lyr]->GetMean();
      double rms  = gr[par][arm][lyr]->GetRMS();
      outf << Form("%d  %d  %f  %f\n", arm, lyr, mean, rms);
      //double xx = mean-3*rms;
      //double yy = mean+3*rms;
      //int b1 = gr[par][arm][lyr]->FindBin( xx );
      //int b2 = gr[par][arm][lyr]->FindBin( yy );
      double inte = gr2[par][arm][lyr]->GetEntries();
      double tota = gr[par][arm][lyr]->GetEntries();
      text->DrawTextNDC(0.6,0.8,Form("ARM%d LYR%d: %.1f",arm,lyr,inte/tota*100));
    }
  outf.close();
  cAMP->cd(16);
  text->DrawTextNDC(0.6,0.05,ParName(par));
  cAMP->SaveAs( Form("C%s_%s.jpg",sname.Data(),run), "jpg" );
}
