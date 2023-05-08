#include <strings.h>
#include <string.h>
#include "vme_function.h"
/*
 * decodeCaenMyError
 */
void decodeCaenMyError(CVErrorCodes *pmyError) {
  char myErrorString[500];
  bzero(myErrorString, 500*sizeof(char));         // inizializzo MyErrorString
  strcpy(myErrorString, CAENVME_DecodeError(*pmyError));

  if(DBGMODE) {
    printf("--> debug: function decodeCaenMyError\n");
    printf("\nIl codice di errore e': %d\n"
           "\t==> %s <==\n\n", *pmyError, myErrorString);
    printf("\nPremi Invio per continuare\n");
    getchar();
  } else {
    printf("error %d: %s\n", *pmyError, myErrorString);
  }
}



/*
 * send signal Z (initialization) to the all camac modules in crate CR
 * CBD 8210: commands CRx N28 A8 F26
*/
uint8_t camacZ(int32_t Handle, uint8_t BR, uint8_t CR) {
  if(DBGMODE) {
    printf("sending camac reset (Z) to CR%d ...", CR);
  }

  uint8_t N = 28;
  uint8_t A = 8;
  uint8_t F = 26;

  uint8_t X = 1;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError;
  uint32_t dataBuffer = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }

  // check status X
  if ((dataBuffer & 0x00004000) != 0x00004000) {
    X = 0;
    printf("\nERROR: status X = 0 (command not accepted)\n");
  }

  if(DBGMODE) {
    printf("done!\n");
  }

  return X;
}


/*
 * send signal C (clear) to the all camac modules in crate CR
 * CBD 8210: commands CRx
*/
uint8_t camacC(int32_t Handle, uint8_t BR, uint8_t CR) {
  if(DBGMODE) {
    printf("sending camac clear (C) to CR%d ...", CR);
  }

  uint8_t N = 28;
  uint8_t A = 9;
  uint8_t F = 26;

  uint8_t X = 1;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError;
  uint32_t dataBuffer = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }

  // check status X
  if ((dataBuffer & 0x00004000) != 0x00004000) {
    X = 0;
    printf("\nERROR: status X = 0 (command not accepted)\n");
  }

  if(DBGMODE) {
    printf("done!\n");
  }

  return X;
}



/*
 * remove signal I (inhibit) to the all camac modules in crate CR
 * CBD 8210: commands CRx
*/
uint8_t camacIrem(int32_t Handle, uint8_t BR, uint8_t CR) {
  if(DBGMODE) {
    printf("removing Inhibit from CR%d ...", CR);
  }

  uint8_t N = 30;
  uint8_t A = 9;
  uint8_t F = 24;

  uint8_t X = 1;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError;
  uint32_t dataBuffer = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }

  // check status X
  if ((dataBuffer & 0x00004000) != 0x00004000) {
    X = 0;
    printf("\nERROR: status X = 0 (command not accepted)\n");
  }

  if(DBGMODE) {
    printf("done!\n");
  }

  return X;
}



/*
 * enable signal I (inhibit) to the all camac modules in crate CR
 * CBD 8210: commands CRx
*/
uint8_t camacIenbl(int32_t Handle, uint8_t BR, uint8_t CR) {
  if(DBGMODE) {
    printf("enabling Inhibit from CR%d ...", CR);
  }

  uint8_t N = 30;
  uint8_t A = 9;
  uint8_t F = 26;

  uint8_t X = 1;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError;
  uint32_t dataBuffer = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }

  // check status X
  if ((dataBuffer & 0x00004000) != 0x00004000) {
    X = 0;
    printf("\nERROR: status X = 0 (command not accepted)\n");
  }

  if(DBGMODE) {
    printf("done!\n");
  }

  return X;
}


/*
 * statusX:
 * controllare lo status Q (stato dell'ultima operazione di lettura camac)
 * sul CES 8210 controllando il bit 14 del registro CSR accessibile con i
 * seguenti parametri: CR0 N29 A0 F0
 */
uint8_t statusX(int32_t Handle, uint8_t BR, uint8_t CR) {

  uint8_t N = 29;
  uint8_t A = 0;
  uint8_t F = 0;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError = CVErrorCodes::cvSuccess;
  uint32_t dataBuffer = 0;
  uint8_t X = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }
  if ( (dataBuffer & 0x00004000) == 0x00004000 ) {
    X = 1;
  }
  else {
    X = 0;
  }

  if(DBGMODE) {
    printf("status of last camac cycle: X = %d\n", X);
  }

  return X;
}


/*
 * statusQ:
 * controllare lo status Q (stato dell'ultima operazione di lettura camac)
 * sul CES 8210 controllando il bit 15 del registro CSR accessibile con i
 * seguenti parametri: CR0 N29 A0 F0
 */
uint8_t statusQ(int32_t Handle, uint8_t BR, uint8_t CR) {

  uint8_t N = 29;
  uint8_t A = 0;
  uint8_t F = 0;

  // returned value for CAENVMElib's functions
  CVErrorCodes myError = CVErrorCodes::cvSuccess;
  uint32_t dataBuffer = 0;
  uint8_t Q = 0;

  myError = (CVErrorCodes) VME_CAMAC_command(Handle, BR, CR, N, A, F, R16, &dataBuffer);
  if (myError != cvSuccess) {
      decodeCaenMyError(&myError);
  }
  if ( (dataBuffer & 0x00008000) == 0x00008000 ) {
    Q = 1;
  }
  else {
    Q = 0;
  }

  if(DBGMODE) {
    printf("status of last camac cycle: Q = %d\n", Q);
  }

  return Q;
}



// Sends a read or write command to CAMAC address defined using N A F values
CVErrorCodes VME_CAMAC_command(int32_t handle,
                               uint8_t BR,    // branch number (0 -> 7)
                               uint8_t CR,    // crate number (0 -> 7)
                               uint8_t N,
                               uint8_t A,
                               uint8_t F,
                               TD_command command,
                               uint32_t *data) {
    // Constructing the offset address from N A F values
    uint32_t address_offset = 0;
    address_offset |= (uint32_t) BR << 19;  // Crate number in branch. 0 is Branch Driver (CBD 8210)
    address_offset |= (uint32_t) CR << 16;  // Crate number in branch. 0 is Branch Driver (CBD 8210)
    address_offset |= (uint32_t) N << 11;   // CAMAC station number: bits 15..11
    address_offset |= (uint32_t) A << 7;    // CAMAC sub-address: bits 10..7
    address_offset |= (uint32_t) F << 2;    // function code: bits 6..2

    CVErrorCodes status = cvInvalidParam;

    if (command == R16) {
        // Reading a 16bit word in one cycle using 16bit addressing: bit 1 = 1
        address_offset |= 0x00000002;
        status = CAENVME_ReadCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, data, cvA24_U_DATA, cvD16);
    } else
    if (command == R24) {
        // Reading a 16bit word from higher part of 32bit word using 24bit addressing: bit 1 = 0
        uint32_t data_fraction = 0;
        address_offset &= 0xFFFFFFFD;

        status = CAENVME_ReadCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, &data_fraction, cvA24_U_DATA, cvD16);
        *data = (data_fraction << 16);

        if (status == cvSuccess) {
            // Reading a 16bit word from lower part of 32bit word using 16bit addressing: bit 1 = 1
            address_offset |= 0x00000002;

            data_fraction = 0;
            status = CAENVME_ReadCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, &data_fraction, cvA24_U_DATA, cvD16);
            *data |= data_fraction;
        }
    } else
    if (command == W16) {
        // Writing a 16bit word in one cycle using 16bit addressing: bit 1 = 1
        address_offset |= 0x00000002;
        status = CAENVME_WriteCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, data, cvA24_U_DATA, cvD16);
    } else
    if (command == W24) {
        // Writing a 16bit word from higher part of 32bit word using 24bit addressing: bit 1 = 0
        address_offset &= 0xFFFFFFFD;
        uint32_t data_fraction = (*data >> 16);
        status = CAENVME_WriteCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, &data_fraction, cvA24_U_DATA, cvD16);
        if (status == cvSuccess) {
            // Reading a 16bit word from lower part of 32bit word using 16bit addressing: bit 1 = 1
            address_offset |= 0x00000002;
            data_fraction = *data & 0xFFFF;   // masking only the last 16 bits of the initial 32bit value
            status = CAENVME_WriteCycle(handle, VME_CAMAC_BASE_ADDRESS | address_offset, &data_fraction, cvA24_U_DATA, cvD16);
        }
    } else {
        printf("ERROR: wrong command.\nShould be one of: r16|r24|w16|w24\n");
    }

    // Decoding the status
    if (status != cvSuccess) {
        printf("ERROR: Command [%d] to address [0x%.8X] with data [0x%.6X] failed\n", command, VME_CAMAC_BASE_ADDRESS | address_offset, *data);
        printf("%s\n", CAENVME_DecodeError(status));
    }

    return status;
}
