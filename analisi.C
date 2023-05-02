#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
using namespace std;

void analisi(){
  const int nmisure = 2200;
  int riv1[nmisure], riv2[nmisure], riv3[nmisure], riv4[nmisure], riv5[nmisure];
  ifstream infile("ADC_3_2200.dat");
  int i = 0;
  while(!infile.eof()){
    int nmis,riv0;
    infile>>nmis>>riv0>>riv1[i]>>riv2[i]>>riv3[i]>>riv4[i]>>riv5[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    if(i%100==0) cout << i << endl;
  }
  infile.close();

//  TCanvas* c1 = new TCanvas();
//  TCanvas* c2 = new TCanvas();
  TCanvas* c3 = new TCanvas();
//  TCanvas* c4 = new TCanvas();
//  TCanvas* c5 = new TCanvas();

  TH1I* histo1 = new TH1I("ADC1","ADC1",1024,0,1024);
  TH1I* histo2 = new TH1I("ADC2","ADC2",1024,0,1024);
  TH1I* histo3 = new TH1I("ADC3","ADC3",1024,0,1024);
  TH1I* histo4 = new TH1I("ADC4","ADC4",1024,0,1024);
  TH1I* histo5 = new TH1I("ADC5","ADC5",1024,0,1024);
  for(int j = 0;j<nmisure;j++){
    histo1->Fill(riv1[j]);
    histo2->Fill(riv2[j]);
    histo3->Fill(riv3[j]);
    histo4->Fill(riv4[j]);
    histo5->Fill(riv5[j]);
  }

//  c1->cd();
//  histo1->Draw();

//  c2->cd();
//  histo2->Draw();

  TF1* fit3 = new TF1("Landau","2200*TMath::Landau(x,[0],[1],1)",108,300);
  histo3->Fit(fit3,"R");
  c3->cd();
  histo3->Draw();
  fit3->Draw("same");

//  c4->cd();
//  histo4->Draw();

//  c5->cd();
//  histo5->Draw();
}