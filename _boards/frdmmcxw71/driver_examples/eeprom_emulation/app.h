/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DEMO_FMC FMC

#define EEPROM_SIZE     1024
#define DATA_VALUE_SIZE 32

#define ECC_SIZE         0x00u
#define RETRY_MAX        0x02u
#define MAX_REPEAT_TIMES 0x05u

#if (DATA_VALUE_SIZE % 4)
#define SOURCE_BUF_SIZE (DATA_VALUE_SIZE / 4 + 1)
#else
#define SOURCE_BUF_SIZE (DATA_VALUE_SIZE / 4)
#endif

#if ((EEPROM_SIZE % DATA_VALUE_SIZE) != 0x00u)
#define MAX_RECORD_NUMBER (EEPROM_SIZE / DATA_VALUE_SIZE + 0x01u)
#else
#define MAX_RECORD_NUMBER (EEPROM_SIZE / DATA_VALUE_SIZE)
#endif

#define FLASH_START_ADDR  0x00000000u /* base address of flash module in byte address space */
#define FLASH_SECTOR_SIZE 0x00002000u /* 8KB for K4W1_TV */
#define SECTOR_SIZE                                                                                            \
    FLASH_SECTOR_SIZE /* Sector size of Eeprom emulation in bytes. It can be one or multiply consecutive flash \
             sectors */
#define START_ADDRESS (FLASH_START_ADDR + 5 * FLASH_SECTOR_SIZE) /* Start address of Eeprom emulation in byte */

#define EXTRA_READY_SECTORS      2
#define EXTRA_ACTIVE_SECTORS     2
#define ACTUAL_READY_SECTORS     2
#define MIN_ACTUAL_READY_SECTORS 2

#define FTFx_PGMCHECK_SIZE 0x04u
#define FTFx_PGM_SIZE      FTFx_DPHRASE_SIZE
#define FTFx_RD1SEC_SIZE   0x10u
#define FTFx_USER_MARGIN   0x01u

/*******************************************************************************
 * Other configuration
 *******************************************************************************/
/* Pre-defined macro for CW10 compiler */
#if (defined __MWERKS__)
#define EnableInterrupts asm(" CPSIE i ");
/* Pre-defined macro for Keil, IAR and GCC ARM compiler */
#elif ((defined __ARMCC_VERSION) || (defined __ICCARM__) || (defined __GNUC__))
#define EnableInterrupts __asm("cpsie i");
#endif

/* Pre-defined macro for Keil */
#ifdef __ARMCC_VERSION
#define __VECTOR_RAM 0x1FFF0000
#endif

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
