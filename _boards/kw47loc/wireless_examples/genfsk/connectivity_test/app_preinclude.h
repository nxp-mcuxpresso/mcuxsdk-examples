/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*
* Copyright 2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 * 	Application Configuration
 ********************************************************************************** */
 
/*! *********************************************************************************
 * 	Drivers Configuration
 ********************************************************************************** */
/* Number of Button required by the application */
#define gAppButtonCnt_c         0

 /* Defines the number of available tsi electrodes for this application */
#define gKBD_TsiElectdCount_c   0

/* Number of LED required by the application */
#define gAppLedCnt_c            2
#define gBoardLedBlue_d         1
#define gBoardLedGreen_d        1
#define BOARD_LOCALIZATION_REVISION_SUPPORT 1

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/* Enable Generic FSK protocol in XCVR */
#define XCVR_GENFSK_ENABLED     1

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
/* Enable Serial Manager interface */
#define gAppUseSerialManager_c          1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  384

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define PoolsDetails_c \
         _block_set_(  32  ,    6 , 0) _eol_  \
         _block_set_(  64  ,    3 , 0) _eol_  \
         _block_set_( 128  ,    4 , 0) _eol_  \
         _block_set_( 512  ,    4 , 0) _eol_

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           0

/* Enable/Disable GENFSK Link Layer DSM */
#define cPWR_GENFSK_LL_Enable           1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              7

/* Disable all pins when entering Low Power */
#define APP_DISABLE_PINS_IN_LOW_POWER   1

/* Default deep sleep duration in ms */
#define cPWR_DeepSleepDurationMs        30000

/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 0

/* Default DCDC Mode used by the application */
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c

/* Default DCDC Battery Level Monitor interval */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  600000

/* The default max TX power is 10dBm: allowed value 0, 7, 10  */
#define gAppMaxTxPowerDbm_c             10U

/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        4

/* Defines number of OS semaphores used */
#define osNumberOfSemaphores    1

/*! *********************************************************************************
 * 	Auto Configuration
 ********************************************************************************** */
/* Disable LEDs when enabling low power */
#if cPWR_UsePowerDownMode || gMWS_UseCoexistence_d
#define gLEDSupported_d 0
#endif

#if gMWS_UseCoexistence_d
#undef gKBD_KeysCount_c
#define gKBD_KeysCount_c        1
#endif

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
