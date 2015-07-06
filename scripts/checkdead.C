void checkdead(const char *run="19020_19035") {
  TString inname;
  TFile *file;
  TH1D *out;
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr)
      for(int sen=0; sen!=24; ++sen)
	for(int mpd=0; mpd!=128; ++mpd) {
	  inname = Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run,arm,lyr,sen,mpd);
	  file = TFile::Open( inname.Data() );
	  out = (TH1D*) file->Get("out");
	  if(out->GetEntries()==0) printf("%d %d %d %d\n",arm,lyr,sen,mpd);
	  file->Close();
	}
  return;
}

