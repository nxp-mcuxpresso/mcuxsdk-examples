/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*
 * Copyright 2021-2024 NXP
 *
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*!
 *  Application specific configuration file only
 *  Board Specific Configuration shall be added to board.h file directly such as :
 *  - Number of buttons on the board,
 *  - Number of LEDs,
 *  - etc...
 */
/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
/* Number of Buttons required by the application */
#define gAppButtonCnt_c                         1U

/* Number of LEDs required by the application */
#define gAppLedCnt_c                            2U

/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d                   0

/*! *********************************************************************************
 *     App Configuration
 ********************************************************************************** */
/*Enable Extended Advertising*/
#define gAppExtAdvEnable_d                      1

/*define the max number of connections this device is able to handle*/
#define gAppMaxConnections_c                    (1U)

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                        1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                        1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                        0

#define gPasskeyValue_c                         999999

/*! Set the Tx power in dBm */
#define mAdvertisingDefaultTxPower_c            0

/*! specifies whether the phy update procedure is going to be initiated in connection or not */
#define gConnInitiatePhyUpdateRequest_c         (0U)

/* Enable 5.0 optional features */
#define gBLE50_d                                1

/* Enable 5.1 optional features */
#define gBLE51_d                                1

/* Enable 5.2 optional features */
#define gBLE52_d                                1

/* Enable EATT */
#define gEATT_d                                 1

/* Number of maximum enhanced ATT bearers supported */
#define gAppEattMaxNoOfBearers_c                3

/* Number of LE_PSM supported */
#define gL2caMaxLePsmSupported_c                3

/* Number of credit-based channels supported */
#define gL2caMaxLeCbChannels_c                  3

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                     1

/* erase NVM pages at download( armgcc only) */
#if ((defined gAppUseNvm_d) && (gAppUseNvm_d != 0)) && defined(__GNUC__)
#define gNvmErasePartitionWhenFlashing_c 1U
#endif

/*! The minimum heap size needed (measured with MEM_STATISTICS) */
#define MinimalHeapSize_c                       11500

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */
#if defined(SDK_OS_FREE_RTOS)
/* The following stack sizes have been chosen based on a worst case scenario. 
 * For different compilers and optimization levels they can be reduced. */

#define gHost_TaskStackSize_c           1800

#define BUTTON_TASK_STACK_SIZE          600

#define SERIAL_MANAGER_TASK_STACK_SIZE  240

#define gMainThreadStackSize_c          864

#define TM_TASK_STACK_SIZE              480

#define gAppIdleTaskStackSize_c         600

/* The size used for the Idle task, in dwords. */
#define configMINIMAL_STACK_SIZE        150

#endif

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */
#define gMaxServicesCount_d                     6
#define gMaxServiceCharCount_d                  6

/* Enable Serial Manager interface */
#define gAppUseSerialManager_c                  1

/*! *********************************************************************************
 *  Auto Configuration
 ********************************************************************************** */

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */

/*
 * Specific configuration of LL pools by block size and number of blocks for this application.
 * Optimized using the MEM_OPTIMIZE_BUFFER_POOL feature in MemManager,
 * we find that the most optimized combination for LL buffers.
 *
 * If LlPoolsDetails_c is not defined, default LL buffer configuration in app_preinclude_common.h
 * will be applied.
 */

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
