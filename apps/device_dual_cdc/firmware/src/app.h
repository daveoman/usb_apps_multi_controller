#if 0
/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,

    /* Application waits for device configuration*/
    APP_STATE_WAIT_FOR_CONFIGURATION,

    /* The application checks if a switch was pressed */
    APP_STATE_CHECK_SWITCH_PRESSED,

    /* Wait for a character receive */
    APP_STATE_SCHEDULE_READ,

    /* A character is received from host */
    APP_STATE_WAIT_FOR_READ_COMPLETE,

    /* Wait for the TX to get completed */
    APP_STATE_SCHEDULE_WRITE,

    /* Wait for the write to complete */
    APP_STATE_WAIT_FOR_WRITE_COMPLETE,

    /* Application Error state*/
    APP_STATE_ERROR,
            
    APP_STATE_RUN

} APP_STATES;

typedef struct
{
    APP_STATES state;
    
     /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE deviceHandle;
    
    /* Device configured state */
    bool isConfigured;    
    
    /* True is switch was pressed */
    bool isSwitchPressed; 
    
     /* True if the switch press needs to be ignored*/
    bool ignoreSwitchPress;
    
     /* Flag determines SOF event occurrence */
    bool sofEventHasOccurred;
    
    /* Switch debounce timer */
    unsigned int switchDebounceTimer;
    
    unsigned int debounceCount;
    
    unsigned int readBuffersize;
} APP_USB_DEVICE_OBJECT;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{   
    /* Application's current state*/
    APP_STATES state;

    APP_USB_DEVICE_OBJECT deviceObject;
    
} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks ( void );

void _AppTaskUsbDevice(APP_USB_DEVICE_OBJECT* deviceObject);
#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

#endif

/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    usb_app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _USB_APP_H
#define _USB_APP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
//#include "txbp_device.h"
#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/* --------------------------------------------------------------------------
 * Endpoint / buffer configuration
 * -------------------------------------------------------------------------- */

#define USBAPP2_EP_BULK_OUT         0x01u
#define USBAPP2_EP_BULK_IN          0x01u    /* Note, direction set during init */

/* Maxes support full or high for fun */
//#define USBAPP2_BULK_MPS            64u      /* Full speed only (512 for high speed) */
#define USBAPP2_RX_BUF_SIZE         512u
#define USBAPP2_TX_BUF_SIZE         512u
#define USBAPP2_CTRL_BUF_SIZE       128u

/* --------------------------------------------------------------------------
 * Application state
 * -------------------------------------------------------------------------- */

typedef enum
{
    USBAPP2_STATE_STARTUP = 0,
    USBAPP2_STATE_INIT,
    USBAPP2_STATE_WAIT_FOR_POWER,
    USBAPP2_STATE_WAIT_FOR_CONFIGURATION,
    USBAPP2_STATE_READY,
    USBAPP2_STATE_SUSPENDED,
    USBAPP2_STATE_ERROR_RECOVERY
} USBAPP2_STATE;

typedef enum
{
    USBAPP2_CTRL_IDLE = 0,
    USBAPP2_CTRL_WAIT_DATA,
    USBAPP2_CTRL_WAIT_STATUS,
    USBAPP2_CTRL_ERROR
} USBAPP2_CTRL_STATE;

typedef enum
{
    USBAPP2_PROTO_OK = 0,
    USBAPP2_PROTO_ERR_BAD_LEN,
    USBAPP2_PROTO_ERR_BAD_OPCODE,
    USBAPP2_PROTO_ERR_BAD_CRC,
    USBAPP2_PROTO_ERR_BUSY,
    USBAPP2_PROTO_ERR_INTERNAL
} USBAPP2_PROTO_ERR;

/* --------------------------------------------------------------------------
 * Diagnostics
 * -------------------------------------------------------------------------- */

typedef struct
{
    uint32_t cntPowerDetected;
    uint32_t cntPowerRemoved;
    uint32_t cntReset;
    uint32_t cntConfigured;
    uint32_t cntDeconfigured;
    uint32_t cntSuspended;
    uint32_t cntResumed;
    uint32_t cntBusError;

    uint32_t cntCtrlSetup;
    uint32_t cntCtrlDataRx;
    uint32_t cntCtrlDataTx;
    uint32_t cntCtrlAbort;

    uint32_t cntRxComplete;
    uint32_t cntTxComplete;

    uint32_t cntProtoOk;
    uint32_t cntProtoErr;

    uint32_t cntRecoveries;
} USBAPP2_DIAG;

/* --------------------------------------------------------------------------
 * Latched event flags from the USB callback
 * -------------------------------------------------------------------------- */

typedef struct
{
    volatile bool powerDetected;
    volatile bool powerRemoved;
    volatile bool reset;
    volatile bool configured;
    volatile bool deconfigured;
    volatile bool suspended;
    volatile bool resumed;
    volatile bool error;

    volatile bool ctrlSetup;
    volatile bool ctrlDataReceived;
    volatile bool ctrlDataSent;
    volatile bool ctrlAborted;

    volatile bool bulkReadComplete;
    volatile bool bulkWriteComplete;
} USBAPP2_EVENTS;

/* --------------------------------------------------------------------------
 * Persistent USB status
 * -------------------------------------------------------------------------- */

typedef struct
{
    bool opened;
    bool attached;
    bool powered;
    bool configured;
    bool suspended;
} USBAPP2_STATUS;

/* --------------------------------------------------------------------------
 * Main USB app object
 * -------------------------------------------------------------------------- */

typedef struct
{
    USB_DEVICE_HANDLE devHandle;

    USBAPP2_STATE state;
    USBAPP2_CTRL_STATE ctrlState;

    USBAPP2_STATUS status;
    USBAPP2_EVENTS evt;
    USBAPP2_DIAG diag;

    USB_DEVICE_TRANSFER_HANDLE rxTransferHandle;
    USB_DEVICE_TRANSFER_HANDLE txTransferHandle;

    bool rxPending;
    bool txPending;
    bool rxArmed;
    
    uint8_t configValue;    /* Only 1 configuration, but track anyway */
    USB_SPEED speed;        /* Track even though we only support full */
    uint8_t altSetting;     /* Only 1 setting, but track anyway */
    
    /* Hardcoded, but still configurable ;-) */
    USB_ENDPOINT_ADDRESS endpointTx;
    USB_ENDPOINT_ADDRESS endpointRx;

    bool protocolPacketReady;
    bool protocolResponseReady;

    size_t rxCount;
    size_t txCount;

    /* The endpoint size is 64 for FS and 512 for HS */
    uint16_t endpointMaxPktSize;    
    uint8_t rxBuf[USBAPP2_RX_BUF_SIZE] CACHE_ALIGN;
    uint8_t txBuf[USBAPP2_TX_BUF_SIZE] CACHE_ALIGN;
    uint8_t ctrlBuf[USBAPP2_CTRL_BUF_SIZE];
    
    USB_SETUP_PACKET ctrlSetupPkt;
    size_t ctrlExpectedLength;
    size_t ctrlActualLength;

    USB_DEVICE_RESULT lastReadResult;
    USB_DEVICE_RESULT lastWriteResult;

    USBAPP2_PROTO_ERR lastProtoErr;
    
    //txbp_ctx_t txbp;
    
} USBAPP2_DATA;

extern USBAPP2_DATA g_usbApp2;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

#if 0
typedef struct
{
   /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE usbDevHandle;

    /* Application state*/
    USBAPP2_STATES state;

    /* Track device configuration */
    bool deviceIsConfigured;

    /* Configuration value */
    uint8_t configValue;

    /* speed */
    USB_SPEED speed;

    /* ep data sent */
    bool epDataWritePending;

    /* ep data received */
    bool epDataReadPending;

    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE writeTranferHandle;

    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE readTranferHandle;

    /* The transmit endpoint address */
    USB_ENDPOINT_ADDRESS endpointTx;

    /* The receive endpoint address */
    USB_ENDPOINT_ADDRESS endpointRx;

    /* Tracks the alternate setting */
    uint8_t altSetting;

    /* Flag determines SOF event occurrence */
    bool sofEventHasOccurred;

    /* The endpoint size is 64 for FS and 512 for HS */
    uint16_t endpointMaxPktSize;
    
    /* How much just came in? */
    uint16_t receivedDataCount;
    
    /* Something is ready to be read */
    bool rxChunkReady;

    /* Our USB packet buffers and stats */
    txbp_ctx_t txbp;
    
} USBAPP2_DATA;

#endif

void USBAPP2_Initialize ( void );
void USBAPP2_Tasks ( void );

#endif /* _USB_APP_H */
/*******************************************************************************
 End of File
 */

