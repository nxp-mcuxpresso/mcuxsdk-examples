/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "fsl_common.h"
#include <stdbool.h>

#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_ANTISPOOFING_INFO

#define VIEW_SRC_WIDTH  RGB_CAMERA_WIDTH
#define VIEW_SRC_HEIGHT RGB_CAMERA_HEIGHT
#define INF_SRC_WIDTH   IR_CAMERA_WIDTH
#define INF_SRC_HEIGHT  IR_CAMERA_HEIGHT

#define LOGO_WIDTH     (320)
#define LOGO_HEIGHT    (150)
#define LOGO_BPP       (3)
#define LOGO_LEFT_POS  (0)
#define LOGO_TOP_POS   (0)
#define LOGO_RIGHT_POS (LOGO_HEIGHT - 1)
#define LOGO_BOTTOM_POS (LOGO_WIDTH - 1)

#define TEXT_WIDTH     (320)
#define TEXT_HEIGHT    (570)
#define TEXT_BPP       (2)
#define TEXT_LEFT_POS  (LOGO_HEIGHT)
#define TEXT_TOP_POS   (0)
#define TEXT_RIGHT_POS (TEXT_LEFT_POS + TEXT_HEIGHT - 1)
#define TEXT_BOTTOM_POS (TEXT_TOP_POS + TEXT_WIDTH - 1)

#define MAX_STRING_SIZE 64
#define MAX_WORD_SIZE 32

/* inference preview area positioning */
#define INFPVW_WIDTH  MAX(MOBILEFACENET_WIDTH, ANTISPOOFING_WIDTH)
#define INFPVW_HEIGHT MAX(MOBILEFACENET_HEIGHT, ANTISPOOFING_HEIGHT)
#define INFPVW_BPP    3  /* RGB888 */
#define INFPVW_LEFT_POS  (0)
#define INFPVW_TOP_POS   (0)
#define INFPVW_RIGHT_POS (INFPVW_HEIGHT - 1)
#define INFPVW_BOTTOM_POS (INFPVW_WIDTH - 1)

/*
 * Configure the view resolution (before any rotation & scaling for display):
 */
#define VIEW_HEIGHT VIEW_SRC_HEIGHT
#define VIEW_WIDTH  VIEW_SRC_WIDTH
#define VIEW_SMALL_DIM MIN(VIEW_WIDTH, VIEW_HEIGHT)
#define VIEW_LARGE_DIM MAX(VIEW_WIDTH, VIEW_HEIGHT)

#define INF_SMALL_DIM MIN(INF_SRC_WIDTH, INF_SRC_HEIGHT)
#define INF_LARGE_DIM MAX(INF_SRC_WIDTH, INF_SRC_HEIGHT)

/* TODO remove this HACK and and include model info header file */
#define MOBILEFACENET_PIXEL_FORMAT MPP_PIXEL_BGR
#define SCRFD_KPS_PIXEL_FORMAT MPP_PIXEL_RGB
#define ANTISPOOFING_PIXEL_FORMAT MPP_PIXEL_GRAY
/* end of TODO */

/*
 * SRC_DISPLAY_FLIP = FLIP_NONE if a static image is used as source
 * SRC_DISPLAY_FLIP = FLIP_HORIZONTAL if a camera is used as source
 */
#define SRC_DISPLAY_FLIP FLIP_HORIZONTAL

/* display small & large dims */
#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM MAX(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

#define RECT_LINE_WIDTH 2

/*
 * Configure the scaled view (after rotation and scaling):
 */
/* if display_aspect_ratio > view_aspect_ratio */
#if (DISPLAY_LARGE_DIM * VIEW_SMALL_DIM > VIEW_LARGE_DIM * DISPLAY_SMALL_DIM)
#define SCALED_VIEW_WIDTH APP_DISPLAY_WIDTH
#define SCALED_VIEW_HEIGHT (APP_DISPLAY_WIDTH * VIEW_WIDTH / VIEW_HEIGHT )
#else /* if display_aspect_ratio < view_aspect_ratio */
#define SCALED_VIEW_WIDTH (APP_DISPLAY_HEIGHT * VIEW_HEIGHT / VIEW_WIDTH )
#define SCALED_VIEW_HEIGHT APP_DISPLAY_HEIGHT
#endif

/*
 * Assuming that the output should be in landscape, SWAP_DIMS is defined depending on the
 * orientation of the view.
 * SWAP_DIMS = 1 if view is not already in landscape (width and height need to be swapped)
 * SWAP_DIMS = 0 if view is in landscape.
 */
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
#define APP_SKIP_CONVERT_FOR_DISPLAY 0
#endif

/* TODO rework */
#if (APP_SKIP_CONVERT_FOR_DISPLAY == 1)
#define SWAP_DIMS 0
#else
#define SWAP_DIMS ((VIEW_WIDTH < VIEW_HEIGHT) ? 1 : 0)
#endif

/* The detection zone is a rectangle that has the same shape as the model input.
 * The rectangle dimensions are calculated based on the display small dim and respecting the model aspect ratio
 * The detection zone width and height depend on the view_aspect_ratio compared to the model aspect_ratio:
 * if the view_aspect_ratio >= model_aspect_ratio then :
 *                  (width, height) = (view_small_dim * model_aspect_ratio, view_small_dim)
 * if the view_aspect_ratio < model_aspect_ratio then :
 *                  (width, height) = (view_small_dim, view_small_dim / model_aspect_ratio)
 *
 **/
#if ((VIEW_WIDTH * SCRFD_KPS_WIDTH) >= (VIEW_HEIGHT * SCRFD_KPS_HEIGHT))
#define DETECTION_ZONE_RECT_HEIGHT VIEW_HEIGHT
#define DETECTION_ZONE_RECT_WIDTH  (VIEW_HEIGHT * SCRFD_KPS_WIDTH / SCRFD_KPS_HEIGHT)
#else
#define DETECTION_ZONE_RECT_HEIGHT (VIEW_WIDTH * SCRFD_KPS_HEIGHT / SCRFD_KPS_WIDTH)
#define DETECTION_ZONE_RECT_WIDTH  VIEW_WIDTH
#endif

/* detection zone top/left offsets */
#define DETECTION_ZONE_RECT_TOP (VIEW_SMALL_DIM - DETECTION_ZONE_RECT_HEIGHT)/2
#define DETECTION_ZONE_RECT_LEFT ((VIEW_LARGE_DIM - DETECTION_ZONE_RECT_WIDTH)/2)

/* face detection accuracy threshold to trig recognition */
#define FACE_DET_THRESHOLD 95

/* to run recognition on detected face, need to increase detected box size by a ratio in percent */
#define FACE_BOX_RATIO 100

/* recognition zone size % of detection zone */
#define RECO_SIZE_PERCENT 100
#define RECO_ZONE_RECT_HEIGHT (DETECTION_ZONE_RECT_HEIGHT * RECO_SIZE_PERCENT / 100)
#define RECO_ZONE_RECT_WIDTH  (DETECTION_ZONE_RECT_WIDTH * RECO_SIZE_PERCENT / 100)

#define RECO_ZONE_RECT_TOP ((VIEW_SMALL_DIM - RECO_ZONE_RECT_HEIGHT)/2)
#define RECO_ZONE_RECT_LEFT ((VIEW_LARGE_DIM - RECO_ZONE_RECT_WIDTH)/2)
#define RECO_ZONE_RECT_RIGHT (RECO_ZONE_RECT_LEFT + RECO_ZONE_RECT_WIDTH - 1)
#define RECO_ZONE_RECT_BOTTOM (RECO_ZONE_RECT_TOP + RECO_ZONE_RECT_HEIGHT - 1)

/*
 *  The computation of the crop size(width and height) and the crop top/left depends on the detection
 *  zone dims and offsets and on the source-display scaling factor SF which is calculated differently
 *  depending on 2 constraints:
 *           * Constraint 1: view aspect ratio compared to the source aspect ratio.
 *           * Constraint 2: SWAP_DIMS value.
 * if the display_aspect_ratio < source_aspect_ratio :
 *            - SWAP_DIMS = 0: SF = VIEW_WIDTH / SRC_WIDTH
 *            - SWAP_DIMS = 1: SF = VIEW_HEIGHT / SRC_HEIGHT
 * if the display_aspect_ratio >= source_aspect_ratio:
 *            - SWAP_DIMS = 0: SF = VIEW_HEIGHT / SRC_HEIGHT
 *            - SWAP_DIMS = 1: SF = VIEW_WIDTH / SRC_WIDTH
 * the crop dims and offsets are calculated in the following way:
 * CROP_SIZE_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_SIZE_LEFT = DETECTION_ZONE_RECT_WIDTH / SF
 * CROP_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_LEFT = DETECTION_ZONE_RECT_LEFT / SF
 * */
#if ((VIEW_LARGE_DIM * INF_SRC_HEIGHT) < (VIEW_SMALL_DIM * INF_SRC_WIDTH))
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * INF_SRC_WIDTH) / VIEW_WIDTH)
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * INF_SRC_WIDTH) / VIEW_WIDTH)

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * INF_SRC_WIDTH) / VIEW_WIDTH)
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * INF_SRC_WIDTH) / VIEW_WIDTH)
#else   /* DISPLAY_ASPECT_RATIO() >= SOURCE_ASPECT_RATIO() */
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * INF_SRC_HEIGHT) / VIEW_HEIGHT)

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#endif  /* DISPLAY_ASPECT_RATIO() < SOURCE_ASPECT_RATIO() */

#define RECO_CROP_TOP  ((RECO_ZONE_RECT_TOP * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#define RECO_CROP_LEFT ((RECO_ZONE_RECT_LEFT * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#define RECO_CROP_SIZE_TOP   ((RECO_ZONE_RECT_HEIGHT * INF_SRC_HEIGHT) / VIEW_HEIGHT)
#define RECO_CROP_SIZE_LEFT  ((RECO_ZONE_RECT_WIDTH * INF_SRC_HEIGHT) / VIEW_HEIGHT)

/* Detected boxes offsets */
#define BOXES_OFFSET_LEFT DETECTION_ZONE_RECT_LEFT
#define BOXES_OFFSET_TOP  DETECTION_ZONE_RECT_TOP

/* max x&y distance between face center and recognition-zone center */
#define MAX_CENTER_DIST (RECO_CROP_SIZE_TOP * 10 / 100)  // set 10% of recognition-zone size

/* min face detection area */
#define MIN_FACE_AREA (RECO_ZONE_RECT_HEIGHT * RECO_ZONE_RECT_WIDTH * 4 / 100) //  4% of recognition-zone size (from view resolution)
#define MAX_FACE_AREA (RECO_ZONE_RECT_HEIGHT * RECO_ZONE_RECT_WIDTH * 30 / 100) // 30% of recognition-zone size (from view resolution)

#define FRAME_ASP_RATIO    ((float)VIEW_SRC_WIDTH / VIEW_SRC_HEIGHT)

#define ABS(a) ((a)>=0 ? a: -(a))

#define OUTPUT_PRINT_PERIOD_MS 100  // console print period
#define OUTPUT_NOTIFY_PERIOD_MS 300 // on-screen notification duration

#define USER_DATA_WAIT_TIMEOUT_MS   5000

#define FACE_LABEL_OUTSIDE          "face outside zone"
#define FACE_LABEL_CENTER           "face not centered"
#define FACE_LABEL_FAR              "face too far"
#define FACE_LABEL_NOT_ALIGNED      "Face not aligned"
#define FACE_LABEL_CLOSE            "face too close"
#define FACE_LABEL_UNCLEAR          "face unclear"
#define FACE_LABEL_POOR_QUALITY     "poor image quality"
#define FACE_LABEL_POOR_BRIGHTNESS  "poor brightness"
#define FACE_LABEL_POOR_CONTRAST    "poor contrast"
#define FACE_LABEL_OK               "face ok"
#define ZONE_LABEL_REGISTERED       "face registered as "
#define ZONE_LABEL_REGISTRATION_END "Registration time expired."
#define ZONE_LABEL_RECO             "face recognition zone"
#define ZONE_LABEL_RECOGNIZING      "Recognizing ... "
#define ZONE_LABEL_REGISTERING      "Registering ..."
#define ZONE_LABEL_RECOGNIZED       "Hello "
#define ZONE_LABEL_NOT_RECOGNIZED   "Face not recognized"
#define ZONE_LABEL_FAKE             "fake Face "
#define ZONE_LABEL_REAL             "real Face "
#define ZONE_LABEL_ANTISPOOFING     "checking liveness..."

#define MAX_LABEL_RECTS     10
/* Rectangle indices */
#define ZONE_BOX_INDEX      0
#define FIRST_FACE_INDEX    1
#define NUM_BOXES_MAX       80
#define LANDMARK_POINT_SIZE 2

/* Image quality thresholds */
#define MIN_BRIGHTNESS_THRESHOLD 20
#define MAX_BRIGHTNESS_THRESHOLD 210
#define MIN_CONTRAST_THRESHOLD   25
#define MAX_CONTRAST_THRESHOLD   220

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

/* define this debug flag to enable recognition preview */
#undef DEBUG_PREVIEW_RECOGNITION

/* defines used for configuration of virtual camera element */
#define IN_ADVANCE_FRAME_ENQUEUE       true
#define DISPLAY_STREAM_TYPE            RGB_STREAM
#define INFERENCE_STREAM_TYPE          IR_STREAM

#endif  /* APP_CONSTANTS_H */
