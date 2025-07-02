/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#ifndef NULL
#define NULL 0
#endif

#include "MCXC444.h"
#include "iec60730b.h"
#include "iec60730b_core.h"
#include "safety_test_items.h"
#include "project_setup_frdmmcxc444.h"
#include "safety_cm0_kinetis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION   1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED      1
#define CLOCK_TEST_ENABLED    1
#define DIO_TEST_ENABLED      1
#define FLASH_TEST_ENABLED    1
#define RAM_TEST_ENABLED      1
#define PC_TEST_ENABLED       1
#define WATCHDOG_ENABLED      1
#define FMSTR_SERIAL_ENABLE   1

#define LPTMR_USED                LPTMR0
#define SYSTICK_RELOAD_VALUE      48000
#define ISR_FREQUENCY             1000 /* Hz */
#define CLOCK_TEST_TOLERANCE      20U /* % */
#define REF_TIMER_CLOCK_FREQUENCY 62500

/*Macros for different reference timer setings*/
#define CLOCK_TEST 1
#define WDOG_TEST  2

/*WDOG test */
#define WDOG_REF_TIMER_BASE   LPTMR0
#define USED_WDOG             SIM
#define RESET_DETECT_REGISTER &(RCM->SRS0)
#define RESET_DETECT_MASK     RCM_SRS0_WDOG_MASK
#define REFRESH_INDEX         FS_COP_WDOG
#define CLEAR_FLAG            0
#define REG_WIDE              FS_WDOG_SRS_WIDE_8b

#define WD_RUN_TEST_CONDITION_SRS0 RCM_SRS0_PIN_MASK | RCM_SRS0_POR_MASK | RCM_SRS0_WAKEUP_MASK | RCM_SRS0_LVD_MASK
#define WD_RUN_TEST_CONDITION_SRS1 RCM_SRS1_SW_MASK /* unsued */
#define WD_CHECK_TEST_CONDITION    RCM_SRS0_WDOG_MASK

#define Watchdog_refresh                   \
    SIM->SRVCOP = SIM_SRVCOP_SRVCOP(0x55); \
    SIM->SRVCOP = SIM_SRVCOP_SRVCOP(0xAA)

#define WD_REF_TIMER_CLOCK_FREQUENCY 62500 /*LPTMR has 62.5kHz clock for WD test */

#define ENDLESS_LOOP_ENABLE         1 /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT       1000
#define WATCHDOG_REFRESH_RATIO      10
#define WATCHDOG_TIMEOUT_VALUE      1024 /* set the appropriate watchdog timeout value */
#define WD_TEST_TOLERANCE           20 /* % */
#define LPO_FREQUENCY               1000 /* frequency of watchdog timer clock */
#define WATCHDOG_CLOCK              LPO_FREQUENCY

/* GPIO macros */
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define DIO_USES_RPGIO 0 /* If RGPIO peripheral is used for DIO tests - set 1 */

#define PIN_MUX_GPIO 1

/*Define of use SIM or PCC*/
#define USE_PCC 0

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0

/*Dio port settings*/
#define DIO_EXPECTED_VALUE 0
#define DIO_WAIT_CYCLE     75

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0

/* Program Counter TEST */
#define PC_TEST_PATTERN 0x20000000 /* test address for Program counter test (in RAM region) */

/* UART macros */
#define UART_USED       LPUART0
#define UART_BAUD_RATE 9600U

/*FLASH TEST MACROS*/
#define HW_FLASH_TEST         0    // HW FLASH test not supported on this device - must be disabled
#define FLASH_TEST_BLOCK_SIZE 0x20 // 0x100

#define FLASH_TEST_CONDITION_SEED 0x0000
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FALSH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFU)
#endif

#define RAM_TEST_BLOCK_SIZE 0x4 /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
#define RAM_TEST_BACKUP_SIZE 0x20 /* must fit with the setup from linker configuration file */

#define STACK_TEST_BLOCK_SIZE 0x10 /* must fit with the setup from linker configuration file */
#endif

#define STACK_TEST_PATTERN 0x77777777

/* ADC test settings */
#define TESTED_ADC ADC0 /*which ADC is use for AIO test*/

#define ADC_RESOLUTION        12
#define ADC_REFERENCE         3.06
#define ADC_BANDGAP_LEVEL     1.0 /* depends on power supply configuration */
#define ADC_DEVIATION_PERCENT 50


#endif /* _SAFETY_CONFIG_H_ */
