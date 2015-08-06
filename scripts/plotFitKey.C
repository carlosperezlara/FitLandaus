const int range = 49152;
int state[range] = { 0 };
TString parnam[9] = {"SIGNAL","LDA","SG1","FR2","FR3","FR4","B","SLO","NCS"};
double parval[9][range];
double parerr[9][range];
double key[range];
double keyerr[range];
double ymin[9] = {   0, 10, 1.0, 0.00, 0.00, 0.00,   0, -1,  0};
double ymax[9] = { 5e6, 31, 5.5, 0.50, 0.10, 0.10, 1e6,  0, 20};

void plotFitKey(char *run = "19020_19035") {
  ReadFiles(run);
  TCanvas *cPar[9];
  TGraphErrors *gPar[9];
  for(int i=0; i!=9; ++i) {
    cPar[i] = new TCanvas();
    gPar[i] = new TGraphErrors(range,key,parval[i],keyerr,parerr[i]);
    style(gPar[i]);
    gPar[i]->Draw("AP");
    gPar[i]->SetTitle( "" );
    gPar[i]->GetYaxis()->SetTitle( parnam[i].Data() );
    gPar[i]->GetXaxis()->SetTitle( "KEY" );
    gPar[i]->GetYaxis()->SetRangeUser(ymin[i],ymax[i]);
  }
}

void ReadFiles(char *run) {
  printf("Reading files: ");
  ifstream fin;
  TString filename;
  for(int i=0; i!=range; ++i) {
    for(int j=0; j!=9; ++j) {
      parval[j][i] = 0;
      parerr[j][i] = 0;
    }
    key[i] = i;
    keyerr[i] = 0;
    filename = Form("%s/fit/HI_KEY%05d_6_82.dat",run,i);
    //printf("Opening %s",filename.Data());
    fin.open( filename.Data() );
    for(int j=0; j!=9; ++j) {
      fin >> parval[j][i] >> parerr[j][i];
      parerr[j][i] = 0;
    }
    parerr[8][i] = 0;
    fin.close();
  }
  printf("[DONE]\n");
  return;
}

void style(TGraphErrors *a) {
  a->SetLineWidth(1);
  //a->SetMarkerStyle(20);
  a->SetMarkerStyle(1);
  a->SetMarkerSize(0.4);
  a->SetMarkerColor(kBlack);
  a->SetLineColor(kBlack);
  a->SetFillColor(kWhite);
}

char* ParName( int par ) {
  if(par==0) return "SIGNAL";
  if(par==1) return "LAMBDA";
  if(par==2) return "SIGMA";
  if(par==3) return "FR2";
  if(par==4) return "FR3";
  if(par==5) return "FR4";
  if(par==6) return "B";
  if(par==7) return "SLO";
  if(par==8) return "CHI2/NDF";
}
