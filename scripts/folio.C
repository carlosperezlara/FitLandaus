void folio(int arm=0,int lyr=0,int sen=12) {
  ofstream fout;
  fout.open( Form("eps/ARM%dLYR%dSEN%d.tex",arm,lyr,sen) );
  fout << "\\documentclass{article}\n";
  fout << "\\usepackage{graphicx}\n";
  fout << "\\usepackage[letterpaper,margin=1in]{geometry}\n";
  fout << "\\begin{document}\n";
  for(int i=0; i!=128; i+=4) {
    fout << Form("\\includegraphics[width=0.245\\textwidth]{eps/ARM%dLYR%dSEN%d/MPD%03d.jpg}\n",arm,lyr,sen,i);
    fout << Form("\\includegraphics[width=0.245\\textwidth]{eps/ARM%dLYR%dSEN%d/MPD%03d.jpg}\n",arm,lyr,sen,i+1);
    fout << Form("\\includegraphics[width=0.245\\textwidth]{eps/ARM%dLYR%dSEN%d/MPD%03d.jpg}\n",arm,lyr,sen,i+2);
    fout << Form("\\includegraphics[width=0.245\\textwidth]{eps/ARM%dLYR%dSEN%d/MPD%03d.jpg}\\\\\n",arm,lyr,sen,i+3);
  }
  fout << "\\end{document}\n";
  fout.close();
}
