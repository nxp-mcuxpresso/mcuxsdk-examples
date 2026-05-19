/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <stdio.h>

#include "image_process.h"
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef USE_USB_CAMERA

#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
#include "fsl_cache.h"
#endif

#define VG_LITE_ALIGN(number, alignment)    \
		(((number) + ((alignment) - 1)) & ~((alignment) - 1))

uint8_t *s_frameBufferAddr;

#if !defined(SDK_DEBUGCONSOLE_UART)
extern int DbgConsole_Printf(const char *fmt_s, ...);

/*! @brief 0U: DEBUGCONSOLE_REDIRECT_TO_TOOLCHAIN, 1U: DEBUGCONSOLE_REDIRECT_TO_SDK, 2U: DEBUGCONSOLE_DISABLE*/
#if defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == 0U)
#define process_echo printf
#elif defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == 1U)
#define process_echo DbgConsole_Printf
#elif defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == 2U)
static inline int USB_DbgConsole_Disabled(void)
{
	return -1;
}
#define process_echo(...) USB_DbgConsole_Disabled()
#endif

#else

#define process_echo printf

#endif


QueueHandle_t usbcameraqueue_mppin = NULL;              /* When a picture is ready, send to this queue*/
QueueHandle_t usbcameraqueue_mppdone = NULL;           /* When mpp has finishes using a picture buffer, it will be placed in this queue */

/*
 * @note Address and size should be aligned to XCACHE_LINESIZE_BYTE due to the cache operation unit
 * FSL_FEATURE_XCACHE_LINESIZE_BYTE. On RT700 that is 16 bytes.
 */

int CacheFlushByAddr(uint32_t addr, uint32_t len, uint8_t invalidate)
{

#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
	if (!invalidate) // This is for sending data, just flush the cache range to RAM
	{
		/* clear the DCACHE */
		DCACHE_CleanByRange(addr, len);
	}
	else /* (Send/Receive) Flush and invalidate so any new access uses the new data*/
	{
		/* clear the DCACHE */
		DCACHE_CleanInvalidateByRange(addr, len);
	}
#endif
	return 1;
}

/*
 * Process an image from the imagebuffer buffer with a size of image size and encoding as given in format
 * the format is based on the IMG_FORMAT_* defines
 * Currently only IMG_FORMAT_JPG is implemented
 */
int Process_Image_Frame(uint8_t *imagebuffer, uint32_t imagesize, uint32_t width, uint32_t height, uint8_t format)
{
	static int counter = 0;
	unsigned char * mpp_buffer;

#if PROCESS_STATS
#if (configGENERATE_RUN_TIME_STATS == 1)
	static char task_stats_buf[512];
#endif
#endif
#ifdef PRINT_FRAME_INFOS
	process_echo("JS:%ukB ", imagesize/1024);
#endif
	if (imagesize <= 1024)
	{
		process_echo("Skipping Image, size:%d\r\n",imagesize);
		return -1;

	}
#if PROCESS_STATS
#if (configGENERATE_RUN_TIME_STATS == 1)
	vTaskGetRunTimeStats(task_stats_buf);
	process_echo("\r\n ****TASK STATS %d ****\r\n", counter);
	process_echo(task_stats_buf);
#endif
#endif

	// Only support MJPEG & YUV422 images
	if ((format != IMG_FORMAT_JPG) && (format != IMG_FORMAT_YUV422))
	{
		process_echo("PI unsupported format fmt:%d\r\n", format);
		return -1;
	}

	counter++;


	usb_camera_msg_t msg;

	msg.cmd = USB_CAMERA_FRAME_READY;
	msg.parameter = imagebuffer;
	msg.size = imagesize;

	if (usbcameraqueue_mppin)
	{
		xQueueSend( usbcameraqueue_mppin, &msg, portMAX_DELAY );

		if( xQueueReceive(usbcameraqueue_mppdone, &msg, portMAX_DELAY ) == pdPASS )
		{
			switch(msg.cmd)
			{
			case USB_CAMERA_FRAME_DONE:
			{
				mpp_buffer = (unsigned char *)msg.parameter;

				if (mpp_buffer != imagebuffer)
				{
					process_echo("Process Image: Unexpected camera queue framebuffer\r\n");
					return -2;
				}
				break;
			}
			default:
			{
				process_echo("Process Image: Unexpected camera queue message\r\n");
				return -3;
			}
			}
		}
		else
		{
			process_echo("Process Image: Unexpected camera queue receive error\r\n");
			return -4;
		}
	}
	else
	{
		process_echo("Process Image: MPP Camera queue not initialized\r\n");
		return -5;
	}

	return 0;
}
#endif /* USE_USB_CAMERA */
