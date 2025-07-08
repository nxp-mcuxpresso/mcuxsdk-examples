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
#include "project_setup_mcimx93evk.h"

#include "safety_cm33_imx.h"

#include "MIMX9352_cm33.h"

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
#define CLOCK_TEST_ENABLED    1
#define DIO_TEST_ENABLED      1
#define FLASH_TEST_ENABLED    1
#define RAM_TEST_ENABLED      1
#define PC_TEST_ENABLED       1
#define FMSTR_SERIAL_ENABLE   1

/********* Clock *********/
#define REF_TIMER_USED                  LPTMR1
#define SYSTICK_RELOAD_VALUE            200000
#define ISR_FREQUENCY                   1000 /* Hz */
#define CLOCK_TEST_TOLERANCE            20U /* % */
#define LPTMR_CLOCK_FREQUENCY           16384
#define REF_TIMER_CLOCK_FREQUENCY       LPTMR_CLOCK_FREQUENCY
/*********  Clock END *********/

#define PC_TEST_PATTERN 0x20011000 /* Test address for Program counter test */

/********* Flashtest *********/
/* FLASH TEST MACROS */
#define HW_FLASH_TEST             0 /* Use HW = 1 SW = 0  flash TEST*/
#define FLASH_TEST_BLOCK_SIZE     0x20
#define CRC_BASE                  (FS_CRC_Type* )0
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

/********* DIO *********/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define DIO_USES_RPGIO 1 /* If RGPIO peripheral is used for DIO tests - set 1 */

#define DIO_WAIT_CYCLE 1000

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0
/********* DIO END *********/

/********* UART *********/
#define APPLICATION_SERIAL_BASE LPUART2
#define UART_BAUD_RATE 9600U
/********* UART END *********/

#endif /* _SAFETY_CONFIG_H_ */
