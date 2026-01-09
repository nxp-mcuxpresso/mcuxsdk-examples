/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*
 * Copyright 2015 Freescale
 * Copyright 2016-2017,2019,2022-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
/*! Enable Debug Console (PRINTF) */
#define gDebugConsoleEnable_d 1

/*! Enable Time Service */
#define gAppUseTimeService_d 1

/*! *********************************************************************************
 *     App Configuration
 ********************************************************************************** */
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d 1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d 1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d 1

/*! Set Passkey value */
#define gPasskeyValue_c 999999

/*! Repeated Attempts - Mitigation for pairing attacks */
#define gRepeatedAttempts_d 0

/*! NVM TABLE is stored in RAM */
#define gNvTableKeptInRam_d TRUE

/*! We want printing in the panic function. */
#define PANIC_ENABLE_LOG 1

/*! Enable/Disable low power feature */
#define ENABLE_LOW_POWER 1

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d 1

/*! Enable or disable use of mutex for use TRNG */
#define gRngUseMutex_c 0

/*! The minimum heap size needed:
    5 blocks of 56
    3 blocks of 104
    1 block of 248
    1 block of 312
    1 block of 392
*/
#define MinimalHeapSize_c 2144

/* Defines number of timers needed by the application */
#if gRepeatedAttempts_d
/*! Set gTmrApplicationTimers_c to 6. */
#define gTmrApplicationTimers_c 6
#else
/*! Set gTmrApplicationTimers_c to 5. */
#define gTmrApplicationTimers_c 5
#endif /* gRepeatedAttempts_d */

/*! *********************************************************************************
 *   NVM Module Configuration - gAppUseNvm_d shall be defined above as 1 or 0
 ********************************************************************************** */

#if gAppUseNvm_d
/* configure NVM module */
#define gNvStorageIncluded_d (1)
/*! Set to 1 to enable fragmentation */
#define gNvFragmentation_Enabled_d (1)
/*! Set to 1 to enable flex NVM */
#define gNvUseFlexNVM_d (0)
#if gNvUseFlexNVM_d
#define gUnmirroredFeatureSet_d (0)
#else
/*! Set to 1 to unmirror feature set */
#define gUnmirroredFeatureSet_d (1)
#endif /* gNvUseFlexNVM_d */
#endif /* gAppUseNvm_d */

/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines the RTOS used */

/*! Defines total heap size used by the OS - 11k */
#define gTotalHeapSize_c 0x5800

/*! Overwrite FreeRTOSConfig (needed by mcxw23 power manager) */
#define configUSE_TICKLESS_IDLE 1

/*! Overwrite FreeRTOSConfig */
#define configMINIMAL_STACK_SIZE ((unsigned short)260)
/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */

/*! Defines the max count of services. */
#define gMaxServicesCount_d 6
/*! Defines the max count of services characters. */
#define gMaxServiceCharCount_d 6

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */

/* Define the max tx power setting in dBm. Allowed values 0, 2 or 6 */
#if !defined(gAppMaxTxPowerDbm_c)
#define gAppMaxTxPowerDbm_c 0
#endif /* !defined(gAppMaxTxPowerDbm_c) */

#if gAppMaxTxPowerDbm_c > 6
#error "gAppMaxTxPowerDbm_c 6dBm is the maximum supported"
#endif

#define gConnDefaultTxPhySettings_c             (gLePhy1MFlag_c | gLePhy2MFlag_c | gLePhyCodedFlag_c)
#define gConnDefaultRxPhySettings_c             (gLePhy1MFlag_c | gLePhy2MFlag_c | gLePhyCodedFlag_c)

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
