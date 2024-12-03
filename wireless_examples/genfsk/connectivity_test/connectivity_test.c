/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
/* Drv */
#include "board.h"
#include "fsl_component_button.h"

/* Fwk */
#include "fsl_component_mem_manager.h"
#include "fsl_component_timer_manager.h"
#include "RNG_Interface.h"
#include "fsl_component_messaging.h"
#include "SecLib.h"
#include "fsl_component_panic.h"
#include "fsl_os_abstraction.h"
#include "fsl_adapter_reset.h"
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "MemManager.h"
#include "TimersManager.h"
#include "RNG_Interface.h"
#include "Messaging.h"
#include "SecLib.h"
#include "fsl_os_abstraction.h"
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

/* KSDK */
#include "board.h"
#include "app.h"
#if defined(KW45B41Z83_SERIES) || defined(KW45B41Z82_SERIES) || defined(K32W1480_SERIES) || defined(MCXW716A_SERIES) || defined(MCXW716C_SERIES) || defined(KW47B42ZB7_cm33_core0_SERIES) || defined(MCXW727C_cm33_core0_SERIES)
#include "fwk_platform.h"
#if !defined(MULTICORE_APP) || (MULTICORE_APP!=1)
#include "platform_genfsk.h"
#else
#include "fwk_platform_genfsk.h"
#endif
#endif

#ifdef cPWR_UsePowerDownMode
#if (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#endif
#endif

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOSConfig.h"
#endif

#include "genfsk_interface.h"
#include "gen_fsk_tests.h"

#include "menus.h"
#include "connectivity_test.h"

#if defined(FPGA_SUPPORT) && (FPGA_SUPPORT == 1)
#include "fwk_platform_fpga.h"
#endif
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#if defined(BOARD_LOCALIZATION_REVISION_SUPPORT) && (BOARD_LOCALIZATION_REVISION_SUPPORT > 0)
#define gAppNumberOfTests_d (6)
#else
#define gAppNumberOfTests_d (5)
#endif /*BOARD_LOCALIZATION_REVISION_SUPPORT > 0*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#define App_NotifySelf() OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtSelfEvent_c)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#define App_NotifySelf() OSA_EventSet(mAppThreadEvt, gCtEvtSelfEvent_c)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

#ifdef FSL_RTOS_FREE_RTOS
#if (configUSE_IDLE_HOOK)
#define mAppIdleHook_c 1
#endif
#endif

#ifndef mAppIdleHook_c
#define mAppIdleHook_c 0
#endif

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#define Serial_Print(a,b,c) SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, (uint8_t *)b, strlen(b))
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/
typedef bool_t ( * pCtTestFunction)(ct_event_t evt, void* pAssocData);
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/*Application main*/
static void App_Thread (uint32_t param);
/*Application event handler*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static void App_HandleEvents(osa_event_flags_t flags);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static void App_HandleEvents(osaEventFlags_t flags);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
/*Function that reads latest byte from Serial Manager*/
static void App_UpdateUartData(uint8_t* pData);
/*Application Init*/
static app_status_t App_InitApp();
/*Print shortcut menu values*/
static void App_PrintTestParameters(bool_t bEraseLine);
/*Generic FSK RX callback*/
static void App_GenFskReceiveCallback(uint8_t *pBuffer,
                                      uint16_t bufferLength,
                                      uint64_t timestamp,
                                      int8_t rssi,
                                      uint8_t crcValid);
/*Generic FSK Notification callback*/
static void App_GenFskEventNotificationCallback(genfskEvent_t event,
                                                genfskEventStatus_t eventStatus);
/*Serial Manager UART RX callback*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static void App_SerialCallback(void* param,
                               serial_manager_callback_message_t *message,
                               serial_manager_status_t status );
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static void App_SerialCallback(void* param);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
/*Application Thread notification function (sends event to application task)*/
static void App_NotifyAppThread(void);
/*Timer callback*/
static void App_TimerCallback(void* param);

/*Handles all keyboard events*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#if (defined(gButtonSupported_d) && (gButtonSupported_d > 0))
static button_status_t BleApp_HandleKeys0(void *buttonHandle, button_callback_message_t *message,void *callbackParam);
static button_status_t BleApp_HandleKeys1(void *buttonHandle, button_callback_message_t *message,void *callbackParam);
#endif /*gButtonSupported_d > 0*/
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack(uint8_t events);
#endif
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

#if (cPWR_UsePowerDownMode)
#if (mAppIdleHook_c)
#define AppIdle_TaskInit()
#define App_Idle_Task()
#else
static osaStatus_t AppIdle_TaskInit(void);
static void App_Idle_Task(osaTaskParam_t argument);
#endif /* (mAppIdleHook_c) */
#endif /* (cPWR_UsePowerDownMode) */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;
/*event used by the application thread*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static OSA_EVENT_HANDLE_DEFINE(mAppThreadEvt);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static osaEventId_t mAppThreadEvt;
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
/*variable to store key pressed by user*/
static uint8_t mAppUartData = 0;
/*variable to store button pressed by user*/
static uint8_t mAppPbData = PB_INVALID;
/*application state*/
static app_states_t mAppState = gAppStateInit_c;
/*set TRUE when user presses [ENTER] on logo screen*/
static bool_t mAppStartApp = FALSE;
/*pointer to test currently running*/
static pCtTestFunction pfCtCurrentTest = NULL;
/*pointer to data associated to each event*/
static void* pEvtAssociatedData = NULL;
/*array of existing tests*/
static pCtTestFunction ppfCtAvailableTests[gAppNumberOfTests_d];
/*structure to store information regarding latest received packet*/
static ct_rx_indication_t mAppRxLatestPacket;
/*latest generic fsk event status*/
static genfskEventStatus_t mAppGenfskStatus;

/*extern GENFSK instance id*/
extern uint8_t mAppGenfskId;
/*extern MCU reset api*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void ResetMCU(void);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
OSA_TASK_DEFINE( App_Idle_Task, gAppIdleTaskPriority_c, 1, gAppIdleTaskStackSize_c, FALSE );
osaTaskId_t gAppIdleTaskId = 0;
#endif
#endif  /* cPWR_UsePowerDownMode */

uint32_t mRxBufferBytesAvailable = 0U;

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static OSA_TASK_HANDLE_DEFINE(s_startTaskHandle);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
/*osa start_task*/
void start_task(void *argument);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
#if !defined(MULTICORE_APP) || (MULTICORE_APP!=1)
        uint8_t pseudoRNGSeed[20] = {0};
#endif

        platformInitialized = 1;

#if !defined(CPU_QN908X) && defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d>0)
        /* Init DCDC module */
        BOARD_DCDCInit();
#endif

#if defined(FPGA_SUPPORT) && (FPGA_SUPPORT == 1)
        PLATFORM_InitRadio();
#endif

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        //hardware_init();
#else
        hardware_init();
#endif
        /* Framework init */
        (void)MEM_Init();

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#if (defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0))
        BUTTON_InstallCallback((button_handle_t)g_buttonHandle[0], BleApp_HandleKeys0, NULL);
        BUTTON_InstallCallback((button_handle_t)g_buttonHandle[1], BleApp_HandleKeys1, NULL);
#endif /*gAppButtonCnt_c > 0*/
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        TMR_Init();

        LED_Init();

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
        KBD_Init(App_KeyboardCallBack);
#endif
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
        SecLib_Init();

        RNG_Init();
#if !defined(MULTICORE_APP) || (MULTICORE_APP!=1)
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[0])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[4])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[8])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[12])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[16])));
        RNG_SetPseudoRandomNoSeed(pseudoRNGSeed);
#endif

        GENFSK_Init();

        

        /* GENFSK LL Init with default register config */
        GENFSK_AllocInstance(&mAppGenfskId, NULL, NULL, NULL);

        /* update the max tx power setting */
        PLATFORM_SetGenfskMaxTxPower(gAppMaxTxPowerDbm_c);

        /*create app thread event*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        OSA_EventCreate((osa_event_handle_t)mAppThreadEvt, TRUE);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        mAppThreadEvt = OSA_EventCreate(TRUE);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
        AppIdle_TaskInit();
#endif
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#else
        /*start serial flashing using all LEDs*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#if (defined(gAppLedCnt_c) && (gAppLedCnt_c >= 1))
          Led1Flashing();
#if (gAppLedCnt_c >= 2)
           Led2Flashing();
#endif
#endif /* gAppLedCnt_c */
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        LED_StartSerialFlash(LED1);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
#endif

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        mAppSerId = gSerMgrIf;
        SerialManager_OpenReadHandle((serial_handle_t)mAppSerId, (serial_read_handle_t)g_connReadHandle);
        SerialManager_OpenWriteHandle((serial_handle_t)mAppSerId, (serial_write_handle_t)g_connWriteHandle);

        (void)SerialManager_InstallRxCallback((serial_read_handle_t)g_connReadHandle, (serial_manager_callback_t)App_SerialCallback, NULL);

        (void)TM_Open((timer_handle_t)mAppTmrId);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        //initialize Serial Manager
        Serial_InitManager();

        Serial_InitInterface(&mAppSerId, 
                             APP_SERIAL_INTERFACE_TYPE, 
                             APP_SERIAL_INTERFACE_INSTANCE);
        /*set baudrate to 115200*/
        Serial_SetBaudRate(mAppSerId, 
                           APP_SERIAL_INTERFACE_SPEED);
        /*set Serial Manager receive callback*/
        Serial_SetRxCallBack(mAppSerId, App_SerialCallback, NULL);

        /*allocate a timer*/
        mAppTmrId = TMR_AllocateTimer();
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

        /*Prints the Welcome screens in the terminal*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        PrintMenu(cu8Logo, (serial_write_handle_t)g_connWriteHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
        PrintMenu(cu8Logo, mAppSerId);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
    }

    /* Call application task */
    App_Thread( param );
}

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void start_task(void *argument)
{
    main_task((uint32_t)argument);
}

static OSA_TASK_DEFINE(start_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0);

int main(void)
{
    /*init OSA: should be called before any other OSA API*/
    OSA_Init();

    BOARD_InitHardware();

#if defined(KW45B41Z83_SERIES) || defined(KW45B41Z82_SERIES) || defined(K32W1480_SERIES) || defined(MCXW716A_SERIES) || defined(MCXW716C_SERIES) || defined(KW47B42ZB7_cm33_core0_SERIES) || defined(MCXW727C_cm33_core0_SERIES)
    PLATFORM_InitGenfsk();
    APP_InitServices();
#else
    APP_InitHardware();
#endif

    (void)OSA_TaskCreate((osa_task_handle_t)s_startTaskHandle, OSA_TASK(start_task), NULL);

    /*start scheduler*/
    OSA_Start();

    /*won't run here*/
    assert(0);
    return 0;
}
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

/*! *********************************************************************************
* \brief  This function represents the Application task.
*         This task reuses the stack alocated for the MainThread.
*         This function is called to process all events for the task. Events
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
********************************************************************************** */
void App_Thread (uint32_t param)
{
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
    osa_event_flags_t mAppThreadEvtFlags = 0;
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
    osaEventFlags_t mAppThreadEvtFlags = 0;
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

    while(1)
    {
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        (void)OSA_EventWait((osa_event_handle_t)mAppThreadEvt, gCtEvtEventsAll_c, FALSE, osaWaitForever_c ,&mAppThreadEvtFlags);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        (void)OSA_EventWait(mAppThreadEvt, gCtEvtEventsAll_c, FALSE, osaWaitForever_c ,&mAppThreadEvtFlags);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
        if(mAppThreadEvtFlags)
        {
            if(mAppStartApp)
            {
                App_HandleEvents(mAppThreadEvtFlags);/*handle app events*/
            }
            else
            {
                if(mAppThreadEvtFlags & gCtEvtUart_c) /*if uart event*/
                {
                    App_UpdateUartData(&mAppUartData); /*read new byte*/
                    if(mAppUartData == '\r')
                    {
                        mAppStartApp = TRUE;
                        /*notify task again to start running*/
                        App_NotifySelf();
                    }
                    else
                    {
                        /*if other key is pressed show screen again*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
                        PrintMenu(cu8Logo, (serial_write_handle_t)g_connWriteHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
                        PrintMenu(cu8Logo, mAppSerId);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
                    }
                }

                if(mAppThreadEvtFlags & gCtEvtKBD_c) /*if KBD event*/
                {
                    mAppStartApp = TRUE;
                    /*notify task again to start running*/
                    App_NotifySelf();
                }
            }
        }
        if(gUseRtos_c == 0) /*if bare-metal break while*/
        {
            break;
        }
    }
}
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#if (defined(gButtonSupported_d) && (gButtonSupported_d > 0))
static button_status_t BleApp_HandleKeys0(void *buttonHandle, button_callback_message_t *message,void *callbackParam)
{
    switch (message->event)
    {
        case kBUTTON_EventOneClick:
        case kBUTTON_EventShortPress:
        {
            mAppPbData = PB1_PRESSED;
            break;
        }

        case kBUTTON_EventLongPress:
        {
            mAppPbData = PB1_LONG_PRESS;
            break;
        }

        default:
        {
            /* No action required */
            break;
        }
    }

    OSA_EventSet(mAppThreadEvt, gCtEvtKBD_c);

    return kStatus_BUTTON_Success;
}

static button_status_t BleApp_HandleKeys1(void *buttonHandle, button_callback_message_t *message,void *callbackParam)
{
    switch (message->event)
    {
        case kBUTTON_EventOneClick:
        case kBUTTON_EventShortPress:
        {
            mAppPbData = PB2_PRESSED;
            break;
        }

        case kBUTTON_EventLongPress:
        {
            mAppPbData = PB2_LONG_PRESS;
            break;
        }

        default:
        {
            /* No action required */
            break;
        }
    }

    OSA_EventSet(mAppThreadEvt, gCtEvtKBD_c);

    return kStatus_BUTTON_Success;
}
#endif /*gButtonSupported_d > 0*/
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack(uint8_t events)
{
    switch (events)
    {
    case gKBD_EventPB1_c:
        mAppPbData = PB1_PRESSED;
        break;
    case gKBD_EventPB2_c:
        mAppPbData = PB2_PRESSED;
        break;
    case gKBD_EventLongPB1_c:
        mAppPbData = PB1_LONG_PRESS;
        break;
    case gKBD_EventLongPB2_c:
        mAppPbData = PB2_LONG_PRESS;
        break;
    default:
        break;
    }

    OSA_EventSet(mAppThreadEvt, gCtEvtKBD_c);
}
#endif /*gKBD_KeysCount_c > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

/*! *********************************************************************************
* \brief  The application event handler
*         This function is called each time there is an OS event for the AppThread
* \param[in]  flags The OS event flags specific to the Connectivity Test App.
*
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void App_HandleEvents(osa_event_flags_t flags)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
void App_HandleEvents(osaEventFlags_t flags)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
{
    app_status_t status;

    if(flags & gCtEvtUart_c)
    {
        App_UpdateUartData(&mAppUartData);
        if(CT_IsShortcutMenuEnabled())
        {
            /*if it is a shortcut key clear the flag so it is not handled twice*/
            if(CT_UpdateShortcutKeyParam(mAppUartData))
            {
                /* Only print the RF parameter list once. In running mode, the RF parameter list will be updated by the
                category of test (Continuous tests, PER test or Range test). */
                if (mAppState != gAppStateRunning_c)
                {
                    App_PrintTestParameters(TRUE);
                }
            }
        }
    }
    switch(mAppState)
    {
    case gAppStateInit_c:
        /*initialize app and map tests*/
        status = App_InitApp();
        if( status != gAppStatusSuccess )
        {
            Serial_Print(mAppSerId, cu8AppError[status], gAllowToBlock_d);
            break;
        }
        /*enter app default state*/
        mAppState = gAppStateIdle_c;
        /*notify app task to move to new state*/
        App_NotifySelf();
        break;
    case gAppStateIdle_c:
        /*print main menu*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        PrintMenu(cu8MainMenu, (serial_write_handle_t)g_connWriteHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
        PrintMenu(cu8MainMenu, mAppSerId);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
        /*print test params*/
        App_PrintTestParameters(FALSE);
#if (cPWR_UsePowerDownMode)
        LED_StopFlashingAllLeds();
#endif /* (cPWR_UsePowerDownMode) */
        mAppState = gAppStateSelectTest_c;
        break;
    case gAppStateSelectTest_c:
        if(flags & gCtEvtUart_c) /*if uart event*/
        {
            if(mAppUartData >= '1' &&
               mAppUartData <= ('0' + gAppNumberOfTests_d)) /*select which test to run*/
            {
                pfCtCurrentTest = ppfCtAvailableTests[mAppUartData - '1'];
                mAppState = gAppStateRunning_c;
                App_NotifySelf();
            }
            else if(mAppUartData == '!')
            {
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
                HAL_ResetMCU();
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
                ResetMCU();
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
            }
        }
        break;
    case gAppStateRunning_c: /*event handling for test currently running*/
        if(flags & gCtEvtRxDone_c)
        {
            pEvtAssociatedData = &mAppRxLatestPacket;
            if(pfCtCurrentTest(gCtEvtRxDone_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtTxDone_c)
        {
            pEvtAssociatedData = &mAppGenfskStatus;
            if(pfCtCurrentTest(gCtEvtTxDone_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtRxFailed_c)
        {
            if(pfCtCurrentTest(gCtEvtRxFailed_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtSeqTimeout_c)
        {
            if(pfCtCurrentTest(gCtEvtSeqTimeout_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtUart_c)
        {
            pEvtAssociatedData = &mAppUartData;
            if(pfCtCurrentTest(gCtEvtUart_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtTimerExpired_c)
        {
            pEvtAssociatedData = NULL;
            if(pfCtCurrentTest(gCtEvtTimerExpired_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
#if (cPWR_UsePowerDownMode)
        if(flags & gCtEvtWakeUp_c)
        {
            if(pfCtCurrentTest(gCtEvtWakeUp_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
#endif /* (cPWR_UsePowerDownMode) */
        if(flags & gCtEvtKBD_c)
        {
            pEvtAssociatedData = &mAppPbData;
            if(pfCtCurrentTest(gCtEvtKBD_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtSelfEvent_c)
        {
            if(pfCtCurrentTest(gCtEvtSelfEvent_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief  This function is called each time SerialManager notifies the application
*         task that a byte was received.
*         The function checks if there are additional bytes in the SerialMgr
*         queue and simulates a new SM event if there is more data.
* \param[in]  pData Pointer to where to store byte read.
*
********************************************************************************** */
static void App_UpdateUartData(uint8_t* pData)
{
    uint32_t u32SerBytesCount = 0;
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))


    if(kStatus_SerialManager_Success == SerialManager_TryRead((serial_read_handle_t)g_connReadHandle, pData, 1U, &u32SerBytesCount))
    {
        mRxBufferBytesAvailable -= u32SerBytesCount;
        if(mRxBufferBytesAvailable > 0U)
        {
            (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtUart_c);
        }
    }
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
    if(gSerial_Success_c == Serial_GetByteFromRxBuffer(mAppSerId, pData, &u32SerBytesCount))
    {
        Serial_RxBufferByteCount(mAppSerId, &u32SerBytesCount);
        if(u32SerBytesCount)
        {
            (void)OSA_EventSet(mAppThreadEvt, gCtEvtUart_c);
        }
    }
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
}

/*! *********************************************************************************
* \brief  Application initialization. It installs the main menu callbacks and
*         calls the Connectivity Test for Generic FSK init.
*
********************************************************************************** */
static app_status_t App_InitApp()
{
   ppfCtAvailableTests[0] = CT_ContinuousTests;
   ppfCtAvailableTests[1] = CT_PacketErrorRate;
   ppfCtAvailableTests[2] = CT_RangeTest;
   ppfCtAvailableTests[3] = CT_TrimAdjust;
   ppfCtAvailableTests[4] = CT_PllNumOffsetAdjust;
#if defined(BOARD_LOCALIZATION_REVISION_SUPPORT) && (BOARD_LOCALIZATION_REVISION_SUPPORT > 0)
   ppfCtAvailableTests[5] = CT_RFSwitchControl;
#endif /*BOARD_LOCALIZATION_REVISION_SUPPORT > 0*/

   /*register callbacks for the generic fsk LL */
   GENFSK_RegisterCallbacks(mAppGenfskId,
                            App_GenFskReceiveCallback,
                            App_GenFskEventNotificationCallback);

   /*init and provide means to notify the app thread from connectivity tests*/
   return CT_GenFskInit(App_NotifyAppThread, App_TimerCallback);
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK receive callback.
*         This function is called each time the Generic FSK Link Layer receives a
*         valid packet
* \param[in]  pBuffer Pointer to receive buffer as byte array
* \param[in]  timestamp Generic FSK timestamp for received packet
* \param[in]  rssi The RSSI measured during the reception of the packet
*
********************************************************************************** */
static void App_GenFskReceiveCallback(uint8_t *pBuffer,
                                      uint16_t bufferLength,
                                      uint64_t timestamp,
                                      int8_t rssi,
                                      uint8_t crcValid)
{
   mAppRxLatestPacket.pBuffer      = pBuffer;
   mAppRxLatestPacket.bufferLength = bufferLength;
   mAppRxLatestPacket.timestamp    = timestamp;
   mAppRxLatestPacket.rssi         = rssi;
   mAppRxLatestPacket.crcValid     = crcValid;

   /*send event to app thread*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
   (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtRxDone_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
   OSA_EventSet(mAppThreadEvt, gCtEvtRxDone_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK event notification callback.
*         This function is called each time the Generic FSK Link Layer has
*         a notification for the upper layer
* \param[in]  event The event that generated the notification
* \param[in]  eventStatus status of the event
*
********************************************************************************** */
static void App_GenFskEventNotificationCallback(genfskEvent_t event,
                                                genfskEventStatus_t eventStatus)
{
   if(event & gGenfskTxEvent)
   {
       mAppGenfskStatus = eventStatus;
       /*send event done*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
       (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtTxDone_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        OSA_EventSet(mAppThreadEvt, gCtEvtTxDone_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
   }
   if(event & gGenfskRxEvent)
   {
       if(eventStatus == gGenfskTimeout)
       {
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
           (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtSeqTimeout_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
           OSA_EventSet(mAppThreadEvt, gCtEvtSeqTimeout_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
       }
       else
       {
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
           (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtRxFailed_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
           OSA_EventSet(mAppThreadEvt, gCtEvtRxFailed_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
       }
   }
#if (cPWR_UsePowerDownMode)
   if(event & gGenfskWakeEvent)
   {
       PWR_DisallowDeviceToSleep();
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
       (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtWakeUp_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
       OSA_EventSet(mAppThreadEvt, gCtEvtWakeUp_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
   }
#endif /* (cPWR_UsePowerDownMode) */
   /*not handling other events in this application*/
}

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static void App_SerialCallback(void* param,
                               serial_manager_callback_message_t *message,
                               serial_manager_status_t status )
{
    mRxBufferBytesAvailable = message->length;
    (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtUart_c);
}
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static void App_SerialCallback(void* param)
{
    OSA_EventSet(mAppThreadEvt, gCtEvtUart_c);
}
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

static void App_NotifyAppThread(void)
{
    App_NotifySelf();
}
static void App_TimerCallback(void* param)
{
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
    (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtTimerExpired_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
    OSA_EventSet(mAppThreadEvt, gCtEvtTimerExpired_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
}
/*! *********************************************************************************
* \brief  Prints the test parameters: mode, channel, TX power and payload length.
*         Some or all of the above parameters are used throughout the tests.
********************************************************************************** */
static void App_PrintTestParameters(bool_t bEraseLine)
{
   PrintTestParams(gaConfigParams, bEraseLine, mAppSerId);
}

#if (cPWR_UsePowerDownMode)
static void App_Idle(void)
{

    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        /* Enter Low Power */
        PWR_EnterLowPower();

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
        /* Woke up on Keyboard Press */
        if(PWRLib_MCU_WakeupReason.Bits.FromKeyBoard)
        {
            KBD_SwitchPressedOnWakeUp();
            PWR_DisallowDeviceToSleep();
        }
#endif
    }
    else
    {
        /* Enter MCU Sleep */
        PWR_EnterSleep();
    }
}
#endif /* cPWR_UsePowerDownMode */

#if (mAppIdleHook_c)
void vApplicationIdleHook(void)
{
#if (cPWR_UsePowerDownMode)
    App_Idle();
#endif
}
#else /* mAppIdleHook_c */
#if (cPWR_UsePowerDownMode)
static void App_Idle_Task(osaTaskParam_t argument)
{
    while(1)
    {
        App_Idle();

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

static osaStatus_t AppIdle_TaskInit(void)
{
    if(gAppIdleTaskId)
    {
        return osaStatus_Error;
    }

    /* Task creation */
    gAppIdleTaskId = OSA_TaskCreate(OSA_TASK(App_Idle_Task), NULL);

    if( NULL == gAppIdleTaskId )
    {
        assert(0);
        return osaStatus_Error;
    }

    return osaStatus_Success;
}
#endif /* cPWR_UsePowerDownMode */
#endif /* mAppIdleHook_c */

