/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"

#include "fsl_sema42.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include <xtensa/config/core.h>
#include <xtensa/xos.h>

#include "fsl_gpio.h"

#include "lc3_codec.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DSP_MAIN_THREAD_STACK_SIZE (20 * 1024)
#define DSP_MAIN_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t dsp_main_thread_stack[DSP_MAIN_THREAD_STACK_SIZE];

static struct rpmsg_lite_instance *rpmsg;
static struct rpmsg_lite_endpoint *ept;
static rpmsg_queue_handle rpmsg_queue;

static uint8_t lc3_msg[2*1024];
static uint8_t lc3_msg_res[2*1024];

#define LC3_MSG_HEADER_SIZE 3

#define LC3_MSG_SUCCESS 0x00
#define LC3_MSG_ERROR 0xff

struct lc3_msg_header {
	uint8_t type;
	uint16_t len;
	uint8_t data[0];
} __attribute__((packed));

struct lc3_msg_res_headr {
	uint8_t type;
	uint16_t len;
	uint8_t res;
	uint8_t data[0];
} __attribute__((packed));

#define LC3_CODEC_NUM 8

static lc3_encoder_t encoder[LC3_CODEC_NUM];
static lc3_decoder_t decoder[LC3_CODEC_NUM];

/*******************************************************************************
 * Code
 ******************************************************************************/
void DSP_PRINTF(const char* ptr, ...)
{
    va_list ap;
    SEMA42_Lock(APP_SEMA42, SEMA_PRINTF_NUM, SEMA_CORE_ID_DSP);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(APP_SEMA42, SEMA_PRINTF_NUM);
}

/*!
 * @brief Main function
 */
int DSP_Main(void *arg, int wake_value)
{
    void *rpmsg_shmem_base;

    SEMA42_Lock(APP_SEMA42, SEMA_STARTUP_NUM, SEMA_CORE_ID_DSP);

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    rpmsg       = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    rpmsg_queue = rpmsg_queue_create(rpmsg);

    rpmsg_lite_wait_for_link_up(rpmsg, RL_BLOCK);

    SEMA42_Unlock(APP_SEMA42, SEMA_STARTUP_NUM);

    ept = rpmsg_lite_create_ept(rpmsg, DSP_EPT_ADDR, rpmsg_queue_rx_cb, (void *)rpmsg_queue);

    memset(encoder, 0, sizeof(encoder));
    memset(decoder, 0, sizeof(decoder));

    while (1)
    {
        int status = 0;
        struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
        struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

        /* msg receive. */
        status = rpmsg_queue_recv(rpmsg, rpmsg_queue, NULL, (char *)&lc3_msg, LC3_MSG_HEADER_SIZE, NULL, RL_BLOCK);
        if (status != RL_SUCCESS)
        {
            xos_fatal_error(status, "Failed to get item from RPMsg queue.\r\n");
        }

        if (msg->len > 0)
        {
            status = rpmsg_queue_recv(rpmsg, rpmsg_queue, NULL, (char *)&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len, NULL, RL_BLOCK);
            if (status != RL_SUCCESS)
            {
                xos_fatal_error(status, "Failed to get item from RPMsg queue.\r\n");
            }
        }

        /* msg process. */
        if (status == RL_SUCCESS)
        {
        	int instans;

            switch(msg->type)
            {
            case 0x00: /* Reset */
                    memset(encoder, 0, sizeof(encoder));
                    memset(decoder, 0, sizeof(decoder));

                    msg_res->len = 1;
                    msg_res->res = LC3_MSG_SUCCESS;
                    break;
            case 0x01: /* Encoder Init. */
                    instans = -1;
                    
                    /* find a index. */
                    for(int i = 0; i < LC3_CODEC_NUM; i++)
                    {
                        if(encoder[i].sample_rate == 0)
                        {
                        	instans = i;
                            break;
                        }
                    }

                    if(index < 0)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        uint32_t sample_rate;
                        uint32_t duration_us;
                        uint8_t enc_bytes;
                        uint8_t sample_bits;

                        memcpy(&sample_rate, &msg->data[0], sizeof(uint32_t));
                        memcpy(&duration_us, &msg->data[4], sizeof(uint32_t));
                        enc_bytes = msg->data[8];
                        sample_bits = msg->data[9];

                        int res = lc3_encoder_init(&encoder[instans], (int)sample_rate, (int)duration_us, (int)enc_bytes, (int)sample_bits);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR;
                        }
                        else
                        {
                            msg_res->len = 2;
                            msg_res->res = LC3_MSG_SUCCESS;
                            msg_res->data[0] = instans;
                        }
                    }
                    break;
            case 0x02: /* Encoder Deinit. */
                    instans = (int)msg->data[0];

                    if(instans >= LC3_CODEC_NUM)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        int res = lc3_encoder_deinit(&encoder[instans]);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR;
                        }
                        else
                        {
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_SUCCESS;
                        }
                    }
                    break;
            case 0x03: /* Encode one frame. */
                    instans = (int)msg->data[0];

                    if(instans >= LC3_CODEC_NUM)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        uint8_t *input = &msg->data[1];
                        uint8_t *ouput = &msg_res->data[1];
                        int res = lc3_encoder(&encoder[instans], input, ouput);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR;
                        }
                        else
                        {
                            msg_res->len = 2 + encoder[instans].enc_bytes;
                            msg_res->res = LC3_MSG_SUCCESS;
                            msg_res->data[0] = (uint8_t)instans;
                        }
                    }
                    break;
            case 0x04: /* Decoder Init. */
                    instans = -1;
                    
                    /* find a instans. */
                    for(int i = 0; i < LC3_CODEC_NUM; i++)
                    {
                        if(decoder[i].sample_rate == 0)
                        {
                            instans = i;
                            break;
                        }
                    }

                    if(instans < 0)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        uint32_t sample_rate;
                        uint32_t duration_us;
                        uint8_t enc_bytes;
                        uint8_t sample_bits;

                        memcpy(&sample_rate, &msg->data[0], sizeof(uint32_t));
                        memcpy(&duration_us, &msg->data[4], sizeof(uint32_t));
                        enc_bytes = msg->data[8];
                        sample_bits = msg->data[9];

                        int res = lc3_decoder_init(&decoder[instans], (int)sample_rate, (int)duration_us, (int)enc_bytes, (int)sample_bits);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR & res;
                        }
                        else
                        {
                            msg_res->len = 2;
                            msg_res->res = LC3_MSG_SUCCESS;
                            msg_res->data[0] = instans;
                        }
                    }
                    break;
            case 0x05: /* Decoder Deinit. */
                    instans = (int)msg->data[0];

                    if(instans >= LC3_CODEC_NUM)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        int res = lc3_decoder_deinit(&decoder[instans]);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR;
                        }
                        else
                        {
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_SUCCESS;
                        }
                    }
                    break;
            case 0x06: /* Decode one frame. */
                    instans = (int)msg->data[0];

                    if(instans >= LC3_CODEC_NUM)
                    {
                        /* error. */
                        msg_res->len = 1;
                        msg_res->res = LC3_MSG_ERROR;
                    }
                    else
                    {
                        uint8_t flag   = msg->data[1];
                        uint8_t *input = &msg->data[2];
                        uint8_t *ouput = &msg_res->data[1];

                        int res = lc3_decoder(&decoder[instans], input, (int)flag, ouput);
                        if(res)
                        {
                            /* error. */
                            msg_res->len = 1;
                            msg_res->res = LC3_MSG_ERROR;
                        }
                        else
                        {
                        	lc3_decoder_t *p_decoder = &decoder[instans];
                            msg_res->len = 2 + LC3_SAMPLES_BYTES_PER_FRAME(p_decoder);
                            msg_res->res = LC3_MSG_SUCCESS;
                            msg_res->data[0] = (uint8_t)instans;
                        }
                    }
                    break;
            default:
            		break;
            }

            msg_res->type = msg->type | 0x80;
        }
        else
        {
            /* receive fail, not likely. */
            msg_res->type = msg->type | 0x80;
            msg_res->len  = 1;
            msg_res->res  = 1;
        }

        /* msg res send. */
        status = rpmsg_lite_send(rpmsg, ept, MCU_EPT_ADDR, (char *)&lc3_msg_res, LC3_MSG_HEADER_SIZE, 0);
		if (status != RL_SUCCESS)
		{
			xos_fatal_error(status, "Failed to send item to RPMsg queue.\r\n");
		}

        if(msg_res->len)
        {
            status = rpmsg_lite_send(rpmsg, ept, MCU_EPT_ADDR, (char *)&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len, 0);
            if (status != RL_SUCCESS)
            {
                xos_fatal_error(status, "Failed to send item to RPMsg queue.\r\n");
            }
        }
    }

    rpmsg_lite_destroy_ept(rpmsg, ept);
    rpmsg_lite_deinit(rpmsg);

    return 0;
}

int main(void)
{
    XosThread thread_main;

    BOARD_InitHardware();

    xos_thread_create(&thread_main, NULL, DSP_Main, NULL, "DSP Main", dsp_main_thread_stack, DSP_MAIN_THREAD_STACK_SIZE,
                      DSP_MAIN_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement. */
    return 0;
}
