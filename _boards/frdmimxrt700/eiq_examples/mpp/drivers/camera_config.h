/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CAMERA_CONFIG_H_
#define _CAMERA_CONFIG_H_

/*
 * FlexIO / EZH camera configuration for OV7670 on FRDM-IMXRT700.
 *
 * Pin mapping (same FlexIO instance as EVK):
 *   XCLK  : FLEXIO_D1  (TIMER1) — PIO2_1
 *   PCLK  : FLEXIO_D15 (TIMER0) — PIO2_15
 *   Data  : FLEXIO_D6..D13      — PIO2_2..PIO2_9
 *   XSYNC : GPIO2_5  (IRQ input)
 *   RESET : GPIO2_0  (output)
 */

/* FlexIO instance used for camera */
#define CAMERA_FLEXIO_INST          FLEXIO0

/* XCLK output frequency (Hz) */
#define CAMERA_XCLK_FREQ_HZ         12000000U

/* FlexIO shifter/timer indices */
#define CAMERA_FLEXIO_PCLK_TIMER    0U
#define CAMERA_FLEXIO_XCLK_TIMER    1U
#define CAMERA_FLEXIO_DATA_PIN_START 6U   /* FLEXIO_D6 */

/* GPIO for XSYNC interrupt */
#define CAMERA_XSYNC_GPIO           GPIO2
#define CAMERA_XSYNC_PIN            5U

/* GPIO for camera RESET */
#define CAMERA_RESET_GPIO           GPIO2
#define CAMERA_RESET_PIN            0U

/* Camera frame parameters */
#define CAMERA_BYTES_PER_PIXEL      2U
#define CAMERA_FRAME_RATE           30U
#define CAMERA_FRAME_BUFFER_COUNT   2U

/* Camera resolution — overridden by APP_CAMERA_WIDTH/HEIGHT from mpp_config.h */
#ifndef APP_CAMERA_WIDTH
#define APP_CAMERA_WIDTH            320U
#endif
#ifndef APP_CAMERA_HEIGHT
#define APP_CAMERA_HEIGHT           240U
#endif

#endif /* _CAMERA_CONFIG_H_ */
