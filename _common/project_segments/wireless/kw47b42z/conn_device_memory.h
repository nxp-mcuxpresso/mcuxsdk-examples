/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file conn_device_memory.h
 * \brief defines memory segments for KW47B42Zxx wireless device to be used in connectivity linker files
 *
 */

#ifndef _CONN_DEVICE_MEMORY_H_
#define _CONN_DEVICE_MEMORY_H_

#if defined(KW47B42ZB7_cm33_core0_SERIES) || defined(KW47B42ZB6_cm33_core0_SERIES) ||\
    defined(KW47B42ZB3_cm33_core0_SERIES) || defined(KW47B42ZB2_cm33_core0_SERIES)

/* 2M flash */
#define FSL_DEVICE_FLASH_START          0x0
#define FSL_DEVICE_FLASH_SIZE           0x200000
#define FSL_DEVICE_FLASH_END            (FSL_DEVICE_FLASH_START + FSL_DEVICE_FLASH_SIZE - 1)
#define FSL_DEVICE_FLASH_SECTOR_SIZE    0x2000
#define FSL_DEVICE_VECT_TABLE_SIZE      0x400

/* 264K RAM */
/* RAM0 assembles CTCM0 and CTCM1 */
#define FSL_DEVICE_RAM0_START  0x04000000
#define FSL_DEVICE_RAM0_SIZE   0x8000
#define FSL_DEVICE_RAM0_END    (FSL_DEVICE_RAM0_START + FSL_DEVICE_RAM0_SIZE - 1)

/* RAM1 assembles STCM0 to STCM8 */
#define FSL_DEVICE_RAM1_START  0x20000000
#define FSL_DEVICE_RAM1_SIZE   0x3A000
#define FSL_DEVICE_RAM1_END    (FSL_DEVICE_RAM1_START + FSL_DEVICE_RAM1_SIZE - 1)

#elif defined(KW47B42Z83_cm33_core0_SERIES) || defined(KW47B42Z96_cm33_core0_SERIES) ||\
      defined(KW47B42Z97_cm33_core0_SERIES)

/* 1M flash */
#define FSL_DEVICE_FLASH_START          0x0
#define FSL_DEVICE_FLASH_SIZE           0x100000
#define FSL_DEVICE_FLASH_END            (FSL_DEVICE_FLASH_START + FSL_DEVICE_FLASH_SIZE - 1)
#define FSL_DEVICE_FLASH_SECTOR_SIZE    0x2000
#define FSL_DEVICE_VECT_TABLE_SIZE      0x400

#if defined(KW47B42Z83_cm33_core0_SERIES)
/* 136K RAM */
/* RAM0 assembles CTCM0 and CTCM1 */
#define FSL_DEVICE_RAM0_START  0x04000000
#define FSL_DEVICE_RAM0_SIZE   0x8000
#define FSL_DEVICE_RAM0_END    (FSL_DEVICE_RAM0_START + FSL_DEVICE_RAM0_SIZE - 1)

/* RAM1 assembles STCM0 to STCM3 */
#define FSL_DEVICE_RAM1_START  0x20000000
#define FSL_DEVICE_RAM1_SIZE   0x18000
#define FSL_DEVICE_RAM1_END    (FSL_DEVICE_RAM1_START + FSL_DEVICE_RAM1_SIZE - 1)

/* Define RAM2 as STCM8 because STCM8 is not contiguous */
#define FSL_DEVICE_RAM2_START  0x20038000
#define FSL_DEVICE_RAM2_SIZE   0x2000
#define FSL_DEVICE_RAM2_END    (FSL_DEVICE_RAM2_START + FSL_DEVICE_RAM2_SIZE - 1)

#else /* KW47B42Z83_cm33_core0_SERIES */

/* 264K RAM */
/* RAM0 assembles CTCM0 and CTCM1 */
#define FSL_DEVICE_RAM0_START  0x04000000
#define FSL_DEVICE_RAM0_SIZE   0x8000
#define FSL_DEVICE_RAM0_END    (FSL_DEVICE_RAM0_START + FSL_DEVICE_RAM0_SIZE - 1)

/* RAM1 assembles STCM0 to STCM8 */
#define FSL_DEVICE_RAM1_START  0x20000000
#define FSL_DEVICE_RAM1_SIZE   0x3A000
#define FSL_DEVICE_RAM1_END    (FSL_DEVICE_RAM1_START + FSL_DEVICE_RAM1_SIZE - 1)

#endif /* KW47B42Z83_cm33_core0_SERIES */

#else
#error "Unsupported device for the selected connectivity device memory header"
#endif

#endif /* _CONN_DEVICE_MEMORY_H_ */
