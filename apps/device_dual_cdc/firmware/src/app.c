#if 0
/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

#ifndef USBAPP_DEBUG
#define USBAPP_DEBUG 1
#endif

#if USBAPP_DEBUG
#define USBAPP_LOG_INFO(fmt, ...) \
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "USBAPP: " fmt "\r\n", ##__VA_ARGS__)
#define USBAPP_LOG_ERR(fmt, ...) \
    SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "USBAPP: " fmt "\r\n", ##__VA_ARGS__)
#else
#define USBAPP_LOG_INFO(...) do {} while (0)
#define USBAPP_LOG_ERR(...)  do {} while (0)
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define APP_READ_BUFFER_SIZE 512
#define APP_USB_SWITCH_DEBOUNCE_COUNT_FS                    150
#define APP_USB_SWITCH_DEBOUNCE_COUNT_HS                    1200
uint8_t __attribute__((aligned(16))) switchPromptUSB[] = "\r\nPUSH BUTTON PRESSED";

uint8_t CACHE_ALIGN readBuffer[2][APP_READ_BUFFER_SIZE];


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/***********************************************
 * Application USB Device Layer Event Handler.
 ***********************************************/
void APP_USBDeviceEventHandler ( USB_DEVICE_EVENT event, void * eventData, uintptr_t context )
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;
    APP_USB_DEVICE_OBJECT* appUsbDeviceObject = (APP_USB_DEVICE_OBJECT*)context; 
       
    if (event != USB_DEVICE_EVENT_SOF) 
        USBAPP_LOG_INFO("Event %d", (int) event);
    
    switch ( event )
    {
        case USB_DEVICE_EVENT_SOF:
            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appUsbDeviceObject->sofEventHasOccurred = true; 
            break;

        case USB_DEVICE_EVENT_RESET:                 
            /* Update LED to show configured state */
            LED1_Off();
            appUsbDeviceObject->isConfigured = false; 

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            if ( configuredEventData->configurationValue == 1)
            {
                /* Mark that the device is now configured */
                appUsbDeviceObject->isConfigured = true;
                LED1_On();
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appUsbDeviceObject->deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appUsbDeviceObject->deviceHandle);
            LED1_Off();
            appUsbDeviceObject->isConfigured = false;
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            break;

        case USB_DEVICE_EVENT_RESUMED:
            break;
            
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void APP_ProcessSwitchPress(APP_USB_DEVICE_OBJECT* deviceObject)
{
    /* This function checks if the switch is pressed and then
     * debounces the switch press*/
    bool switchPressed;

    switchPressed = ( SWITCH_STATE_PRESSED == SWITCH_Get() ? true : false ) ;

    if(switchPressed)
    {
        if(deviceObject->ignoreSwitchPress)
        {
            /* This means the key press is in progress */
            if(deviceObject->sofEventHasOccurred)
            {
                /* A timer event has occurred. Update the debounce timer */
                deviceObject->switchDebounceTimer ++;
                deviceObject->sofEventHasOccurred = false;
                if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_FULL)
                {
                    deviceObject->debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_FS;
                }
                else if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_HIGH)
                {
                    deviceObject->debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_HS;
                }
                if(deviceObject->switchDebounceTimer == deviceObject->debounceCount)
                {
                    /* Indicate that we have valid switch press. The switch is
                     * pressed flag will be cleared by the application tasks
                     * routine. We should be ready for the next key press.*/
                    deviceObject->isSwitchPressed = true;
                    deviceObject->switchDebounceTimer = 0;
                    deviceObject->ignoreSwitchPress = false;
                }
            }
        }
        else
        {
            /* We have a fresh key press */
            deviceObject->ignoreSwitchPress = true;
            deviceObject->switchDebounceTimer = 0;
        }
    }
    else
    {
        /* No key press. Reset all the indicators. */
        deviceObject->ignoreSwitchPress = false;
        deviceObject->switchDebounceTimer = 0;
        deviceObject->sofEventHasOccurred = false;
    }
}

/*****************************************************
 * This function is called in every step of the
 * application state machine.
 *****************************************************/

bool APP_StateReset(APP_USB_DEVICE_OBJECT* deviceObject)
{
    /* This function returns true if the device
     * was reset  */

    bool retVal;

    if(deviceObject->isConfigured == false)
    {
        deviceObject->state = APP_STATE_WAIT_FOR_CONFIGURATION;
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return(retVal);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void __InitializeDeviceObject(void)
{
    /* Device Layer Handle  */
    appData.deviceObject.deviceHandle = USB_DEVICE_HANDLE_INVALID ;

    /* Device configured status */
    appData.deviceObject.isConfigured = false;

    /* Initialize Ignore switch flag */
    appData.deviceObject.ignoreSwitchPress = false;

    /* Reset the switch debounce counter */
    appData.deviceObject.switchDebounceTimer = 0;

    /* Reset other flags */
    appData.deviceObject.sofEventHasOccurred = false;
    appData.deviceObject.isSwitchPressed = false;
    
    USBAPP_LOG_INFO("Init object complete");
}
/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    __InitializeDeviceObject(); 
}



/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks (void )
{
    /* Update the application state machine based
     * on the current state */

    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            if(appData.deviceObject.deviceHandle == USB_DEVICE_HANDLE_INVALID)
            {
                appData.deviceObject.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );
            }
            
           if((appData.deviceObject.deviceHandle != USB_DEVICE_HANDLE_INVALID))
            {
                USB_DEVICE_EventHandlerSet(appData.deviceObject.deviceHandle, APP_USBDeviceEventHandler, (uintptr_t)&appData.deviceObject);
               
                appData.state = APP_STATE_RUN; 
                appData.deviceObject.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                USBAPP_LOG_INFO("Handles acquired moving to config wait");
                
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;
        case APP_STATE_RUN:
             _AppTaskUsbDevice(&appData.deviceObject);
            break; 
        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}

void _AppTaskUsbDevice(APP_USB_DEVICE_OBJECT* deviceObject)
{
    int i; 
    switch(deviceObject->state)
    {
        case APP_STATE_WAIT_FOR_CONFIGURATION:
            
            /* Check if the device was configured */
            if(deviceObject->isConfigured)
            {
                 if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_FULL)
                {
                    deviceObject->readBuffersize = 64 ;
                }
                else if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_HIGH)
                {
                    deviceObject->readBuffersize = 512 ;
                }
               
               deviceObject->state = APP_STATE_SCHEDULE_READ; 
            }
            
          
            break;
        case APP_STATE_SCHEDULE_READ:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            /* If a read is complete, then schedule a read
             * else wait for the current read to complete */

            deviceObject->state = APP_STATE_WAIT_FOR_READ_COMPLETE;
            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:
        case APP_STATE_CHECK_SWITCH_PRESSED:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            APP_ProcessSwitchPress(deviceObject);
            break;

        
        case APP_STATE_SCHEDULE_WRITE:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            /* Setup the write */

            deviceObject->state = APP_STATE_WAIT_FOR_WRITE_COMPLETE;

            if(deviceObject->isSwitchPressed)
            {
                /* If the switch was pressed, then send the switch prompt*/
                deviceObject->isSwitchPressed = false;
            }
            else
            {
            }

            break;

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            break;

        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}
/*******************************************************************************
 End of File
 */

#endif
/* usb_app.c Copyright 2026 Quest Controls, Inc.  All rights reserved. */

#include <stdio.h>
#include <string.h>
#include "configuration.h"
#include "system/debug/sys_debug.h"
//#include "txbp_device.h"
//#include "usb_app.h"
#include "app.h"
//#include "runtimeStuff.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#if defined (__PIC32MZ__) || defined (__PIC32MX__) || defined(__PIC32CX1025SG41128__) || defined (_SAMD21J18A_H_) || defined (_SAME54P20A_H_) ||  defined (__PIC32MM__) ||  defined (_PIC32CM5164LE00100_H_) || defined (DRV_USBHS_DEVICE_SUPPORT) || defined (DRV_USBFS_DEVICE_SUPPORT)|| defined (__PIC32CK2051GC01144__) || defined (_PIC32CM5112GC00100_H_)
#define APP_EP_BULK_OUT 1
#define APP_EP_BULK_IN 1
#else
#define APP_EP_BULK_OUT 1
#define APP_EP_BULK_IN 2
#endif 

USBAPP2_DATA g_usbApp2;

/* --------------------------------------------------------------------------
 * Debug helpers
 * -------------------------------------------------------------------------- */

#ifndef USBAPP2_DEBUG
#define USBAPP2_DEBUG 1
#endif

#if USBAPP2_DEBUG
#define USBAPP2_LOG_INFO(fmt, ...) \
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "USBAPP2: " fmt "\r\n", ##__VA_ARGS__)
#define USBAPP2_LOG_ERR(fmt, ...) \
    SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "USBAPP2: " fmt "\r\n", ##__VA_ARGS__)
#else
#define USBAPP2_LOG_INFO(...) do {} while (0)
#define USBAPP2_LOG_ERR(...)  do {} while (0)
#endif

/* --------------------------------------------------------------------------
 * Forward declarations
 * -------------------------------------------------------------------------- */

USB_DEVICE_EVENT_RESPONSE USBAPP2_DeviceEventHandler(
    USB_DEVICE_EVENT event,
    void *eventData,
    uintptr_t context
);

static void USBAPP2_ResetEvents(USBAPP2_DATA *usb);
static void USBAPP2_ResetControlState(USBAPP2_DATA *usb);
static void USBAPP2_ResetDataPath(USBAPP2_DATA *usb);
static void USBAPP2_ResetRuntimeState(USBAPP2_DATA *usb);
static void USBAPP2_ResetForBusReset(USBAPP2_DATA *usb);
static void USBAPP2_ResetForDeconfig(USBAPP2_DATA *usb);

static bool USBAPP2_Attach(USBAPP2_DATA *usb);
static bool USBAPP2_Detach(USBAPP2_DATA *usb);
static bool USBAPP2_ArmRead(USBAPP2_DATA *usb);
static bool USBAPP2_StartWrite(USBAPP2_DATA *usb, const uint8_t *data, size_t len);
static void USBAPP2_DoRecovery(USBAPP2_DATA *usb);

static void USBAPP2_HandleCtrlSetup(USBAPP2_DATA *usb);
static void USBAPP2_HandleCtrlDataReceived(USBAPP2_DATA *usb);
static void USBAPP2_HandleNewProtocolChunk(USBAPP2_DATA *usb);
static void USBAPP2_TryProtocolIncoming(USBAPP2_DATA *usb);
static void USBAPP2_TryProtocolOutgoing(USBAPP2_DATA *usb);
static void USBAPP2_BuildProtoError(USBAPP2_DATA *usb, USBAPP2_PROTO_ERR err);
static void USBAPP2_CheckSpeedAndEndpoints(USBAPP2_DATA *usb);
static void USBAPP2_InitBuffers(USBAPP2_DATA *usb);

static const char *USBAPP2_CtrlStateName(USBAPP2_CTRL_STATE state);
static const char *USBAPP2_BoolStr(bool v);
static const char *USBAPP2_ResultName(USB_DEVICE_RESULT r);
static const char *USBAPP2_SpeedName(USB_SPEED speed);
static void USBAPP2_DumpState(const USBAPP2_DATA *usb, const char *tag);

/* --------------------------------------------------------------------------
 * Init / reset helpers
 * -------------------------------------------------------------------------- */

static const char *USBAPP2_DeviceEventName(USB_DEVICE_EVENT event)
{
    switch (event)
    {
        case USB_DEVICE_EVENT_RESET:                         return "RESET";
        case USB_DEVICE_EVENT_SUSPENDED:                     return "SUSPENDED";
        case USB_DEVICE_EVENT_RESUMED:                       return "RESUMED";
        case USB_DEVICE_EVENT_ERROR:                         return "ERROR";
        case USB_DEVICE_EVENT_SOF:                           return "SOF";
        case USB_DEVICE_EVENT_POWER_DETECTED:                return "POWER_DETECTED";
        case USB_DEVICE_EVENT_POWER_REMOVED:                 return "POWER_REMOVED";
        case USB_DEVICE_EVENT_CONFIGURED:                    return "CONFIGURED";
        case USB_DEVICE_EVENT_DECONFIGURED:                  return "DECONFIGURED";
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_ABORTED:      return "CTRL_ABORTED";
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:return "CTRL_DATA_RX";
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST:return "CTRL_SETUP";
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_SENT:    return "CTRL_DATA_TX";
        case USB_DEVICE_EVENT_ENDPOINT_READ_COMPLETE:        return "EP_READ_COMPLETE";
        case USB_DEVICE_EVENT_ENDPOINT_WRITE_COMPLETE:       return "EP_WRITE_COMPLETE";
        case USB_DEVICE_EVENT_SET_DESCRIPTOR:                return "SET_DESCRIPTOR";
        case USB_DEVICE_EVENT_SYNCH_FRAME:                   return "SYNCH_FRAME";
        default:                                             return "UNKNOWN";
    }
}

static const char *USBAPP2_CtrlStateName(USBAPP2_CTRL_STATE state)
{
    switch (state)
    {
        case USBAPP2_CTRL_IDLE:        return "IDLE";
        case USBAPP2_CTRL_WAIT_DATA:   return "WAIT_DATA";
        case USBAPP2_CTRL_WAIT_STATUS: return "WAIT_STATUS";
        case USBAPP2_CTRL_ERROR:       return "ERROR";
        default:                      return "UNKNOWN";
    }
}

static const char *USBAPP2_BoolStr(bool v)
{
    return v ? "1" : "0";
}

static const char *USBAPP2_ResultName(USB_DEVICE_RESULT r)
{
    switch (r)
    {
        case USB_DEVICE_RESULT_OK:                  return "OK";
        case USB_DEVICE_RESULT_ERROR:               return "ERROR";
        case USB_DEVICE_RESULT_ERROR_PARAMETER_INVALID: return "PARAM_INVALID";
        //case USB_DEVICE_RESULT_ERROR_INSTANCE_INVALID:  return "INSTANCE_INVALID";
        case USB_DEVICE_RESULT_ERROR_TRANSFER_QUEUE_FULL:return "QUEUE_FULL";
        //case USB_DEVICE_RESULT_ERROR_TRANSFER_SIZE_INVALID:return "SIZE_INVALID";
        case USB_DEVICE_RESULT_ERROR_ENDPOINT_INVALID:return "EP_INVALID";
        case USB_DEVICE_RESULT_ERROR_ENDPOINT_NOT_CONFIGURED:return "EP_NOT_CFG";
        //case USB_DEVICE_RESULT_ERROR_DEVICE_NOT_CONFIGURED:return "DEV_NOT_CFG";
        //case USB_DEVICE_RESULT_ERROR_INTERFACE_ALTERNATE_SETTING_INVALID:return "ALT_INVALID";
        //case USB_DEVICE_RESULT_ERROR_CONTROL_TRANSFER_DATA_SIZE_INVALID:return "CTRL_SIZE_INVALID";
        default: return "OTHER";
    }
}

static const char *USBAPP2_SpeedName(USB_SPEED speed)
{
    switch (speed)
    {
        case USB_SPEED_LOW:  return "LOW";
        case USB_SPEED_FULL: return "FULL";
        case USB_SPEED_HIGH: return "HIGH";
        default:             return "UNKNOWN";
    }
}

static const char *USBAPP2_StateName(USBAPP2_STATE state)
{
    switch (state)
    {
        case USBAPP2_STATE_STARTUP:                return "STARTUP";
        case USBAPP2_STATE_INIT:                   return "INIT";        
        case USBAPP2_STATE_WAIT_FOR_POWER:         return "WAIT_FOR_POWER";
        case USBAPP2_STATE_WAIT_FOR_CONFIGURATION: return "WAIT_FOR_CONFIGURATION";
        case USBAPP2_STATE_READY:                  return "READY";
        case USBAPP2_STATE_SUSPENDED:              return "SUSPENDED";
        case USBAPP2_STATE_ERROR_RECOVERY:         return "ERROR_RECOVERY";
        default:                                  return "UNKNOWN";
    }
}

static void USBAPP2_SetState(USBAPP2_DATA *usb, USBAPP2_STATE newState)
{
    if ((usb != NULL) && (usb->state != newState))
    {
        USBAPP2_LOG_INFO("state %s -> %s",
                        USBAPP2_StateName(usb->state),
                        USBAPP2_StateName(newState));
#if 0
        USBAPP2_DumpState(usb, "before-state-change");
        usb->state = newState;
        USBAPP2_DumpState(usb, "after-state-change");
#endif
    usb->state = newState;
    }
}

static void USBAPP2_DumpState(const USBAPP2_DATA *usb, const char *tag)
{
    if (usb == NULL)
    {
        USBAPP2_LOG_ERR("dump(%s): usb=NULL", (tag != NULL) ? tag : "");
        return;
    }

    USBAPP2_LOG_INFO("==== DUMP BEGIN: %s ====", (tag != NULL) ? tag : "");

    USBAPP2_LOG_INFO("state: app=%s ctrl=%s",
                    USBAPP2_StateName(usb->state),
                    USBAPP2_CtrlStateName(usb->ctrlState));

    USBAPP2_LOG_INFO("status: opened=%s attached=%s powered=%s configured=%s suspended=%s",
                    USBAPP2_BoolStr(usb->status.opened),
                    USBAPP2_BoolStr(usb->status.attached),
                    USBAPP2_BoolStr(usb->status.powered),
                    USBAPP2_BoolStr(usb->status.configured),
                    USBAPP2_BoolStr(usb->status.suspended));

    USBAPP2_LOG_INFO("events: pDet=%s pRem=%s reset=%s cfg=%s decfg=%s susp=%s resum=%s err=%s",
                    USBAPP2_BoolStr(usb->evt.powerDetected),
                    USBAPP2_BoolStr(usb->evt.powerRemoved),
                    USBAPP2_BoolStr(usb->evt.reset),
                    USBAPP2_BoolStr(usb->evt.configured),
                    USBAPP2_BoolStr(usb->evt.deconfigured),
                    USBAPP2_BoolStr(usb->evt.suspended),
                    USBAPP2_BoolStr(usb->evt.resumed),
                    USBAPP2_BoolStr(usb->evt.error));

    USBAPP2_LOG_INFO("events2: ctrlSetup=%s ctrlRx=%s ctrlTx=%s ctrlAbort=%s bulkRxC=%s bulkTxC=%s",
                    USBAPP2_BoolStr(usb->evt.ctrlSetup),
                    USBAPP2_BoolStr(usb->evt.ctrlDataReceived),
                    USBAPP2_BoolStr(usb->evt.ctrlDataSent),
                    USBAPP2_BoolStr(usb->evt.ctrlAborted),
                    USBAPP2_BoolStr(usb->evt.bulkReadComplete),
                    USBAPP2_BoolStr(usb->evt.bulkWriteComplete));

    USBAPP2_LOG_INFO("xfer: rxPending=%s txPending=%s rxArmed=%s protoPkt=%s protoRsp=%s",
                    USBAPP2_BoolStr(usb->rxPending),
                    USBAPP2_BoolStr(usb->txPending),
                    USBAPP2_BoolStr(usb->rxArmed),
                    USBAPP2_BoolStr(usb->protocolPacketReady),
                    USBAPP2_BoolStr(usb->protocolResponseReady));

    USBAPP2_LOG_INFO("cfg: cfgValue=%u alt=%u speed=%s epRx=0x%02X epTx=0x%02X mps=%u",
                    (unsigned)usb->configValue,
                    (unsigned)usb->altSetting,
                    USBAPP2_SpeedName(usb->speed),
                    (unsigned)usb->endpointRx,
                    (unsigned)usb->endpointTx,
                    (unsigned)usb->endpointMaxPktSize);

    USBAPP2_LOG_INFO("counts: rx=%u tx=%u ctrlExp=%u ctrlAct=%u",
                    (unsigned)usb->rxCount,
                    (unsigned)usb->txCount,
                    (unsigned)usb->ctrlExpectedLength,
                    (unsigned)usb->ctrlActualLength);

    USBAPP2_LOG_INFO("results: lastRead=%s(%d) lastWrite=%s(%d) lastProtoErr=%d",
                    USBAPP2_ResultName(usb->lastReadResult),
                    (int)usb->lastReadResult,
                    USBAPP2_ResultName(usb->lastWriteResult),
                    (int)usb->lastWriteResult,
                    (int)usb->lastProtoErr);

    USBAPP2_LOG_INFO("handles: dev=0x%08lx rx=0x%08lx tx=0x%08lx",
                    (unsigned long)usb->devHandle,
                    (unsigned long)usb->rxTransferHandle,
                    (unsigned long)usb->txTransferHandle);

    USBAPP2_LOG_INFO("diag1: pDet=%lu pRem=%lu reset=%lu cfg=%lu decfg=%lu susp=%lu resum=%lu err=%lu",
                    (unsigned long)usb->diag.cntPowerDetected,
                    (unsigned long)usb->diag.cntPowerRemoved,
                    (unsigned long)usb->diag.cntReset,
                    (unsigned long)usb->diag.cntConfigured,
                    (unsigned long)usb->diag.cntDeconfigured,
                    (unsigned long)usb->diag.cntSuspended,
                    (unsigned long)usb->diag.cntResumed,
                    (unsigned long)usb->diag.cntBusError);

    USBAPP2_LOG_INFO("diag2: ctrlSetup=%lu ctrlRx=%lu ctrlTx=%lu ctrlAbort=%lu rxC=%lu txC=%lu",
                    (unsigned long)usb->diag.cntCtrlSetup,
                    (unsigned long)usb->diag.cntCtrlDataRx,
                    (unsigned long)usb->diag.cntCtrlDataTx,
                    (unsigned long)usb->diag.cntCtrlAbort,
                    (unsigned long)usb->diag.cntRxComplete,
                    (unsigned long)usb->diag.cntTxComplete);

    USBAPP2_LOG_INFO("diag3: protoOk=%lu protoErr=%lu recoveries=%lu",
                    (unsigned long)usb->diag.cntProtoOk,
                    (unsigned long)usb->diag.cntProtoErr,
                    (unsigned long)usb->diag.cntRecoveries);

    USBAPP2_LOG_INFO("setup: bmReq=0x%02X bReq=0x%02X wValue=0x%04X wIndex=0x%04X wLength=%u",
                    usb->ctrlSetupPkt.bmRequestType,
                    usb->ctrlSetupPkt.bRequest,
                    usb->ctrlSetupPkt.wValue,
                    usb->ctrlSetupPkt.wIndex,
                    (unsigned)usb->ctrlSetupPkt.wLength);

    USBAPP2_LOG_INFO("==== DUMP END: %s ====", (tag != NULL) ? tag : "");
}

void USBAPP2_Initialize(void)
{
    memset(&g_usbApp2, 0, sizeof(g_usbApp2));

    g_usbApp2.devHandle = USB_DEVICE_HANDLE_INVALID;
    g_usbApp2.state = USBAPP2_STATE_STARTUP;
    g_usbApp2.ctrlState = USBAPP2_CTRL_IDLE;
    g_usbApp2.rxTransferHandle = USB_DEVICE_TRANSFER_HANDLE_INVALID;
    g_usbApp2.txTransferHandle = USB_DEVICE_TRANSFER_HANDLE_INVALID;
    g_usbApp2.lastReadResult = USB_DEVICE_RESULT_ERROR;
    g_usbApp2.lastWriteResult = USB_DEVICE_RESULT_ERROR;
    g_usbApp2.lastProtoErr = USBAPP2_PROTO_OK;
    g_usbApp2.endpointRx = (USBAPP2_EP_BULK_OUT | USB_EP_DIRECTION_OUT);
    g_usbApp2.endpointTx = (USBAPP2_EP_BULK_IN | USB_EP_DIRECTION_IN);

    USBAPP2_LOG_INFO("initialize");
}

static void USBAPP2_InitBuffers(USBAPP2_DATA *usb) {
#if 0
    txbp_rxring_init(&usb->txbp.rx);
    txbp_txq_init(&usb->txbp.txq);
    memset(&usb->txbp.ctrs, 0, sizeof (usb->txbp.ctrs));
#endif
}

static void USBAPP2_ResetEvents(USBAPP2_DATA *usb)
{
    memset((void *)&usb->evt, 0, sizeof(usb->evt));
}

static void USBAPP2_ResetControlState(USBAPP2_DATA *usb)
{
    usb->ctrlState = USBAPP2_CTRL_IDLE;
    usb->ctrlExpectedLength = 0u;
    usb->ctrlActualLength = 0u;
    memset(&usb->ctrlSetupPkt, 0, sizeof(usb->ctrlSetupPkt));
    memset(usb->ctrlBuf, 0, sizeof(usb->ctrlBuf));
}

static void USBAPP2_ResetDataPath(USBAPP2_DATA *usb)
{
    USBAPP2_LOG_INFO("reset data path");
        
    usb->rxPending = false;
    usb->txPending = false;
    usb->rxArmed = false;

    usb->protocolPacketReady = false;
    usb->protocolResponseReady = false;

    usb->rxCount = 0u;
    usb->txCount = 0u;

    usb->rxTransferHandle = USB_DEVICE_TRANSFER_HANDLE_INVALID;
    usb->txTransferHandle = USB_DEVICE_TRANSFER_HANDLE_INVALID;

    usb->lastReadResult = USB_DEVICE_RESULT_ERROR;
    usb->lastWriteResult = USB_DEVICE_RESULT_ERROR;

    memset(usb->rxBuf, 0, sizeof(usb->rxBuf));
    memset(usb->txBuf, 0, sizeof(usb->txBuf));
}

static void USBAPP2_ResetRuntimeState(USBAPP2_DATA *usb)
{
    USBAPP2_LOG_INFO("reset runtime state");

    USBAPP2_ResetControlState(usb);
    USBAPP2_ResetDataPath(usb);

    usb->status.configured = false;
    usb->status.suspended = false;
}

static void USBAPP2_ResetForBusReset(USBAPP2_DATA *usb)
{
    USBAPP2_LOG_INFO("reset for bus reset");

    USBAPP2_ResetControlState(usb);
    USBAPP2_ResetDataPath(usb);

    usb->status.configured = false;
    usb->status.suspended = false;
}

static void USBAPP2_ResetForDeconfig(USBAPP2_DATA *usb)
{
    USBAPP2_LOG_INFO("reset for deconfigure");

    USB_DEVICE_EndpointDisable(usb->devHandle, usb->endpointRx);
    USB_DEVICE_EndpointDisable(usb->devHandle, usb->endpointTx);
        
    USBAPP2_ResetControlState(usb);
    USBAPP2_ResetDataPath(usb);

    usb->status.configured = false;
}

/* --------------------------------------------------------------------------
 * USB callback: only latch events + copy tiny event data
 * -------------------------------------------------------------------------- */

void USBAPP2_DeviceEventHandler(
    USB_DEVICE_EVENT event,
    void *eventData,
    uintptr_t context
)
{
    USBAPP2_DATA *usb = (USBAPP2_DATA *)context;

    if (event != USB_DEVICE_EVENT_SOF) {
        USBAPP2_LOG_INFO("Event %d (%s)", (int) event, USBAPP2_DeviceEventName(event));
    }
    
    if (usb == NULL)
    {
        return;
    }

    switch (event)
    {
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /* We don't use VBUS, so shouldn't ever get this event */
            
            // This means the device detected a valid VBUS voltage and is
            // attached to the USB. The application can now call
            // USB_DEVICE_Attach() function to enable D+/D- pull up
            // resistors. 
            //USB_DEVICE_Attach(usb->devHandle);            
            
            usb->evt.powerDetected = true;
            usb->diag.cntPowerDetected++;
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* We don't use VBUS, so shouldn't ever get this event */
            
            // This means the device is not attached to the USB.
            // The application should now call the USB_DEVICE_Detach()
            // function.
            //USB_DEVICE_Detach(usb->devHandle);            

            usb->evt.powerRemoved = true;
            usb->diag.cntPowerRemoved++;
            break;

        case USB_DEVICE_EVENT_RESET:
            // Reset signaling was detected on the bus. The 
            // application can find out the attach speed.
            //usbappData.deviceIsConfigured = false;
            //attachSpeed = USB_DEVICE_ActiveSpeedGet(usbappData.usbDevHandle);
            
            usb->evt.reset = true;
            usb->diag.cntReset++;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            // This means that device is configured and the application can
            // start using the device functionality. The application must
            // register function driver event handlersI have one device
            // level event.  The pData parameter will be a pointer to a
            // USB_DEVICE_EVENT_DATA_CONFIGURED data type that contains the
            // active configuration number.
            /* Grab the configuration (it should be 1)*/
            usb->configValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData)->configurationValue;
            //if(*configurationValue == 1 )
            //{
                /* The device is in configured state */

                /* Reset endpoint data send & receive flag  */
             //   usbappData.deviceIsConfigured = true;
            //}            

            usb->evt.configured = true;
            usb->diag.cntConfigured++;
            break;

        case USB_DEVICE_EVENT_DECONFIGURED:
            // This indicates that host has deconfigured the device i.e., it
            // has set the configuration as 0. All function driver instances
            // would have been deinitialized.
            //usbappData.deviceIsConfigured = false;            
            
            usb->evt.deconfigured = true;
            usb->diag.cntDeconfigured++;
            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            // The bus is idle. There was no activity detected.
            // The application can switch to a low power mode after
            // exiting the event handler.            
            usb->evt.suspended = true;
            usb->diag.cntSuspended++;
            break;

        case USB_DEVICE_EVENT_RESUMED:
            // We shouldn't see this, as we aren't configured for any low power modes */
            
            // This means that the resume signaling was detected on the
            // bus. The application can bring the device out of power
            // saving mode.            
            usb->evt.resumed = true;
            usb->diag.cntResumed++;
            break;

        case USB_DEVICE_EVENT_ERROR:
            // This means an unknown error has occurred on the bus.
            // The application can try detaching and attaching the
            // device again.            
            usb->evt.error = true;
            usb->diag.cntBusError++;
            break;

        case USB_DEVICE_EVENT_SOF:
            // A start of frame was received. This is a periodic event and
            // can be used by the application for timing related activities.
            // pData will point to a USB_DEVICE_EVENT_DATA_SOF type data
            // containing the frame number. In USB Device Stack, this
            // event is generated if USB_DEVICE_SOF_EVENT_ENABLE is
            // defined in System Configuration.

            //frameNumber = ((USB_DEVICE_EVENT_DATA_SOF *)pData)->frameNumber;
            break;
            
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST:
        {
            // This means that the setup stage of the control transfer is in
            // progress and a setup packet has been received. The pData
            // parameter will point to a USB_SETUP_PACKET data type The
            // application can process the command and update its control
            // transfer state machine. The application for example could call
            // the USB_DEVICE_ControlReceive function (as shown here) to
            // submit the buffer that would receive data in case of a
            // control read transfer.
            /* This means we have received a setup packet */
#if 0
            setupPacket = (USB_SETUP_PACKET *)eventData;
            if(setupPacket->bRequest == USB_REQUEST_SET_INTERFACE)
            {
                /* If we have got the SET_INTERFACE request, we just acknowledge
                 for now. This demo has only one alternate setting which is already
                 active. */
                USB_DEVICE_ControlStatus(usbappData.usbDevHandle,USB_DEVICE_CONTROL_STATUS_OK);
            }
            else if(setupPacket->bRequest == USB_REQUEST_GET_INTERFACE)
            {
                /* We have only one alternate setting and this setting 0. So
                 * we send this information to the host. */

                USB_DEVICE_ControlSend(usbappData.usbDevHandle, &usbappData.altSetting, 1);
            }
            else
            {
                /* We have received a request that we cannot handle. Stall it*/
                USB_DEVICE_ControlStatus(usbappData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            }
            

            //setupEventData = (USB_SETUP_PACKET *)pData;

            // Submit a buffer to receive 32 bytes in the  control write transfer.
            //USB_DEVICE_ControlReceive(usbappData.usbDevHandle, data, 32); 
#endif      
            const USB_SETUP_PACKET *setup = (const USB_SETUP_PACKET *)eventData;

            if (setup != NULL)
            {
                memcpy(&usb->ctrlSetupPkt, setup, sizeof(usb->ctrlSetupPkt));
                usb->evt.ctrlSetup = true;
            }

            usb->diag.cntCtrlSetup++;
            break;
        }

        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            // This means that data in the data stage of the control write
            // transfer has been received. The application can either accept
            // the received data by calling the USB_DEVICE_ControlStatus
            // function with USB_DEVICE_CONTROL_STATUS_OK flag (as shown in
            // this example) or it can reject it by calling the
            // USB_DEVICE_ControlStatus function with
            // USB_DEVICE_CONTROL_STATUS_ERROR flag. 

            //USB_DEVICE_ControlStatus(usbappData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
            
            usb->evt.ctrlDataReceived = true;
            usb->diag.cntCtrlDataRx++;
            break;
           
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_SENT:
            // This means that data in the data stage of the control
            // read transfer has been sent. 
            
            usb->evt.ctrlDataSent = true;
            usb->diag.cntCtrlDataTx++;
            break;

        case USB_DEVICE_EVENT_CONTROL_TRANSFER_ABORTED:
            // This means the host has aborted the control transfer. The
            // application can reset its control transfer state machine.
            
            usb->evt.ctrlAborted = true;
            usb->diag.cntCtrlAbort++;
            break;

        case USB_DEVICE_EVENT_ENDPOINT_READ_COMPLETE:
        {
            // This means schedule endpoint read operation has completed.
            // The application should interpret pData as a pointer to 
            // a USB_DEVICE_EVENT_DATA_ENDPOINT_READ_COMPLETE type.
#if 0
            pData = (USB_DEVICE_EVENT_DATA_ENDPOINT_READ_COMPLETE *) eventData;
            // (if pData != NULL && pData->endpointAddress == USBAPP2_EP_BULK_OUT))
            // probably should check for pData->status == USB_DEVICE_RESULT_OK first
            usbappData.receivedDataCount = pData->length;
            usbappData.rxChunkReady = true;
            usbappData.epDataReadPending = false
#endif
                    
            USB_DEVICE_EVENT_DATA_ENDPOINT_READ_COMPLETE *rd =
                (USB_DEVICE_EVENT_DATA_ENDPOINT_READ_COMPLETE *)eventData;

            if (rd != NULL)
            {
                usb->rxCount = rd->length;
                usb->lastReadResult = rd->status;
                usb->evt.bulkReadComplete = true;
                usb->diag.cntRxComplete++;
            }
            break;
        }
        case USB_DEVICE_EVENT_ENDPOINT_WRITE_COMPLETE:
        {
            // This means schedule endpoint write operation has completed.
            // The application should interpret pData as a pointer to 
            // a USB_DEVICE_EVENT_DATA_ENDPOINT_WRITE_COMPLETE type.
            /* Endpoint write is complete */
            
            USB_DEVICE_EVENT_DATA_ENDPOINT_WRITE_COMPLETE *wr =
                (USB_DEVICE_EVENT_DATA_ENDPOINT_WRITE_COMPLETE *)eventData;

            if (wr != NULL)
            {
                usb->txCount = wr->length;
                usb->lastWriteResult = wr->status;
                usb->evt.bulkWriteComplete = true;
                usb->diag.cntTxComplete++;
            }
            break;
        }
        
        case USB_DEVICE_EVENT_SET_DESCRIPTOR:

            // This means the Host has sent a Set Descriptor request. The
            // application should interpret pData as a
            // USB_DEVICE_EVENT_DATA_SET_DESCRIPTOR pointer type containing the
            // details of the Set Descriptor request. In USB Device
            // Stack, this event is generated if 
            // USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE is defined in the
            // system configuration. The application can use
            // USB_DEVICE_ControlSend, USB_DEVICE_ControlReceive and/or
            // the USB_DEVICE_ControlStatus functions to complete the
            // control transfer.

            break;

        case USB_DEVICE_EVENT_SYNCH_FRAME:

            // This means the host has sent a Sync Frame Request. The
            // application should interpret pData as a
            // USB_DEVICE_EVENT_DATA_SYNCH_FRAME pointer type. In USB Device
            // Stack, this event is generated if 
            // USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE is defined in the
            // system configuration. The application should respond be
            // sending the 2 byte frame number using the
            // USB_DEVICE_ControlSend function. 

            //USB_DEVICE_ControlSend(usbappData.usbDevHandle, &frameNumber, 2);
            break;
            
        default:
            break;
    }

    //return USB_DEVICE_EVENT_RESPONSE_NONE;
}

/* --------------------------------------------------------------------------
 * USB API wrappers
 * -------------------------------------------------------------------------- */

static void USBAPP2_CheckSpeedAndEndpoints(USBAPP2_DATA *usb)
{
    if (USB_DEVICE_ActiveSpeedGet(usb->devHandle) == USB_SPEED_FULL) {
        USBAPP2_LOG_INFO("USB full speed config");
        usb->speed = USB_SPEED_FULL;
        usb->endpointMaxPktSize = 64;
    } else if (USB_DEVICE_ActiveSpeedGet(usb->devHandle) == USB_SPEED_HIGH) {
        USBAPP2_LOG_INFO("USB high speed config");
        usb->speed = USB_SPEED_HIGH;
        usb->endpointMaxPktSize = 512;
    } else {
        USBAPP2_LOG_ERR("USB speed UNKNOWN..setting to FULL");
        usb->speed = USB_SPEED_FULL;
        usb->endpointMaxPktSize = 64;
    }
    
    if (USB_DEVICE_EndpointIsEnabled(usb->devHandle, usb->endpointRx) == false ) {
        /* Enable Read Endpoint */
        USB_DEVICE_EndpointEnable(usb->devHandle, 0, usb->endpointRx,
                USB_TRANSFER_TYPE_BULK, usb->endpointMaxPktSize);
        USBAPP2_LOG_INFO("USB read endpoint enabled");                      
    } else {
        USBAPP2_LOG_INFO("USB read endpoint already enabled");
    }
    if (USB_DEVICE_EndpointIsEnabled(usb->devHandle, usb->endpointTx) == false ) {
        /* Enable Write Endpoint */
        USB_DEVICE_EndpointEnable(usb->devHandle, 0, usb->endpointTx,
                USB_TRANSFER_TYPE_BULK, usb->endpointMaxPktSize);
        USBAPP2_LOG_INFO("USB write endpoint enabled");
    } else {
        USBAPP2_LOG_INFO("USB write endpoint already enabled");
    }
}

static bool USBAPP2_Attach(USBAPP2_DATA *usb)
{
    if ((usb == NULL) ||
        !usb->status.opened ||
        usb->status.attached)
    {
        return false;
    }

    USBAPP2_LOG_INFO("attaching device");
    usb->status.attached = true;    
    USB_DEVICE_Attach(usb->devHandle);
    usb->status.attached = true;
    return true;
}

static bool USBAPP2_Detach(USBAPP2_DATA *usb)
{
    if ((usb == NULL) ||
        !usb->status.opened ||
        !usb->status.attached)
    {
        return false;
    }

    USBAPP2_LOG_INFO("detaching device");
    USB_DEVICE_Detach(usb->devHandle);
    usb->status.attached = false;
    return true;
}

static bool USBAPP2_ArmRead(USBAPP2_DATA *usb)
{
    USB_DEVICE_RESULT res;

    if ((usb == NULL) ||
        !usb->status.configured ||
        usb->status.suspended ||
        usb->rxPending)
    {
        return false;
    }

    usb->rxCount = 0u;
    usb->lastReadResult = USB_DEVICE_RESULT_ERROR;
    usb->protocolPacketReady = false;

    res = USB_DEVICE_EndpointRead(
        usb->devHandle,
        &usb->rxTransferHandle,
        usb->endpointRx,
        usb->rxBuf,
        usb->endpointMaxPktSize
    );

    if (res == USB_DEVICE_RESULT_OK)
    {
        usb->rxPending = true;
        usb->rxArmed = true;
        //USBAPP2_LOG_INFO("armed bulk OUT read ep=0x%02X len=%u",
          //              usb->endpointRx,
            //            (unsigned)usb->endpointMaxPktSize);
        return true;
    }

    USBAPP2_LOG_ERR("EndpointRead failed rc=%d", (int)res);
    usb->evt.error = true;
    return false;
}

static USB_DEVICE_TRANSFER_FLAGS USBAPP2_WriteFlag(size_t len, size_t maxPacket)
{
    if ((len % maxPacket) == 0u)
    {
        return USB_DEVICE_TRANSFER_FLAGS_MORE_DATA_PENDING;
    }

    return USB_DEVICE_TRANSFER_FLAGS_DATA_COMPLETE;
}

static bool USBAPP2_StartWrite(USBAPP2_DATA *usb, const uint8_t *data, size_t len)
{
    USB_DEVICE_RESULT res;

    if ((usb == NULL) ||
        (data == NULL) ||
        (len == 0u) ||
        (len > sizeof(usb->txBuf)) ||
        !usb->status.configured ||
        usb->status.suspended ||
        usb->txPending)
    {
        return false;
    }

    memcpy(usb->txBuf, data, len);
    usb->txCount = 0u;
    usb->lastWriteResult = USB_DEVICE_RESULT_ERROR;
    usb->txPending = true;
    
    res = USB_DEVICE_EndpointWrite(
        usb->devHandle,
        &usb->txTransferHandle,
        usb->endpointTx,
        usb->txBuf,
        len,
        USBAPP2_WriteFlag(len, usb->endpointMaxPktSize)
    );

    if (res == USB_DEVICE_RESULT_OK)
    {
        usb->txPending = true;
        //USBAPP2_LOG_INFO("started bulk IN write ep=0x%02X len=%u",
          //              usb->endpointTx,
            //            (unsigned)len);
        return true;
    }

    USBAPP2_LOG_ERR("EndpointWrite failed rc=%d", (int)res);
    usb->txPending = false;    
    usb->evt.error = true;
    return false;
}

/* --------------------------------------------------------------------------
 * Recovery
 * -------------------------------------------------------------------------- */

static void USBAPP2_DoRecovery(USBAPP2_DATA *usb)
{
#if 0
    usb->diag.cntRecoveries++;

    USBAPP2_LOG_ERR("recovery start: powered=%u attached=%u configured=%u suspended=%u "
                   "lastRead=%d lastWrite=%d protoErr=%d recoveries=%lu",
                   usb->status.powered ? 1u : 0u,
                   usb->status.attached ? 1u : 0u,
                   usb->status.configured ? 1u : 0u,
                   usb->status.suspended ? 1u : 0u,
                   (int)usb->lastReadResult,
                   (int)usb->lastWriteResult,
                   (int)usb->lastProtoErr,
                   (unsigned long)usb->diag.cntRecoveries);

    USBAPP2_ResetControlState(usb);
    USBAPP2_ResetDataPath(usb);

    if (usb->status.attached)
    {
        (void)USBAPP2_Detach(usb);
    }

    usb->status.configured = false;
    usb->status.suspended = false;

    if (usb->status.powered)
    {
        (void)USBAPP2_Attach(usb);
    }

    USBAPP2_LOG_INFO("recovery complete");
#endif
}

/* --------------------------------------------------------------------------
 * Control transfer handling (EP0)
 * -------------------------------------------------------------------------- */

static void USBAPP2_HandleCtrlSetup(USBAPP2_DATA *usb)
{
    const USB_SETUP_PACKET *s = &usb->ctrlSetupPkt;

    USBAPP2_LOG_INFO("ctrl setup bmReq=0x%02X bReq=0x%02X wValue=0x%04X wIndex=0x%04X wLength=%u",
                    s->bmRequestType,
                    s->bRequest,
                    s->wValue,
                    s->wIndex,
                    (unsigned)s->wLength);

    /* This is only for custom vendor-specific EP0 requests.
     * Standard enumeration requests are handled by the stack.
     */
    if ((s->bmRequestType & 0x60u) != 0x40u)
    {
        USBAPP2_LOG_INFO("ctrl setup not vendor-specific; ignoring");
        return;
    }

    switch (s->bRequest)
    {
        case 0x01u: /* Example: GET_INFO, device-to-host */
        {
            static const uint8_t info[] = { 'M', 'I', 'O', '1', 0x01, 0x00 };

            USBAPP2_LOG_INFO("ctrl vendor GET_INFO");
            USB_DEVICE_ControlSend(usb->devHandle, (void *)info, sizeof(info));
            usb->ctrlState = USBAPP2_CTRL_WAIT_STATUS;
            break;
        }

        case 0x02u: /* Example: SET_MODE, host-to-device */
        {
            USBAPP2_LOG_INFO("ctrl vendor SET_MODE len=%u", (unsigned)s->wLength);

            if (s->wLength > sizeof(usb->ctrlBuf))
            {
                USBAPP2_LOG_ERR("ctrl SET_MODE length too large: %u", (unsigned)s->wLength);
                (void)USB_DEVICE_ControlStatus(
                    usb->devHandle,
                    USB_DEVICE_CONTROL_STATUS_ERROR
                );
                usb->ctrlState = USBAPP2_CTRL_ERROR;
                break;
            }

            usb->ctrlExpectedLength = s->wLength;
            usb->ctrlActualLength = 0u;

            if (usb->ctrlExpectedLength == 0u)
            {
                (void)USB_DEVICE_ControlStatus(
                    usb->devHandle,
                    USB_DEVICE_CONTROL_STATUS_OK
                );
                usb->ctrlState = USBAPP2_CTRL_IDLE;
            }
            else
            {
                USB_DEVICE_ControlReceive(
                    usb->devHandle,
                    usb->ctrlBuf,
                    usb->ctrlExpectedLength
                );
                usb->ctrlState = USBAPP2_CTRL_WAIT_DATA;
            }
            break;
        }

        default:
        {
            USBAPP2_LOG_ERR("unsupported vendor ctrl request bReq=0x%02X", s->bRequest);
            (void)USB_DEVICE_ControlStatus(
                usb->devHandle,
                USB_DEVICE_CONTROL_STATUS_ERROR
            );
            usb->ctrlState = USBAPP2_CTRL_ERROR;
            break;
        }
    }
}

static void USBAPP2_HandleCtrlDataReceived(USBAPP2_DATA *usb)
{
    USBAPP2_LOG_INFO("ctrl data received len=%u", (unsigned)usb->ctrlExpectedLength);

    if (usb->ctrlState != USBAPP2_CTRL_WAIT_DATA)
    {
        USBAPP2_LOG_ERR("ctrl data received in unexpected state=%d", (int)usb->ctrlState);
        USBAPP2_ResetControlState(usb);
        return;
    }

    if ((usb->ctrlExpectedLength == 0u) ||
        (usb->ctrlExpectedLength > sizeof(usb->ctrlBuf)))
    {
        USBAPP2_LOG_ERR("ctrl data invalid length=%u", (unsigned)usb->ctrlExpectedLength);
        (void)USB_DEVICE_ControlStatus(
            usb->devHandle,
            USB_DEVICE_CONTROL_STATUS_ERROR
        );
        usb->ctrlState = USBAPP2_CTRL_ERROR;
        return;
    }

    /* Apply ctrlBuf here if you actually use custom EP0 OUT requests. */
    USBAPP2_LOG_INFO("ctrl data accepted");

    (void)USB_DEVICE_ControlStatus(
        usb->devHandle,
        USB_DEVICE_CONTROL_STATUS_OK
    );

    usb->ctrlState = USBAPP2_CTRL_IDLE;
}

/* --------------------------------------------------------------------------
 * Bulk protocol handling
 * -------------------------------------------------------------------------- */

static void USBAPP2_BuildProtoError(USBAPP2_DATA *usb, USBAPP2_PROTO_ERR err)
{
    USBAPP2_LOG_ERR("protocol error=%d", (int)err);

    usb->txBuf[0] = 0xEEu;
    usb->txBuf[1] = (uint8_t)err;
    usb->txBuf[2] = 0x00u;
    usb->txBuf[3] = 0x00u;
    usb->lastProtoErr = err;
    usb->protocolResponseReady = true;
    usb->diag.cntProtoErr++;
}
#define DO_LOOPBACK
#ifdef DO_LOOPBACK
uint8_t loopStoreIn[512] CACHE_ALIGN;
uint8_t loopStoreOut[512] CACHE_ALIGN;
uint16_t loopCount = 0;
bool gotOne = false;
#endif
static void USBAPP2_HandleNewProtocolChunk(USBAPP2_DATA *usb)
{
    usb->protocolPacketReady = false;

    if (usb->rxCount >= 1u) {
        USBAPP2_LOG_INFO("processing incoming packet len=%u",
                        (unsigned)usb->rxCount);
        /* Feed into the ring */
#ifdef  DO_LOOPBACK
        memcpy(loopStoreIn, usb->rxBuf, usb->rxCount);
        loopCount = usb->rxCount;
        gotOne = true;
#endif
        //txbp_rxring_write(&usb->txbp.rx, usb->rxBuf, usb->rxCount);
        usb->rxCount = 0;
    } else {
        USBAPP2_LOG_INFO("zero len incoming packet?");
    }
}

static void USBAPP2_TryProtocolIncoming(USBAPP2_DATA *usb)
{
#if 0
    // ---- Parse a bounded number of frames per pass (prevents starvation)
    for (int i = 0; i < 8; i++) {
        if (!txbp_parse_one_from_ring(&usb->txbp))
            break;
        usb->lastProtoErr = USBAPP2_PROTO_OK;
        usb->diag.cntProtoOk++;        
    }
#endif
}

static void USBAPP2_TryProtocolOutgoing(USBAPP2_DATA *usb)
{
#ifdef DO_LOOPBACK
    if (!usb->txPending && gotOne ) {
            memcpy(loopStoreOut, loopStoreIn, loopCount);        
            if (!USBAPP2_StartWrite(usb, loopStoreOut, loopCount)) {
                USBAPP2_LOG_INFO("USB outgoing data packet StartWrite error"); 
            }
            gotOne = false;
            loopCount = 0;
    }
#endif
#if 0
    // ---- TX pump: send next queued response/event if USB IN not busy
    if (!usb->txPending && usb->txbp.txq.count > 0) {
        txbp_txframe_t *f = NULL;
        if (txbp_txq_peek(&usb->txbp.txq, &f) == 0) {
            usb->txPending = true;
            USBAPP2_LOG_INFO("USB outgoing data packet len (%d)", f->len);                  

            USB_DEVICE_EndpointWrite(
                    usb->devHandle, 
                    &usb->txTransferHandle,
                    usb->endpointTx, 
                    f->data, 
                    f->len, 
                    USB_DEVICE_TRANSFER_FLAGS_DATA_COMPLETE);
            
            txbp_txq_consume(&usb->txbp.txq);
        }
    }    
#endif
}

#if 0
    usb->lastProtoErr = USBAPP2_PROTO_OK;
    usb->protocolResponseReady = true;
    usb->diag.cntProtoOk++;
        USBAPP2_LOG_INFO("protocol packet processed ok, txLen=%u", (unsigned)txLen);
    }

    if (usb->protocolResponseReady && !usb->txPending)
    {
        usb->protocolResponseReady = false;
        (void)USBAPP2_StartWrite(usb, usb->txBuf, txLen);
    }
#endif
    
/* --------------------------------------------------------------------------
 * Main task state machine
 * -------------------------------------------------------------------------- */

void USBAPP2_Tasks(void)
{
    USBAPP2_DATA *usb = &g_usbApp2;

    switch (usb->state)
    {
        case USBAPP2_STATE_STARTUP:
        {
#if 0
            if (gSysruntime.bits.bStartedInDiags && !gSysruntime.bits.bDiagAllowUSB) {
                /* If we started up in diags, they need to allow us forward */
                return;
            }
#endif
            /* Otherwise, allow usb now */
            USBAPP2_SetState(usb, USBAPP2_STATE_INIT);
            break;
        }

        case USBAPP2_STATE_INIT:
        {
            usb->devHandle = USB_DEVICE_Open(
                USB_DEVICE_INDEX_0,
                DRV_IO_INTENT_READWRITE
            );

            if (usb->devHandle != USB_DEVICE_HANDLE_INVALID)
            {
                USB_DEVICE_EventHandlerSet(
                    usb->devHandle,
                    USBAPP2_DeviceEventHandler,
                    (uintptr_t)usb
                );

                usb->status.opened = true;
                USBAPP2_LOG_INFO("USB_DEVICE_Open success");
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_POWER);
            }
            else
            {
                /* Device layer not ready yet...try later */
                USBAPP2_LOG_ERR("USB_DEVICE_Open failed");
            }

            break;
        }

        case USBAPP2_STATE_WAIT_FOR_POWER:
        {
#if 0
            /*
             * We don't ever hit this state, as we don't use VBUS.
             * I left the code in for future, but we just transition out.
             */
            usb->evt.powerDetected = false;
            usb->status.powered = true;

            USBAPP2_LOG_INFO("event: power detected");
            (void)USBAPP2_Attach(usb);
            USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
#endif
            if (usb->evt.powerDetected)
            {
                usb->evt.powerDetected = false;
                usb->status.powered = true;

                USBAPP2_LOG_INFO("event: power detected");
                //USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                (void)USBAPP2_Attach(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
            }

            if (usb->evt.powerRemoved)
            {
                usb->evt.powerRemoved = false;
                usb->status.powered = false;

                USBAPP2_LOG_INFO("event: power removed");
                USBAPP2_ResetRuntimeState(usb);
            }

            if (usb->evt.error)
            {
                usb->evt.error = false;
                USBAPP2_LOG_ERR("event: bus/device error while waiting for power");
                USBAPP2_SetState(usb, USBAPP2_STATE_ERROR_RECOVERY);
            }
            
            break;
        }

        case USBAPP2_STATE_WAIT_FOR_CONFIGURATION:
        {
            /* No VBUS stuff, but keep for debug */
            if (usb->evt.powerRemoved)
            {
                usb->evt.powerRemoved = false;

                USBAPP2_LOG_INFO("event: power removed");
                usb->status.powered = false;
                (void)USBAPP2_Detach(usb);
                USBAPP2_ResetRuntimeState(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_POWER);
                break;
            }

            if (usb->evt.reset)
            {
                usb->evt.reset = false;
                USBAPP2_LOG_INFO("event: bus reset");
                USBAPP2_ResetForBusReset(usb);
            }

            if (usb->evt.deconfigured)
            {
                usb->evt.deconfigured = false;
                USBAPP2_LOG_INFO("event: deconfigured");
                USBAPP2_ResetForDeconfig(usb);
            }

            /* No ctrl messaging, but keep for debug for now */
            if (usb->evt.ctrlSetup)
            {
                usb->evt.ctrlSetup = false;
                USBAPP2_LOG_INFO("event: ctrl handle ctrl setup");                
                USBAPP2_HandleCtrlSetup(usb);
            }

            if (usb->evt.ctrlDataReceived)
            {
                usb->evt.ctrlDataReceived = false;
                USBAPP2_LOG_INFO("event: ctrl handle ctrl recv");                     
                USBAPP2_HandleCtrlDataReceived(usb);
            }

            if (usb->evt.ctrlDataSent)
            {
                usb->evt.ctrlDataSent = false;
                USBAPP2_LOG_INFO("event: ctrl data sent");
                if (usb->ctrlState == USBAPP2_CTRL_WAIT_STATUS)
                {
                    usb->ctrlState = USBAPP2_CTRL_IDLE;
                }
            }

            if (usb->evt.ctrlAborted)
            {
                usb->evt.ctrlAborted = false;
                USBAPP2_LOG_ERR("event: ctrl aborted");
                USBAPP2_ResetControlState(usb);
            }

            if (usb->evt.configured)
            {
                usb->evt.configured = false;
                usb->status.configured = true;
                usb->status.suspended = false;

                USBAPP2_LOG_INFO("event: configured");
           
                /* 
                 * Could (should?) check the configurationValue
                 * and make sure it's 1 (as that's all we have here).
                 * Could check speed here and set max packet
                 * but we're always full speed at 64 bytes
                 */
                USBAPP2_CheckSpeedAndEndpoints(usb);
                USBAPP2_InitBuffers(usb);
                (void)USBAPP2_ArmRead(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_READY);
            }

            /* Could get this during an idle bus */           
            if (usb->evt.suspended)
            {
                usb->evt.suspended = false;
                usb->status.suspended = true;

                if (usb->status.configured)
                {
                    USBAPP2_LOG_INFO("event: suspended while configured. Changing state to SUSPENDED ");
                    USBAPP2_SetState(usb, USBAPP2_STATE_SUSPENDED);
                }
                else
                {
                    USBAPP2_LOG_INFO("event: suspended while waiting for configuration. Not changing state ");
                }
            }            

            /* Host will send this when it starts back up after an idle (suspended) */
            if (usb->evt.resumed)
            {
                usb->evt.resumed = false;
                usb->status.suspended = false;
                USBAPP2_LOG_INFO("event: resumed");
            }

            if (usb->evt.error)
            {
                usb->evt.error = false;
                USBAPP2_LOG_ERR("event: bus/device error while waiting for configuration");
                USBAPP2_SetState(usb, USBAPP2_STATE_ERROR_RECOVERY);
            }

            break;
        }

        case USBAPP2_STATE_READY:
        {
            /* This shouldn't happen either, but keep for debug */            
            if (usb->evt.powerRemoved)
            {
                usb->evt.powerRemoved = false;

                USBAPP2_LOG_INFO("event: power removed");
                usb->status.powered = false;
                (void)USBAPP2_Detach(usb);
                USBAPP2_ResetRuntimeState(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_POWER);
                break;
            }

            if (usb->evt.reset)
            {
                usb->evt.reset = false;
                USBAPP2_LOG_INFO("event: bus reset");
                USBAPP2_ResetForBusReset(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                break;
            }

            if (usb->evt.deconfigured)
            {
                usb->evt.deconfigured = false;
                USBAPP2_LOG_INFO("event: deconfigured");
                USBAPP2_ResetForDeconfig(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                break;
            }

            /* This shouldn't happen either, but keep for debug */            
            if (usb->evt.suspended)
            {
                usb->evt.suspended = false;
                usb->status.suspended = true;
                USBAPP2_LOG_INFO("event: suspended");
                USBAPP2_SetState(usb, USBAPP2_STATE_SUSPENDED);
                break;
            }

            /* No usb ctrl messaging yet, but keep for debug */
            if (usb->evt.ctrlSetup)
            {
                usb->evt.ctrlSetup = false;
                USBAPP2_HandleCtrlSetup(usb);
            }

            if (usb->evt.ctrlDataReceived)
            {
                usb->evt.ctrlDataReceived = false;
                USBAPP2_HandleCtrlDataReceived(usb);
            }

            if (usb->evt.ctrlDataSent)
            {
                usb->evt.ctrlDataSent = false;
                USBAPP2_LOG_INFO("event: ctrl data sent");
                if (usb->ctrlState == USBAPP2_CTRL_WAIT_STATUS)
                {
                    usb->ctrlState = USBAPP2_CTRL_IDLE;
                }
            }

            if (usb->evt.ctrlAborted)
            {
                usb->evt.ctrlAborted = false;
                USBAPP2_LOG_ERR("event: ctrl aborted");
                USBAPP2_ResetControlState(usb);
            }

            if (usb->evt.bulkReadComplete)
            {
                usb->evt.bulkReadComplete = false;
                usb->rxPending = false;
                usb->rxArmed = false;

                if (usb->lastReadResult == USB_DEVICE_RESULT_OK)
                {
                    //USBAPP2_LOG_INFO("bulk OUT complete len=%u", (unsigned)usb->rxCount);
                    usb->protocolPacketReady = true;
                }
                else
                {
                    USBAPP2_LOG_ERR("bulk OUT complete with error rc=%d", (int)usb->lastReadResult);
                    usb->lastProtoErr = USBAPP2_PROTO_ERR_INTERNAL;
                    usb->diag.cntProtoErr++;
                }
            }

            if (usb->protocolPacketReady)
            {
                USBAPP2_HandleNewProtocolChunk(usb);
            }

            /* Re-arm quickly */
            if (!usb->rxPending && !usb->rxArmed)
            {
                (void)USBAPP2_ArmRead(usb);
            }

            (void)USBAPP2_TryProtocolIncoming(usb);
      
            if (usb->evt.bulkWriteComplete)
            {
                usb->evt.bulkWriteComplete = false;
                usb->txPending = false;

                if (usb->lastWriteResult == USB_DEVICE_RESULT_OK)
                {
                    //USBAPP2_LOG_INFO("bulk IN complete len=%u", (unsigned)usb->txCount);
                }
                else
                {
                    //USBAPP2_LOG_ERR("bulk IN complete with error rc=%d", (int)usb->lastWriteResult);
                    usb->evt.error = true;
                }
            }            
            
            (void)USBAPP2_TryProtocolOutgoing(usb);
                        
            if (usb->evt.error)
            {
                usb->evt.error = false;
                USBAPP2_LOG_ERR("event: bus/device error in READY");
                USBAPP2_SetState(usb, USBAPP2_STATE_ERROR_RECOVERY);
            }

            break;
        }

        /* An idle bus takes us here and is usually followed by a bus reset to get us out */
        case USBAPP2_STATE_SUSPENDED:
        {
            if (usb->evt.powerRemoved)
            {
                usb->evt.powerRemoved = false;

                USBAPP2_LOG_INFO("event: power removed");
                usb->status.powered = false;
                usb->status.suspended = false;

                (void)USBAPP2_Detach(usb);
                USBAPP2_ResetRuntimeState(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_POWER);
                break;
            }

            if (usb->evt.reset)
            {
                usb->evt.reset = false;
                usb->status.suspended = false;
                USBAPP2_LOG_INFO("event: bus reset");
                USBAPP2_ResetForBusReset(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                break;
            }

            if (usb->evt.deconfigured)
            {
                usb->evt.deconfigured = false;
                usb->status.suspended = false;
                USBAPP2_LOG_INFO("event: deconfigured");
                USBAPP2_ResetForDeconfig(usb);
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                break;
            }

            if (usb->evt.resumed)
            {
                usb->evt.resumed = false;
                usb->status.suspended = false;

                USBAPP2_LOG_INFO("event: resumed");

                if (usb->status.configured)
                {
                    if (!usb->rxPending && !usb->rxArmed)
                    {
                        (void)USBAPP2_ArmRead(usb);
                    }
                    USBAPP2_SetState(usb, USBAPP2_STATE_READY);
                }
                else
                {
                    USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
                }
            }

            if (usb->evt.error)
            {
                usb->evt.error = false;
                USBAPP2_LOG_ERR("event: bus/device error in SUSPENDED");
                USBAPP2_SetState(usb, USBAPP2_STATE_ERROR_RECOVERY);
            }

            break;
        }

        case USBAPP2_STATE_ERROR_RECOVERY:
        {
            USBAPP2_DoRecovery(usb);
            USBAPP2_ResetEvents(usb);

            if (usb->status.powered)
            {
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_CONFIGURATION);
            }
            else
            {
                USBAPP2_SetState(usb, USBAPP2_STATE_WAIT_FOR_POWER);
            }
            break;
        }

        default:
        {
            USBAPP2_LOG_ERR("unexpected state=%d", (int)usb->state);
            USBAPP2_SetState(usb, USBAPP2_STATE_STARTUP);
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */

