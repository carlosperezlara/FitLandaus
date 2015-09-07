int state[49152]={0};
TString spar[9] = {"AMP","LDA","SG1","FR2","FR3","FR4","BGR","SLO","NCS"};
Float_t par[9][128] = {0};
Float_t epar[9][128] = {0};
Float_t x[128] = {0};
Float_t ex[128] = {0};

void drawSensor(char *run = "428211_429133_5s", int sen=0) {
  LoadState();
  for(int i=0; i!=128; ++i) x[i] = sen*128+i;
  LoadData(run,sen*128);
  TGraphErrors *hi[9];
  for(int i=0; i!=9; ++i) {
    hi[i] = MakeGraph(i);
    new TCanvas();
    hi[i]->Draw( "APL" );
  }
}

void LoadState() {
  ifstream fin;
  fin.open("bad.lst");
  int read;
  for(;fin.good();) {
    fin >> read;
    if(!fin.good()) break;
    else state[read] = 1;
  }
}
void LoadData(char *run,int init) {
  ifstream fin;
  TString filename;
  Float_t peep;
  for(int i=0; i!=128; ++i) {
    filename = Form("%s/SEN%03d/HI_KEY%05d.dat",run,(init+i)/128,init+i);
    fin.open( filename.Data() );
    fin >> peep;
    if(!fin.good()) {
    } else {
      par[0][i] = peep;
      fin >> epar[0][i];
      fin >> par[1][i] >> epar[1][i];
      fin >> par[2][i] >> epar[2][i];
      fin >> par[3][i] >> epar[3][i];
      fin >> par[4][i] >> epar[4][i];
      fin >> par[5][i] >> epar[5][i];
      fin >> par[6][i] >> epar[6][i];
      fin >> par[7][i] >> epar[7][i];
      fin >> par[8][i];
    }
    fin.close();
    if( state[init+i] ) 
      for(int j=0; j!=9; ++j) {
	par[j][i] = 0;
	epar[j][i] = 0;
      }
  }
  return;
}

TGraphErrors* MakeGraph(int i) {
  TGraphErrors *ret = new TGraphErrors(128,x,par[i],ex,epar[i]);
  ret->GetXaxis()->SetTitle( "KEY" );
  ret->GetYaxis()->SetTitle( spar[i].Data() );
  ret->SetMarkerStyle( 20 );
  return ret;
}
