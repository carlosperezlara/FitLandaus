#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>

TF1* GetFit(const char *run, const char *outname, int lyr) {
  //TF1 *ret = new TF1("fit_H1",
  //                   "[0]*((1-[3])*TMath::Landau(x,[1],[2],1)+[3]*TMath::Landau(x,2.14*[1],2*[2],1))");
  const char *la1 = "(1-[3]-[4]-[5])*TMath::Landau(x,1.00*[1],1.0*[2],1)";
  const char *la2 = "[3]*TMath::Landau(x,2.14*[1],2.0*[2],1)";
  const char *la3 = "[4]*TMath::Landau(x,3.33*[1],3.0*[2],1)";
  const char *la4 = "[5]*TMath::Landau(x,4.55*[1],4.0*[2],1)";
  TF1 *ret = new TF1("fit_H1",
                     Form("[0]*(%s+%s+%s+%s)",la1,la2,la3,la4) );
  ret->SetParName(0,"A");
  ret->SetParName(1,"lambda");
  ret->SetParName(2,"sigma");
  ret->SetParName(3,"f2");
  ret->SetParameter(0,1e4);  ret->SetParLimits(0,1e2,1e7);
  ret->SetParameter(1,22);   ret->SetParLimits(1,5,40);
  ret->SetParameter(2,5.0);  ret->SetParLimits(2,1.8,10.0);
  if(lyr>1) {
    ret->SetParameter(1,15);   ret->SetParLimits(1,2,30);
    ret->SetParameter(2,5.0);  ret->SetParLimits(2,0.8,10.0);
  }
  ret->SetParameter(3,0.20); ret->SetParLimits(3,-1e-10,0.40);
  ret->SetParameter(4,0.0);
  ret->SetParameter(5,0.0);
  ret->SetLineColor(kRed-3);

  ifstream infit;
  infit.open( Form("%s/fit/%s.dat",run,outname) );
  double tmp;
  bool found=false;
  for(int n=0; n!=ret->GetNpar(); ++n) {
    infit >> tmp;
    if(!infit.good()) break;
    ret->SetParameter(n,tmp);
    infit >> tmp;
    found = true;
  }
  infit.close();
  if(found) {
    cout << " Previous fit results found" << endl;
    ret->SetParLimits(4,-1e-10,0.20);
    ret->SetParLimits(5,-1e-10,0.20);
  } else {
    ret->SetParLimits(4,+1,-1);
    ret->SetParLimits(5,+1,-1);
  }
  return ret;
}

TF1* GetMIP(TF1 *fit, int n, int color=kBlue-3) {
  double a = fit->GetParameter(0);
  double b = fit->GetParameter(1);
  double c = fit->GetParameter(2);
  switch(n) {
  case(1):
    a *= (1-fit->GetParameter(3)-fit->GetParameter(4)-fit->GetParameter(5));
    break;
  case(2):
    a *= fit->GetParameter(3);
    b *= 2.14;
    c *= 2;
    break;
  case(3):
    a *= fit->GetParameter(4);
    b *= 3.33;
    c *= 3;
    break;
  case(4):
    a *= fit->GetParameter(5);
    b *= 4.55;
    c *= 4;
    break;
  }
  TF1 *ret = new TF1(Form("MIP%d",n), Form("%f*TMath::Landau(x,%f,%f,1)",a,b,c),0,150);
  ret->SetLineColor(color);
  return ret;
}

void fit(const char *run="19020_19035",
	 //const char *run="19037_19049",
	 //const char *run="19052_19057",
	 int arm=0, int lyr=0, int sen=13, int mpd=31,
	 bool draw=true,
	 //double xfit_min=6.5, double xfit_max=62.5,
	 double xfit_min=14.5, double xfit_max=82.5,
	 int minentries=50) {
  gSystem->Exec( Form("mkdir -p %s/fit",run) );
  gSystem->Exec( Form("mkdir -p %s/fiteps",run) );
  // data
  TString inname = Form("%s/adc/HI_ARM%d_LYR%d_S%d_M%d.root",run,arm,lyr,sen,mpd);
  TString outname = Form("HI_ARM%d_LYR%d_S%d_M%d_%.0f_%.0f",arm,lyr,sen,mpd,xfit_min,xfit_max);
  TFile *file = new TFile( inname.Data() );
  cout << inname.Data() << endl;

  TH1D *out = (TH1D*) file->Get("out");
  int bmin = out->GetXaxis()->FindBin(xfit_min);
  int bmax = out->GetXaxis()->FindBin(xfit_max);
  int entries = out->Integral(bmin,bmax);
  if(entries<minentries) {
    cout << "not enough entries: ";
    cout << entries << endl;
    return;
  }

  // fit
  TF1 *fitH = GetFit(run,outname.Data(),lyr);
  out->Fit(fitH,"MELIR","",xfit_min,xfit_max);
  if(fitH->GetParameter(1)<xfit_min) {
    cout << "Reducing fit range by 2" << endl;
    out->Fit(fitH,"MELIR","",xfit_min-2,xfit_max);
  }
  TF1 *MIPH1 = GetMIP(fitH,1,kCyan-3);
  TF1 *MIPH2 = GetMIP(fitH,2,kGreen-3);
  TF1 *MIPH3 = GetMIP(fitH,3,kOrange-3);
  TF1 *MIPH4 = GetMIP(fitH,4,kMagenta-3);

  double amp = fitH->GetParameter(0);
  double eamp= fitH->GetParError(0);
  double lda = fitH->GetParameter(1);
  double elda= fitH->GetParError(1);
  double sg1 = fitH->GetParameter(2);
  double esg1= fitH->GetParError(2);
  double fr2 = fitH->GetParameter(3);
  double efr2= fitH->GetParError(3);
  double fr3 = fitH->GetParameter(4);
  double efr3= fitH->GetParError(4);
  double fr4 = fitH->GetParameter(5);
  double efr4= fitH->GetParError(5);
  double fr1 = 1 - fr2 - fr3 - fr4;
  double ncs = fitH->GetChisquare()/fitH->GetNDF();

  // saving fit
  ofstream outfit;
  outfit.open( Form("%s/fit/%s.dat",run,outname.Data()) );
  outfit << amp << " " << eamp << endl;
  outfit << lda << " " << elda << endl;
  outfit << sg1 << " " << esg1 << endl;
  outfit << fr2 << " " << efr2 << endl;
  outfit << fr3 << " " << efr3 << endl;
  outfit << fr4 << " " << efr4 << endl;
  outfit << ncs << endl;
  outfit.close();
  cout << "Parameters saved to ";
  cout << outname.Data() << ".dat" << endl;

  // draw
  if(!draw) return;

  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  TCanvas *main = new TCanvas("main","main");
  //main->SetLogy(1);
  out->Draw("HE");
  double ymax = entries/10;
  out->GetYaxis()->SetRangeUser(0.5,ymax);
  out->GetXaxis()->SetRangeUser(-5,85);
  out->Sumw2();
  out->SetLineColor(kBlack);
  out->SetMarkerStyle(20);
  //out->SetMarkerSize(0.3);
  out->SetTitle("");
  out->GetXaxis()->SetTitle("ADC-PED (a.u.)");

  MIPH1->Draw("SAME");
  MIPH2->Draw("SAME");
  MIPH3->Draw("SAME");
  MIPH4->Draw("SAME");
  fitH->SetRange(xfit_min,xfit_max);
  fitH->Draw("SAME");
  TLatex *text = new TLatex();
  text->DrawLatex(0, (1.03*(ymax)), inname.Data() );
  text->DrawLatex(30, (0.83*(ymax)), Form("Entries  %d",entries) );
  text->DrawLatex(60, (0.83*(ymax)), Form("#Chi^{2} / NDF  %.2f",ncs) );
  text->SetTextColor(kRed-3);
  text->DrawLatex(60, (0.73*(ymax)), Form("#Alpha  %.0f #pm %.0f",fitH->GetParameter(0),fitH->GetParError(0)) );
  text->DrawLatex(30, (0.73*(ymax)), Form("#lambda  %.1f #pm %.1f",lda,elda) );
  text->DrawLatex(30, (0.63*(ymax)), Form("#sigma  %.1f #pm %.1f",sg1,esg1) );
  text->SetTextColor(kCyan-3);
  text->DrawLatex(60, (0.63*(ymax)), Form("f_{1}  %.2f",fr1) );
  text->SetTextColor(kGreen-3);
  text->DrawLatex(60, (0.53*(ymax)), Form("f_{2}  %.2f #pm %.2f",fr2,efr2) );
  text->SetTextColor(kOrange-3);
  text->DrawLatex(60, (0.43*(ymax)), Form("f_{3}  %.2f #pm %.2f",fr3,efr3) );
  text->SetTextColor(kMagenta-3);
  text->DrawLatex(60, (0.33*(ymax)), Form("f_{4}  %.2f #pm %.2f",fr4,efr4) );
  text->SetTextColor(kBlack);
  text->SetTextSize(0.035);
  text->DrawLatex(5, (0.93*(ymax)), "#Alpha { f_{1} L(x,#lambda,#sigma) + f_{2} L(x,2.14#lambda,2#sigma) + f_{3} L(x,3.33#lambda,3#sigma) + f_{4} L(x,4,55#lambda,4#sigma)}");
  //main->SaveAs( Form("%s/fiteps/%s.eps",run,outname.Data()), "eps" );
  //main->SaveAs( Form("%s/fiteps/%s.jpg",run,outname.Data()), "jpg" );
  main->SaveAs( Form("%s/fiteps/%s.png",run,outname.Data()), "png" );
  return;
}

