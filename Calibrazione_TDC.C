#include <Riostream.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TF1.h>
#include <TMath.h>
#include <vector>
#include <algorithm>
using namespace std;

//NOTA: 15ns == 64.3 canali, quindi 1ns ==4.3 canali

double pearsoncoeff(double X[], double Y[], int n){
    double sum_X=0, sum_Y=0, sum_XY=0;
    double squareSum_X=0, squareSum_Y=0;

    for(int i=0; i<n; i++){
        sum_X+=X[i];
        sum_Y+=Y[i];
        sum_XY+=X[i]*Y[i];

        squareSum_X+=X[i]*X[i];
        squareSum_Y+=Y[i]*Y[i];
    }
    
    double corr= (double)(n*sum_XY-sum_X*sum_Y)/sqrt((n*squareSum_X-sum_X*sum_X)*(n*squareSum_Y-sum_Y*sum_Y));
    return corr;
}

double posterior_error(double X[], double Y[], int n, double m, double q){
    double var=0;
    double temp1=0;
    for(int i=0; i<n; i++){
        temp1 = temp1 + (Y[i]-m*X[i]-q)*(Y[i]-m*X[i]-q);
    }
    var = temp1/(n-2);
    return sqrt(var);
}

void calibrazione(double time = 0, double chn = 0){
    const int nmisure = 7;

    double delay[nmisure] = {-45., -30., -15., 0., 15., 30., 45.};
    double canale[nmisure] = {216, 278, 338, 398, 464, 532, 602};
    double scanale[nmisure] = {1, 1, 1, 1, 1, 1, 1};

    TF1* fit_line = new TF1("Line","[0]*x+[1]", -45.,45.);
    TGraphErrors* plot = new TGraphErrors(nmisure,  delay, canale, NULL, scanale);
    TFitResultPtr r = plot->Fit(fit_line,"","", -45.,45.);
    cout << "Prob: " << fit_line->GetProb() << "; X^2: " << fit_line->GetChisquare() << endl;

    double m = fit_line->GetParameter(0);
    double q = fit_line->GetParameter(1);

    double lin_corr_coef = pearsoncoeff(delay, canale, nmisure);
    std::cout << "Pearson coefficient: " << lin_corr_coef << std::endl << std::endl;

    double p_err = posterior_error(delay, canale, nmisure, m, q);
    std::cout << "Posterior error: " << p_err << std::endl << std::endl;

    double scanale_post[nmisure];
    for(int i=0; i<nmisure; i++){
        scanale_post[i]=p_err;
    }

    TCanvas* t2 = new TCanvas();
    TF1* fit_line_2 = new TF1("Line a posterior","[0]*x+[1]",-45., 45.);
    TGraphErrors* plot_2 = new TGraphErrors(nmisure,  delay, canale, NULL, scanale_post);
    gStyle->SetOptStat(0);
    plot_2->GetXaxis()->SetTitle("Delay (ns)");
    histon45->GetYaxis()->SetTitle("Canali");
    plot_2->Fit(fit_line_2,"","", -45.,45.);
    plot_2->Draw();
    fit_line_2->Draw("same");
    m = fit_line_2->GetParameter(0);
    double sm = fit_line_2->GetParError(0);
    q = fit_line_2->GetParameter(1);
    double sq = fit_line_2->GetParError(1);
    cout << endl << "Prob: " << fit_line_2->GetProb() << "; X^2/ndf: " << fit_line_2->GetChisquare()/(nmisure-2)<< endl;

    std::cout << std::endl << "------------------------------"<< std::endl;
    std::cout << "|  Errore dei cavi: " << sqrt(p_err*p_err-1)/m << "ns |"<< std::endl;
    std::cout << "|  Chn: " << time*m <<" +- "<< sqrt(sm*time*sm*time + sq*sq)<<"  |" << std::endl;
    std::cout << "|  Time: " << (chn-q)/m << "ns |" << std::endl;
    std::cout << std::endl << "------------------------------"<< std::endl;
} 