/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ISI_EXAMPLE_H_
#define _ISI_EXAMPLE_H_

#include "fsl_common.h"

#define ISI_MIPI_CSI2 0
#define ISI_CI_PI     1

#define APP_CAMERA_CONTROL_FLAGS 0U

#define APP_MIPI_CSI_LANES       2U
#define APP_MIPI_CSI_VC          0U

#define APP_ISI          ((ISI_Type *)MEDIA__ISI_BASE)
#define APP_MIPI_CSI     CSI2_RX_BASE

/*
 * ISI input port 2 = MIPI_CSI -> REFORMATTER -> ISI.
 * RT2660 MEDIA_SS port mapping:
 *   0=DC0, 1=DC1, 2=MIPI_CSI, 3=reserved, 4=CI_PI, 5=memory.
 */
#define APP_MIPI_CSI_ISI_PORT    2U

#define APP_ISI_IRQn             MEDIA_ISI_IRQn
#define APP_ISI_IRQHandler       MEDIA_ISI_IRQHandler

#define APP_FB_ALIGN_BYTE        16U

/* -----------------------------------------------------------------------
 * OV5640 QVGA 30fps, 2 lanes:
 *   Bit rate per lane ~ (320*240*2*8*30) / 2 ~ 184 Mbit/s
 *   Use 500 Mbit/s row from Table 845 (conservative):
 *     Data lane: u_PRG_RXHS_SETTLE  = 8  -> T-HS-SETTLE  = 132 ns
 *     Clock lane: uc_PRG_RXHS_SETTLE = 16 -> T-CLK-SETTLE = 180 ns
 *
 * For 1080P 30fps, 2 lanes (~1 Gbit/s per lane):
 *     Data lane: u_PRG_RXHS_SETTLE  = 9  -> T-HS-SETTLE  = 126 ns
 *     Clock lane: uc_PRG_RXHS_SETTLE = 16 -> T-CLK-SETTLE = 180 ns
 * ----------------------------------------------------------------------- */
#define APP_MIPI_CSI_DATA_SETTLE 8U
#define APP_MIPI_CSI_CLK_SETTLE  16U

/*
 * FIFO flush count (RM Section 123.7.2.3).
 * Rule: use 7 when core_clk / byte_clk >= 10.
 *
 * core_clk = 333 MHz (PERI5/2)
 * byte_clk = bit_rate / 8:
 *   QVGA  30fps: 168/8 = 21 MHz  → ratio = 15.8 → use 7
 *   VGA   30fps: 168/8 = 21 MHz  → ratio = 15.8 → use 7
 *   720P  30fps: 504/8 = 63 MHz  → ratio =  5.3 → 3 sufficient
 *   1080P 30fps: 504/8 = 63 MHz  → ratio =  5.3 → 3 sufficient
 *
 * Use 7 conservatively to cover all resolutions safely.
 */
#define APP_MIPI_CSI_FLUSH_COUNT 7U

#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_PrepareCamera(void);

void BOARD_InitCameraInterface(void);

#if defined(__cplusplus)
}
#endif

#endif /* _ISI_EXAMPLE_H_ */
