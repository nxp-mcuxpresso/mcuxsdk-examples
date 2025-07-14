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
#include "project_setup_frdmrw612.h"

#include "safety_cm33_RW61x.h"

#include "RW612.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_io_mux.h"

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
#define FLASH_TEST_ENABLED    1
#define RAM_TEST_ENABLED      1
#define PC_TEST_ENABLED       1
#define FMSTR_SERIAL_ENABLE   1

/********* Clock *********/
#define REF_TIMER_USED            CTIMER0
#define REF_TIMER_CLOCK_FREQUENCY 16e06

#define SYSTICK_FREQ 260000000 /* 260MHz */
#define SYSTICK_RELOAD_VALUE 260000 /* 260MHz/260kHz = 1ms period */
#define ISR_FREQUENCY (SYSTICK_FREQ / SYSTICK_RELOAD_VALUE)

#define CLOCK_TEST_TOLERANCE        20U /* % */
/*********  Clock END *********/

#define PC_TEST_PATTERN 0x20120000 /* Test address for Program counter test */

/********* Flashtest *********/
/* Hyper flash size */
#define HW_FLASH_TEST             1 /* Use HW = 1 SW = 0  flash TEST*/
#define FLASH_TEST_BLOCK_SIZE     0x20
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF /* 0xFFFFFFFF CRC32, 0x0000 CRC16 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FLASH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFFFFFUL)
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
#define APPLICATION_SERIAL_BASE USART3
#define UART_BAUD_RATE 9600U
/********* UART END *********/

#endif /* _SAFETY_CONFIG_H_ */
