#include <Riostream.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
using namespace std;

#define CONST_C 2.99792458e8  
#define CONST_NSMIURE 5300
#define CONST_TDCCONV 4.267e9

void direzioni_arrivo(){
//definizione delle posizioni, mettendo l'origine al centro del rivelatore 4, l'asse x lungo la congiungente 4-3 e l'asse y lungo 4-5
//per semplicità impongo che il quadrilatero formato dai rivelatori esterni sia un rettangolo,
//(?) prendendo come valore dei lati paralleli la media +- semidispersione (?)
  double l = 11.092, L = 11.729;   //lato lungo x, lato lungo y
  double dl = 0.065, dL = 0.053; //incertezze

//timing degli hit sui rivelatori
//arrays con i tempi di hit per i rivelatori in quintupla 
  int dump = 0;
  double t1_dump[CONST_NSMIURE], t2_dump[CONST_NSMIURE], t3_dump[CONST_NSMIURE], t4_dump[CONST_NSMIURE], t5_dump[CONST_NSMIURE]; 

  //PARTE IN CUI FACCIO I CONTI RISPETTO ALL'UNO

  double t21 = 25., t31 = 12., t41 = 10., t51 = 10.;
  int count = 0;
  bool fuori_scala[CONST_NSMIURE];
  ifstream TDCfile("TDC_QUINTUPLA_FINALE_5300.dat");
  while(!TDCfile.eof()){
    TDCfile>>dump>>dump>>t1_dump[count]>>t2_dump[count]>>t3_dump[count]>>t4_dump[count]>>t5_dump[count]>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump>>dump;
    if(t1_dump[count]>1000||t2_dump[count]>1000||t3_dump[count]>1000||t4_dump[count]>1000||t5_dump[count]>1000){
      fuori_scala[count] = 1;
//      cout << count+1 << endl;
    }
    else
      fuori_scala[count] = 0;
    count++;
  }
  TDCfile.close();

  count = 0;
  int count_fuoriscala = 0;
  while(count<CONST_NSMIURE){
    if(fuori_scala[count]==1)
      count_fuoriscala++;
    count++;
  }
  
  int real_nmis = CONST_NSMIURE - count_fuoriscala;
  cout << real_nmis << endl;
  double t1_real[real_nmis], t2_real[real_nmis], t3_real[real_nmis], t4_real[real_nmis], t5_real[real_nmis];
  int real_count = 0;
  count = 0;
  while(count<CONST_NSMIURE){
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
  TH1D *histTheta = new TH1D("Angolo zenitale", "N(Theta)", 100, 0., TMath::PiOver2()); 
  TH1D *histPhi = new TH1D("Angolo azimutale", "N(Phi)", 400, 0., TMath::TwoPi());
  count = 0;
  while(count<real_nmis){
    t24 = (double)t2_real[count] - (double)t4_real[count];
    t35 = (double)t3_real[count] - (double)t5_real[count];
    A = CONST_C/(2.*l)*((double)t24+(double)t35);
    B = CONST_C/(2.*L)*((double)t24-(double)t35);
    theta = TMath::ASin(TMath::Sqrt(A*A+B*B));
    if(TMath::Sqrt(A*A+B*B)<1.)
      histTheta->Fill(theta);
      else
      theta = 0.;
    if(A>=0.&&B>=0.)
      phi = TMath::ATan(B/A);
      else if(A<0.&&B>=0.)
        phi = TMath::ATan(B/A) + TMath::Pi();
        else if(A<0.&&B<0.)
          phi = TMath::ATan(B/A) + TMath::Pi();
          else
            phi = TMath::ATan(B/A) + TMath::TwoPi();
/*   if(theta<1.58 && theta>1.56){
      cout << count+1 << "\tA = " << A << "; \tB = " << B << "; \tSqrt(A*A+B*B) = " << TMath::Sqrt(A*A+B*B) << "; \tt24 = " << t24 << "; \tt35 = " << t35 << "; \tphi = " << phi*180./TMath::Pi() << "; \ttheta = " << theta*180./TMath::Pi() << endl;
    }*/
    histPhi->Fill(phi);

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
  air_pressure_avg *= 1.02; //converto in g/cm2
  TF1 *thetaFit = new TF1("N(theta)", "[0]*TMath::Sin(x)*TMath::Cos(x)*TMath::Exp(-[1]/([2]*TMath::Cos(x)))", 0., TMath::PiOver2());
  double norm = histTheta->Integral();
//  thetaFit->SetParameter(0,norm);
  thetaFit->SetParameter(1,air_pressure_avg);
  thetaFit->SetParLimits(2,120.,180.);
  //FINE FIT DI THETA

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();

  c1->cd();
  histTheta->Fit(thetaFit, "", "", 0.05, 0.70);

  cout << "X0 = " << air_pressure_avg << endl;
  cout << "ChiQuadro = " << thetaFit->GetChisquare() << "; \tNdof = " << thetaFit->GetNDF() << "; \tChiQuadro Ridotto = " << thetaFit->GetChisquare()/thetaFit->GetNDF() << "; \t P = " << thetaFit->GetProb() << endl;

  histTheta->Draw();
  c2->cd();
  histPhi->SetMinimum(0.);
  histPhi->Fit("pol0");
  histPhi->Draw();


/*  TCanvas *cPad = new TCanvas("cPad","Istogrammi delle DeltaT", 0, 0, 1600, 800);
  cPad->Divide(2,1);
  cPad->cd(1);
  histTheta->Fit(thetaFit);
  histTheta->Draw();
  cPad->cd(2);
  histPhi->SetMinimum(0.);
  histPhi->Fit("pol0");
  histPhi->Draw();*/

  cout << "Integrale histTheta = " << histTheta->Integral() << endl;

  //ANALISI DELLO SPESSORE
  double T = 0.;
  TH1D *histT = new TH1D("Spessore", "Spessore", 100, -60.e-9, +60.e-9);
  count = 0;
  while(count<real_nmis){
    T = t2_real[count] - t3_real[count] - t5_real[count] + t4_real[count];
    histT->Fill(T);
    count++;
  }

  TCanvas *c3 = new TCanvas();
  c3->cd();
  histT->Draw();
  histT->Fit("gaus");

  cout << "Spessore = (" << CONST_C*histT->GetStdDev()/2. << " +- " << CONST_C*histT->GetStdDevError()/2. << ") m" << endl;

  //FINE ANALISI DELLO SPESSORE

}