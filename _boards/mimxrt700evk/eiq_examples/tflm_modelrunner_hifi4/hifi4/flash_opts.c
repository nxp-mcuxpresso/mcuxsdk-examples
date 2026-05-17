/*
* Copyright 2024 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include "flash_opts.h"
#include "fsl_debug_console.h"

status_t FlashInit(FlashConfig *config){
  /* XSPI init */
  	return kStatus_Success;
}

status_t FlashErase(FlashConfig *config, uint32_t start, uint32_t length){
         return kStatus_Success;
}

status_t FlashProgram(FlashConfig *config, uint32_t start, uint32_t *src, uint32_t length){
        return kStatus_Success ;
}
