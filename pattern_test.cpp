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

/*Librerie locali*/
//#include "ConfigFile.h"
//#include "vme.h"
//#include "vme_function.h"

// #define CBDBASE ((unsigned long) 0x00800000) DA CANCELLARE
#define VME_CAMAC_BASE_ADDRESS 0x800000
#define BASE_ADDRESS 0x007AE100

using namespace std;

int main (int argc, char *argv[]) {
	uint32_t address_offset_multiplicity = 0x000024;
	uint32_t address_offset_pattern = 0x000022;
	uint16_t data_fraction_multiplicity = 0;
	uint16_t data_fraction_pattern = 0;
	CAENVME_ReadCycle(Handle, BASE_ADDRESS | address_offset_multiplicity, &data_fraction_multiplicity, cvA24_U_DATA, cvD16);
	CAENVME_ReadCycle(Handle, BASE_ADDRESS | address_offset_pattern, &data_fraction_pattern, cvA24_U_DATA, cvD16);

	printf("value : %X [%x]\n",data_fraction_multiplicity,data_fraction_multiplicity);
	printf("value : %X [%x]\n",data_fraction_pattern,data_fraction_pattern);

	of_pattern<<"  "<<data_fraction_multiplicity<<"   "<<data_fraction_pattern<<endl;

	uint32_t mask6 = 0x0001;
	uint32_t mask7 = 0x0002;
	uint32_t mask8 = 0x0004;



	uint32_t riv1_pat = data_fraction_pattern & mask6;
	uint32_t riv2_pat = data_fraction_pattern & mask7;
	uint32_t riv3_pat = data_fraction_pattern & mask8;

	uint32_t riv1_mult = data_fraction_pattern & mask6;
	uint32_t riv2_mult = data_fraction_pattern & mask7;
	uint32_t riv3_mult = data_fraction_pattern & mask8;
}