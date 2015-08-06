void convert(char *input) {
  ifstream file;
  ofstream outf;
  file.open( input );
  outf.open( Form("%s.cvt",input) );
  int key;
  for(;;) {
    file >> key;
    if(!file.good()) break;
    int arm = key/(8*4*12*64);
    int pkt = (key%(8*4*12*64))/(4*12*64);
    int chipmap = key%(4*12*64);
    int sen = chipmap/128;
    int roc = chipmap%128;
    outf << Form("KEY %d ==> ARM %d PKT %d SEN %d ROC %d\n",key,arm,pkt,sen,roc );
  }
  file.close();
  outf.close();
}
