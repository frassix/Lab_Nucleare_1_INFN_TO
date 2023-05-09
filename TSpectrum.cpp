#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
#include <vector>
#include <algorithm>
using namespace std;

int TSpectrum(){

    //Apro i file .dat (7 in totale)
    const int nmisure = 1000;

    int riv1_n45[nmisure];
    ifstream infile1("TDC_2_-45ns_1000.dat");
    int i = 0;
    while(!infile1.eof()){
    int nmis;
    infile1>>nmis>>nmis>>nmis>>riv1_n45[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile1.close();

    int riv1_n30[nmisure];
    ifstream infile2("TDC_2_-30ns_1000.dat");
    i = 0;
    while(!infile2.eof()){
    int nmis;
    infile2>>nmis>>nmis>>nmis>>riv1_n30[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile2.close();

    int riv1_n15[nmisure];
    ifstream infile3("TDC_2_-15ns_1000.dat");
    i = 0;
    while(!infile3.eof()){
    int nmis;
    infile3>>nmis>>nmis>>nmis>>riv1_n15[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile3.close();

    int riv1_0[nmisure];
    ifstream infile4("TDC_2_0ns_1000.dat");
    i = 0;
    while(!infile4.eof()){
    int nmis;
    infile4>>nmis>>nmis>>nmis>>riv1_0[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile4.close();

    int riv1_p15[nmisure];
    ifstream infile5("TDC_2_+15ns_1000.dat");
    i = 0;
    while(!infile5.eof()){
    int nmis;
    infile5>>nmis>>nmis>>nmis>>riv1_p15[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile5.close();

    int riv1_p30[nmisure];
    ifstream infile6("TDC_2_+30ns_1000.dat");
    i = 0;
    while(!infile6.eof()){
    int nmis;
    infile6>>nmis>>nmis>>nmis>>riv1_p30[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile6.close();

    int riv1_p45[nmisure];
    ifstream infile7("TDC_2_+45ns_1000.dat");
    i = 0;
    while(!infile7.eof()){
    int nmis;
    infile7>>nmis>>nmis>>nmis>>riv1_p45[i]>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis>>nmis;
    i++;
    }
    infile7.close();


    TH1I* histon45 = new TH1I("Spettro TDC al variare del delay","Spettro TDC al variare del delay",1000,0,1000);
    TH1I* histon30 = new TH1I("-30","-30",1000,0,1000);
    TH1I* histon15 = new TH1I("-15","-15",1000,0,1000);
    TH1I* histo0 = new TH1I("0","0",1000,0,1000);
    TH1I* histop15 = new TH1I("+15","+15",1000,0,1000);
    TH1I* histop30 = new TH1I("+30","+30",1000,0,1000);
    TH1I* histop45 = new TH1I("+45","+45",1000,0,1000);
    for(int j = 0;j<nmisure;j++){
      histon45->Fill(riv1_n45[j]);
      histon30->Fill(riv1_n30[j]);
      histon15->Fill(riv1_n15[j]);
      histo0->Fill(riv1_0[j]);
      histop15->Fill(riv1_p15[j]);
      histop30->Fill(riv1_p30[j]);
      histop45->Fill(riv1_p45[j]);
    }

    TCanvas* t2 = new TCanvas();
    gStyle->SetOptStat(0);
    histon45->GetXaxis()->SetTitle("Canali di TDC");
    histon45->GetYaxis()->SetTitle("Conteggi");

    histon45->SetLineColor(kBlue);
    histon30->SetLineColor(kRed);
    histon15->SetLineColor(kOrange);
    histo0->SetLineColor(kGreen);
    histop15->SetLineColor(kBlack);
    histop30->SetLineColor(kPink);
    histop45->SetLineColor(kBlack);

    histon45->Draw("same");
    histon30->Draw("same");
    histon15->Draw("same");
    histo0->Draw("same");
    histop15->Draw("same");
    histop30->Draw("same");
    histop45->Draw("same");

    return(1);
}