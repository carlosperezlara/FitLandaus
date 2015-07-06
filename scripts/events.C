void events() {
  int nevts[500], rnum[500];
  int nruns = getNEVTS(rnum,nevts,3);
  TGraph *gr = new TGraph(nruns,rnum,nevts);
  gr->Draw("A*");
}

int getNEVTS(int a[500], int b[500], int bin) {
  TFile *rfile;
  TH1D *hist;
  ifstream fin;
  fin.open("runs.lst");
  TString sfile;
  int n=0;
  for(;;++n) {
    fin >> sfile;
    if(!fin.good()) break;
    a[n] = sfile.Atoi();
    b[n] = -1;
    rfile = new TFile( sfile.Data() );
    hist = (TH1D*) rfile->Get( "Events" );
    if(!hist) continue;
    b[n] = hist->GetBinContent( bin );
  }
  return n;
}
