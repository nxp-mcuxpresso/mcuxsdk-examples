/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cstring>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "ethosu_log.h"

#include "ethosu_driver.h"
#include "ethosu_core_interface.h"
#include "inference_parser.hpp"
#include "inference_process.hpp"

#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"

#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ETHOSU_BASE_ADDRESS     0x4A900000
#define ETHOSU_IRQ		178

#define APP_TASK_STACK_SIZE (10 * 1024)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif

#define FAST_MEMORY_ADDRESS	0x20480000
#define FAST_MEMORY_SIZE	0x60000

#if (!defined(__ICCARM__))
using namespace std;
using namespace InferenceProcess;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

using namespace EthosU;

struct ethosu_driver ethosu_drv;

static TaskHandle_t app_task_handle = NULL;

static int sendVersionRsp(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
		    struct rpmsg_lite_endpoint *volatile ethosu_ept,
		    volatile uint32_t remote_addr)
{
    struct ethosu_u65_msg_version ver = {
        .major     = ETHOSU_U65_MSG_VERSION_MAJOR,
	.minor     = ETHOSU_U65_MSG_VERSION_MINOR,
	.patch     = ETHOSU_U65_MSG_VERSION_PATCH,
	._reserved = 0
    };
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_VERSION_RSP,
	.length = sizeof(ethosu_u65_msg_version)
    };
    int32_t result;
    void *tx_buf;
    uint32_t size;

    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));
    memcpy((ethosu_u65_msg_version *)((char *)tx_buf + sizeof(ethosu_u65_msg)),
	   &ver, sizeof(ethosu_u65_msg_version));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
				    sizeof(ethosu_u65_msg) +
				    sizeof(ethosu_u65_msg_version));
    if (result != 0)
        return result;

    return 0;
}

static int sendPong(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
		    struct rpmsg_lite_endpoint *volatile ethosu_ept,
		    volatile uint32_t remote_addr)
{
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_PONG,
	.length = sizeof(ethosu_u65_msg)
    };
    int32_t result;
    void *tx_buf;
    uint32_t size;

    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
				    sizeof(ethosu_u65_msg));
    if (result != 0)
        return result;

    return 0;
}

static int sendCapabilitiesRsp(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
			       struct rpmsg_lite_endpoint *volatile ethosu_ept,
			       volatile uint32_t remote_addr,
			       ethosu_u65_capabilities_req *req, uint32_t reqlen)
{
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_CAPABILITIES_RSP,
	.length = sizeof(ethosu_u65_msg_capabilities_rsp)
    };
    int32_t result;
    void *tx_buf;
    uint32_t size;
    struct ethosu_u65_msg_capabilities_rsp rsp;
    struct ethosu_driver_version ver;
    struct ethosu_hw_info hw;

    ethosu_get_driver_version(&ver);
    ethosu_get_hw_info(&ethosu_drv, &hw);

    rsp.user_arg = req->user_arg;

    rsp.version_status = hw.version.version_status;
    rsp.version_minor = hw.version.version_minor;
    rsp.version_major = hw.version.version_major;
    rsp.product_major = hw.version.product_major;
    rsp.arch_patch_rev = hw.version.arch_patch_rev;
    rsp.arch_minor_rev = hw.version.arch_minor_rev;
    rsp.arch_major_rev = hw.version.arch_major_rev;

    rsp.driver_patch_rev = ver.patch;
    rsp.driver_minor_rev = ver.minor;
    rsp.driver_major_rev = ver.major;

    rsp.macs_per_cc = hw.cfg.macs_per_cc;
    rsp.cmd_stream_version = hw.cfg.cmd_stream_version;
    rsp.custom_dma = hw.cfg.custom_dma;

    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));
    memcpy((ethosu_u65_msg_version *)((char *)tx_buf + sizeof(ethosu_u65_msg)),
	   &rsp, sizeof(ethosu_u65_msg_capabilities_rsp));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
				    sizeof(ethosu_u65_msg) +
				    sizeof(ethosu_u65_msg_capabilities_rsp));
    if (result != 0)
        return result;

    return 0;
}

static int handleInferenceReq(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
			      struct rpmsg_lite_endpoint *volatile ethosu_ept,
			      volatile uint32_t remote_addr,
			      ethosu_u65_inference_req *req, uint32_t reqlen)
{
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_INFERENCE_RSP,
	.length = sizeof(ethosu_u65_inference_rsp)
    };
    int32_t result;
    ethosu_u65_inference_rsp rsp;
    void *tx_buf;
    uint32_t size;

    LOG_INFO("InferenceReq. network={0x%x, %u}, ifm={%u, 0x%x, %u}, ofm={%u, 0x%x, %u}, user_arg={0x%x}\r\n",
	   req->network.buffer.ptr, req->network.buffer.size, req->ifm_count,
	   req->ifm[0].ptr, req->ifm[0].size, req->ofm_count,
	   req->ofm[0].ptr, req->ofm[0].size, req->user_arg);

    InferenceProcess::DataPtr networkModel(reinterpret_cast<void *>(req->network.buffer.ptr), req->network.buffer.size);

    bool isEthosuOp = (req->inference_type == ETHOSU_U65_INFERENCE_OP);
    uint8_t *tensorArena = NULL;
    size_t tensorArenaSize = 0;
    std::vector<InferenceProcess::DataPtr> ifm;
    if (isEthosuOp) {
        for (uint32_t i = 0; i < req->ifm_count; ++i) {
            ifm.push_back(InferenceProcess::DataPtr(reinterpret_cast<void *>(req->ifm[i].ptr), req->ifm[i].size));
        }
        // EthosuOp inference don't need tensor arena memory
    } else {
        // ifm[0] is used as tensor arena memory
        tensorArena = reinterpret_cast<uint8_t *>(req->ifm[0].ptr);
        tensorArenaSize = req->ifm[0].size;
        for (uint32_t i = 1; i < req->ifm_count; ++i) {
            ifm.push_back(InferenceProcess::DataPtr(reinterpret_cast<void *>(req->ifm[i].ptr), req->ifm[i].size));
        }
    }

    std::vector<InferenceProcess::DataPtr> ofm;
    for (uint32_t i = 0; i < req->ofm_count; ++i) {
        ofm.push_back(InferenceProcess::DataPtr(reinterpret_cast<void *>(req->ofm[i].ptr), req->ofm[i].size));
    }

    std::vector<InferenceProcess::DataPtr> expectedOutput;
    std::vector<uint8_t> pmuEventConfig(ETHOSU_U65_PMU_MAX);
    for (uint32_t i = 0; i < ETHOSU_U65_PMU_MAX; i++) {
        pmuEventConfig[i] = req->pmu_event_config[i];
    }

    InferenceProcess::InferenceJob job("job", networkModel, ifm, ofm, expectedOutput, pmuEventConfig,
                                       req->pmu_cycle_counter_enable, &ethosu_drv, 0, nullptr,
                                       req->flash_offset, req->arena_offset, isEthosuOp);
    job.invalidate();

    InferenceProcess::InferenceProcess inferenceprocess(tensorArena, tensorArenaSize);
    bool failed = inferenceprocess.runJob(job);
    for (size_t i = 0; i < job.ethosuMonitor.numEvents; i++) {
        rsp.pmu_event_config[i] = job.ethosuMonitor.ethosuEventIds[i];
    }
    rsp.pmu_cycle_counter_enable = job.ethosuMonitor.pmuCycleCounterEnable;
    for (size_t i = 0; i < job.ethosuMonitor.numEvents; i++) {
        rsp.pmu_event_count[i] = job.ethosuMonitor.eventCount[i];
    }
    rsp.pmu_cycle_counter_count = job.ethosuMonitor.pmuCycleCounterCount;
    job.clean();

    rsp.user_arg = req->user_arg;
    rsp.ofm_count = job.output.size();
    rsp.status = failed ? ETHOSU_U65_STATUS_ERROR : ETHOSU_U65_STATUS_OK;

    for (size_t i = 0; i < ofm.size(); ++i) {
        rsp.ofm_size[i] = job.output[i].size;
    }

    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));
    memcpy((ethosu_u65_inference_rsp *)((char *)tx_buf + sizeof(ethosu_u65_msg)),
	   &rsp, sizeof(ethosu_u65_inference_rsp));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
                                    sizeof(ethosu_u65_inference_rsp) + sizeof(ethosu_u65_msg));
    if (result != 0)
        return result;

    return 0;
}

static int sendNetworkInfoRsp(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
			      struct rpmsg_lite_endpoint *volatile ethosu_ept,
			      volatile uint32_t remote_addr,
			      ethosu_u65_network_info_req *req, uint32_t reqlen)
{
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_NETWORK_INFO_RSP,
	.length = sizeof(ethosu_u65_network_info_rsp)
    };
    int32_t result;
    void *tx_buf;
    uint32_t size;
    struct ethosu_u65_network_info_rsp rsp;
    InferenceProcess::InferenceParser parser;

    rsp.user_arg = req->user_arg;
    rsp.ifm_count = 0;
    rsp.ofm_count = 0;

#if 0
    bool failed = parser.parseModel((const void*)req->network.buffer.ptr, rsp.desc,
				    InferenceProcess::makeArray(rsp.ifm_size, rsp.ifm_count, ETHOSU_U65_BUFFER_MAX),
				    InferenceProcess::makeArray(rsp.ofm_size, rsp.ofm_count, ETHOSU_U65_BUFFER_MAX));

#else
    InferenceProcess::ModelInfo info;
    bool failed = parser.getModelInfo((const void*)req->network.buffer.ptr, rsp.desc, info);

    rsp.ifm_count = info.inputDims.size();
    rsp.ofm_count = info.outputDims.size();
    rsp.is_vela = info.isVelaModel;

    for (uint32_t i = 0; i < rsp.ifm_count; i++) {
        rsp.ifm_size[i] = info.inputDims[i];
        rsp.ifm_types[i] = info.inputTypes[i];
        rsp.ifm_offset[i] = info.inputDataOffset[i];
        rsp.ifm_dims[i] = info.inputShapes[i].size();
        for (uint32_t j = 0; j < rsp.ifm_dims[i]; j ++)
            rsp.ifm_shapes[i][j] = info.inputShapes[i][j];
    }
    for (uint32_t i = 0; i < rsp.ofm_count; i++) {
        rsp.ofm_size[i] = info.outputDims[i];
        rsp.ofm_types[i] = info.outputTypes[i];
        rsp.ofm_offset[i] = info.outputDataOffset[i];
        rsp.ofm_dims[i] = info.outputShapes[i].size();
        for (uint32_t j = 0; j < rsp.ofm_dims[i]; j ++)
            rsp.ofm_shapes[i][j] = info.outputShapes[i][j];
    }
#endif

    rsp.status = failed ? ETHOSU_U65_STATUS_ERROR : ETHOSU_U65_STATUS_OK;
    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));
    memcpy((ethosu_u65_network_info_rsp *)((char *)tx_buf + sizeof(ethosu_u65_msg)),
	   &rsp, sizeof(ethosu_u65_network_info_rsp));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
				    sizeof(ethosu_u65_msg) +
				    sizeof(ethosu_u65_network_info_rsp));
    if (result != 0)
        return result;

    return 0;
}

static int sendCancelInferenceRsp(struct rpmsg_lite_instance *volatile ethosu_rpmsg,
				  struct rpmsg_lite_endpoint *volatile ethosu_ept,
				  volatile uint32_t remote_addr,
				  ethosu_u65_cancel_inference_req *req, uint32_t reqlen)
{
    struct ethosu_u65_msg msg = {
        .magic  = ETHOSU_U65_MSG_MAGIC,
	.type   = ETHOSU_U65_MSG_CANCEL_INFERENCE_RSP,
	.length = sizeof(ethosu_u65_cancel_inference_rsp)
    };
    int32_t result;
    void *tx_buf;
    uint32_t size;
    struct ethosu_u65_cancel_inference_rsp rsp;

    rsp.user_arg = req->user_arg;
    rsp.status = ETHOSU_U65_STATUS_OK;

    tx_buf = rpmsg_lite_alloc_tx_buffer(ethosu_rpmsg, &size, RL_BLOCK);

    memcpy((ethosu_u65_msg *)tx_buf, &msg, sizeof(ethosu_u65_msg));
    memcpy((ethosu_u65_cancel_inference_rsp *)((char *)tx_buf + sizeof(ethosu_u65_msg)),
	   &rsp, sizeof(ethosu_u65_cancel_inference_rsp));

    result = rpmsg_lite_send_nocopy(ethosu_rpmsg, ethosu_ept, remote_addr, tx_buf,
				    sizeof(ethosu_u65_msg) +
				    sizeof(ethosu_u65_cancel_inference_rsp));
    if (result != 0)
        return result;

    return 0;
}

static int handlePowerReq(struct ethosu_u65_power_req *req)
{
    if (req->type == ETHOSU_U65_POWER_REQ_SUSPEND)
    {
        ethosu_deinit(&ethosu_drv);
        ethosu_suspend();
	BOARD_McoreSUSPEND();
    }
    else if (req->type == ETHOSU_U65_POWER_REQ_RESUME) {
        if (ethosu_init(&ethosu_drv, (void *)ETHOSU_BASE_ADDRESS, (void *)FAST_MEMORY_ADDRESS, FAST_MEMORY_SIZE, 0, 0)) {
            PRINTF("Failed to initialize Arm Ethos-U\n");
            return 1;
        }
        ethosu_resume();
    }

    return 0;
}

static void app_task(void *param)
{
    volatile uint32_t remote_addr;
    struct rpmsg_lite_endpoint *volatile ethosu_ept;
    volatile rpmsg_queue_handle ethosu_queue;
    struct rpmsg_lite_instance *volatile ethosu_rpmsg;
    struct ethosu_u65_msg *msg;
    int32_t result;
    void *rx_buf;
    uint32_t len;

    /* Print the initial banner */
    PRINTF("ETHOS-U FreeRTOS RTOS API Demo...\r\n");

#ifdef MCMGR_USED
    uint32_t startupData;

    /* Get the startup data */
    (void)MCMGR_GetStartupData(kMCMGR_Core1, &startupData);

    ethosu_rpmsg = rpmsg_lite_remote_init((void *)startupData, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);

    /* Signal the other core we are ready */
    (void)MCMGR_SignalReady(kMCMGR_Core1);
#else
    ethosu_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#endif /* MCMGR_USED */

    while (0 == rpmsg_lite_is_link_up(ethosu_rpmsg))
        ;

    PRINTF("RPMSG_LITE is link up\r\n");
    ethosu_queue = rpmsg_queue_create(ethosu_rpmsg);
    ethosu_ept   = rpmsg_lite_create_ept(ethosu_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, ethosu_queue);
    (void)rpmsg_ns_announce(ethosu_rpmsg, ethosu_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, RL_NS_CREATE);

    PRINTF("Nameservice sent, ready for incoming messages...\r\n");

    for (;;)
    {
        /* Get RPMsg rx buffer with message */
        result =
            rpmsg_queue_recv_nocopy(ethosu_rpmsg, ethosu_queue, (uint32_t *)&remote_addr, (char **)&rx_buf, &len, RL_BLOCK);
        if (result != 0)
            assert(false);

	msg = (ethosu_u65_msg *)rx_buf;

	switch (msg->type) {
	case ETHOSU_U65_MSG_VERSION_REQ:
	    LOG_INFO("Receive Message Version Req");
	    sendVersionRsp(ethosu_rpmsg, ethosu_ept, remote_addr);
	    break;
	case ETHOSU_U65_MSG_PING:
	    LOG_INFO("Receive Message Ping");
	    sendPong(ethosu_rpmsg, ethosu_ept, remote_addr);
	    break;
	case ETHOSU_U65_MSG_INFERENCE_REQ:
	    LOG_INFO("Receive Message Inference Request");
	    handleInferenceReq(ethosu_rpmsg, ethosu_ept, remote_addr,
			       (ethosu_u65_inference_req *)((char *)rx_buf + sizeof(ethosu_u65_msg)),
			       sizeof(ethosu_u65_inference_req));
	    break;
	case ETHOSU_U65_MSG_CAPABILITIES_REQ:
	    LOG_INFO("Receive Message Capabilities Request");
	    sendCapabilitiesRsp(ethosu_rpmsg, ethosu_ept, remote_addr,
			       (ethosu_u65_capabilities_req *)((char *)rx_buf + sizeof(ethosu_u65_msg)),
			       sizeof(ethosu_u65_capabilities_req));
	    break;
	case ETHOSU_U65_MSG_NETWORK_INFO_REQ:
	    LOG_INFO("Receive Message Network Info Request");
	    sendNetworkInfoRsp(ethosu_rpmsg, ethosu_ept, remote_addr,
			       (ethosu_u65_network_info_req *)((char *)rx_buf + sizeof(ethosu_u65_msg)),
			       sizeof(ethosu_u65_network_info_req));
	    break;
	case ETHOSU_U65_MSG_CANCEL_INFERENCE_REQ:
	    LOG_INFO("Receive Message Cancel Inference Request");
	    sendCancelInferenceRsp(ethosu_rpmsg, ethosu_ept, remote_addr,
				   (ethosu_u65_cancel_inference_req *)((char *)rx_buf + sizeof(ethosu_u65_msg)),
			       sizeof(ethosu_u65_cancel_inference_req));
	    break;
	case ETHOSU_U65_MSG_POWER_REQ:
	    LOG_INFO("Receive Message Power Request");
	    handlePowerReq((ethosu_u65_power_req *)((char *)rx_buf + sizeof(ethosu_u65_msg)));
	    break;
	default:
	    break;
	}

        /* Release held RPMsg rx buffer */
        result = rpmsg_queue_nocopy_free(ethosu_rpmsg, rx_buf);
        if (result != 0)
	    assert(false);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Initialize Arm Ethos-U\r\n");
    if (ethosu_init(&ethosu_drv, (void *)ETHOSU_BASE_ADDRESS, (void *)FAST_MEMORY_ADDRESS, FAST_MEMORY_SIZE, 0, 0)) {
        PRINTF("Failed to initialize Arm Ethos-U\n");
	return 1;
    }

    NVIC_SetVector((IRQn_Type)ETHOSU_IRQ, (uint32_t)&ethosu_irq_handler);
    NVIC_EnableIRQ((IRQn_Type)ETHOSU_IRQ);

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        PRINTF("Failed to create application task\r\n");
        for (;;)
            ;
    }

    vTaskStartScheduler();

    PRINTF("Failed to start FreeRTOS on core0.\r\n");
    for (;;)
        ;
}
