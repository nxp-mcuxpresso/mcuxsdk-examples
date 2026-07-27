/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_AUDIO_CONFIG_H__
#define __USB_AUDIO_CONFIG_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Packet size and interval. */
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
#define HS_ISO_OUT_ENDP_INTERVAL (0x01)
#else
#define HS_ISO_OUT_ENDP_INTERVAL (0x04) /*interval must be 1ms for usb audio 1.0 */
#endif
#define HS_ISO_IN_ENDP_INTERVAL (0x04)

/* the threshold transfer count that can tolerance by frame */
#define AUDIO_SYNC_DATA_BASED_ADJUST_THRESHOLD (4U)

/* For ip3511hs in high speed mode, microframe can not be obtained and only for frame, the used feedback solution
 * requires us to have to use larger latency and buffer size to avoid buffer overflow or underflow. Sync mode can use
 * low latency (<1ms) even if on ip3511hs */
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
/* 6 means 16 mico frames (6*125us), make sure the latency is smaller than 1ms for ehci high speed */
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_TRANSFER_COUNT (0x06U)
/* 2 units size buffer (1 unit means the size to play during 1ms) */
#define AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT (2U)
#endif

#define AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL  (16 * 2)
#define AUDIO_RECORDER_DATA_WHOLE_BUFFER_COUNT_NORMAL (16 * 2)

/* feedback calculate interval */
#define AUDIO_CALCULATE_Ff_INTERVAL (16U)
/* the threshold transfer count that can tolerance by frame */
#define USB_AUDIO_PLAY_BUFFER_FEEDBACK_TOLERANCE_THRESHOLD (4U)
/* feedback value discard times, the first feedback vaules are discarded */
#define AUDIO_SPEAKER_FEEDBACK_DISCARD_COUNT (4U)

/* pll adjustment calculate interval */
#define AUDIO_PLL_ADJUST_INTERVAL (8U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/*${prototype:end}*/

#endif /* _USB_AUDIO_CONFIG_H_ */
