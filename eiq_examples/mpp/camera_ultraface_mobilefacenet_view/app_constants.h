/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "fsl_common.h"

/* TODO remove this HACK and and include model info header file */
#define MOBILEFACENET_WIDTH 96
#define MOBILEFACENET_HEIGHT 96
#define ULTRAFACE_WIDTH 128
#define ULTRAFACE_HEIGHT 128
/* end of TODO */

/*
 * SWAP_DIMS = 1 if source/display dims are reversed
 * SWAP_DIMS = 0 if source/display have the same orientation
 */
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
#define SWAP_DIMS 0
#else
#define SWAP_DIMS (((APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_90) || (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_270)) ? 1 : 0)
#endif

/*
 * SRC_DISPLAY_FLIP = FLIP_NONE if a static image is used as source
 * SRC_DISPLAY_FLIP = FLIP_HORIZONTAL if a camera is used as source
 */
#define SRC_DISPLAY_FLIP FLIP_HORIZONTAL

/* display small & large dims */
#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM MAX(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

#define RECT_LINE_WIDTH 2
#define SRC_WIDTH  APP_CAMERA_WIDTH
#define SRC_HEIGHT APP_CAMERA_HEIGHT

/* source large & small dims */
#define SRC_LARGE_DIM MAX(SRC_WIDTH,SRC_HEIGHT)
#define SRC_SMALL_DIM MIN(SRC_WIDTH,SRC_HEIGHT)

/* models aspect ratio */
#define RECO_ASPECT_RATIO   (1.0f * MOBILEFACENET_WIDTH / MOBILEFACENET_HEIGHT)
#define DETECT_ASPECT_RATIO   (1.0f * ULTRAFACE_WIDTH / ULTRAFACE_HEIGHT)
/* output is displayed in landscape mode */
#define DISPLAY_ASPECT_RATIO (1.0f * DISPLAY_LARGE_DIM / DISPLAY_SMALL_DIM)
/* camera aspect ratio */
#define CAMERA_ASPECT_RATIO  (1.0f * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT)

/* The detection zone is a rectangle that has the same shape as the model input.
 * The rectangle dimensions are calculated based on the display small dim and respecting the model aspect ratio
 * The detection zone width and height depend on the display_aspect_ratio compared to the model aspect_ratio:
 * if the display_aspect_ratio >= model_aspect_ratio then :
 *                  (width, height) = (display_small_dim * model_aspect_ratio, display_small_dim)
 * if the display_aspect_ratio < model_aspect_ratio then :
 *                  (width, height) = (display_small_dim, display_small_dim / model_aspect_ratio)
 *
 * */
#define DETECTION_ZONE_RECT_HEIGHT ((DISPLAY_ASPECT_RATIO >= DETECT_ASPECT_RATIO) ? \
        DISPLAY_SMALL_DIM : (DISPLAY_SMALL_DIM / DETECT_ASPECT_RATIO))
#define DETECTION_ZONE_RECT_WIDTH  ((DISPLAY_ASPECT_RATIO >= DETECT_ASPECT_RATIO) ? \
        (DISPLAY_SMALL_DIM * DETECT_ASPECT_RATIO) : DISPLAY_SMALL_DIM)

/* recognition zone size 70% of detection zone */
#define RECO_ZONE_RECT_HEIGHT (DETECTION_ZONE_RECT_HEIGHT * 70 / 100)
#define RECO_ZONE_RECT_WIDTH  (DETECTION_ZONE_RECT_WIDTH * 70 / 100)

/*
 * The detection zone offsets are defined in the following way:
 * The static image is placed in the top left corner of the display and its detection zone is centered on image.
 * The camera output detection zone is placed in the middle of the display.
 *
 * */
#define DETECTION_ZONE_RECT_TOP (DISPLAY_SMALL_DIM - DETECTION_ZONE_RECT_HEIGHT)/2
#define RECO_ZONE_RECT_TOP (DISPLAY_SMALL_DIM - RECO_ZONE_RECT_HEIGHT)/2

/* SRC_ZOOM is zoom factor applied on source, it is used to compute size of SRC on screen */
#if ((DISPLAY_LARGE_DIM * SRC_HEIGHT) < (DISPLAY_SMALL_DIM * SRC_WIDTH))
#define SRC_ZOOM (1.0f * DISPLAY_LARGE_DIM / SRC_WIDTH)
#else
#define SRC_ZOOM (1.0f * DISPLAY_SMALL_DIM / SRC_HEIGHT)
#endif
/*
 * DETECTION_ZONE_RECT_LEFT = ((SRC_LARGE_DIM * SRC_ZOOM) - DETECTION_ZONE_RECT_WIDTH)/2 if a static image is used as source
 * DETECTION_ZONE_RECT_LEFT = (DISPLAY_LARGE_DIM - DETECTION_ZONE_RECT_WIDTH)/2 if a camera is used as source
 */
#define DETECTION_ZONE_RECT_LEFT ((DISPLAY_LARGE_DIM - DETECTION_ZONE_RECT_WIDTH)/2)
#define RECO_ZONE_RECT_LEFT ((DISPLAY_LARGE_DIM - RECO_ZONE_RECT_WIDTH)/2)
#define RECO_ZONE_RECT_RIGHT (RECO_ZONE_RECT_LEFT + RECO_ZONE_RECT_WIDTH - 1)
#define RECO_ZONE_RECT_BOTTOM (RECO_ZONE_RECT_TOP + RECO_ZONE_RECT_HEIGHT - 1)

/*
 *  The computation of the crop size(width and height) and the crop top/left depends on the detection
 *  zone dims and offsets and on the source-display scaling factor SF which is calculated differently
 *  depending on 2 constraints:
 *           * Constraint 1: display aspect ratio compared to the source aspect ratio.
 *           * Constraint 2: SWAP_DIMS value.
 * if the display_aspect_ratio < source_aspect_ratio :
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_WIDTH / SRC_WIDTH
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_HEIGHT / SRC_HEIGHT
 * if the display_aspect_ratio >= source_aspect_ratio:
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_HEIGHT / SRC_HEIGHT
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_WIDTH / SRC_WIDTH
 * the crop dims and offsets are calculated in the following way:
 * CROP_SIZE_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_SIZE_LEFT = DETECTION_ZONE_RECT_WIDTH / SF
 * CROP_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_LEFT = DETECTION_ZONE_RECT_LEFT / SF
 * */
#if ((DISPLAY_LARGE_DIM * SRC_HEIGHT) < (DISPLAY_SMALL_DIM * SRC_WIDTH))
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * SRC_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * SRC_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * SRC_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * SRC_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#else   /* DISPLAY_ASPECT_RATIO() >= SOURCE_ASPECT_RATIO() */
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#endif  /* DISPLAY_ASPECT_RATIO() < SOURCE_ASPECT_RATIO() */

#define RECO_CROP_TOP  ((RECO_ZONE_RECT_TOP * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define RECO_CROP_LEFT ((RECO_ZONE_RECT_LEFT * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define RECO_CROP_SIZE_TOP   ((RECO_ZONE_RECT_HEIGHT * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define RECO_CROP_SIZE_LEFT  ((RECO_ZONE_RECT_WIDTH * SRC_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))

/* Detected boxes offsets */
#define BOXES_OFFSET_LEFT DETECTION_ZONE_RECT_LEFT
#define BOXES_OFFSET_TOP  DETECTION_ZONE_RECT_TOP

/* max x&y distance between face center and recognition-zone center */
#define MAX_CENTER_DIST (RECO_CROP_SIZE_TOP * 10 / 100)  // set 10% of recognition-zone size

/* min face detection area */
#define MIN_FACE_AREA (RECO_CROP_SIZE_TOP * RECO_CROP_SIZE_LEFT * 30 / 100) // 30% of recognition-zone size
#define MAX_FACE_AREA (RECO_CROP_SIZE_TOP * RECO_CROP_SIZE_LEFT * 50 / 100) // 50% of recognition-zone size

#define ABS(a) ((a)>=0 ? a: -(a))

#define OUTPUT_PRINT_PERIOD_MS 1000  // console print period
#define OUTPUT_NOTIFY_PERIOD_MS 3000 // on-screen notification duration

#define FACE_LABEL_OUTSIDE  "face outside zone"
#define FACE_LABEL_CENTER   "face not centered"
#define FACE_LABEL_FAR      "face too far"
#define FACE_LABEL_CLOSE    "face too close"
#define FACE_LABEL_OK       "face ok"
#define ZONE_LABEL_REGISTERED   "face registered as "
#define ZONE_LABEL_RECO         "face recognition zone"
#define ZONE_LABEL_REGISTERING  "recognizing face..."
#define ZONE_LABEL_RECOGNIZED   "Hello "

#define MAX_LABEL_RECTS     10
#define NUM_BOXES_MAX       80

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

#endif  /* APP_CONSTANTS_H */
