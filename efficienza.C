#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
using namespace std;


void polar_angle(double G_est=0.38, double sG_est=0.01){
  ifstream infile("montecarlosim.txt");
  int i = 0;
  const int n = 101;
  double m[n], G2[n], G3[n], Geo[n];
  while(!infile.eof()){
    infile>>m[i]>>G2[i]>>G3[i];
    Geo[i]=G3[i]/G2[i];
    i++;
  }

  TCanvas* t1 = new TCanvas("Flux angle","Flux angle",80,80,1500,1000);
  TGraphErrors* tg1 = new TGraphErrors(n, m, Geo, NULL, NULL);
  TF1* l1 = new TF1("Line1","[0]*x+[1]", 0.,3.3);
  TF1* l2 = new TF1("Line2","[0]*x+[1]", 3.3, 6.6);
  TF1* l3 = new TF1("Line3","[0]*x+[1]", 6.6, 10.);
  tg1->Fit(l1,"","",0,3.3);
  tg1->Fit(l2,"","",3.3,6.6);
  tg1->Fit(l3,"","",6.6,10);
  
  double m1 = l1->GetParameter(0);
  double q1 = l1->GetParameter(1);
  double m2 = l2->GetParameter(0);
  double q2 = l2->GetParameter(1);
  double m3 = l3->GetParameter(0);
  double q3 = l3->GetParameter(1);
  double x_est = 0;
  double sx_up = 0;
  double sx_down = 0;

  if((G_est-q1)/m1<3.3){
    x_est = (G_est-q1)/m1;
  }
  else if ((3.3<=(G_est-q2)/m2) && ((G_est-q2)/m2<6.6)){
    x_est = (G_est-q2)/m2;
  }
  else{
    x_est = (G_est-q3)/m3;
  }

  if((G_est+sG_est-q1)/m1<3.3){
    sx_up = (G_est+sG_est-q1)/m1;
  }
  else if ((3.3<=(G_est+sG_est-q2)/m2) && ((G_est+sG_est-q2)/m2<6.6)){
    sx_up = (G_est+sG_est-q2)/m2;
  }
  else{
    sx_up = (G_est+sG_est-q3)/m3;
  }

  if((G_est-sG_est-q1)/m1<3.3){
    sx_down = (G_est-sG_est-q1)/m1;
  }
  else if ((3.3<=(G_est-sG_est-q2)/m2) && ((G_est-sG_est-q2)/m2<6.6)){
    sx_down = (G_est-sG_est-q2)/m2;
  }
  else{
    sx_down = (G_est-sG_est-q3)/m3;
  }

  TLine *Orz1 = new TLine(0., G_est+sG_est, sx_up, G_est+sG_est);
  TLine *Orz2 = new TLine(0., G_est, x_est, G_est);
  TLine *Orz3 = new TLine(0., G_est-sG_est, sx_down, G_est-sG_est);
  Orz2->SetLineColor(kRed);
  Orz2->SetLineStyle(kDotted);
  Orz2->SetLineWidth(4);

  TLine *Ver1 = new TLine(sx_up, 0., sx_up, G_est+sG_est);
  TLine *Ver2 = new TLine(x_est, 0., x_est, G_est);
  TLine *Ver3 = new TLine(sx_down, G_est-sG_est, sx_down, 0.);
  Ver2->SetLineColor(kRed);
  Ver2->SetLineStyle(kDotted);
  Ver2->SetLineWidth(4);

  tg1->SetTitle("Fattore geometrico vs fattore di potenza");
  tg1->SetMarkerColor(4);
  tg1->SetMarkerStyle(21);
  tg1->GetXaxis()->SetTitle("Fattore di potenza");
  tg1->GetYaxis()->SetTitle("Fattore geometrico");
  tg1->Draw("ap");
  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");
  Orz1->Draw("same");
  Orz2->Draw("same");
  Orz3->Draw("same");
  Ver1->Draw("same");
  Ver2->Draw("same");
  Ver3->Draw("same");
}