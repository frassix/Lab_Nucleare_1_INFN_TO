#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
#include <string>
using namespace std;


//mettere nel primo argomento un file.dat relativo alla singola del primo, mentre nel secondo argomento mettere il file da cui voglio prendere il piedestallo del rivelatore 1. Gli ultimi due argomenti sono i rispettivi numeri di misure. ATTENZIONE: il programma così scritto è basato sul primo rivelatore, perché pesca sempre la terza colonna del file.dat...per prendere un altro rivelatore, basta spostare gli array riv1_1 e riv1_2, nel while, sulla colonna corrispondente al rivelatore desiderato

void spettro_ADC(const string &myfile_1, const string &myfile_2, const int nmisure1, const int nmisure2)
{

 int riv1_1[nmisure1],riv1_2[nmisure2];
    
 ifstream infile_1;
 infile_1.open(myfile_1.c_str());
    
  int bins[1024];
  int i = 0;
    
  while(!infile_1.eof())
  {
    int nmis;
    infile_1>>nmis>>nmis>>riv1_1[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis; //QUA E' SULLA TERZA COLONNA PERCHE' STO GUARDANDO IL PRIMO RIVELATORE IN SINGOLA
    i++;
  }
  infile_1.close();
    
    
  ifstream infile_2;
  infile_2.open(myfile_2.c_str());
  i=0;
    
  while(!infile_2.eof())
  {
    int nmis;
    infile_2>>nmis>>nmis>>riv1_2[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis; //QUA E' SULLA TERZA COLONNA PERCHE' STO GUARDANDO IL PIEDESTALLO DEL PRIMO RIVELATORE, TRIGGERANDO SU UN ALTRO RIVELATORE
    i++;
  }
    infile_2.close();
    
  TH1I* histo0_1 = new TH1I("Poly_1","Cerca minimo",256,0,1024);
  TH1I* histo1_1 = new TH1I("ADC_sig_bkg","ADC_sig_bkg",256,0,1024);
  TH1I* histo1_2 = new TH1I("ADC_bkg","ADC_bkg",15,0,60);
 
  for(int j = 0; j<nmisure1; j++)
  {
    histo0_1->Fill(riv1_1[j]);
    histo1_1->Fill(riv1_1[j]);
  }
                                                     
  for(int j = 0; j<nmisure2; j++)
 {
    histo1_2->Fill(riv1_2[j]);
 }
    

  TCanvas* t1 = new TCanvas();
  TF1* fit_poly_1 = new TF1("Polinomio di Grado 4","[0]*x*x*x*x+[1]*x*x*x+[2]*x*x+[3]*x+[4]",0.,1024.);
  histo0_1->Fit(fit_poly_1,"","",60.,110.);
  cout<<"PROB POLI 1 ="<<fit_poly_1->GetProb()<<endl;
  cout<<"X2 POLI 1 ="<<fit_poly_1->GetChisquare()<<endl;
    
  int min1 = int(fit_poly_1->GetMinimumX(70,110));
  cout<< "MINIMO 1 = "<< min1 <<endl;
    
  int norm1=0;
  for(int i=0; i<1024; i++)
  {
    bins[i] = int(histo1_1->GetBinContent(i));
    if(i>=min1)
    {
      norm1+=bins[i];
    }
  }
    cout<<"La normalizzazione 1 è: "<<norm1<<endl;
    
    
  TCanvas* t2 = new TCanvas();
  t2->Divide(2,1);
  t2->cd(1);
  TF1* fit1 = new TF1("Landau_1","[2]*TMath::Landau(x,[0],[1],1)",0,1024);
  TF1* Gauss1_1 = new TF1("Gauss1_1","gaus",0,1024);
  Gauss1_1->SetLineColor(kBlue);
  fit1->SetParameter(2,norm1);
    
  histo1_1->Fit(fit1,"","",min1-5,700);
  histo1_1->Fit(Gauss1_1,"","",0,min1);
  cout<<"PROB LANDAU 1 ="<<fit1->GetProb()<<endl;
  cout<<"X2 LANDAU 1 ="<<fit1->GetChisquare()<<endl;
    
  histo1_1->Draw();
  fit1->Draw("same");
  Gauss1_1->Draw("same");
    
  t2->cd(2);
    
  //Fit Gauss per rappresentare la parte di piedestallo
  TF1* Gauss1_2 = new TF1("Gauss_Pied","gaus",0,60);
  histo1_2->Fit(Gauss1_2,"","",0,60);
  //histo1_2->Draw();
  //Gauss1_2->Draw("same");

}
