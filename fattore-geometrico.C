
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sstream>

using namespace std;


int main() {

 int ix1,iy1,ifi1,iintteta1;
 double x1,y1,fi1,teta1,intteta1;
 double rintteta1;
 double a,b,c;
 double x2,x3,y2,y3;
 double z1=175.; //cm
 double z2=100.; //cm
 double z3=0.;
 double rn;
 ofstream file;
 file.open("montecarlosim.txt");

 
	 srand (time(NULL));

	for(int n=0;n<101;n++){ 
 
        rn=n/10.;		
        int k1=0;
        int k2=0;
        int k3=0;

        while(1) {

            k1=k1+1;
            ix1=rand()%10000;		//numero casuale compreso fra 0 e 10000
            x1=ix1/10000.*80.;		//posizione x del punto compreso fra 0 cm e 80 cm (con precisione di 10 micron) 
            iy1=rand()%10000;		//numero casuale compreso fra 0 e 10000
            y1=iy1/10000.*80.;		//posizione y del punto compreso fra 0 cm e 80 cm (con precisione di 10 micron) 
            ifi1=rand()%10000;		//numero casuale compreso fra 0 e 10000
            fi1=ifi1/10000.*6.28;		//angolo phi compreso fra 0 e 2 pi
            iintteta1=rand()%10000;	//numero casuale compreso fra 0 e 10000
            intteta1=iintteta1/10000.; 	//numero casuale (su cui si calcola theta) compreso fra 0 e 1 
        //	 cout << "coordinate: "<< x1 << " " << y1 << " " << fi1 << " " <<intteta1<<endl;
         
            for (int i=0; i<157; i++){	//157 = pi/2
                teta1=i/100.;
                rintteta1= 1.-pow(cos(teta1),rn+1.);		//1-cos(theta)^(rn+1)
                if(rintteta1 > intteta1) break;				//theta minimo tale che cos(theta)^(rn+1) < intteta1 [0,1]
            }		 

            a=sin(fi1)*sin(teta1);
            b=cos(fi1)*sin(teta1);
            c=cos(teta1);

            x2=(z2-z1)/c*a+x1;
            y2=(z2-z1)/c*b+y1;
            x3=(z3-z1)/c*a+x1;
            y3=(z3-z1)/c*b+y1;
            
            if((0 < x2) && (x2 < 80.) && (0 < y2) && (y2 < 80.)) k2=k2+1; 
            if((0 < x3) && (x3 < 80.) && (0 < y3) && (y3 < 80.)) k3=k3+1; 

            if(k3 == 10000) {
                cout << rn << " " << 100.*k2/k1 << " " << 100.*k3/k1 << endl;
                file << rn << " " << 100.*k2/k1 << " " << 100.*k3/k1 << endl;
                break;	 
            }
        }

        

    }
    file.close();
    return(0);	 
} 

