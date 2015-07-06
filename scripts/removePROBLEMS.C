void removePROBLEMS() {
  ifstream inf;
  inf.open("PROBLEM1.lst");
  TString a, b, arm, lyr, sen, mpd;
  for(;;) {
    inf >> a;
    cout << a.Data() << "  -> ";
    if(!inf.good()) break;
    arm = ((TObjString*) a.Tokenize("_")->At(2))->GetString();
    lyr = ((TObjString*) a.Tokenize("_")->At(3))->GetString();
    sen = ((TObjString*) a.Tokenize("_")->At(4))->GetString();
    mpd = ((TObjString*) a.Tokenize("_")->At(5))->GetString();
    b = Form("19020_19035/fit/LO_%s_%s_%s_%s_2_18.dat",arm.Data(),lyr.Data(),sen.Data(),mpd.Data());
    cout << b.Data() << endl;
    gSystem->Exec( Form("rm %s",b.Data()) );
  }
}
