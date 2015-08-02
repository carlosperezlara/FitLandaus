void extractHistograms(char *run = "MYRUNNUMBER") {
  TString cmd = Form("mkdir -p %s/adc",run);
  printf( "%s\n", cmd.Data() );
  gSystem->Exec( cmd.Data() );
  TFile *file = new TFile( Form("%s.root",run) );
  TH2D *a = (TH2D*) file->Get("adcHI");
  TH1D *ghi[2][8][24][128];
  for(int key=0; key!=49152; ++key) {
    int arm = key/(8*4*12*64);
    int pkt = (key%(8*4*12*64))/(4*12*64);
    int chipmap = key%(4*12*64);
    int sen = chipmap/128;
    int roc = chipmap%128;
    printf("A%d P%d S%d R%d\n",arm,pkt,sen,roc);
    ghi[arm][pkt][sen][roc] = a->ProjectionY( Form("HI_KEY%05d",key),key+1,key+1);
    ghi[arm][pkt][sen][roc]->SetName("out");
    ghi[arm][pkt][sen][roc]->SaveAs(  Form("%s/adc/HI_KEY%05d.root",run,key)  );
  }
}
