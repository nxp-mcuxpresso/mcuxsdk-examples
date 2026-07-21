/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "iec60730b.h"
#include "iec60730b_core.h"
#include "safety_test_items.h"
#include "project_setup_frdmmcxn947t.h"

#include "safety_cm33_mcx.h"

#include "MCXN947T_cm33_core0.h"

#include "clock_config.h"
#include "pin_mux.h"

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
#define PC_TEST_ENABLED       0
#define TSI_TEST_ENABLED      0
#define WATCHDOG_ENABLED      0
#define FMSTR_SERIAL_ENABLE   1

/********* Clock *********/
#define REF_TIMER_USED            CTIMER4
#define USED_CLOCK_SOURCE         kFRO12M_to_CTIMER4
#define CLOCK_DIVIDER             kCLOCK_DivCtimer4Clk
#define CLOCK_DIVIDE_VALUE        1u
#define REF_TIMER_CLOCK_FREQUENCY 12e06

#define SYSTICK_RELOAD_VALUE      150000
#define ISR_FREQUENCY             1000 /* Hz */
#define CLOCK_TEST_TOLERANCE        20U /* % */
/*********  Clock END *********/

/********* Watchdog *********/
#define USED_WDOG             WWDT0
#define WDOG_REF_TIMER_BASE   CTIMER4
#define RESET_DETECT_REGISTER &(CMC0->SRS)
#define RESET_DETECT_MASK     CMC_SRS_WWDT0_MASK

#define Watchdog_refresh \
    USED_WDOG->FEED = 0xAA;   \
    USED_WDOG->FEED = 0x55

#define ENDLESS_LOOP_ENABLE         1 /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT 1000
#define WATCHDOG_REFRESH_RATIO       1
#define WATCHDOG_TIMEOUT_VALUE       100000 /* 4ms refresh period (1000 / 250kHz) */
#define WD_REF_TIMER_CLOCK_FREQUENCY 12e06
#define WATCHDOG_CLOCK               250000
#define WD_TEST_TOLERANCE   20 /* % */

#define WD_RUN_TEST_CONDITION                                                                                  \
    (CMC_SRS_WAKEUP_MASK | CMC_SRS_POR_MASK | CMC_SRS_VD_MASK | \
     CMC_SRS_FATAL_MASK | CMC_SRS_PIN_MASK | CMC_SRS_DAP_MASK | CMC_SRS_RSTACK_MASK | \
     CMC_SRS_LPACK_MASK | CMC_SRS_SCG_MASK | CMC_SRS_SW_MASK | CMC_SRS_LOCKUP_MASK |\
     CMC_SRS_CDOG0_MASK | CMC_SRS_JTAG_MASK)
#define WD_CHECK_TEST_CONDITION CMC_SRS_WWDT0_MASK
/********* Watchdog END *********/

/********* GPIO macros *********/
/* GPIO macros */
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define DIO_USES_RPGIO 1 /* If RGPIO peripheral is used for DIO tests - set 1 */

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0

/* DIO test */
#define DIO_WAIT_CYCLE     100

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0

/********* GPIO macros END *********/

#define PC_TEST_PATTERN 0x04017FF0 /* Test address for Program counter test */

/********* Flashtest *********/
/* FLASH TEST MACROS */
#define HW_FLASH_TEST             1 /* Use HW = 1 SW = 0  flash TEST*/
#define FLASH_TEST_BLOCK_SIZE     0x20
#define CRC_BASE                  (FS_CRC_Type* )CRC0_BASE
#define USED_CRC                  CRC0
#define FLASH_TEST_CONDITION_SEED 0x0000 /* 0xFFFFFFFF CRC32, 0x0000 CRC16 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FLASH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFU)
#endif
/********* Flashtest END *********/

/********* RAM *********/
#define RAM_TEST_BLOCK_SIZE 0x4 /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* IAR + KEIL */
#define RAM_TEST_BACKUP_SIZE  0x20 /* must fit with the setup from linker configuration file */
#define STACK_TEST_BLOCK_SIZE 0x10 /* must fit with the setup from linker configuration file */
#endif
/********* RAM END *********/

#define STACK_TEST_PATTERN 0x77777777

/********* UART *********/
#define APPLICATION_SERIAL_BASE LPUART4
#define UART_BAUD_RATE 9600U
/********* UART END *********/

/********* ADC *********/
#define TESTED_ADC              ADC0
#define ADC_RESOLUTION          12
#define ADC_REFERENCE           3.3
#define ADC_BANDGAP_LEVEL       1.8 /* depends on power supply configuration */
#define ADC_DEVIATION_PERCENT   25


/********* ADC END *********/

#ifndef TSI
  #define TSI TSI0
#endif

#define TSI_CNT_MAX             (65535U)
#define TSI_DEVIATION_PERCENT   (50) /* Used for TSI typ. signal level comparison */
#define TSI_MIN_DELTA_PERCENT   (50) /* Used for Delta comparison for TSI input stimulated test */
#define TSI_CNT_LOW_LIMIT(val)  (((val) * (100 - TSI_DEVIATION_PERCENT)) / 100)
#define TSI_CNT_HIGH_LIMIT(val) (((val) * (100 + TSI_DEVIATION_PERCENT)) / 100)
#define TSI_DELTA(val, sval)    (((sval) - (val)))
#define TSI_COUNTER_LIMIT       10000 /* defined by user */

/* Used TSI channel  in SAFETY test */
#define TSI0_CH0  0
#define TSI0_CH1  1
#define TSI0_CH2  2
#define TSI0_CH3  3
#define TSI0_CH4  4
#define TSI0_CH5  5
#define TSI0_CH6  6
#define TSI0_CH7  7
#define TSI0_CH8  8
#define TSI0_CH9  9
#define TSI0_CH10 10
#define TSI0_CH11 11
#define TSI0_CH12 12
#define TSI0_CH13 13
#define TSI0_CH14 14
#define TSI0_CH15 15
#define TSI0_CH16 16
#define TSI0_CH17 17
#define TSI0_CH18 18
#define TSI0_CH19 19
#define TSI0_CH20 20
#define TSI0_CH21 21
#define TSI0_CH22 22
#define TSI0_CH23 23

/*Macro for  CLOCK init, this port are used by TSI and clock must be enable */
#define TSI_PORT_USE           \
    {                          \
        PORT0, PORT1, 0 \
    } // 0 = NULL it must be there as a termination

/* TSI channel limits for the Normal and Stimulated state (by internal pull device) */
#define TSI_VAL0   (8800U) /* Normal state */
#define TSI_SVAL0 (13100U)  /* Stimulated state */

#define TSI_VAL1   (9700U)
#define TSI_SVAL1 (15200U)

/*------------------------- TSI mutual section -------------------------------*/

/* TSI channel limits for the Normal and Stimulated state (by internal pull device) */
#define TSI_M_VAL0  (25800U) /* Normal state */
#define TSI_M_SVAL0 (30500U) /* Stimulated state */

#endif /* _SAFETY_CONFIG_H_ */
