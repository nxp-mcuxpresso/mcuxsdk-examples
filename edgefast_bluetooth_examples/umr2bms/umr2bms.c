/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/types.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/atomic.h>
#include <errno/errno.h>
#include <toolchain.h>
#include <porting.h>
#include "fsl_debug_console.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/byteorder.h>
#include <bluetooth/conn.h>
#include <bluetooth/audio/audio.h>

#include "umr2bms.h"

#include "le_audio_shell.h"

#include "broadcast_media_sender.h"
#include "unicast_media_receiver.h"

static StackType_t bms_stack[ configMINIMAL_STACK_SIZE * 8 ];
static StaticTask_t bms_task_buffer;

static StackType_t umr_stack[ configMINIMAL_STACK_SIZE * 8 ];
static StaticTask_t umr_task_buffer;

enum {
	UMR_TO_BMS_FLAG_AUDIO_ENABLED,
	UMR_TO_BMS_FLAG_AUDIO_STARTED,
	UMR_TO_BMS_FLAG_AUDIO_STOPED,
	UMR_TO_BMS_FLAG_AUDIO_DISABLED,
	UMR_TO_BMS_FLAG_AUDIO_RELEASED,
};

static atomic_t flags;
static struct k_sem sem;

void umr_to_bms_task(void *param)
{
	int err;

	k_sem_init(&sem, 0, 0xFF);

	/* shell init. */
	le_audio_shell_init();

	/* bluetooth init. */
	err = bt_enable(NULL);
	if (err != 0) {
		PRINTF("Bluetooth init failed (err %d)\r\n", err);
		while(1);
	}

	PRINTF("Bluetooth initialized\r\n");

	/* Host msd init. */
#if (defined(BT_BLE_PLATFORM_INIT_ESCAPE) && (BT_BLE_PLATFORM_INIT_ESCAPE > 0))
	USB_HostMsdFatfsInit();
	PRINTF("FatFs initialized\r\n");
#endif

	if (NULL == xTaskCreateStatic(unicast_media_receiver_task, "unicast_media_receiver_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, umr_stack, &umr_task_buffer))
	{
		PRINTF("unicast_media_receiver_task creation failed!\r\r\n");
		while (1)
		;
	}

	while (1) {
		k_sem_take(&sem, K_FOREVER);
		if (atomic_test_and_clear_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_ENABLED)) {
			bms_init();
		}
		if (atomic_test_and_clear_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STARTED)) {
			bms_start();
		}
		if (atomic_test_and_clear_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STOPED)) {
			bms_stop();
		}
		if (atomic_test_and_clear_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_DISABLED)) {
			bms_stop();
		}
		if (atomic_test_and_clear_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_RELEASED)) {
			bms_deinit();
		}
	}
}

void umr_to_bms_vol_control(uint8_t volume, uint8_t mute)
{
	if (mute) {
		atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STOPED);
	} else {
		atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STARTED);
	}
	k_sem_give(&sem);
}

int umr_to_bms_audio_codec_cfg(struct bt_audio_codec_cfg *cfg)
{
	return bms_codec_cfg(cfg);
}

int umr_to_bms_audio_codec_qos(struct bt_audio_codec_qos *qos)
{
	return bms_qos(qos);
}

int umr_to_bms_audio_enable(void)
{
	atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_ENABLED);
	k_sem_give(&sem);
	return 0;
}

int umr_to_bms_audio_start(void)
{
	atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STARTED);
	k_sem_give(&sem);
	return 0;
}

int umr_to_bms_audio_stop(void)
{
	atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_STOPED);
	k_sem_give(&sem);
	return 0;
}

int umr_to_bms_audio_disable(void)
{
	atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_DISABLED);
	k_sem_give(&sem);
	return 0;
}

int umr_to_bms_audio_release(void)
{
	atomic_set_bit(&flags, UMR_TO_BMS_FLAG_AUDIO_RELEASED);
	k_sem_give(&sem);
	return 0;
}

int umr_to_bms_stream_recv(enum bt_audio_location chan_allocation, const struct bt_iso_recv_info *info, struct net_buf *buf)
{
	if ((info->flags & BT_ISO_FLAGS_VALID) == 0) {
		buf = NULL;
	}
	return bms_stream_send(chan_allocation, buf);
}