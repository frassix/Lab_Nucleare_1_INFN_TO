
#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
using namespace std;

//Cerca il minimo facendo la derivata da step a step, dove inverte il segno (aumenta) è minimo cercato
int TDerivate(int x[1024]){
  int p_sign = 1; //previous sign: serve per registrare il segno dello step precedente
  int min = 0;
  for(int i = 1; i < 1024; i++){
      int delta = x[i]-x[i-1];
      int sign = abs(delta)/delta;

      if(sign!=0 && p_sign!=0){
          if (sign > p_sign){
              return i-1;
          }
      }
      p_sign = sign;
  }
  return min; //non serve davvero essendo che abbiamo sempre il minimo, ma non voglio warning
}





void analisi(){
  const int nmisure = 2200;
  int riv1[nmisure], riv2[nmisure], riv3[nmisure], riv4[nmisure], riv5[nmisure];
  ifstream infile("ADC_3_2200.dat");
  int i = 0;
  int bins[1024];
  while(!infile.eof()){
    int nmis,riv0;
    infile>>nmis>>riv0>>riv1[i]>>riv2[i]>>riv3[i]>>riv4[i]>>riv5[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
  }
  infile.close();

//  TCanvas* c1 = new TCanvas();
//  TCanvas* c2 = new TCanvas();
//  TCanvas* c4 = new TCanvas();
//  TCanvas* c5 = new TCanvas();
  TH1I* histo0 = new TH1I("TEST","TEST",256,0,1024);
  TH1I* histo1 = new TH1I("ADC1","ADC1",256,0,1024);
  TH1I* histo2 = new TH1I("ADC2","ADC2",1024,0,1024);
  TH1I* histo3 = new TH1I("ADC3","ADC3",256,0,1024);
  TH1I* histo4 = new TH1I("ADC4","ADC4",1024,0,1024);
  TH1I* histo5 = new TH1I("ADC5","ADC5",1024,0,1024);
  for(int j = 0;j<nmisure;j++){
    histo0->Fill(riv3[j]);
    histo1->Fill(riv1[j]);
    histo2->Fill(riv2[j]);
    histo3->Fill(riv3[j]);
    histo4->Fill(riv4[j]);
    histo5->Fill(riv5[j]);
  }

  //Prova con plot polinomiale di 4° grado
  TCanvas* t1 = new TCanvas();
  TF1* fit_poly = new TF1("Poly 4","[0]*x*x*x*x+[1]*x*x*x+[2]*x*x+[3]*x+[4]",70.,160.);
  histo0->Fit(fit_poly,"","",70.,160.);
  int min = int(fit_poly->GetMinimumX(80,160));
  //int min = int(histo0->GetBin(min_value));
//  int min = TDerivate(bins);
  std::cout << "Minimo: "<< min << std::endl;
  histo0->Draw();
  fit_poly->Draw("same");

  int norm=0;
  for(int i=0; i<1024; i++){
    bins[i] = int(histo0->GetBinContent(i));
    if(i>=min){
      norm+=bins[i];
    }
  }

  
//  c1->cd();
//  histo1->Draw();

//  c2->cd();
//  histo2->Draw();
  TCanvas* t2 = new TCanvas();
  TF1* fit3 = new TF1("Landau","[2]*TMath::Landau(x,[0],[1],1)",min,300);
  TF1* Gauss3 = new TF1("Gauss","gaus",0,min);
  fit3->SetParameter(2,norm);
  histo3->Fit(fit3,"R");
  histo3->Fit(Gauss3,"","",0,min);
  Gauss3->SetLineColor(kBlue);

  histo3->Draw();
  fit3->Draw("same");
  Gauss3->Draw("same");


//  c4->cd();
//  histo4->Draw();

//  c5->cd();
//  histo5->Draw();
}