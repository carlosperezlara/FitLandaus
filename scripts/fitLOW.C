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

double alp;
double lda;
double sgm;
double f2;
double f3;
double f4;
double f1=0;
double elda;
double esgm;

TF1* GetFit(const char *run, const char *inname, const char *outname) {
  ifstream infit;
  infit.open( Form("%s/fit/%s.dat",run,inname) );
  double tmp;
  bool found;
  infit >> alp >> tmp >> lda >> elda >> sgm >> esgm >> f2 >> tmp >> f3 >> tmp >> f4;
  infit.close();
  cout << alp << endl;
  cout << lda << endl;
  cout << sgm << endl;
  cout << f2 << endl;
  cout << f3 << endl;
  cout << f4 << endl;

  double f1 = 1-f2-f3-f4;
  if(1-f2<0.01) return NULL;  
  const char *la1 = Form("%f*TMath::Landau(x,1.00*%f*[0],1.0*%f*[1],1)",f1,lda,sgm);
  const char *la2 = Form("%f*TMath::Landau(x,2.14*%f*[0],2.0*%f*[1],1)",f2,lda,sgm);
  const char *la3 = Form("%f*TMath::Landau(x,3.33*%f*[0],3.0*%f*[1],1)",f3,lda,sgm);
  const char *la4 = Form("%f*TMath::Landau(x,4.55*%f*[0],4.0*%f*[1],1)",f4,lda,sgm);
  TF1 *ret = new TF1("fit_H1", Form("%f*(%s+%s+%s+%s)",alp,la1,la2,la3,la4) );
  ret->SetParName(0,"sLDA");
  ret->SetParName(1,"SSGM");
  ret->SetParameter(0,1.0);  ret->SetParLimits(0,1e-3,2.0);
  ret->SetParameter(1,1.0);  ret->SetParLimits(1,1e-3,2.0);
  ret->SetLineColor(kRed-3);
  infit.open( Form("%s/fit/%s.dat",run,outname) );
  tmp;
  found=false;
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
  }
  cout << lda << endl;
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

void fitLOW(const char *run="19020_19035",
	    int arm=0, int lyr=2, int sen=21, int mpd=0,
	    bool draw=true,
	    //int old_xfit_min=12, int old_xfit_max=82,
	    int old_xfit_min=8, int old_xfit_max=62,
	    double xfit_min=2.5, double xfit_max=18.5) {
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  TString inname = Form("%s/adc/LO_ARM%d_LYR%d_S%d_M%d.root",run,arm,lyr,sen,mpd);
  TString prename = Form("HI_ARM%d_LYR%d_S%d_M%d_%d_%d",arm,lyr,sen,mpd,old_xfit_min,old_xfit_max);
  TString outname = Form("LO_ARM%d_LYR%d_S%d_M%d_%.0f_%.0f",arm,lyr,sen,mpd,xfit_min,xfit_max);
  TFile *file = new TFile( inname.Data() );
  cout << inname.Data() << endl;
  cout << prename.Data() << endl;
  cout << outname.Data() << endl;

  TF1 *fitH = GetFit(run,prename.Data(),outname.Data());
  if(!fitH) return;
  if(lda<1) return;

  // data
  TH1D *out = (TH1D*) file->Get("out");
  double ymax = out->GetMaximum();
  int entries = out->Integral(xfit_min,xfit_max);
  out->Sumw2();
  out->SetLineColor(kBlack);
  out->SetMarkerStyle(20);
  out->SetTitle("");

  // fit
  TCanvas *main = new TCanvas("main","main");
  main->SetLogy(1);
  out->Draw("HE");
  out->GetYaxis()->SetRangeUser(0.5,ymax);
  out->GetXaxis()->SetRangeUser(-4.5,50.5);

  out->Fit(fitH,"QENIML","",xfit_min,xfit_max);
  TF1 *MIPH1 = GetMIP(fitH,1,kCyan-3);
  TF1 *MIPH2 = GetMIP(fitH,2,kGreen-3);
  TF1 *MIPH3 = GetMIP(fitH,3,kOrange-3);
  TF1 *MIPH4 = GetMIP(fitH,4,kMagenta-3);

  double sLDA = fitH->GetParameter(0);
  double eLDA= fitH->GetParError(0);
  double sSGM = fitH->GetParameter(1);
  double eSGM = fitH->GetParError(1);
  double ncs = fitH->GetChisquare()/fitH->GetNDF();

  double ldalow = lda*sLDA;
  double sgmlow = sgm*sSGM;

  double eldalow = TMath::Abs(ldalow)*TMath::Sqrt( elda/lda*elda/lda + eLDA/sLDA*eLDA/sLDA );
  double esgmlow = TMath::Abs(sgmlow)*TMath::Sqrt( esgm/sgm*esgm/sgm + eSGM/sSGM*eSGM/sSGM );

  ofstream outfit;
  outfit.open( Form("%s/fit/%s.dat",run,outname.Data()) );
  outfit << sLDA << " " << eLDA << endl;
  outfit << sSGM << " " << eSGM << endl;
  outfit << ldalow << endl;
  outfit << sgmlow << endl;
  outfit << ncs << endl;
  outfit.close();

  // draw
  if(!draw) return;

  MIPH1->Draw("SAME");
  MIPH2->Draw("SAME");
  MIPH3->Draw("SAME");
  MIPH4->Draw("SAME");
  fitH->SetRange(xfit_min,xfit_max);
  fitH->Draw("SAME");
  TLatex *text = new TLatex();
  text->DrawLatex(1, TMath::Exp(1.03*TMath::Log(ymax)), inname );
  text->DrawLatex(20, TMath::Exp(0.93*TMath::Log(ymax)), Form("Entries  %d",entries) );
  text->SetTextColor(kRed-3);
  text->DrawLatex(44, TMath::Exp(0.83*TMath::Log(ymax)),  Form("#chi^{2} / NDF  %.2f",ncs) );
  text->DrawLatex(20, TMath::Exp(0.83*TMath::Log(ymax)),  Form("S_{#lambda}  %.3f #pm %.3f",sLDA,eLDA) );
  text->DrawLatex(20, TMath::Exp(0.73*TMath::Log(ymax)),  Form("S_{#sigma}  %.3f #pm %.3f",sSGM,eSGM) );
  text->SetTextColor(kBlue-3);
  text->DrawLatex(20, TMath::Exp(0.60*TMath::Log(ymax)),  Form("#lambda^{H}  %.1f #pm %.1f   #rightarrow  #lambda^{L}  %.1f #pm %.1f",lda,elda,ldalow,eldalow) );
  text->DrawLatex(20, TMath::Exp(0.50*TMath::Log(ymax)),  Form("#sigma^{H}  %.1f #pm %.1f   #rightarrow  #sigma^{L}  %.1f #pm %.1f",sgm,esgm,sgmlow,esgmlow) );
  text->SetTextColor(kBlack);
  text->SetTextSize(0.04);
  main->SaveAs( Form("%s/fiteps/%s.png",run,outname.Data()), "png" );
  return;
}
