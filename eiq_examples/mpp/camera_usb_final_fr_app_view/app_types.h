/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_TYPES_H_
#define APP_TYPES_H_

#include "mpp_api_types.h"
#include "mobilefacenet_output_postproc_quantized.h"
#include "antispoofing_output_postproc_quantized.h"
#include "scrfd_kps_output_postproc.h"
#include "app_msg.h"
#include "app_constants.h"

typedef enum _e_cur_model {
    MODEL_SCRFD_KPS,
    MODEL_MOBILEFACENET,
    MODEL_ANTISPOOFING
} e_cur_model;

typedef enum _e_state {
    STATE_DETECTING,
    STATE_DETECTED,
    STATE_CHECKING_ANTISPOOFING,
    STATE_SPOOF, /* ANTISPOOFING: Face fake */
    STATE_REAL,   /* ANTISPOOFING: Face real */
    STATE_RECOGNIZING,
    STATE_RECOGNIZED,
    STATE_NOT_RECOGNIZED,
    STATE_NOTIFYING_USER,
    STATE_USER_NOTIFIED
} e_state;

/* Define image indices for the composition array */
typedef enum {
    COMPOSE_LOGO_INDEX = 0,
    COMPOSE_TEXT_INDEX = 1,
    COMPOSE_INFPVW_INDEX = 2,
    COMPOSE_MAX_IMAGES
} compose_image_index_t;

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_t mp_split;
    mpp_t mp_bg;
    mpp_elem_handle_t labrect_elem; /* label-rect element handle */
    mpp_elem_handle_t infer_elem;   /* inferenc element handle */
    mpp_elem_handle_t compose_elem;   /* inferenc element handle */
    mpp_elem_handle_t cam_elem;       /* camera element handle */
    mpp_elem_handle_t img_quality_check_elem;
    e_cur_model cur_model;          /* model that is currently running */
    e_cur_model last_model;         /* model that last provided output tensor(s) */
    recognition_result result;
    antispoofing_result liveness;  
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    mpp_landmark_t landmarks[NUM_BOXES_MAX * SCRFD_NUM_LANDMARKS]; /* landmarks for display */
    box_data final_boxes[NUM_BOXES_MAX];
    mpp_element_params_t *p_params_compose;
    uint32_t accessing;     /* boolean protecting access */
    int detected_count;     /* number of detected boxes */
    int inference_time_ms;
    e_state state;          /* state-machine for the app */
    face_t * db;            /* embeddings database */
    int db_max;             /* embeddings database max size. */
    mpp_stats_t *api_stats; /* global stats */
    mpp_stats_t *mp_stats;  /* main pipeline stats */
    mpp_stats_t *mp_bg_stats;  /* bg pipeline stats */
    int registration_mode_on;
    int registration_timeout;
    uint8_t *inference_view;    /* image seen by inference element (RGB888) */
    char registration_name[NAME_SIZE];
    bool image_brightness_ok;   /* image brightness check result */
    bool image_contrast_ok;     /* image contrast check result */
} user_data_t;

typedef struct _mpp_app_params_t {
    volatile uint16_t *mcmgr_event_data_p;
    struct rpmsg_lite_instance *rpmsg_inst_p;
    float recognition_threshold;
} mpp_app_params_t;

#endif /* APP_TYPES_H_ */
