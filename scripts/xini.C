void xini(const char *run="428211_429133_5s", int sen=0) {
  gSystem->Exec( Form("mkdir -p %s/SEN%03d",run,sen) );
  TString inname;
  TFile *file;
  TH1D *out;
  ofstream ofi;
  ofi.open( Form("%s/SEN%03d/xini.dat",run,sen) );
  int bmin;
  for(int key=sen*128; key!=(sen+1)*128; ++key) {
    inname = Form("%s/adc/HI_KEY%05d.root",run,key);
    file = new TFile( inname.Data() );
    out = (TH1D*) file->Get("out");
    int nbin = 3;
    for(int i=0; i!=51; ++i) {
      if( out->GetBinContent(i) > 0 ) --nbin;
      if(nbin==0) {
	bmin = i;
	break;
      }
    }
    cout << key << " | " << bmin << " => ";
    if(bmin<10) bmin=10;
    if(bmin>50) bmin=1;
    cout << bmin << endl;
    ofi << key << " " << bmin << endl;
  }
  ofi.close();
}
