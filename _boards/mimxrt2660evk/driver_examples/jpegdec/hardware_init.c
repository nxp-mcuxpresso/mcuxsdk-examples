/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${variable:start}*/
JPEG_DECODER_Type g_appJpegDec = {
    .core    = MEDIA__JPEGDEC,
    .wrapper = MEDIA__JPGDECWRP,
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();

#if (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
    BOARD_InitDcifDpiPins();
#elif (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    BOARD_InitDcifDbiPins();
#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091)
    BOARD_InitMIPIPanelPins();
#endif

    /* Enable jpegdec clock gate. */
    CLOCK_EnableClock(kCLOCK_MEDIA_jpeg_decoder);

    /* Pulse the jpegdec peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_JPEG_DECODER);
}
/*${function:end}*/
