#include <Riostream.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TFile.h>
#include <TMath.h>
using namespace std;

#define CONST_NMISURE 5300       //5300 SE RISPETTO A 1, 2000 SE RISPETTO A 9
#define CONST_TDCCONV1 4.267e9
#define CONST_TDCCONV2 1.e-9

void calibrazione_tempi(){

//MACRO IN CUI FACCIO I CONTI RISPETTO AL 9

//metto tutti i valori delle TDC in degli arrays
/*  double t1[CONST_NMISURE], t2[CONST_NMISURE], t3[CONST_NMISURE], t4[CONST_NMISURE], t5[CONST_NMISURE];
  double dump = 0;
  int count = 0;
  TH1D *h21 = new TH1D("h19", "T1-T9", 400, -200, +200);
  TH1D *h31 = new TH1D("h29", "T2-T9", 400, -200, +200);
  TH1D *h41 = new TH1D("h39", "T3-T9", 400, -200, +200);
  TH1D *h51 = new TH1D("h49", "T4-T9", 400, -200, +200);
// TCanvas *c1 = new TCanvas();
//  TCanvas *c2 = new TCanvas();
//  TCanvas *c3 = new TCanvas();
//  TCanvas *c4 = new TCanvas();
//  TCanvas *c5 = new TCanvas();
  ifstream TDC19("TDC_19_doppia_+15ns_2000.dat");
  while(count<CONST_NMISURE){
    TDC19>>dump>>dump>>t1[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    count++;
  }
  TDC19.close();
//  c1->cd();
//  h19->Draw();

  count = 0;
  ifstream TDC29("TDC_29_doppia_+15ns_2000.dat");
  while(count<CONST_NMISURE){
    TDC29>>dump>>dump>>dump>>t2[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    count++;
  }
  TDC29.close();
//  c2->cd();
//  h29->Draw();

  count = 0;
  ifstream TDC39("TDC_39_doppia_+15ns_2000.dat");
  while(count<CONST_NMISURE){
    TDC39>>dump>>dump>>dump>>dump>>t3[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    count++;
  }
  TDC39.close();
//  c3->cd();
//  h39->Draw();

  count = 0;
  ifstream TDC49("TDC_49_doppia_+15ns_2000.dat");
  while(count<CONST_NMISURE){
    TDC49>>dump>>dump>>dump>>dump>>dump>>t4[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    count++;
  }
  TDC49.close();
//  c4->cd();
//  h49->Draw();

  count = 0;
  ifstream TDC59("TDC_59_doppia_+15ns_2000.dat");
  while(count<CONST_NMISURE){
    TDC59>>dump>>dump>>dump>>dump>>dump>>dump>>t5[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    count++;
  }
  TDC59.close();
//  c5->cd();
//  h59->Draw();

  count = 0;
  while(count<CONST_NMISURE){
    h21->Fill(t2[count]-t1[count]);
    h31->Fill(t3[count]-t1[count]);
    h41->Fill(t4[count]-t1[count]);
    h51->Fill(t5[count]-t1[count]);
    count++;
  }

  h21->Fit("gaus");
  h31->Fit("gaus");
  h41->Fit("gaus");
  h51->Fit("gaus");

  TCanvas *cPad = new TCanvas("cPad","Istogrammi delle DeltaT", 0, 0, 1600, 800);
  cPad->Divide(2,2);
  cPad->cd(1);
  h21->Draw();
  cPad->cd(2);
  h31->Draw();
  cPad->cd(3);
  h41->Draw();
  cPad->cd(4);
  h51->Draw();*/

//FINE MACRO IN CUI FACCIO I CONTI RISPETTO AL NOVE

//MACRO IN CUI FACCIO I CONTI RISPETTO ALL'UNO

  double t1[CONST_NMISURE], t2[CONST_NMISURE], t3[CONST_NMISURE], t4[CONST_NMISURE], t5[CONST_NMISURE];
  double dump = 0;
  double t21[CONST_NMISURE], t31[CONST_NMISURE], t41[CONST_NMISURE], t51[CONST_NMISURE];
  int count = 0;
  TH1D *h21 = new TH1D("h21", "T2-T1", 400, -200., +200.);
  TH1D *h31 = new TH1D("h31", "T3-T1", 400, -200., +200.);
  TH1D *h41 = new TH1D("h41", "T4-T1", 400, -200., +200.);
  TH1D *h51 = new TH1D("h51", "T5-T1", 400, -200., +200.);
// TCanvas *c1 = new TCanvas();
//  TCanvas *c2 = new TCanvas();
//  TCanvas *c3 = new TCanvas();
//  TCanvas *c4 = new TCanvas();
  ifstream TDC("TDC_QUINTUPLA_FINALE_5300.dat");
  bool fuori_scala[CONST_NMISURE];
  while(count<CONST_NMISURE){
    TDC>>dump>>dump>>t1[count]>>t2[count]>>t3[count]>>t4[count]>>t5[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    if(t1[count]>1000||t2[count]>1000||t3[count]>1000||t4[count]>1000||t5[count]>1000){
      fuori_scala[count] = 1;
//      cout << count+1 << endl;
    }
    else
      fuori_scala[count] = 0;
    count++;
  }

  cout << "Maschera riempita" << endl;

  int count_fuoriscala = 0;
  count = 0;
  while(count<CONST_NMISURE){
    if(fuori_scala[count]==1)
      count_fuoriscala++;
    count++;
  }

  cout << "Fuori scala contati" << endl;

  int real_nmis = CONST_NMISURE - count_fuoriscala;
  cout << real_nmis << endl;
  double t1_real[real_nmis], t2_real[real_nmis], t3_real[real_nmis], t4_real[real_nmis], t5_real[real_nmis];
  int real_count = 0;
  count = 0;
  while(count<CONST_NMISURE){
    if(fuori_scala[count]==0){
      t1_real[real_count] = t1[count];
      t2_real[real_count] = t2[count];
      t3_real[real_count] = t3[count];
      t4_real[real_count] = t4[count];
      t5_real[real_count] = t5[count];
      real_count++;
      count++;
    }
    else if(fuori_scala[count] == 1)
      count++;
  }

  cout << "Array di misure reali riempiti" << endl;

  count = 0;
  while(count<real_nmis){
    t21[count] = t2_real[count] - t1_real[count];
    t31[count] = t3_real[count] - t1_real[count];
    t41[count] = t4_real[count] - t1_real[count];
    t51[count] = t5_real[count] - t1_real[count];
    h21->Fill(t21[count]);
    h31->Fill(t31[count]);
    h41->Fill(t41[count]);
    h51->Fill(t51[count]);
    count++;
  }
  TDC.close();
  
  h21->Fit("gaus");
  h31->Fit("gaus");
  h41->Fit("gaus");
  h51->Fit("gaus");

  TCanvas *cPad = new TCanvas("cPad","Istogrammi delle DeltaT", 0, 0, 1600, 800);
  cPad->Divide(2,2);
  cPad->cd(1);
  h21->Draw();
  cPad->cd(2);
  h31->Draw();
  cPad->cd(3);
  h41->Draw();
  cPad->cd(4);
  h51->Draw();

//FINE MACRO IN CUI FACCIO I CONTI RISPETTO ALL'UNO

}