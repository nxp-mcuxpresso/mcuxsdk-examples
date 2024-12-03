/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_TYPES_H_
#define APP_TYPES_H_

#include "mpp_api_types.h"
#include "models/mobilefacenet/mobilefacenet_output_postproc_quantized.h"
#include "models/ultraface_slim_quant_int8/ultraface_output_postproc.h"

typedef enum _e_cur_model {
    MODEL_ULTRAFACE,
    MODEL_MOBILEFACENET
} e_cur_model;

typedef enum _e_state {
    STATE_DETECTING,
    STATE_DETECTED,
    STATE_RECOGNIZING,
    STATE_RECOGNIZED,
    STATE_REGISTERING,  // TODO use this state to let user input its name
    STATE_REGISTERED,
    STATE_NOTIFYING_USER,
    STATE_USER_NOTIFIED
} e_state;

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_elem_handle_t labrect_elem; /* label-rect element handle */
    mpp_elem_handle_t infer_elem;   /* inferenc element handle */
    e_cur_model cur_model;          /* model that is currently running */
    e_cur_model last_model;         /* model that last provided output tensor(s) */
    recognition_result result;
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    box_data final_boxes[NUM_BOXES_MAX];
    uint32_t accessing;     /* boolean protecting access */
    int detected_count;     /* number of detected boxes */
    int inference_time_ms;
    e_state state;          /* state-machine for the app */
} user_data_t;

#endif /* APP_TYPES_H_ */
