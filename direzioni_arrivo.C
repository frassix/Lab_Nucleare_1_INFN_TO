#include <Riostream.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
using namespace std;

#define CONST_C 2.99792458e8  
#define CONST_NMISURE 5300
#define CONST_TDCCONV 4.267e9

//NOTA BENE: ANALISI DI THETA E PHI CON DELTAT DEL 9, ANALISI DELLO SPESSORE CON DELTAT DELL'1

void direzioni_arrivo(){
//definizione delle posizioni, mettendo l'origine al centro del rivelatore 4, l'asse x lungo la congiungente 4-3 e l'asse y lungo 4-5
//per semplicità impongo che il quadrilatero formato dai rivelatori esterni sia un rettangolo,
//(?) prendendo come valore dei lati paralleli la media +- semidispersione (?)
  double l = 11.092, L = 11.729;   //lato lungo x, lato lungo y
  double dl = 0.065, dL = 0.053; //incertezze

//timing degli hit sui rivelatori
//arrays con i tempi di hit per i rivelatori in quintupla 
  int dump = 0;
  double t1_dump[CONST_NMISURE], t2_dump[CONST_NMISURE], t3_dump[CONST_NMISURE], t4_dump[CONST_NMISURE], t5_dump[CONST_NMISURE]; 

  //PARTE IN CUI FACCIO I CONTI RISPETTO ALL'UNO

  double t21 = 31., t31 = 16., t41 = 15., t51 = 11.;     //NB RISPETTO ALL'1
//  double t21 = 25., t31 = 12., t41 = 10., t51 = 10.;   //NB RISPETTO AL 9
  int count = 0;
  bool fuori_scala[CONST_NMISURE];
  ifstream TDCfile("TDC_QUINTUPLA_FINALE_5300.dat");
  while(!TDCfile.eof()){
    TDCfile>>dump>>dump>>t1_dump[count]>>t2_dump[count]>>t3_dump[count]>>t4_dump[count]>>t5_dump[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    //creo array maschera per verificare quali sono fuori scala
    if(t1_dump[count]>1000||t2_dump[count]>1000||t3_dump[count]>1000||t4_dump[count]>1000||t5_dump[count]>1000){
      fuori_scala[count] = 1;
//      cout << count+1 << endl;
    }
    else
      fuori_scala[count] = 0;
    count++;
  }
  TDCfile.close();

  //conto i fuori scala
  count = 0;
  int count_fuoriscala = 0;
  while(count<CONST_NMISURE){
    if(fuori_scala[count]==1)
      count_fuoriscala++;
    count++;
  }
  
  //riempio degli array nuovi contenenti solo i valori in scala di TDC, cioè quelli che sono compatibili con una quintupla
  int real_nmis = CONST_NMISURE - count_fuoriscala;
  cout << real_nmis << endl;
  double t1_real[real_nmis], t2_real[real_nmis], t3_real[real_nmis], t4_real[real_nmis], t5_real[real_nmis];
  int real_count = 0;
  count = 0;
  while(count<CONST_NMISURE){
    if(fuori_scala[count]==0){
      t2_real[real_count] = (1./CONST_TDCCONV)*(t2_dump[count] - t21);
      t3_real[real_count] = (1./CONST_TDCCONV)*(t3_dump[count] - t31);
      t4_real[real_count] = (1./CONST_TDCCONV)*(t4_dump[count] - t41);
      t5_real[real_count] = (1./CONST_TDCCONV)*(t5_dump[count] - t51);
      real_count++;
      count++;
    }
    else if(fuori_scala[count] == 1)
      count++;
  }
//  cout << real_count << endl;


  //FINE DELLA PARTE IN CUI FACCIO I CONTI RISPETTO ALL'UNO

  double t24, t35;
  double A, B; //costanti da cui ricavo \theta e \phi
  double theta, phi; //angolo zenitale e azimutale
  double T = 0.;
  TH1D *histT = new TH1D("Spessore", "Spessore", 121, -60.e-9, +60.e-9);
  TH1D *histTheta = new TH1D("Angolo zenitale", "N(Theta)", 100, 0., TMath::PiOver2()); 
  TH1D *histPhi = new TH1D("Angolo azimutale", "N(Phi)", 400, 0., TMath::TwoPi());
  count = 0;
  while(count<real_nmis){
    t24 = (double)t2_real[count] - (double)t4_real[count];
    t35 = (double)t3_real[count] - (double)t5_real[count];
    A = CONST_C/(2.*l)*((double)t24+(double)t35);
    B = CONST_C/(2.*L)*((double)t24-(double)t35);
  
    theta = TMath::ASin(TMath::Sqrt(A*A+B*B));
    if(TMath::Sqrt(A*A+B*B)<1.){
      histTheta->Fill(theta);
      if(A>=0.&&B>=0.)
        phi = TMath::ATan(B/A);
        else if(A<0.&&B>=0.)
          phi = TMath::ATan(B/A) + TMath::Pi();
          else if(A<0.&&B<0.)
            phi = TMath::ATan(B/A) + TMath::Pi();
            else
              phi = TMath::ATan(B/A) + TMath::TwoPi();
      histPhi->Fill(phi);
      T = t2_real[count] - t3_real[count] - t5_real[count] + t4_real[count];
      histT->Fill(T);
    }
      else
       theta = 0.;
    count++;
  }

  //FIT DI THETA CON LA FUNZIONE DATA
  double air_pressure[4] = {988.8, 985.6, 1014.9, 1017.6}; //hPa = mbar
  double air_pressure_avg = 0.;
  count = 0;
  while(count<4){
    air_pressure_avg += air_pressure[count];
    count++;
  }
  air_pressure_avg /= 4;    //calcolo media pressione
  air_pressure_avg *= 1.02; //converto in g/cm2, viene 1021.76
  TF1 *thetaFit = new TF1("N(theta)", "[0]*TMath::Sin(x)*TMath::Cos(x)*TMath::Exp(-[1]/TMath::Cos(x))", 0., TMath::PiOver2());
//  thetaFit->SetParameter(1,air_pressure_avg);
//  thetaFit->SetParLimits(2,120.,180.);

  //FINE FIT DI THETA

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  TCanvas *c3 = new TCanvas();

  c1->cd();
  histTheta->Fit(thetaFit, "", "", 0., 0.8);

  cout << "Lambda = " << air_pressure_avg/thetaFit->GetParameter(1) << endl;
  cout << "X0 = " << air_pressure_avg << endl;
  cout << "ChiQuadro = " << thetaFit->GetChisquare() << "; \tNdof = " << thetaFit->GetNDF() << "; \tChiQuadro Ridotto = " << thetaFit->GetChisquare()/thetaFit->GetNDF() << "; \t P = " << thetaFit->GetProb() << endl;

  histTheta->Draw();
  c2->cd();
  histPhi->SetMinimum(0.);
  histPhi->Fit("pol0");
  histPhi->Draw();

  c3->cd();
  histT->Fit("gaus");
  histT->Draw();

//  cout << "Spessore = (" << CONST_C*histT->GetStdDev()/2. << " +- " << CONST_C*histT->GetStdDevError()/2. << ") m" << endl;

}