#include <CAENVMElib.h>

/*
 * for debugging purposes: if compiled with -D _DBGMODE
 * then enables the prints
 */
#ifdef _DBGMODE
#define DBGMODE 1
#else
#define DBGMODE 0
#endif

// Base address used for addressing of CAMAC modules trough VME CAMAC branch driver [CES CBD 8210]
// corresponds to the specific positions and configurations of modules in the setup
#define VME_CAMAC_BASE_ADDRESS 0x800000


/*
 * this function decodes the error stored in myError.
 * The tye CVErrorCodes is defined in CAENVMElib.h
 */
void decodeCaenMyError(CVErrorCodes *myError);

/*
 * send Z (reset) command to crate CRx
 */
uint8_t camacZ(int32_t Handle, uint8_t BR, uint8_t CR);

/*
 * send C (clear) command to crate CRx
 */
uint8_t camacC(int32_t Handle, uint8_t BR, uint8_t CR);

/*
 * remove I (inhibit) signal from crate CRx
 */
uint8_t camacIrem(int32_t Handle, uint8_t BR, uint8_t CR);

/*
 * enable I (inhibit) signal from crate CRx
 */
uint8_t camacIenbl(int32_t Handle, uint8_t BR, uint8_t CR);

/*
 * check the status Q. If true, data is validated.
 */
uint8_t statusX(int32_t Handle, uint8_t BR, uint8_t CR);

/*
 * check the status Q. If true, data is validated.
 */
uint8_t statusQ(int32_t Handle, uint8_t BR, uint8_t CR);





// new enum type for selecting if read 16/24 or write 16/24 bits
typedef enum {  R16,          // read 16 bits
                R24,          // read 24 bits (low and high D16 word)
                W16,          // write 16 bits
                W24           // write 24 bits (low and high D16 word)
              } TD_command;


// Sends a read or write command to CAMAC address defined using N A F values
CVErrorCodes VME_CAMAC_command(int32_t handle,
                               		uint8_t BR,
                                    uint8_t CR,
                                    uint8_t N,
                                    uint8_t A,
                                    uint8_t F,
                                    TD_command command,
                                    uint32_t *data);

// Maps error codes from CAEN VME library to error codes in CAEN Common library
//CAENComm_ErrorCode VME_to_Comm_error(CVErrorCodes error_vme);


