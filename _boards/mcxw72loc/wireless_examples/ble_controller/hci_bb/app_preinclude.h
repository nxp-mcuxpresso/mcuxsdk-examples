/*
 * Copyright 2024 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_


/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
/* Number of Button required by the application */
#define gAppButtonCnt_c         0

/* Number of LED required by the application */
#define gAppLedCnt_c            0

#define gDebugConsoleEnable_d   0

#define gAppHighSystemClockFrequency_d 1

/* To Use 3M baudrate on UART 0 with flow control */
#define gBoardUse3MbOnUart0_d   0

/* To use 460800 baudrate on UART 1 without flow control, increase SM Ring buffer size */
// #define BOARD_APP_UART_BAUDRATE 460800
// #define SERIAL_MANAGER_RING_BUFFER_SIZE (1024U)

#define BOARD_LOCALIZATION_REVISION_SUPPORT 1

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */

/* hybrid genfsk enablement */
#define gAppEnableHybridGenfsk_d   1

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* Do not modify. Not used for this application */
#define gAppUseNvm_d                    0

/* Enable Serial Manager interface */
#define gAppUseSerialManager_c          1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c             384

/* Enable debug throught SWO trace */
#define gDbg_SwoEnabled_d       0

#if (defined(gDbg_SwoEnabled_d) && (gDbg_SwoEnabled_d == 1))
 /* Define to 1 if you want to configure the DWT/ITM/TPIU-SWO via SW i.s.o.the probe */
#define DBG_SWO_INIT_VIA_SW 1
/* Define to 1 to route SWO signal on GPIO */
/* Setting shall be done on Main Application Core PPB */
#define DBG_SWO_PIN_ENABLE 1
/* CoreSight Funnel SWO route Muxing configuration    */
/* Setting shall be done on Main Application Core PPB */
#define DBG_SWO_CORE_MAIN_CORE 1 // Main Core
#define DBG_SWO_CORE_NBU_CORE  2 // Nbu Core
#define DBG_SWO_FUNNEL_MUXING DBG_SWO_CORE_NBU_CORE
#endif /* gDbg_SwoEnabled_d == 1 */

#if !defined(__MCUXPRESSO)
#define HCIBB_DBG_NBU_ENABLE    1
#endif

/*
 * TimerManager Configuration
 */
#define FSL_OSA_BM_TIMER_CONFIG         FSL_OSA_BM_TIMER_SYSTICK

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */


/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/

/* Define as 1 in order to configure FEM to use external PA.
   FEM in NBU is activated by API function PLATFORM_InitFEM.
   Remark: FEM may conflict with Localisation/Coex features in term of RF_GPO use. */
//#define gAppConfigureFEM                       1

/* Define as 1 when use with an external WIFI chip offering Coexistence control to BLE.
   Coexistence in NBU is activated by API function PLATFORM_InitCOEX.
   Remark: Coexistence may conflict with Localisation/FEM feature in term of RF_GPO use. */
//#define gAppConfigureCOEX                      1

/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
/* enable periodic advertiser list */
#define gAppExtAdvEnable_d                   1
#define gLlScanPeriodicAdvertiserListSize_c (8U)
/* disable autonomous feature exchange */
#define gL1AutonomousFeatureExchange_d 0

#define RL_BUFFER_COUNT (4U)

/*By default DTM through HCI interface is used (gAppUseDtm2Wire not defined).
    If DTM 2-wire interface is prefered, please define gAppUseDtm2Wire.*/
/*#define gAppUseDtm2Wire                      1*/

#define gEnableCoverage                        0
#if (defined(gEnableCoverage) && (gEnableCoverage == 1))
#undef gDebugConsoleEnable_d
#define gDebugConsoleEnable_d 1
#endif

/* define the max tx power setting, allowed value 0, 7 or 10 */
#define gAppMaxTxPowerDbm_c                    10U

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */