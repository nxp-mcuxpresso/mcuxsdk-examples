/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_TYPES_H_
#define APP_TYPES_H_

#include "mpp_api_types.h"
#include "mobilefacenet_output_postproc_quantized.h"
#include "ultraface_output_postproc.h"
#include "antispoofing_output_postproc_quantized.h"

typedef enum _e_cur_model {
    MODEL_ULTRAFACE,
    MODEL_MOBILEFACENET,
    MODEL_ANTISPOOFING
} e_cur_model;

typedef enum _e_state {
    STATE_DETECTING,
    STATE_DETECTED,
    STATE_RECOGNIZING,
    STATE_RECOGNIZED,
    STATE_REGISTERING,
    STATE_REGISTERED,
    STATE_REGISTRATION_CANCELLED,
    STATE_NOTIFYING_USER,
    STATE_USER_NOTIFIED,
    STATE_CHECKING_ANTISPOOFING,
    STATE_SPOOF, /* ANTISPOOFING: Face fake */
	STATE_REAL   /* ANTISPOOFING: Face real */
} e_state;

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_elem_handle_t labrect_elem; /* label-rect element handle */
    mpp_elem_handle_t infer_elem;   /* inferenc element handle */
    e_cur_model cur_model;          /* model that is currently running */
    e_cur_model last_model;         /* model that last provided output tensor(s) */
    recognition_result result;
    antispoofing_result liveness;  
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    box_data final_boxes[NUM_BOXES_MAX];
    uint32_t accessing;     /* boolean protecting access */
    int detected_count;     /* number of detected boxes */
    uint32_t inference_time_ms;
    e_state state;          /* state-machine for the app */
    face_t * db;            /* embeddings database */
    int db_max;             /* embeddings database max size. */
    mpp_stats_t *api_stats; /* global stats */
} user_data_t;

#endif /* APP_TYPES_H_ */
