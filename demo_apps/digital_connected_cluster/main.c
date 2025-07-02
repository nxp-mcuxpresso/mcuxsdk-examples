
/* 
 * Copyright 2020-2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include <porting.h>
#include <string.h>
#include <errno/errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include "FreeRTOS.h"
#include "task.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include "app_pbap_pce.h"
#include <bluetooth/map_mce.h>
#include <bluetooth/hfp_hf.h>
#include <bluetooth/hfp_ag.h>
#include "app_a2dp_source.h"
#include "app_a2dp_sink.h"
#include "app_handsfree_ag.h"
#include "app_handsfree.h"
#include "app_pbap_pce.h"
#include "app_map_mce.h"
#include "app_dcc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
	BOARD_InitHardware();

    if (xTaskCreate(app_dcc_task, "app_dcc_task", configMINIMAL_STACK_SIZE * 12, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("app_dcc_task is failed!\n");
        while (1);
    }

    if (xTaskCreate(read_audio, "read_audio_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
      {
                PRINTF("read_audio task creation failed!\r\n");
                while (1)
                    ;
      }

    vTaskStartScheduler();
    for (;;);
}
