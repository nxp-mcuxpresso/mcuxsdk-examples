/*! *********************************************************************************
 * \addtogroup Main
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2022-2024, 2026 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the main entry point for a bare-metal application.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
#include "fwk_platform_definitions.h"
#include "app.h"
#include "fsl_os_abstraction.h"
#include "fwk_platform_ble.h"
#if (NXP_RADIO_GEN >= 470)
#include "fwk_platform_lcl.h"
#endif

#include "fwk_platform.h"
#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
#include "PWR_Interface.h"
#endif

#if (defined(DBG_SWO_INIT_VIA_SW) && (DBG_SWO_INIT_VIA_SW == 1))
#include "fwk_debug_swo.h"
#endif

#if defined(gAppUseSensors_d) && (gAppUseSensors_d > 0)
#include "sensors.h"
#endif

#define USE_OSA_API_IN_MAIN     0

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if USE_OSA_API_IN_MAIN
static OSA_TASK_HANDLE_DEFINE(s_startTaskHandle);
#endif

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
extern void main_task(uint32_t param);

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if USE_OSA_API_IN_MAIN
void start_task(void *argument)
{
    main_task((uint32_t)argument);
}

static OSA_TASK_DEFINE(start_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0);
#endif

#if (defined(DBG_SWO_INIT_VIA_SW) && (DBG_SWO_INIT_VIA_SW == 1))
void init_debug_swo(void) {
   /* SWO configuration for debug purposes on Main core*/
   trace_swo_config_t swo_cfg;
   swo_cfg.baudRate    = 6000000;          // MCU-Link maximum SWO baudrate supported 6MHz
   swo_cfg.itmPort     = 0;                // For now, we use ITM stimuli port 0
   swo_cfg.protocol    = kSwoProtocolNrz;  // Protocol UART NRZ for trace output from the TPIU
   swo_cfg.traceId     = 1;                // Used to identify trace source for multi core debug

   swo_cfg.clockRate = SystemCoreClock; // retrieve System CPU clock frequency
   //Initialise SWO cfg via SW
   DBG_SwInit_SWO(&swo_cfg);
}
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

int main(void)
{
    /* Init OSA: should be called before any other OSA API */
    OSA_Init();

    BOARD_InitHardware();

    APP_InitServices();

    if (APP_InitBle() != 0)
    {
      assert(0);
    }

#if (NXP_RADIO_GEN >= 470)
    PLATFORM_InitLcl();
#endif
#if (defined(gAppConfigureCOEX) && (gAppConfigureCOEX == 1))
    {
        /* Customized configuration: see default_COEX_config[] for details. Be careful ! */
//        uint8_t COEX_config[24U] = {
//            0x01,0x01,0x01,0x05,0x01, 0x01, 0x01,0x00,0x00,0x00,0x00, 0x04,0x01, 0x37,0x5D,0x5A, 0x00,0x00, 0x5A, 0x5A,0x5A,0x00, 0x01,0x01};
//        uint8_t COEX_status = PLATFORM_InitCOEX(COEX_config, sizeof(COEX_config));
        uint8_t COEX_status = PLATFORM_InitCOEX(NULL, 0U /*N/A*/);   /* use default configuration */
        assert(COEX_status==0U);
    }
#endif
#if (defined(gAppConfigureFEM) && (gAppConfigureFEM == 1))
    {
        /* Customized configuration: see default_FEM_config[] for details. Be careful ! */
//        uint8_t FEM_config[16U] = {
//            1U/*dual mode*/, 0U/*Disabled*/, 1U/*Enabled*/, 0U/*Disabled*/, 0U/*Disabled*/, 0U/*TSM GPIO*/, 0U/*TSM GPIO*/,
//            0U/*TSM GPIO*/, 0U/*TSM GPIO*/, 0U/*?*/, 0U/*MUST=0*/, 0U/*?*/, 0U/*MUST=0*/, 0U/*HIGH*/, 0U/*HIGH*/, 5U/*RF_GPO[11:8]*/};
//        uint8_t FEM_status = PLATFORM_InitFEM(FEM_config, sizeof(FEM_config));
        uint8_t FEM_status = PLATFORM_InitFEM(NULL, 0U /*N/A*/);   /* use default configuration */
        assert(FEM_status==0U);
    }
#endif

#if defined(gAppUseSensors_d) && (gAppUseSensors_d > 0)
    /* for periodic temperature measurement */
    SENSORS_TriggerPeriodicTemperatureMeasurement(1000U);
#endif

#if (defined(DBG_SWO_INIT_VIA_SW) && (DBG_SWO_INIT_VIA_SW == 1))
    /* Configure and initialize SWO on MAIN core*/
    init_debug_swo();
#endif

#if USE_OSA_API_IN_MAIN  /* keep using the OSA interface  */
    (void)OSA_TaskCreate((osa_task_handle_t)s_startTaskHandle, OSA_TASK(start_task), NULL);

    /*start scheduler*/
    OSA_Start();

#else  /* Example of baremetal loop if user doesn't want to use OSA API */

#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    /* OSA_Start() is not called in this implementation but nevertheless we need to start
      timers for the bare metal scheduler for delays, wait on timeout , etc.. */
    extern void OSA_TimeInit(void);
    OSA_TimeInit();
#endif

    while(TRUE)
    {
        OSA_ProcessTasks();

        main_task(0U);

        OSA_DisableIRQGlobal();

        /* Check if some connectivity tasks have turned to ready state from interrupts or
              if messages are to be processed in Application process */
        if ( OSA_TaskShouldYield() == FALSE )
        {
#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
            PWR_EnterLowPower();
#else
            __WFI();
#endif
        }

        OSA_EnableIRQGlobal();
    }
#endif

    /* Won't run here */
    assert(0);
    return 0;
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
