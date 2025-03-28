/*
 * Copyright 2020-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ETHOSU_CORE_INTERFACE_H
#define ETHOSU_CORE_INTERFACE_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace EthosU {
#endif

/** Maximum number of IFM/OFM buffers per inference */
#define ETHOSU_U65_BUFFER_MAX 4
/** Maximum number of dimensions for input and output */
#define ETHOSU_U65_DIM_MAX 4

/** Maximum number of PMU counters to be returned for inference */
#define ETHOSU_U65_PMU_MAX 4

#define ETHOSU_U65_MSG_MAGIC 0x41457631
#define ETHOSU_U65_MSG_VERSION_MAJOR 0
#define ETHOSU_U65_MSG_VERSION_MINOR 2
#define ETHOSU_U65_MSG_VERSION_PATCH 0

#define ETHOSU_U65_INFERENCE_MODEL 0
#define ETHOSU_U65_INFERENCE_OP    1

/**
 * enum ethosu_u65_msg_type - Message types
 *
 * Types for the messages sent between the host and the core subsystem.
 */
enum ethosu_u65_msg_type {
	ETHOSU_U65_MSG_ERR = 1,
	ETHOSU_U65_MSG_PING,
	ETHOSU_U65_MSG_PONG,
	ETHOSU_U65_MSG_INFERENCE_REQ,
	ETHOSU_U65_MSG_INFERENCE_RSP,
	ETHOSU_U65_MSG_VERSION_REQ,
	ETHOSU_U65_MSG_VERSION_RSP,
	ETHOSU_U65_MSG_CAPABILITIES_REQ,
	ETHOSU_U65_MSG_CAPABILITIES_RSP,
	ETHOSU_U65_MSG_NETWORK_INFO_REQ,
	ETHOSU_U65_MSG_NETWORK_INFO_RSP,
	ETHOSU_U65_MSG_CANCEL_INFERENCE_REQ,
	ETHOSU_U65_MSG_CANCEL_INFERENCE_RSP,
	ETHOSU_U65_MSG_POWER_REQ,
	ETHOSU_U65_MSG_POWER_RSP,
	ETHOSU_U65_MSG_MAX
};

/**
 * struct ethosu_u65_msg - Message header
 */
struct ethosu_u65_msg {
	uint32_t magic;
	uint32_t type;
	uint32_t length;
};

/**
 * struct ethosu_u65_queue_header - Message queue header
 */
struct ethosu_u65_queue_header {
	uint32_t size;
	uint32_t read;
	uint32_t write;
};

/**
 * struct ethosu_u65_queue - Message queue
 *
 * Dynamically sized message queue.
 */
struct ethosu_u65_queue {
	struct ethosu_u65_queue_header header;
	uint8_t                         data[];
};

/**
 * enum ethosu_u65_status - Status
 */
enum ethosu_u65_status {
	ETHOSU_U65_STATUS_OK,
	ETHOSU_U65_STATUS_ERROR,
	ETHOSU_U65_STATUS_RUNNING,
	ETHOSU_U65_STATUS_REJECTED,
	ETHOSU_U65_STATUS_ABORTED,
	ETHOSU_U65_STATUS_ABORTING,
};

/**
 * struct ethosu_u65_buffer - Buffer descriptor
 *
 * Pointer and size to a buffer within the Ethos-U
 * address space.
 */
struct ethosu_u65_buffer {
	uint32_t ptr;
	uint32_t size;
};

/**
 * enum ethosu_u65_network_type - Network buffer type
 */
enum ethosu_u65_network_type {
	ETHOSU_U65_NETWORK_BUFFER = 1,
	ETHOSU_U65_NETWORK_INDEX
};

/**
 * struct ethosu_u65_network_buffer - Network buffer
 */
struct ethosu_u65_network_buffer {
	uint32_t type;
	union {
		struct ethosu_u65_buffer buffer;
		uint32_t                  index;
	};
};

/**
 * struct ethosu_u65_inference_req - Inference request
 */
struct ethosu_u65_inference_req {
	uint64_t                  user_arg;
	uint32_t                  ifm_count;
	struct ethosu_u65_buffer ifm[ETHOSU_U65_BUFFER_MAX];
	uint32_t                  ofm_count;
	struct ethosu_u65_buffer ofm[ETHOSU_U65_BUFFER_MAX];
	struct ethosu_u65_network_buffer network;
	uint8_t                   pmu_event_config[ETHOSU_U65_PMU_MAX];
	uint32_t                  pmu_cycle_counter_enable;
        uint32_t                  arena_offset;
	uint32_t                  flash_offset;
	uint32_t                  inference_type;
};

/**
 * struct ethosu_u65_inference_rsp - Inference response
 */
struct ethosu_u65_inference_rsp {
	uint64_t user_arg;
	uint32_t ofm_count;
	uint32_t ofm_size[ETHOSU_U65_BUFFER_MAX];
	uint32_t status;
	uint8_t  pmu_event_config[ETHOSU_U65_PMU_MAX];
	uint32_t pmu_event_count[ETHOSU_U65_PMU_MAX];
	uint32_t pmu_cycle_counter_enable;
	uint64_t pmu_cycle_counter_count;
};

/**
 * struct ethosu_u65_network_info_req - Network information request
 */
struct ethosu_u65_network_info_req {
	uint64_t                          user_arg;
	struct ethosu_u65_network_buffer network;
};

/**
 * struct ethosu_u65_network_info_rsp - Network information response
 */
struct ethosu_u65_network_info_rsp {
	uint64_t user_arg;
	char     desc[32];
	uint32_t is_vela;
	uint32_t ifm_count;
	uint32_t ifm_size[ETHOSU_U65_BUFFER_MAX];
	uint32_t ifm_types[ETHOSU_U65_BUFFER_MAX];
	uint32_t ifm_offset[ETHOSU_U65_BUFFER_MAX];
	uint32_t ifm_dims[ETHOSU_U65_BUFFER_MAX];
	uint32_t ifm_shapes[ETHOSU_U65_BUFFER_MAX][ETHOSU_U65_DIM_MAX];
	uint32_t ofm_count;
	uint32_t ofm_size[ETHOSU_U65_BUFFER_MAX];
	uint32_t ofm_types[ETHOSU_U65_BUFFER_MAX];
	uint32_t ofm_offset[ETHOSU_U65_BUFFER_MAX];
	uint32_t ofm_dims[ETHOSU_U65_BUFFER_MAX];
	uint32_t ofm_shapes[ETHOSU_U65_BUFFER_MAX][ETHOSU_U65_DIM_MAX];
	uint32_t status;
};

/**
 * struct ethosu_u65_msg_version - Message protocol version
 */
struct ethosu_u65_msg_version {
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
	uint8_t _reserved;
};

/**
 * struct ethosu_u65_capabilities_req - Message capabilities request
 */
struct ethosu_u65_capabilities_req {
	uint64_t user_arg;
};

/**
 * struct ethosu_u65_capabilities_rsp - Message capabilities response
 */
struct ethosu_u65_msg_capabilities_rsp {
	uint64_t user_arg;
	uint32_t version_status;
	uint32_t version_minor;
	uint32_t version_major;
	uint32_t product_major;
	uint32_t arch_patch_rev;
	uint32_t arch_minor_rev;
	uint32_t arch_major_rev;
	uint32_t driver_patch_rev;
	uint32_t driver_minor_rev;
	uint32_t driver_major_rev;
	uint32_t macs_per_cc;
	uint32_t cmd_stream_version;
	uint32_t custom_dma;
};

/**
 * struct ethosu_u65_cancel_inference_req - Message cancel inference request
 */
struct ethosu_u65_cancel_inference_req {
	uint64_t user_arg;
	uint64_t inference_handle;
};

/**
 * struct ethosu_u65_cancel_inference_rsp - Message cancel inference response
 */
struct ethosu_u65_cancel_inference_rsp {
	uint64_t user_arg;
	uint32_t status;
};

enum ethosu_u65_power_req_type {
	ETHOSU_U65_POWER_REQ_SUSPEND = 0,
	ETHOSU_U65_POWER_REQ_RESUME
};

/**
 * struct ethosu_u65_power_req - Message power request
 */
struct ethosu_u65_power_req {
	enum ethosu_u65_power_req_type type;
};

/**
 * enum ethosu_u65_msg_err_type - Error types
 */
enum ethosu_u65_msg_err_type {
	ETHOSU_U65_MSG_ERR_GENERIC = 0,
	ETHOSU_U65_MSG_ERR_UNSUPPORTED_TYPE,
	ETHOSU_U65_MSG_ERR_INVALID_PAYLOAD,
	ETHOSU_U65_MSG_ERR_INVALID_SIZE,
	ETHOSU_U65_MSG_ERR_INVALID_MAGIC,
	ETHOSU_U65_MSG_ERR_MAX
};

/**
 * struct ethosu_u65_msg_err - Error message struct
 */
struct ethosu_u65_msg_err {
	uint32_t type;     /* optional use of extra error code */
	char     msg[128];
};
#ifdef __cplusplus
} /*namespace EthosU */
#endif

#endif
