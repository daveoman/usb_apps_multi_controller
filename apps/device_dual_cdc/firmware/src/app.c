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

