/*Librerie di C++*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <CAENVMElib.h>

//#include "pcivme.h"
//#include "pcivme_ni.h"


/*Librerie di ROOT*/
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TApplication.h>

/*Librerie locali*/
#include "ConfigFile.h"
//#include "vme.h"
#include "vme_function.h"

// #define CBDBASE ((unsigned long) 0x00800000) DA CANCELLARE
#define VME_CAMAC_BASE_ADDRESS 0x800000
#define BASE_ADDRESS 0x007AE100

using namespace std;

int main (int argc, char *argv[]) {

	string outfileTDC1,outfileTDC2;
	string outfileADC1,outfileADC2;
	string outfilePattern;
	int nev; // numero di conteggi
	int n_LAM_test; // numero di volte che testa il LAM
	int daq_typ;

	cout << " n_LAM_test = " << n_LAM_test << endl;	
	

	cout<<"Getting input from: read_all.inp"<<endl;
	ConfigFile config( "read_all.inp" ); // crea un file di configurazione
	config.readInto( outfileTDC1, "outfileTDC1" );
	config.readInto( outfileTDC2, "outfileTDC2" );
	config.readInto( outfileADC1, "outfileADC1" );
	config.readInto( outfileADC2, "outfileADC2" );
	config.readInto( outfilePattern, "outfilePattern" );
	config.readInto( nev, "nev" ); // legge il numero di conteggi da fare
	config.readInto ( n_LAM_test, "n_LAM_test" ); // legge il numero di volte che deve testare il LAM
	config.readInto ( daq_typ, "daq_typ" ); // legge il tipo di acqusizione d fare: 0 -> entrambi ADC+TDC, 1->ADC, 2->TDC

	cout << " n_LAM_test = " << n_LAM_test << endl;

  /*
   *  OPEN USB CONNECTION TO V1718
   */

  // returned value for CAENVMElib's functions
  CVErrorCodes myError = CVErrorCodes::cvSuccess;

  // stuff needed for initializing the V1718 bridge
  CVBoardTypes BdType = cvV1718;
  short Link = 0;
  short BdNum = 0;
  int32_t Handle;
  int errStatus = 0;                 // variabile dentro cui salvo lo stato di uscita delle funzioni pcivme


  // try to connect to the V1718
  myError = CAENVME_Init(BdType, Link, BdNum, &Handle);

  // if doesn't connect, return error and quit
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
      return(myError);
  }

	/* Definisce l'indirizzo del CAMAC  */
	uint8_t c=1; // Definisce il numero del Crate
	uint8_t b=0; // Definisce il numero del Branch
	uint8_t n=13; // Definisce il numero dello slot del TDC
	uint8_t n2=17; // Definisce il numero dello slot del TDC2
	uint8_t m=11; // Definisce il numero dello slot dell'ADC
	uint8_t X = 0;

	uint8_t qTDC=0;
	uint8_t qTDC2=0;
	uint8_t qADC=0; // Dichiara le variabili per il LAM del TDC e dell'ADC
	int cont;

	/*Definizione delle variabili dati*/
	uint32_t dataTDC1_ch1, dataTDC1_ch2, dataTDC1_ch3, dataTDC1_ch4, dataTDC1_ch5, dataTDC1_ch6, dataTDC1_ch7, dataTDC1_ch8;
	uint32_t dataTDC2_ch1, dataTDC2_ch2, dataTDC2_ch3, dataTDC2_ch4, dataTDC2_ch5, dataTDC2_ch6, dataTDC2_ch7, dataTDC2_ch8;
	uint32_t dataADC_ch0, dataADC_ch1, dataADC_ch2, dataADC_ch3, dataADC_ch4, dataADC_ch5, dataADC_ch6, dataADC_ch7, dataADC_ch8, dataADC_ch9, dataADC_ch10, dataADC_ch11;

	uint32_t dataBuffer = 0;
	time_t seconds;seconds = time (NULL);

	int j=1; // Descrive il numero dell'evento   


	/* Crea i files di ROOT */
	TFile *foutTDC = new TFile(outfileTDC1.c_str(),"RECREATE"); // Crea un file di ROOT per il TDC
	TFile *foutADC = new TFile(outfileADC1.c_str(),"RECREATE"); // Crea un file di ROOT per l'ADC

	/* Crea gli istogrammi di ROOT */  
	TH1F *TDC1 = new TH1F("tdc1","spettro TDC scintillatore 1",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 1
	TH1F *TDC2 = new TH1F("tdc2","spettro TDC scintillatore 2",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 2
	TH1F *TDC3 = new TH1F("tdc3","spettro TDC scintillatore 3",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 3
	TH1F *TDC4 = new TH1F("tdc4","spettro TDC scintillatore 4",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 4
	TH1F *TDC5 = new TH1F("tdc5","spettro TDC scintillatore 5",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 5  
	TH1F *TDC6 = new TH1F("tdc6","spettro TDC scintillatore 6",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 6
	TH1F *TDC7 = new TH1F("tdc7","spettro TDC scintillatore 7",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 7
	TH1F *TDC8 = new TH1F("tdc8","spettro TDC scintillatore 8",4000,-0.5,3999.5); // Crea un istogramma di ROOT con lo spettro del TDC per lo scintillatore 8  

	TH1F *TDC21 = new TH1F("tdc21","spettro TDC scintillatore 21",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC22 = new TH1F("tdc22","spettro TDC scintillatore 22",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC23 = new TH1F("tdc23","spettro TDC scintillatore 23",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC24 = new TH1F("tdc24","spettro TDC scintillatore 24",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC25 = new TH1F("tdc25","spettro TDC scintillatore 25",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per    
	TH1F *TDC26 = new TH1F("tdc26","spettro TDC scintillatore 26",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC27 = new TH1F("tdc27","spettro TDC scintillatore 27",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 
	TH1F *TDC28 = new TH1F("tdc28","spettro TDC scintillatore 28",4096,-0.5,4095.5); // Crea un istogramma di ROOT con lo spettro del TDC per 

	TH1F *ADC0 = new TH1F("adc0","spettro canale 0",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 0
	TH1F *ADC1 = new TH1F("adc1","spettro canale 1",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 1
	TH1F *ADC2 = new TH1F("adc2","spettro canale 2",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 2
	TH1F *ADC3 = new TH1F("adc3","spettro canale 3",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 3
	TH1F *ADC4 = new TH1F("adc4","spettro canale 4",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 4
	TH1F *ADC5 = new TH1F("adc5","spettro canale 5",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 5
	TH1F *ADC6 = new TH1F("adc6","spettro canale 6",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 6
	TH1F *ADC7 = new TH1F("adc7","spettro canale 7",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 7
	TH1F *ADC8 = new TH1F("adc8","spettro canale 8",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 8
	TH1F *ADC9 = new TH1F("adc9","spettro canale 9",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 9
	TH1F *ADC10 = new TH1F("adc10","spettro canale 10",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 10
	TH1F *ADC11 = new TH1F("adc11","spettro canale 11",2000,-0.5,1999.5); // Crea un istogramma di ROOT con lo spettro del ADC per lo scintillatore 11

	TH1F *sum = new TH1F("sum","spettro somma",2000,-0.5,1999.5); //Crea un oggetto di ROOT che somma gli istogrammi



	/* Apre i files di input */    
	ofstream of_TDC;
	of_TDC.open(outfileTDC2.c_str()); // Apre il file ASCII di output per il TDC

	ofstream of_ADC;
	of_ADC.open(outfileADC2.c_str()); // Apre il file ASCII di output per il ADC

	ofstream of_pattern;
	of_pattern.open(outfilePattern.c_str()); // Apre il file ASCII di output per il pattern


	//printf("Q: %d\n", statusQ(Handle,b,c));
	fprintf(stderr, "Resetting CAMAC crate %d (branch %d).\n", c, b);
	X = camacZ(Handle,b,c); //Camac reset 
	printf("reset --> X: %d\n", X);
	// CCCZ(A24, CBDBASE, b, c);
	X = camacC(Handle,b,c); // Camac Clear
	printf("clear --> X: %d\n", X);


	fprintf(stderr, "Set Inhibit to crate %d (branch %d).\t", c, b);
	X = camacIenbl(Handle,b,c);
	printf("Inhibit enable --> X: %d\n", X);
	// CCCI(A24, CBDBASE, b, c, 0); forse da implementare. In caso da mettere in VME_function.h e VME_function.c


	/*
	/   TDC 1
	*/
	fprintf(stderr, "Enable LAM on TDC1 (N=%d)\t", n);
	VME_CAMAC_command(Handle,b,c,n,0,26,R16,&dataBuffer); //enable LAM TDC 1
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);


	fprintf(stderr, "Clear TDC1 (N=%d)\t", n);
	VME_CAMAC_command(Handle,b,c,n,0,9,R16,&dataBuffer); //clear LAM TDC 1
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);


	/*
	/   TDC 2 
	*/
	fprintf(stderr, "Enable LAM on TDC2 (N=%d)\t", n2);
	VME_CAMAC_command(Handle,b,c,n2,0,26,R16,&dataBuffer); //enable LAM TDC 2
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);


	fprintf(stderr, "Clear TDC2 (N=%d)\t", n2);
	VME_CAMAC_command(Handle,b,c,n2,0,9,R16,&dataBuffer); //clear LAM TDC 2
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);



	/*
	/   ADC
	*/
	fprintf(stderr, "Enable LAM on ADC (N=%d)\t", m);
	VME_CAMAC_command(Handle,b,c,m,0,26,R16,&dataBuffer); //enable LAM ADC
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);


	fprintf(stderr, "Clear ADC (N=%d)\t", m);
	VME_CAMAC_command(Handle,b,c,m,0,9,R16,&dataBuffer); //clear LAM ADC
	// check status X
	if ((dataBuffer & 0x00004000) != 0x00004000) {
	  X = 0;
	  printf("\nERROR: status X = 0 (command not accepted)\n");
	} else {
	  X = 1;
	}
	fprintf(stderr, "--> X = %d\n", X);



	fprintf(stderr, "Remove Inhibit from crate %d (branch %d).\t", c, b);
	X = camacIrem(Handle,b,c);
	printf("Inhibit removed --> X: %d\n", X);

	/* Testa il LAM */ 
	while(j<=nev) {
		qTDC=0; // imposta il LAM del TDC a 0
		qTDC2=0; // imposta il LAM del TDC2 a 0
		qADC=0; // imposta il LAM dell'ADC a 0
		cont=0;

		// cout << "Riga 240, sono nel ciclo while " << endl;


		while( (qADC!=1 && daq_typ==1) || (qTDC!=1 && daq_typ==2 )  ||  ((qTDC!=1 || qADC!=1)&&(daq_typ==0)) ||  ((qTDC!=1 || qTDC2!=1 ||qADC!=1)&&(daq_typ==3)) ) {  // richiede solo uno dei due Q

			  myError = (CVErrorCodes) VME_CAMAC_command(Handle, b, c, n, 0, 8, R16, &dataBuffer);
			//  printf( "   ---> checkLAM: dataBuffer = 0x%.8X " , dataBuffer);
			  if (myError != cvSuccess) {
			      decodeCaenMyError(&myError);
			  }
			  if ( (dataBuffer & 0x00008000) == 0x00008000 ) {
			    qTDC = 1;
			   // printf( "qTDC = test Q = True: %d", qTDC);
			  }
			  else {
			    qTDC = 0;
			   // printf( " test Q = False: %d", qTDC);

			  }
			  // printf( "Q_TDC = %d\t", qTDC);

			  myError = (CVErrorCodes) VME_CAMAC_command(Handle, b, c, n2, 0, 8, R16, &dataBuffer);
			//  printf( "   ---> checkLAM: dataBuffer = 0x%.8X " , dataBuffer);
			  if (myError != cvSuccess) {
			      decodeCaenMyError(&myError);
			  }
			  if ( (dataBuffer & 0x00008000) == 0x00008000 ) {
			    qTDC2 = 1;
			   // printf( " test Q = True: %d", qTDC2);
			  }
			  else {
			    qTDC2 = 0;
			   // printf( " test Q = False: %d", qTDC2);

			  }
			  // printf( "Q_TDC2 = %d\t", qTDC2);


			  myError = (CVErrorCodes) VME_CAMAC_command(Handle, b, c, m, 0, 8, R16, &dataBuffer);
			//  printf( "   ---> checkLAM: dataBuffer = 0x%.8X " , dataBuffer);
			  if (myError != cvSuccess) {
			      decodeCaenMyError(&myError);
			  }
			  if ( (dataBuffer & 0x00008000) == 0x00008000 ) {
			    qADC = 1;
			   // printf( " test Q = True: %d", qADC);
			  }
			  else {
			    qADC = 0;
			   // printf( " test Q = False: %d", qADC);

			  }
			  // printf( "Q_ADC = %d\tcont = %d\n", qADC, cont);


			if(cont==n_LAM_test){
    	         cout<<"Q not found ("<<cont<<" times checked)! Event "<<j<<endl;
	            break;
            }

            cont++;
		 	
		}
//		fprintf(stderr, "\n---> terminato test LAM <---\n");


	            
		if(cont==n_LAM_test) {
			VME_CAMAC_command(Handle,b,c,n,0,9,R16,&dataBuffer); //clear and disable LAM TDC 1 
			VME_CAMAC_command(Handle,b,c,n,0,10,R16,&dataBuffer); //clear LAM ADC 
			VME_CAMAC_command(Handle,b,c,n,0,26,R16,&dataBuffer); //enable LAM ADC

			VME_CAMAC_command(Handle,b,c,n2,0,9,R16,&dataBuffer); //clear and disable LAM TDC2 
			VME_CAMAC_command(Handle,b,c,n2,0,10,R16,&dataBuffer); //clear LAM ADC 
			VME_CAMAC_command(Handle,b,c,n2,0,26,R16,&dataBuffer); //enable LAM ADC

			VME_CAMAC_command(Handle,b,c,m,0,9,R16,&dataBuffer); //clear and disable LAM ADC 
			VME_CAMAC_command(Handle,b,c,m,0,10,R16,&dataBuffer); //clear LAM ADC 
			VME_CAMAC_command(Handle,b,c,m,0,26,R16,&dataBuffer); //enable LAM ADC
			continue;
		}
      	// if(qADC==1 && qTDC==1) //legge (e scrive) i dati solo se e' stato trovato il LAM in entrambi
    if( (qADC==1 && daq_typ==1) ||(qTDC==1 && daq_typ==2) || (qADC==1 && qTDC==1 && daq_typ==0) || (qADC==1 && qTDC==1 && qTDC2==1 && daq_typ==3)  ) {  // legge (e scrive) i dati solo se e' stato trovato il LAM di uno
       
	 		j++;	
	 		
			VME_CAMAC_command(Handle,b,c,n,0,0,R24,&dataTDC1_ch1); 	//read data from channel 0 TDC 1
			VME_CAMAC_command(Handle,b,c,n,1,0,R24,&dataTDC1_ch2); 	//read data from channel 1 TDC 1
			VME_CAMAC_command(Handle,b,c,n,2,0,R24,&dataTDC1_ch3); 	//read data from channel 2 TDC 1
			VME_CAMAC_command(Handle,b,c,n,3,0,R24,&dataTDC1_ch4); 	//read data from channel 3 TDC 1
			VME_CAMAC_command(Handle,b,c,n,4,0,R24,&dataTDC1_ch5); 	//read data from channel 4 TDC 1
			VME_CAMAC_command(Handle,b,c,n,5,0,R24,&dataTDC1_ch6); 	//read data from channel 5 TDC 1
			VME_CAMAC_command(Handle,b,c,n,6,0,R24,&dataTDC1_ch7); 	//read data from channel 6 TDC 1
			VME_CAMAC_command(Handle,b,c,n,7,0,R24,&dataTDC1_ch8); 	//read data from channel 7 TDC 1

			VME_CAMAC_command(Handle,b,c,n2,0,0,R24,&dataTDC2_ch1);	//read data from channel 0 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,1,0,R24,&dataTDC2_ch2);	//read data from channel 1 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,2,0,R24,&dataTDC2_ch3);	//read data from channel 2 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,3,0,R24,&dataTDC2_ch4);	//read data from channel 3 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,4,0,R24,&dataTDC2_ch5);	//read data from channel 4 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,5,0,R24,&dataTDC2_ch6);	//read data from channel 5 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,6,0,R24,&dataTDC2_ch7);	//read data from channel 6 TDC 2
			VME_CAMAC_command(Handle,b,c,n2,7,0,R24,&dataTDC2_ch8);	//read data from channel 7 TDC 2

			VME_CAMAC_command(Handle,b,c,m,0,2,R24,&dataADC_ch0); 	//read data from channel 0 ADC
			VME_CAMAC_command(Handle,b,c,m,1,2,R24,&dataADC_ch1); 	//read data from channel 1 ADC
			VME_CAMAC_command(Handle,b,c,m,2,2,R24,&dataADC_ch2); 	//read data from channel 2 ADC
			VME_CAMAC_command(Handle,b,c,m,3,2,R24,&dataADC_ch3); 	//read data from channel 3 ADC
			VME_CAMAC_command(Handle,b,c,m,4,2,R24,&dataADC_ch4); 	//read data from channel 4 ADC
			VME_CAMAC_command(Handle,b,c,m,5,2,R24,&dataADC_ch5); 	//read data from channel 5 ADC
			VME_CAMAC_command(Handle,b,c,m,6,2,R24,&dataADC_ch6); 	//read data from channel 6 ADC
			VME_CAMAC_command(Handle,b,c,m,7,2,R24,&dataADC_ch7); 	//read data from channel 7 ADC
			VME_CAMAC_command(Handle,b,c,m,8,2,R24,&dataADC_ch8); 	//read data from channel 8 ADC
			VME_CAMAC_command(Handle,b,c,m,9,2,R24,&dataADC_ch9); 	//read data from channel 9 ADC
			VME_CAMAC_command(Handle,b,c,m,10,2,R24,&dataADC_ch10);	//read data from channel 10 ADC
			VME_CAMAC_command(Handle,b,c,m,11,2,R24,&dataADC_ch11);	//read data from channel 11 ADC


      seconds = time (NULL); //get the time	
                
			TDC1->Fill(dataTDC1_ch1);
			TDC2->Fill(dataTDC1_ch2);
			TDC3->Fill(dataTDC1_ch3);
			TDC4->Fill(dataTDC1_ch4);
			TDC5->Fill(dataTDC1_ch5);
			TDC6->Fill(dataTDC1_ch6);
			TDC7->Fill(dataTDC1_ch7);
			TDC8->Fill(dataTDC1_ch8);
			   
			TDC21->Fill(dataTDC1_ch1);
			TDC22->Fill(dataTDC1_ch2);
			TDC23->Fill(dataTDC1_ch3);
			TDC24->Fill(dataTDC1_ch4);
			TDC25->Fill(dataTDC1_ch5);
			TDC26->Fill(dataTDC1_ch6);
			TDC27->Fill(dataTDC1_ch7);
			TDC28->Fill(dataTDC1_ch8);
	                       
			ADC0->Fill(dataADC_ch0);
			ADC1->Fill(dataADC_ch1);
			ADC2->Fill(dataADC_ch2);
			ADC3->Fill(dataADC_ch3);
			ADC4->Fill(dataADC_ch4);
			ADC5->Fill(dataADC_ch5);
			ADC6->Fill(dataADC_ch6);
			ADC7->Fill(dataADC_ch7);
			ADC8->Fill(dataADC_ch8);
			ADC9->Fill(dataADC_ch9);
			ADC10->Fill(dataADC_ch10);
			ADC11->Fill(dataADC_ch11);

			sum->Add(ADC1);
			sum->Add(ADC2);
			sum->Add(ADC3);
			sum->Add(ADC4);
			sum->Add(ADC5);

			uint32_t address_offset_multiplicity = 0x000024;
			uint32_t address_offset_pattern = 0x000022;
			uint16_t data_fraction_multiplicity = 0;
			uint16_t data_fraction_pattern = 0;
			CAENVME_ReadCycle(Handle, BASE_ADDRESS | address_offset_multiplicity, &data_fraction_multiplicity, cvA24_U_DATA, cvD16);
			CAENVME_ReadCycle(Handle, BASE_ADDRESS | address_offset_pattern, &data_fraction_pattern, cvA24_U_DATA, cvD16);

			uint32_t mask6 = 0x0001;
			uint32_t mask7 = 0x0002;
			uint32_t mask8 = 0x0004;
		
			uint32_t riv1_pat = 0;
			uint32_t riv1_mult = 0;
			if((data_fraction_pattern & mask6)==0x0001){
				riv1_pat = 1;
				if((data_fraction_multiplicity & mask6)==0x0001){
					riv1_mult = 1;
				}
			}
			uint32_t riv2_pat = 0;
			uint32_t riv2_mult = 0;
			if((data_fraction_pattern & mask7)==0x0002){
				riv2_pat = 1;
				if((data_fraction_multiplicity & mask7)==0x0002){
					riv2_mult = 1;
				}
			}
			uint32_t riv3_pat = 0;
			uint32_t riv3_mult = 0;
			if((data_fraction_pattern & mask8)==0x0004){
				riv3_pat = 1;
				if((data_fraction_multiplicity & mask8)==0x0004){
					riv3_mult = 1;
				}
			}
			
			of_pattern<<"Pattern "<<riv1_pat<<"  "<<riv2_pat<<"  "<<riv3_pat<<"; Multiplicity "<<  riv1_mult<<"  "<<riv2_mult<<"  "<<riv3_mult   <<endl;

			of_TDC<<j-1<<" " << seconds << " "<<dataTDC1_ch1<<" "<<dataTDC1_ch2<<" "<<dataTDC1_ch3<<" "<<dataTDC1_ch4<<" "<<dataTDC1_ch5<<" "<<dataTDC1_ch6<<" "<<dataTDC1_ch7<<" "<<dataTDC1_ch8<<" "<<dataTDC2_ch1<<" "<<dataTDC2_ch2<<" "<<dataTDC2_ch3<<" "<<dataTDC2_ch4<<" "<<dataTDC2_ch5<<" "<<dataTDC2_ch6<<" "<<dataTDC2_ch7<<" "<<dataTDC2_ch8 <<endl;    

			cout<<j-1<<" " << seconds << " "<<dataTDC1_ch1<<" "<<dataTDC1_ch2<<" "<<dataTDC1_ch3<<" "<<dataTDC1_ch4<<" "<<dataTDC1_ch5<<" "<<dataTDC1_ch6<<" "<<dataTDC1_ch7<<" "<<dataTDC1_ch8<<" "<<dataTDC2_ch1<<" "<<dataTDC2_ch2<<" "<<dataTDC2_ch3<<" "<<dataTDC2_ch4<<" "<<dataTDC2_ch5<<" "<<dataTDC2_ch6<<" "<<dataTDC2_ch7<<" "<<dataTDC2_ch8 <<endl;    

			of_ADC<<j-1<<" "<<dataADC_ch0<<" "<<dataADC_ch1<<" "<<dataADC_ch2<<" "<<dataADC_ch3<<" "<<dataADC_ch4<<" "<<dataADC_ch5<<" "<<dataADC_ch6<<" "<<dataADC_ch7<<" "<<dataADC_ch8<<" "<<dataADC_ch9<<" "<<dataADC_ch10<<" "<<dataADC_ch11<<endl;

			cout<<j-1<<" "<<dataADC_ch0<<" "<<dataADC_ch1<<" "<<dataADC_ch2<<" "<<dataADC_ch3<<" "<<dataADC_ch4<<" "<<dataADC_ch5<<" "<<dataADC_ch6<<" "<<dataADC_ch7<<" "<<dataADC_ch8<<" "<<dataADC_ch9<<" "<<dataADC_ch10<<" "<<dataADC_ch11<<endl;


			VME_CAMAC_command(Handle,b,c,n,0,9,R16,&dataBuffer); //clear and disable LAM TDC 1 
			VME_CAMAC_command(Handle,b,c,n,0,10,R16,&dataBuffer); //clear LAM ADC 
			VME_CAMAC_command(Handle,b,c,n,0,26,R16,&dataBuffer); //enable LAM ADC

			VME_CAMAC_command(Handle,b,c,n2,0,9,R16,&dataBuffer); //clear and disable LAM TDC2 
			VME_CAMAC_command(Handle,b,c,n2,0,10,R16,&dataBuffer); //clear LAM ADC 
			VME_CAMAC_command(Handle,b,c,n2,0,26,R16,&dataBuffer); //enable LAM ADC

			VME_CAMAC_command(Handle,b,c,m,0,9,R16,&dataBuffer); //clear and disable LAM ADC 
			VME_CAMAC_command(Handle,b,c,m,0,10,R16,&dataBuffer); //clear LAM ADC 
			 // VME_CAMAC_command(Handle,b,c,m,0,26,R16,&dataBuffer); //enable LAM ADC 	//Questa era commentata per oscuri motivi anche nella vecchia versione del codice....


		} 

	} // close "while(j<nev)"


	/* Imposta i colori per gli istogrammi */
	TDC1->SetLineColor(8); // colora lo spettro di verde
	TDC2->SetLineColor(9); // colora lo spettro di blu
	TDC3->SetLineColor(6); // colora lo spettro di fucsia
	TDC4->SetLineColor(50); // colora lo spettro di rosso
	TDC5->SetLineColor(41); // colora lo spettro di marroncino

	ADC1->SetLineColor(8); // colora lo spettro di verde
	ADC2->SetLineColor(9); // colora lo spettro di blu
	ADC3->SetLineColor(6); // colora lo spettro di fucsia
	ADC4->SetLineColor(50); // colora lo spettro di rosso
	ADC5->SetLineColor(41); // colora lo spettro di marroncino
	    
	/* Disegna gli istogrammi degli spettri per il TDC */
	foutTDC->cd(); // seleziona il file "foutTDC" come quello su cui scrivere

	TCanvas *plotTDC = new TCanvas("tdc","Picchi TDC",0); // Crea un Canvas di ROOT

	TDC1->Draw(); // disegna lo spettro del canale 1
	TDC2->Draw("same"); // sovrappone lo spettro del canale 2 a quello precedente
	TDC3->Draw("same"); // sovrappone lo spettro del canale 3 a quelli precedenti
	TDC4->Draw("same"); // sovrappone lo spettro del canale 4 a quelli precedenti
	TDC5->Draw("same"); // sovrappone lo spettro del canale 5 a quelli precedenti
	   
	plotTDC->Write();  // Scrive il Canvas "plotTDC"

	TDC1->Write(); // scrive l'istogramma dello spettro dello scintillatore 1
	TDC2->Write(); // scrive l'istogramma dello spettro dello scintillatore 2
	TDC3->Write(); // scrive l'istogramma dello spettro dello scintillatore 3
	TDC4->Write(); // scrive l'istogramma dello spettro dello scintillatore 4
	TDC5->Write(); // scrive l'istogramma dello spettro dello scintillatore 5
	TDC6->Write(); // scrive l'istogramma dello spettro dello scintillatore 6
	TDC7->Write(); // scrive l'istogramma dello spettro dello scintillatore 7
	TDC8->Write(); // scrive l'istogramma dello spettro dello scintillatore 8
	TDC21->Write(); // scrive l'istogramma dello spettro dello scintillatore 21
	TDC22->Write(); // scrive l'istogramma dello spettro dello scintillatore 22
	TDC23->Write(); // scrive l'istogramma dello spettro dello scintillatore 23
	TDC24->Write(); // scrive l'istogramma dello spettro dello scintillatore 24
	TDC25->Write(); // scrive l'istogramma dello spettro dello scintillatore 25
	TDC26->Write(); // scrive l'istogramma dello spettro dello scintillatore 26
	TDC27->Write(); // scrive l'istogramma dello spettro dello scintillatore 27
	TDC28->Write(); // scrive l'istogramma dello spettro dello scintillatore 28
	    
	foutTDC->Close(); // Chiude il file di output del TDC
	plotTDC->Close(); // Chiude il Canvas "plotTDC"
	of_TDC.close(); // Chiude il file ASCII di output per il TDC
	of_pattern.close();
	/* Disegna gli istogrammi degli spettri per l'ADC */
	foutADC->cd(); // seleziona il file "foutADC" come quello su cui scrivere

	TCanvas *plotADC = new TCanvas("adc","Picchi ADC",0); // Crea un Canvas di ROOT

	ADC1->Draw(); // disegna lo spettro del canale 1
	ADC2->Draw("same"); // sovrappone lo spettro del canale 2 a quello precedente
	ADC3->Draw("same"); // sovrappone lo spettro del canale 3 a quelli precedenti
	ADC4->Draw("same"); // sovrappone lo spettro del canale 4 a quelli precedenti
	ADC5->Draw("same"); // sovrappone lo spettro del canale 5 a quelli precedenti

	sum->Draw(); // disegna il grafico della somma degli spettri

	plotADC->Write(); // Scrive il Canvas "plotADC"

	ADC0->Write(); // scrive l'istogramma dello spettro dello scintillatore 0
	ADC1->Write(); // scrive l'istogramma dello spettro dello scintillatore 1
	ADC2->Write(); // scrive l'istogramma dello spettro dello scintillatore 2
	ADC3->Write(); // scrive l'istogramma dello spettro dello scintillatore 3
	ADC4->Write(); // scrive l'istogramma dello spettro dello scintillatore 4
	ADC5->Write(); // scrive l'istogramma dello spettro dello scintillatore 5
	ADC6->Write(); // scrive l'istogramma dello spettro dello scintillatore 6
	ADC7->Write(); // scrive l'istogramma dello spettro dello scintillatore 7
	ADC8->Write(); // scrive l'istogramma dello spettro dello scintillatore 8
	ADC9->Write(); // scrive l'istogramma dello spettro dello scintillatore 9
	ADC10->Write(); // scrive l'istogramma dello spettro dello scintillatore 10
	ADC11->Write(); // scrive l'istogramma dello spettro dello scintillatore 11

	sum->Write();

	foutADC->Close(); // Chiude il file di output del ADC
	plotADC->Close(); // Chiude il Canvas "plotADC"
	of_ADC.close(); // Chiude il file ASCII di output per l'ADC


	errStatus = CAENVME_End(Handle);                                          // errStatus deve essere 0
	printf("VMEclose exit status: %d\n", errStatus);

	return 0;
}
