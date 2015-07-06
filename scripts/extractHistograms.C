void extractHistograms(char *run = "MYRUNNUMBER") {
  TString cmd = Form("mkdir -p %s/adc",run);
  printf( "%s\n", cmd.Data() );
  gSystem->Exec( cmd.Data() );
  TFile *file = new TFile( Form("%s.root",run) );
  TH2D *a = (TH2D*) file->Get("adcHI");
  TH2D *b = (TH2D*) file->Get("adcLO");
  TH1D *ghi[2][8][24][128];
  TH1D *glo[2][8][24][128];
  for(int bins=0; bins!=49152; ++bins) {
    int sel = bins;
    int arm = sel/24576;
    int lyr = (sel%24576)/3072;
    int sen = ((sel%24576)%3072)/128;
    int mpd = (((sel%24576)%3072)%128);
    ghi[arm][lyr][sen][mpd] = a->ProjectionY( Form("HICH%d_ARM%d_LYR%d_SEN%d_MPD%d",sel,arm,lyr,sen,mpd),sel+1,sel+1);
    ghi[arm][lyr][sen][mpd]->SetName("out");
    ghi[arm][lyr][sen][mpd]->SaveAs(  Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run,arm,lyr,sen,mpd)  );
    glo[arm][lyr][sen][mpd] = b->ProjectionY( Form("LOCH%d_ARM%d_LYR%d_SEN%d_MPD%d",sel,arm,lyr,sen,mpd),sel+1,sel+1);
    glo[arm][lyr][sen][mpd]->SetName("out");
    glo[arm][lyr][sen][mpd]->SaveAs(  Form("%s/adc/LO_ARM%d_LYR%d_S%d_M%d.root",run,arm,lyr,sen,mpd)  );
  }
}
