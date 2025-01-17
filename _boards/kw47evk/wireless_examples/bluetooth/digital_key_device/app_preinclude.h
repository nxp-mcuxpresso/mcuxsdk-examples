/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*!
 *  Application specific configuration file only
 *  Board Specific Configuration shall be added to board.h file directly such as :
 *  - Number of button on the board,
 *  - Number of LEDs,
 *  - etc...
 */
/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
/* Number of Button required by the application */
#define gAppButtonCnt_c                 1

/* Number of LED required by the application */
#define gAppLedCnt_c                    2

/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d           0

/*! *********************************************************************************
 *     App Configuration
 ********************************************************************************** */
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                1

/*! Repeated Attempts - Mitigation for pairing attacks */
#define gRepeatedAttempts_d             0

#define gPasskeyValue_c                 999999

#define gMaxServicesCount_d             6

/*! Set the Tx power in dBm */
#define mAdvertisingDefaultTxPower_c    0

#define gAppMaxConnections_c            2U

/* Must open an L2CAP channel for each CCC peer */
#define gL2caMaxLeCbChannels_c          gAppMaxConnections_c

/*! BLE CCC Digital Key UUID */
#define gBleSig_CCC_DK_UUID_d           0xFFF5U

#define gAppUseShellInApplication_d     1

#define gAppLowpowerEnabled_d           0
/* Disable LEDs when enabling low power */
#if (defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d>0))
  #undef gAppLedCnt_c
  #define gAppLedCnt_c                    0
#endif
/*! *********************************************************************************
 *     CCC Configuration
 ********************************************************************************** */
#define gcScanWindowCCC_c               154U /* 96 ms */
#define gcScanIntervalCCC_c             880U /* 550 ms */
#define gcConnectionIntervalCCC_c       24U /* interval = gcConnectionIntervalCCC_c * 1.25ms */

#define gDKMessageMaxLength_c           (255U)
#define mAppLeCbInitialCredits_c        (32768U)

#define gDummyPayload_c       {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}

#define gDummyPayloadLength_c           16

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
#define MinimalHeapSize_c               14000

#define SHELL_BUFFER_SIZE               (128U)
#define SHELL_TASK_STACK_SIZE           (1200U)
#define gMainThreadStackSize_c          (1024U)
#define TM_TASK_STACK_SIZE              (512U)
#define BUTTON_TASK_STACK_SIZE          (512U)
#define SERIAL_MANAGER_TASK_STACK_SIZE  (512U)
#define gHost_TaskStackSize_c           (1600U)
/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */
 /* Enable Serial Manager interface */
#define gAppUseSerialManager_c                1

/* Enable 5.0 optional features */
#define gBLE50_d                              1

/* Enable 5.1 optional features */
#define gBLE51_d                              1

/* Enable 5.2 optional features */
#define gBLE52_d                              1

/* Enable EATT */
#define gEATT_d                               1

/* Disable GATT caching */
#define gGattCaching_d                        0

/* Disable GATT automatic robust caching */
#define gGattAutomaticRobustCachingSupport_d  0

/* See documentation on how to enable DBAF */
#define gBLE60_DecisionBasedAdvertisingFilteringSupport_d FALSE
/*! *********************************************************************************
 *  Auto Configuration
 ********************************************************************************** */

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
#define gAppExtAdvEnable_d                      1
#define gLlScanPeriodicAdvertiserListSize_c     8

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
