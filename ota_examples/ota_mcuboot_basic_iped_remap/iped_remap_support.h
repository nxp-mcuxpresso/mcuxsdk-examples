/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __IPED_REMAP_SUPPORT_H__
#define __IPED_REMAP_SUPPORT_H__

#include <stdint.h>
#include <stdlib.h>
#include "mflash_drv.h"


/*
 * Size of write buffer used for overwrite-only mode has to be adjusted if IPED 
 * encryption unit is used so that the size of data chunks written to flash are 
 * always a multiple of 4 pages in size.
 */
#define IPED_BUF_SIZE (4*MFLASH_PAGE_SIZE)

// Initialize the buffer
status_t iped_ota_init(void);

// Configures IPED metadata for selected slot (before OTA update)
status_t iped_ota_configure_slot(uint32_t slot);

// Add data to buffer and process complete chunks
status_t iped_ota_process_data(const uint8_t *data, size_t len, uint32_t addr);

// Flush remaining data (call this when done receiving data)
status_t iped_ota_finish(void);

// Initialize IPED for selected region (this should be called only from mcuboot context)
status_t iped_boot_initialiaze_slot(uint32_t slot);

uint32_t iped_ota_get_region_max_size(uint32_t size);

#endif // __IPED_REMAP_SUPPORT_H__
