/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "MCXE247.h"
#include "iec60730b.h"
#include "iec60730b_core.h"
#include "safety_test_items.h"
#include "project_setup_frdmmcxe247.h"
#include "safety_cm4_cm7_mcx.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *)0)
#endif
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION   1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED      0
#define CLOCK_TEST_ENABLED    0
#define DIO_TEST_ENABLED      0
#define FLASH_TEST_ENABLED    0
#define RAM_TEST_ENABLED      1
#define PC_TEST_ENABLED       1
#define WATCHDOG_ENABLED      0
#define FMSTR_SERIAL_ENABLE   1

/* CLock Test */
#define LPTMR_USED                LPTMR0
#define SYSTICK_RELOAD_VALUE      48000
#define ISR_FREQUENCY             1000 /* Hz */
#define CLOCK_TEST_TOLERANCE      20U /* % */
#define LPTMR_CLOCK_FREQUENCY     4000000
#define REF_TIMER_CLOCK_FREQUENCY LPTMR_CLOCK_FREQUENCY

/********* Watchdog *********/
#define WDOG_REF_TIMER_BASE   LPTMR0
#define USED_WDOG             WDOG
#define RESET_DETECT_REGISTER &(RCM->SRS)
#define RESET_DETECT_MASK     RCM_SRS_WDOG_MASK
#define CLEAR_FLAG            0
#define REFRESH_INDEX         FS_KINETIS_WDOG
#define REG_WIDE              FS_WDOG_SRS_WIDE_32b

#define WD_RUN_TEST_CONDITION_SRS0 RCM_SRS_LVD_MASK | RCM_SRS_LOC_MASK | RCM_SRS_LOL_MASK | RCM_SRS_PIN_MASK | RCM_SRS_POR_MASK | RCM_SRS_JTAG_MASK | RCM_SRS_LOCKUP_MASK | RCM_SRS_SW_MASK | RCM_SRS_MDM_AP_MASK | RCM_SRS_SACKERR_MASK
#define WD_CHECK_TEST_CONDITION    RCM_SRS_WDOG_MASK

#define Watchdog_refresh WDOG->CNT = 0xB480A602;

#define WD_REF_TIMER_CLOCK_FREQUENCY    REF_TIMER_CLOCK_FREQUENCY
#define ENDLESS_LOOP_ENABLE             1 /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT           1000
#define WATCHDOG_REFRESH_RATIO          1
#define WATCHDOG_TIMEOUT_VALUE          1024 /* set the appropriate watchdog timeout value */
#define LPO_FREQUENCY                   128000 /* frequency of watchdog timer clock */
#define WATCHDOG_CLOCK                  LPO_FREQUENCY
#define WD_TEST_TOLERANCE               20 /* % */
/********* Watchdog END *********/

/********* Flashtest *********/
/* FLASH TEST MACROS */
#define HW_FLASH_TEST             1 /* Use HW = 1 SW = 0  flash TEST*/
#define FLASH_TEST_BLOCK_SIZE     0x20
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF /* 0xFFFFFFFF CRC32, 0x0000 CRC16 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FLASH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFU)
#endif
/********* Flashtest END *********/


#define RAM_TEST_BLOCK_SIZE 0x4 /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
  #define RAM_TEST_BACKUP_SIZE 0x20 /* must fit with the setup from linker configuration file */
  #define STACK_TEST_BLOCK_SIZE 0x10 /* must fit with the setup from linker configuration file */
#endif

#define STACK_TEST_PATTERN 0x77777777

/********* ADC *********/
#define TESTED_ADC              ADC0
#define ADC_RESOLUTION          12
#define ADC_REFERENCE           3.3
#define ADC_BANDGAP_LEVEL       1.3 /* depends on power supply configuration */
#define ADC_DEVIATION_PERCENT   25
/********* ADC END *********/

/*Program Counter TEST*/
#define PC_TEST_PATTERN 0x20010000 /* test address for Program counter test (in RAM region) */

/********* DIO *********/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define DIO_USES_RPGIO 0 /* If RGPIO peripheral is used for DIO tests - set 1 */

#define PIN_MUX_GPIO 0x1U

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0

/*Dio port settings*/
#define DIO_WAIT_CYCLE 100

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0
/********* DIO END *********/

/********* UART *********/
#define UART_BAUD_RATE 9600U
#define APPLICATION_SERIAL_BASE (uint32_t)LPUART2
/********* UART END *********/

#endif /* _SAFETY_CONFIG_H_ */
