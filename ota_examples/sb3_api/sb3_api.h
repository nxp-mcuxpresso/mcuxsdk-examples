/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SB3_API_H_
#define _SB3_API_H_

#include "fsl_common.h"

typedef struct mbi_image_info
{
    uint32_t length;
    uint8_t  type;
    uint16_t img_version;
    uint32_t execaddr;
    uint32_t cert_offset;
    uint32_t cert_size;
    uint32_t fw_version;
} mci_image_info_t;

/********************************************************************************
 * IAP Helpes Functions
 *******************************************************************************/

/* tests if flah remapping is active */
int is_remap_active(void);

/* Tests presence of SB3 magic and parse SB3 file length if wanted */
int sb3_parse_header(const void *header, uint32_t *sb3_len);

/* tests presence of signed image */
int mbi_image_info_sanity_check(const uint32_t *image);

/* parses basic information about MBI image */
void parse_mbi_image_info(const uint32_t *image, struct mbi_image_info *info);

/* Return non-zero value if the device is provisioned */
int sb3_check_provisioning(void);

/* ROM IAP - initialization call */
status_t sb3_api_init(void);

/* ROM IAP - pump function used to sequentially pass SB3 data to ROM for processing */
status_t sb3_api_pump(uint8_t *data, size_t len);

/* ROM IAP - finall call after succesfull processing by pump function */
void sb3_api_finalize(void);

/* ROM IAP - used to free allocated memory*/
status_t sb3_api_deinit(void);

#endif // _SB3_API_H_
