/* -------------------------------------------------------------------------- */
/*                             Copyright 2025 NXP                             */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "clock_config.h"
#include "fsl_nor_flash.h"
#include "fsl_spifi_nor_flash.h"
#include "pin_mux.h"

/* -------------------------------------------------------------------------- */
/*                            Private memory declarations                     */
/* -------------------------------------------------------------------------- */

static void BOARD_SpifiClockInit(spifi_nor_clock_init_t param)
{
    /* Always use FRO 32MHz. */
}

static spifi_mem_nor_config_t MX25R6435FM2IL0Config =
{
    .clockInit = BOARD_SpifiClockInit,
    .cmd_format = kSPIFI_CommandAllSerial,
    .quad_mode_setting = kSerialNorQuadMode_StatusReg1_Bit6,
};

static nor_config_t norConfig = {
  .memControlConfig = &MX25R6435FM2IL0Config,
  .driverBaseAddr = SPIFI0,
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_InitExternalFlash(void)
{
    BOARD_InitBootClocks();
    /* Set SPIFI clock source */
    CLOCK_AttachClk(kFRO_HF_to_SPIFI);
    /* Set the clock divider */
    CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, 1U, false);

    BOARD_InitExtFlashPins();
}

nor_config_t * BOARD_GetExtFlashConfig(void)
{
    return &norConfig;
}
