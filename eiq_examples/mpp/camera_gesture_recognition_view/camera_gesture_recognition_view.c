/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @brief This example application shows usage of MultiMedia Pipeline to build a gesture recognition graph with multiple branches:
 *
 * Pipeline Architecture:
 * 2D camera -> [optional JPEG decoder] -> split into 2 branches:
 *
 * Branch 1 (Hand Detection):
 *   -> image converter -> background task: Blaze hand detector (TFLite) -> null sink
 *
 * Branch 2 (Hand Landmark & Gesture Recognition):
 *   -> image converter -> background task: Hand landmark detector (TFLite)
 *                      -> Gesture embedder (TFLite)
 *                      -> Gesture classifier (TFLite) -> null sink
 *
 * Display Branch (from camera source):
 *   -> [optional image converter] -> draw labeled rectangles and landmarks -> [optional rotate] -> display
 *
 * Features:
 * - Dynamic pipeline switching: Blaze detector runs until hand is detected, then switches to landmark/gesture pipeline
 * - Hand landmark detection provides 21 3D keypoints for gesture recognition
 * - Gesture classification identifies hand gestures (e.g., thumbs up, peace sign, etc.)
 * - Real-time visualization of detection zone, bounding boxes, and hand landmarks on display
 * - Stripe mode support for reduced memory footprint (when supported by camera and display)
 * - All inference models run in preemptable background tasks using TensorFlow Lite Micro
 * - Results (hand detection, landmarks, gesture classification, inference times) displayed on UART console
 */

#include <math.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "atomic.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"

#include "mpp_api_types_internal.h"
#include "hal_vdec_dev.h"

#include "draw_text.h"

#ifndef APP_STRIPE_MODE
#define APP_STRIPE_MODE 0
#endif

#ifndef PRINT_API_STATS
#define PRINT_API_STATS 0
#endif

#ifndef PRINT_MPP_STATS
#define PRINT_MPP_STATS 0
#endif

#ifndef PRINT_ELEM_STATS
#define PRINT_ELEM_STATS 0
#endif

#ifndef DEBUG_FRAME_ID
#define DEBUG_FRAME_ID 0
#endif

#ifndef DISPLAY_BLAZE_PALM_RECTS_LDMKS
#define DISPLAY_BLAZE_PALM_RECTS_LDMKS 0
#endif

#ifndef PRINT_JPEG_VALIDATION_STATS
#define PRINT_JPEG_VALIDATION_STATS 0
#endif

#ifndef DISPLAY_TEXT
#define DISPLAY_TEXT 1
#endif

/* Model data input */
#include APP_TFLITE_BLAZE_DETECTOR_PTQ_DATA
#include APP_TFLITE_HAND_LANDMARK_DATA
#include APP_TFLITE_GESTURE_EMBEDDER_DATA
#include APP_TFLITE_GESTURE_CLASSIFIER_DATA

/* Model info */
#include APP_TFLITE_BLAZE_DETECTOR_PTQ_INFO
#include APP_TFLITE_HAND_LANDMARK_INFO
#include APP_TFLITE_GESTURE_EMBEDDER_INFO
#include APP_TFLITE_GESTURE_CLASSIFIER_INFO

#include "blaze_detector_ptq_output_postproc.h"
#include "hand_landmark_quant_output_postproc.h"
#include "canned_gesture_classifier_output_postproc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * SWAP_DIMS = 1 if source/display dims are reversed
 * SWAP_DIMS = 0 if source/display have the same orientation
 */
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
#define SWAP_DIMS 0
#else
#define SWAP_DIMS (((APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_90) || (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_270)) ? 1 : 0)
#endif

/* display small and large dims */
#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM MAX(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

/* View small and large dims */
#define VIEW_SMALL_DIM MIN(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT)
#define VIEW_LARGE_DIM MAX(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT)

/* if display_aspect_ratio > view_aspect_ratio */
#if (DISPLAY_LARGE_DIM * VIEW_SMALL_DIM > VIEW_LARGE_DIM * DISPLAY_SMALL_DIM)
#define SCALED_VIEW_WIDTH APP_DISPLAY_WIDTH
#define SCALED_VIEW_HEIGHT (APP_DISPLAY_WIDTH * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT )
#else /* if display_aspect_ratio < view_aspect_ratio */
#define SCALED_VIEW_WIDTH (APP_DISPLAY_HEIGHT * APP_CAMERA_HEIGHT / APP_CAMERA_WIDTH )
#define SCALED_VIEW_HEIGHT APP_DISPLAY_HEIGHT
#endif

#define MODEL_ASPECT_RATIO   (1.0f * BLAZE_DETECTOR_WIDTH / BLAZE_DETECTOR_HEIGHT)
/* output is displayed in landscape mode */
#define DISPLAY_ASPECT_RATIO (1.0f * DISPLAY_LARGE_DIM / DISPLAY_SMALL_DIM)
/* camera aspect ratio */
#define CAMERA_ASPECT_RATIO  (1.0f * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT)

/*
 * The detection zone is a rectangle centered on the camera view. It has the same shape as the model input.
 * The rectangle dimensions are calculated based on the camera height and respecting the model aspect ratio
 * The detection zone width and height depend on the camera_aspect_ratio compared to the model_aspect_ratio:
 * if the camera_aspect_ratio >= model_aspect_ratio then :
 *                  (width, height) = (camera_height * model_aspect_ratio, camera_height)
 * if the camera_aspect_ratio < model_aspect_ratio then :
 *                  (width, height) = (camera_height, camera_height / model_aspect_ratio)
 *
 * */
#define DETECTION_ZONE_RECT_HEIGHT ((CAMERA_ASPECT_RATIO >= MODEL_ASPECT_RATIO) ? \
        APP_CAMERA_HEIGHT : (APP_CAMERA_HEIGHT / MODEL_ASPECT_RATIO))
#define DETECTION_ZONE_RECT_WIDTH  ((CAMERA_ASPECT_RATIO >= MODEL_ASPECT_RATIO) ? \
        (APP_CAMERA_HEIGHT * MODEL_ASPECT_RATIO) : APP_CAMERA_HEIGHT)

/* detection zone top/left offsets */
#define DETECTION_ZONE_RECT_TOP  (APP_CAMERA_HEIGHT - DETECTION_ZONE_RECT_HEIGHT)/2
#define DETECTION_ZONE_RECT_LEFT (APP_CAMERA_WIDTH - DETECTION_ZONE_RECT_WIDTH)/2

/*
  * Crop parameters are the same as the DETECTION_ZONE parameters
  * They define the region of interest from the camera frame that will be fed to the model
  * Because the labeled rectangles are added before upscaling and other operations needed for display,
  * it is safe to assume that the crop parameters have the same values as the detection zone parameters
  */
#define CROP_SIZE_TOP       DETECTION_ZONE_RECT_HEIGHT
#define CROP_SIZE_LEFT      DETECTION_ZONE_RECT_WIDTH
#define CROP_TOP            DETECTION_ZONE_RECT_TOP
#define CROP_LEFT           DETECTION_ZONE_RECT_LEFT

/* Detected boxes offsets */
#define BOXES_OFFSET_LEFT DETECTION_ZONE_RECT_LEFT
#define BOXES_OFFSET_TOP  DETECTION_ZONE_RECT_TOP

#define OUTPUT_PRINT_PERIOD_MS 1000

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] =  APP_CAMERA_NAME;

#define BLAZE_DETECTION_LABEL "hand"

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

#define MAX_LABEL_RECTS     10
#define MAX_LANDMARKS       MAX(MODEL_NUM_LANDMARKS, MODEL_NUM_3D_LANDMARKS)
#define NUM_BOXES_MAX       MIN(APP_MAX_BOXES, BLAZE_DETECTOR_NUM_BOXES) /* max nb of boxes to filter */

/* landmark line width */
#define LANDMARK_POINT_SIZE 4
/* label rect line width */
#define RECT_LINE_WIDTH 2

/* Text area dimensions and positioning */
#define TEXT_WIDTH          (320)
#define TEXT_HEIGHT         (720)
#define TEXT_BPP            (2)
#define TEXT_LEFT_POS_90    (0)
#define TEXT_TOP_POS_90     (960)
#define TEXT_RIGHT_POS_90   (TEXT_LEFT_POS_90 + TEXT_HEIGHT - 1)
#define TEXT_BOTTOM_POS_90  (TEXT_TOP_POS_90 + TEXT_WIDTH - 1)
#define TEXT_LEFT_POS_270   (0)
#define TEXT_TOP_POS_270    (0)
#define TEXT_RIGHT_POS_270  (TEXT_LEFT_POS_270 + TEXT_HEIGHT - 1)
#define TEXT_BOTTOM_POS_270 (TEXT_TOP_POS_270 + TEXT_WIDTH - 1)

typedef struct _convert_params_t {
    uint32_t left;
    uint32_t right;
    uint32_t top;
    uint32_t bottom;
    float custom_rotation;
    float vglite_m[3][3];
} convert_params_t;

/* New structure for 2D landmark conversion parameters */
typedef struct _landmark_convert_params_t {
    const coord_t* src_landmark;
    mpp_landmark_t* dst_landmark;
    uint32_t hand_idx;
    uint32_t landmark_idx;
    mpp_color_t *color;
} landmark_convert_params_t;

/* New structure for 3D landmark conversion parameters */
typedef struct _landmark_3d_convert_params_t {
    const coord_3d_t* src_landmark;
    mpp_landmark_t* dst_landmark;
    uint32_t hand_idx;
    uint32_t landmark_idx;
    uint32_t detection_w;
    uint32_t detection_h;
    mpp_color_t *color;
    uint32_t offset_left;
    uint32_t offset_top;
} landmark_3d_convert_params_t;

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_t blaze_detector_input_convert_mp;
    mpp_t blaze_detector_mp;
    mpp_t landmark_input_convert_mp;
    mpp_t landmark_mp;
    mpp_elem_handle_t label_rect_elem;
    mpp_elem_handle_t blaze_palm_elem;
    mpp_elem_handle_t landmark_input_convert_elem;
    mpp_elem_handle_t hand_ldmk_elem;
    mpp_elem_handle_t gesture_emb_elem;
    mpp_elem_handle_t gesture_cls_elem;
    mpp_elem_handle_t compose_elem;
    mpp_element_params_t landmark_input_convert_params;
    mpp_element_params_t label_rect_params;
    convert_params_t last_hand_ldk_crop_params;
    box_data boxes[NUM_BOXES_MAX];
    mpp_labeled_rect_t rectangles[MAX_LABEL_RECTS];
    mpp_landmark_t landmarks[MAX_LABEL_RECTS * MAX_LANDMARKS];
    hand_data hand_data;
    float has_hand_rotation;
    float has_hand_matrix[3][3];
    gesture_data gesture_data;
    uint32_t accessing; /* boolean protecting access */
    uint32_t detected_count; /* number of detected boxes */
    uint32_t blaze_detect_inference_time_ms;
    uint32_t hand_ldmk_inference_time_ms;
    uint32_t gesture_emb_inference_time_ms;
    uint32_t gesture_cls_inference_time_ms;
    uint32_t blaze_detector_frame_id;
    uint32_t hand_ldmk_convert_frame_id;
    uint32_t hand_ldmk_frame_id;
} user_data_t;

/* New structure for statistics pointers */
typedef struct _stats_collection_t {
    mpp_stats_t *api_stats;
    mpp_stats_t *mpp_stats;
    mpp_stats_t *mpp_split_stats; /* array of 2 */
    mpp_stats_t *mpp_stats_blaze;
    mpp_stats_t *mpp_stats_bg_ldmk;
    mpp_stats_t *decoder_stats;
    mpp_stats_t *convert_blaze_stats;
    mpp_stats_t *blaze_detector_stats;
    mpp_stats_t *convert_hand_ldmk_stats;
    mpp_stats_t *hand_landmark_stats;
    mpp_stats_t *gesture_embedder_stats;
    mpp_stats_t *gesture_classifier_stats;
    mpp_stats_t *convert_display_stats;
    mpp_stats_t *labels_stats;
#if !DISPLAY_TEXT
    mpp_stats_t *rotate_stats;
#else
    mpp_stats_t *compose_stats;
#endif
} stats_collection_t;

/* info to be dislayed in text area */
typedef struct {
    char gesture[GESTURE_CLASSIFIER_MAX_LABEL_LENGTH];
    float hand_confidence;
    float confidence;
    int left_hand;
    int blaze_detector_inference_time;
    int hand_landmark_inference_time;
    int gesture_embbeder_inference_time;
    int gesture_classifier_inference_time;
} text_info_t;

typedef struct {
    int gesture_start_x;
    int gesture_stop_x;
    int hand_confidence_start_x;
    int hand_confidence_stop_x;
    int confidence_start_x;
    int confidence_stop_x;
    int left_hand_start_x;
    int left_hand_stop_x;
    int blaze_detector_start_x;
    int blaze_detector_stop_x;
    int hand_landmark_start_x;
    int hand_landmark_stop_x;
    int gesture_embedder_start_x;
    int gesture_embedder_stop_x;
    int gesture_classifier_start_x;
    int gesture_classifier_stop_x;
} text_position_t;

/* Define image indices for the composition array */
typedef enum {
    COMPOSE_TEXT_INDEX = 1,
    COMPOSE_MAX_IMAGES
} compose_image_index_t;

/*******************************************************************************
  * Variables
  ******************************************************************************/
text_position_t text_position = {0};
text_context_t  text_ctx = {0};
uint8_t g_text_img[TEXT_WIDTH*TEXT_HEIGHT*TEXT_BPP] __attribute__((aligned(64)));

/*******************************************************************************
  * Prototypes
  ******************************************************************************/
static void app_task(void *params);
void HAL_DCACHE_CleanByRange(uint32_t addr, uint32_t size);

/*******************************************************************************
  * Code
  ******************************************************************************/
 /*!
 * @brief drawing of text by cpu into a buffer.
 */
static void init_text_area(void* buf, uint32_t size, text_info_t* text)
{
    int space4items  = get_font_height() + 6;
    int start_y_middle = get_font_height()/2 + 2;
    int row_indentation = get_font_width()/2;
    int x_pos = 0;

    /*Clear the buffer. */
    memset(buf,0,size);
    init_text_buf(&text_ctx, buf, size, TEXT_WIDTH, TEXT_HEIGHT, TEXT_WIDTH);

    draw_text_line(&text_ctx, "THIS IS A DEMO", row_indentation,start_y_middle);
    start_y_middle += get_font_height();

    draw_text_line(&text_ctx, "Gesture:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    ", row_indentation, start_y_middle );
    text_position.gesture_start_x = x_pos;
    text_position.gesture_stop_x = x_pos;

    start_y_middle += get_font_height();
    draw_text_line(&text_ctx, "Confidence:", row_indentation, start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    ", row_indentation, start_y_middle );
    text_position.confidence_start_x = x_pos;
    text_position.confidence_stop_x = x_pos;

    start_y_middle += get_font_height();
    draw_text_line(&text_ctx, "Hand score:", row_indentation, start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    ", row_indentation, start_y_middle );
    text_position.hand_confidence_start_x = x_pos;
    text_position.hand_confidence_stop_x = x_pos;

    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "Left hand: ", row_indentation, start_y_middle);
    text_position.left_hand_start_x = x_pos;
    text_position.left_hand_start_x = x_pos;

    start_y_middle += space4items;
    draw_text_line(&text_ctx, "Inf times:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    detect:", row_indentation, start_y_middle );
    text_position.blaze_detector_start_x = x_pos;
    text_position.blaze_detector_stop_x = x_pos;
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    ldmk:", row_indentation, start_y_middle );
    text_position.hand_landmark_start_x = x_pos;
    text_position.hand_landmark_stop_x = x_pos;
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    embed:", row_indentation, start_y_middle );
    text_position.gesture_embedder_start_x = x_pos;
    text_position.gesture_embedder_stop_x = x_pos;
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    clasify:", row_indentation, start_y_middle );
    text_position.gesture_classifier_start_x = x_pos;
    text_position.gesture_classifier_stop_x = x_pos;

    //make sure all data are pushed to the memory.
    HAL_DCACHE_CleanByRange((uint32_t)buf,size);
}

 /*!
 * @brief drawing of text by cpu into a buffer.
 */
static void draw_text_area(text_info_t* text)
{
    int space4items  = get_font_height() + 6;
    int start_y_middle = get_font_height()/2 + 2;
    char tmp[GESTURE_CLASSIFIER_MAX_LABEL_LENGTH];
    int x_start, clear_width = 0;

    start_y_middle += 2 * get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%s",text->gesture);
    x_start = text_position.gesture_start_x;
    clear_width = text_position.gesture_stop_x - text_position.gesture_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.gesture_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += 2 * get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%2d%%",(int)(text->confidence));
    x_start = text_position.confidence_start_x;
    clear_width = text_position.confidence_stop_x - text_position.confidence_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.confidence_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += 2 * get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%2d%%",(int)(text->hand_confidence));
    x_start = text_position.hand_confidence_start_x;
    clear_width = text_position.hand_confidence_stop_x - text_position.hand_confidence_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.hand_confidence_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%s",text->left_hand == 1 ? "yes" : "no");
    x_start = text_position.left_hand_start_x;
    clear_width = text_position.left_hand_stop_x - text_position.left_hand_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.left_hand_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += space4items + get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->blaze_detector_inference_time);
    x_start = text_position.blaze_detector_start_x;
    clear_width = text_position.blaze_detector_stop_x - text_position.blaze_detector_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.blaze_detector_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->hand_landmark_inference_time);
    x_start = text_position.hand_landmark_start_x;
    clear_width = text_position.hand_landmark_stop_x - text_position.hand_landmark_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.hand_landmark_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->gesture_embbeder_inference_time);
    x_start = text_position.gesture_embedder_start_x;
    clear_width = text_position.gesture_embedder_stop_x - text_position.gesture_embedder_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.gesture_embedder_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->gesture_classifier_inference_time);
    x_start = text_position.gesture_classifier_start_x;
    clear_width = text_position.gesture_classifier_stop_x - text_position.gesture_classifier_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.gesture_classifier_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    //make sure all data are pushed to the memory.
    HAL_DCACHE_CleanByRange((uint32_t) text_ctx.fb, text_ctx.size);
}

/*!
  * @brief Application entry point.
  */
int main()
{
    BaseType_t ret;
    TaskHandle_t handle = NULL;

    /* Init board hardware. */
    BOARD_Init();

    ret = xTaskCreate(
          app_task,
          "app_task",
          configMINIMAL_STACK_SIZE + 1000,
          NULL,
          APP_DEFAULT_PRIO,
          &handle);

    if (pdPASS != ret) {
        PRINTF("Failed to create app_task task");
        while (1);
    }

    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);
    return 0;
}

void swap_rows(float matrix[3][3 * 2], int row1, int row2) {
    int N = 3;
    for (int j = 0; j < N * 2; j++) {
        float temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}

int inverse_matrix(float (*matrix)[3], float (*inverse)[3]) {
    int N = 3;
    float augmented[3][3 * 2];
    float temp;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            augmented[i][j] = matrix[i][j];
        }
        for (int j = N; j < N * 2; j++) {
            augmented[i][j] = (i == j - N) ? 1.0f : 0.0f;
        }
    }

    for (int i = 0; i < N; i++) {
        if (fabs(augmented[i][i]) < 1e-6) {
            int swapRow = -1;
            for (int k = i + 1; k < N; k++) {
                if (fabs(augmented[k][i]) > 1e-6) {
                    swapRow = k;
                    break;
                }
            }

            if (swapRow == -1) {
                return -1;
            }

            swap_rows(augmented, i, swapRow);
        }

        temp = augmented[i][i];
        for (int j = 0; j < N * 2; j++) {
            augmented[i][j] /= temp;
        }

        for (int k = 0; k < N; k++) {
            if (k != i && fabs(augmented[k][i]) > 1e-6) {
                temp = augmented[k][i];
                for (int j = 0; j < N * 2; j++) {
                    augmented[k][j] -= augmented[i][j] * temp;
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            inverse[i][j] = augmented[i][j + N];
        }
    }

    return 0;
}

/**
  * Convert landmark coordinates from detection space to view dimensions
  */
void convert_landmark_coordinates(const landmark_convert_params_t* params) {
    /* Set basic landmark properties */
    params->dst_landmark->clear = 0; /* don't clear landmark */
    params->dst_landmark->width = LANDMARK_POINT_SIZE; /* landmark point size */
    params->dst_landmark->stripe = false;

    /* Convert coordinates from detection space to view dimensions */
    params->dst_landmark->x = (params->src_landmark->x * DETECTION_ZONE_RECT_WIDTH / BLAZE_DETECTOR_WIDTH) + BOXES_OFFSET_LEFT;
    params->dst_landmark->y = (params->src_landmark->y * DETECTION_ZONE_RECT_HEIGHT / BLAZE_DETECTOR_HEIGHT) + BOXES_OFFSET_TOP;

    /* Encode hand and landmark index in tag */
    params->dst_landmark->tag = (params->hand_idx << 8) | params->landmark_idx;

    /* Set color based on input param */
    params->dst_landmark->color = *params->color;
}

/**
 * Convert 3D landmark coordinates from detection space to view dimensions
 */
void convert_landmark_coordinates_3d(const landmark_3d_convert_params_t* params) {
    /* Set basic landmark properties */
    params->dst_landmark->clear = 0; /* don't clear landmark */
    params->dst_landmark->width = LANDMARK_POINT_SIZE; /* landmark point size */
    params->dst_landmark->stripe = false;

    /* Convert coordinates from detection space to view dimensions */
    params->dst_landmark->x = (((int16_t)params->src_landmark->x) * params->detection_w / HAND_LANDMARK_WIDTH) + params->offset_left;
    params->dst_landmark->y = (((int16_t)params->src_landmark->y) * params->detection_h / HAND_LANDMARK_HEIGHT) + params->offset_top;

    /* Encode hand and landmark index in tag */
    params->dst_landmark->tag = (params->hand_idx << 8) | params->landmark_idx;

    /* Set color based on input param */
    params->dst_landmark->color = *params->color;
}


/* Translate boxes into labeled rectangles using display characteristics */
void boxes_to_rects(box_data boxes[], uint32_t num_boxes, uint32_t max_boxes, mpp_labeled_rect_t *rects)
{
    uint32_t box_counter = 1;

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes && box_counter < max_boxes; i++) {
        if (boxes[i].area == 0)
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes[i].left * DETECTION_ZONE_RECT_WIDTH)/ BLAZE_DETECTOR_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].right = (int)((boxes[i].right * DETECTION_ZONE_RECT_WIDTH)/ BLAZE_DETECTOR_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].bottom = (int)((boxes[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/BLAZE_DETECTOR_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].top = (int)((boxes[i].top * DETECTION_ZONE_RECT_HEIGHT)/BLAZE_DETECTOR_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;
        uint8_t label_size = sizeof(rects[box_counter].label);
        strncpy((char *) rects[box_counter].label, BLAZE_DETECTION_LABEL, label_size-1);
        rects[box_counter].label[label_size-1] = '\0';  /* in case label has been truncated */

        box_counter++;
    }
}

/*******************************************************************************
 * Event Handler Functions
 ******************************************************************************/

/**
 * Handle convert parameters ready event for hand landmark conversion
 */
static void handle_convert_params_ready(user_data_t *app_priv, const mpp_convert_cb_param_t *convert_params)
{
    switch (convert_params->id) {
    case HAND_LANDMARK_CONVERT_ELEM_ID:
#if DEBUG_FRAME_ID
        _elem_t *elem = (_elem_t *) app_priv->landmark_input_convert_elem;
        app_priv->hand_ldmk_frame_id = elem->io.in_buf[0]->frame_id;
        PRINTF("Converted frame id %d with params %d %d %d %d\r\n", app_priv->hand_ldmk_frame_id,
                            convert_params->left,
                            convert_params->top,
                            convert_params->right,
                            convert_params->bottom);
#endif
        app_priv->last_hand_ldk_crop_params.left = convert_params->left;
        app_priv->last_hand_ldk_crop_params.top = convert_params->top;
        app_priv->last_hand_ldk_crop_params.right = convert_params->right;
        app_priv->last_hand_ldk_crop_params.bottom = convert_params->bottom;
        app_priv->last_hand_ldk_crop_params.custom_rotation = convert_params->rotation;
        memcpy((void *)&app_priv->last_hand_ldk_crop_params.vglite_m[0],
               (void *)convert_params->vg_lite_m,
                sizeof(app_priv->last_hand_ldk_crop_params.vglite_m));
        break;

    default:
        /* nothing to do */
        break;
    }
}

/**
 * Update labeled rectangles and landmarks for blaze palm detector
 */
static void update_blaze_palm_labels(user_data_t *app_priv, uint32_t hand_idx)
{
#if DISPLAY_BLAZE_PALM_RECTS_LDMKS
    /* update labeled rectangle */
    if ( (app_priv->mp != NULL) && (app_priv->label_rect_elem != 0) ) {
        /* detected_count contains at least the detection zone box */
        app_priv->label_rect_params.labels.detected_rect = app_priv->detected_count + 1;
        app_priv->label_rect_params.labels.max_rect = MAX_LABEL_RECTS;
        app_priv->label_rect_params.labels.rectangles = app_priv->rectangles;

        /* Clear landmarks array */
        memset(&app_priv->landmarks[0], 0, sizeof(app_priv->landmarks));

        /* params init */
        app_priv->label_rect_params.labels.max_landmk = MAX_LABEL_RECTS * MAX_LANDMARKS;
        app_priv->label_rect_params.labels.detected_landmk = 0;
        app_priv->label_rect_params.labels.landmarks = app_priv->landmarks;

        /* Fill landmarks for each detected hand */
        uint32_t landmark_idx = 0;
        mpp_color_t landmark_color = {.rgb = {.R = 0xff, .G = 0x00, .B = 0x00}};
        /* Add all landmarks for this hand */
        if (app_priv->detected_count)
        {
            for (uint32_t lm_idx = 0; lm_idx < MODEL_NUM_LANDMARKS && landmark_idx < (MAX_LABEL_RECTS * MAX_LANDMARKS); lm_idx++) {
                landmark_convert_params_t convert_params = {
                    .src_landmark = &app_priv->boxes[hand_idx].landmarks[lm_idx],
                    .dst_landmark = &app_priv->landmarks[landmark_idx],
                    .hand_idx = hand_idx,
                    .landmark_idx = lm_idx,
                    .color = &landmark_color
                };
                convert_landmark_coordinates(&convert_params);
                landmark_idx++;
                app_priv->label_rect_params.labels.detected_landmk++;
            }
        }
        /* Needed to clear previous landmarks if none detected */
        if (app_priv->label_rect_params.labels.detected_landmk == 0)
        {
            app_priv->label_rect_params.labels.detected_landmk = 1;
            app_priv->landmarks[0].clear = 1;
        }

        if (mpp_element_update(app_priv->mp, app_priv->label_rect_elem, &app_priv->label_rect_params, true) != MPP_SUCCESS)
            PRINTF("Error: Failed to update landmark input convert element\r\n");
    }
#else
    /* detected_count contains at least the detection zone box */
    app_priv->label_rect_params.labels.detected_rect = 1;
    app_priv->label_rect_params.labels.max_rect = MAX_LABEL_RECTS;
    app_priv->label_rect_params.labels.rectangles = app_priv->rectangles;
    app_priv->label_rect_params.labels.max_landmk = MAX_LABEL_RECTS * MAX_LANDMARKS;
    app_priv->label_rect_params.labels.detected_landmk = 0;
    app_priv->label_rect_params.labels.landmarks = app_priv->landmarks;
    /* Clear landmarks array */
    memset(&app_priv->landmarks[0], 0, sizeof(app_priv->landmarks));
#endif /* DISPLAY_BLAZE_PALM_RECTS_LDMKS */
}

/**
 * Update landmark input converter parameters based on detected hand
 */
static void update_landmark_converter_params(user_data_t *app_priv)
{
    /* Update convert element for landmark detector model inference */
    /* crop box of the hand
     * Use box 1 as the first box is reserved for the detection zone rectangle */
#if (APP_SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
    uint32_t temp_width = app_priv->rectangles[1].right - app_priv->rectangles[1].left;
    app_priv->landmark_input_convert_params.convert.crop.left = APP_CAMERA_WIDTH - app_priv->rectangles[1].right - 1;
    app_priv->landmark_input_convert_params.convert.crop.right = app_priv->landmark_input_convert_params.convert.crop.left + temp_width;
    app_priv->landmark_input_convert_params.convert.crop.top = app_priv->rectangles[1].top;
    app_priv->landmark_input_convert_params.convert.crop.bottom = app_priv->rectangles[1].bottom;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_VERTICAL)
    uint32_t temp_height = app_priv->rectangles[1].bottom - app_priv->rectangles[1].top;
    app_priv->landmark_input_convert_params.convert.crop.left = app_priv->rectangles[1].left;
    app_priv->landmark_input_convert_params.convert.crop.right = app_priv->rectangles[1].right;
    app_priv->landmark_input_convert_params.convert.crop.top = APP_CAMERA_HEIGHT - app_priv->rectangles[1].bottom - 1;
    app_priv->landmark_input_convert_params.convert.crop.bottom = app_priv->landmark_input_convert_params.convert.crop.top + temp_height;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_BOTH)
    uint32_t temp_width = app_priv->rectangles[1].right - app_priv->rectangles[1].left;
    uint32_t temp_height = app_priv->rectangles[1].bottom - app_priv->rectangles[1].top;
    app_priv->landmark_input_convert_params.convert.crop.left = APP_CAMERA_WIDTH - app_priv->rectangles[1].right - 1;
    app_priv->landmark_input_convert_params.convert.crop.right = app_priv->landmark_input_convert_params.convert.crop.left + temp_width;
    app_priv->landmark_input_convert_params.convert.crop.top = APP_CAMERA_HEIGHT - app_priv->rectangles[1].bottom - 1;
    app_priv->landmark_input_convert_params.convert.crop.bottom = app_priv->landmark_input_convert_params.convert.crop.top + temp_height;
#else
    app_priv->landmark_input_convert_params.convert.crop.left = app_priv->rectangles[1].left;
    app_priv->landmark_input_convert_params.convert.crop.right = app_priv->rectangles[1].right;
    app_priv->landmark_input_convert_params.convert.crop.top = app_priv->rectangles[1].top;
    app_priv->landmark_input_convert_params.convert.crop.bottom = app_priv->rectangles[1].bottom;
#endif
    app_priv->landmark_input_convert_params.convert.ops |= MPP_CONVERT_CROP;

#if DEBUG_FRAME_ID
    _elem_t *elem = (_elem_t *) app_priv->blaze_palm_elem;
    app_priv->blaze_detector_frame_id = elem->io.in_buf[0]->frame_id;
    PRINTF("Updated elem convert with params %d %d %d %d form frame id %d\r\n",
                    app_priv->rectangles[1].left,
                    app_priv->rectangles[1].top,
                    app_priv->rectangles[1].right,
                    app_priv->rectangles[1].bottom,
                    app_priv->blaze_detector_frame_id);
#endif /* DEBUG_FRAME_ID */

    /* then add rotation based on the model output */
    if (app_priv->boxes[1].rotation != 0.0f)
    {
        app_priv->landmark_input_convert_params.convert.angle = ROTATE_CUSTOM;
        app_priv->landmark_input_convert_params.convert.custom_angle = app_priv->boxes[1].rotation;
    }
    else
    {
        app_priv->landmark_input_convert_params.convert.angle = ROTATE_0;
        app_priv->landmark_input_convert_params.convert.custom_angle = 0.0f;
    }
    app_priv->landmark_input_convert_params.convert.ops |=  MPP_CONVERT_ROTATE;
}

/**
 * Start/stop pipelines based on hand detection
 */
static void manage_pipelines_on_detection(user_data_t *app_priv)
{
    if (app_priv->detected_count)
    {
        /* Stop the detector pipeline and start landmark pipeline */
        if (mpp_is_running(app_priv->blaze_detector_input_convert_mp) && (mpp_stop(app_priv->blaze_detector_input_convert_mp) != MPP_SUCCESS))
            PRINTF("Error: Failed to stop blaze detector input pipeline branch\r\n");
        if (!mpp_is_running(app_priv->landmark_mp) && (mpp_start(app_priv->landmark_mp, 0, false) != MPP_SUCCESS))
            PRINTF("Error:  Failed to start landmark detection pipeline branch\r\n");
        if (!mpp_is_running(app_priv->landmark_input_convert_mp))
        {
            if (mpp_start(app_priv->landmark_input_convert_mp, 0, true) != MPP_SUCCESS)
                PRINTF("Error:  Failed to start landmark input convert pipeline branch\r\n");
        }
        else
        {
            PRINTF("Error: landmark input convert branch should not be started at this point\r\n");
        }
    }
    else
    {
        if (mpp_is_running(app_priv->landmark_mp) && mpp_stop(app_priv->landmark_mp) != MPP_SUCCESS)
            PRINTF("Error: Failed to stop landmark detection pipeline branch\r\n");
        app_priv->hand_data.has_hand = false;
    }
}

/**
 * Handle blaze detector inference output
 */
static void handle_blaze_detector_output(user_data_t *app_priv, const mpp_inference_cb_param_t *inf_output, uint32_t *hand_idx)
{
    status_t ret;

    ret = BlazeDetectorPtq_ProcessOutput(
            inf_output,
            app_priv->boxes,
            NUM_BOXES_MAX,
            true);
    if (ret != kStatus_Success)
        PRINTF("mpp_event_listener: process output error!");

    app_priv->detected_count = 0;
    app_priv->blaze_detect_inference_time_ms = inf_output->inference_time_ms;

    /* count valid results */
    for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
    {
        if (app_priv->boxes[i].score > 0)
        {
            app_priv->detected_count++;
            *hand_idx = i;
        }
    }

    if (app_priv->detected_count > 1)
    {
        PRINTF("Error: Multiple detections found (%d), expected single hand\n", app_priv->detected_count);
        return;
    }

    boxes_to_rects(app_priv->boxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, app_priv->rectangles);

    update_blaze_palm_labels(app_priv, *hand_idx);

    if (app_priv->detected_count)
    {
        update_landmark_converter_params(app_priv);

        /* Update convert element for landmark detector model inference */
        if (mpp_element_update(app_priv->landmark_input_convert_mp,
                               app_priv->landmark_input_convert_elem,
                               &app_priv->landmark_input_convert_params, true) != MPP_SUCCESS)
            PRINTF("Error: Failed to update landmark input convert element\r\n");
    }

    manage_pipelines_on_detection(app_priv);
    app_priv->inference_frame_num++;
}

/**
 * Update landmarks for hand landmark detection
 */
static void update_hand_landmarks(user_data_t *app_priv, uint32_t hand_idx)
{
    if (app_priv->hand_data.has_hand)
    {
#if DISPLAY_BLAZE_PALM_RECTS_LDMKS
        app_priv->label_rect_params.labels.detected_landmk = MODEL_NUM_LANDMARKS;
#else
        app_priv->label_rect_params.labels.detected_landmk = 0;
#endif
        uint32_t landmark_idx = app_priv->label_rect_params.labels.detected_landmk;
        mpp_color_t landmark_color = {.rgb = {.R = 0x00, .G = 0xff, .B = 0x00}};
#if DEBUG_FRAME_ID
        _elem_t *elem = (_elem_t *) app_priv->hand_ldmk_elem;
        app_priv->hand_ldmk_frame_id = elem->io.in_buf[0]->frame_id;
        PRINTF("Converting labels for frame id %d with params %d %d %d %d\r\n", app_priv->hand_ldmk_frame_id,
                            app_priv->last_hand_ldk_crop_params.left,
                            app_priv->last_hand_ldk_crop_params.top,
                            app_priv->last_hand_ldk_crop_params.right,
                            app_priv->last_hand_ldk_crop_params.bottom);
#endif
        memcpy(&app_priv->has_hand_matrix[0][0],
               &app_priv->last_hand_ldk_crop_params.vglite_m[0][0],
               sizeof(app_priv->has_hand_matrix));
        app_priv->has_hand_rotation = app_priv->last_hand_ldk_crop_params.custom_rotation;

        /* Calculate inverse VGLite matrix once */
        float inv_matrix[3][3];
        if (app_priv->has_hand_rotation != 0.0f)
            inverse_matrix(app_priv->has_hand_matrix, inv_matrix);

        /* Convert landmark coordinates from model input space to display space */
        for (uint32_t lm_idx = 0; lm_idx < MODEL_NUM_3D_LANDMARKS && landmark_idx < (MAX_LABEL_RECTS * MAX_LANDMARKS); lm_idx++)
        {
            if (app_priv->has_hand_rotation == 0.0f)
            {
                uint32_t inp_w, inp_h, offset_l, offset_t;
                inp_w = app_priv->last_hand_ldk_crop_params.right - app_priv->last_hand_ldk_crop_params.left;
                inp_h = app_priv->last_hand_ldk_crop_params.bottom - app_priv->last_hand_ldk_crop_params.top;
#if (APP_SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
                offset_l = APP_CAMERA_WIDTH - app_priv->last_hand_ldk_crop_params.right - 1;
                offset_t = app_priv->last_hand_ldk_crop_params.top;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_VERTICAL)
                offset_l = app_priv->last_hand_ldk_crop_params.left;
                offset_t = APP_CAMERA_HEIGHT - app_priv->last_hand_ldk_crop_params.bottom - 1;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_BOTH)
                offset_l = APP_CAMERA_WIDTH - app_priv->last_hand_ldk_crop_params.right - 1;
                offset_t = APP_CAMERA_HEIGHT - app_priv->last_hand_ldk_crop_params.bottom - 1;
#else
                offset_l = app_priv->last_hand_ldk_crop_params.left;
                offset_t = app_priv->last_hand_ldk_crop_params.top;
#endif

                landmark_3d_convert_params_t convert_params = {
                    .src_landmark = &app_priv->hand_data.landmarks[lm_idx],
                    .dst_landmark = &app_priv->landmarks[landmark_idx],
                    .hand_idx = hand_idx,
                    .landmark_idx = lm_idx,
                    .detection_w = inp_w,
                    .detection_h = inp_h,
                    .color = &landmark_color,
                    .offset_left = offset_l,
                    .offset_top = offset_t
                };
                convert_landmark_coordinates_3d(&convert_params);
            }
            else
            {
                // Get the landmark coordinates from the hand detection crop region
                float lm_x = app_priv->hand_data.landmarks[lm_idx].x;
                float lm_y = app_priv->hand_data.landmarks[lm_idx].y;

                // Apply inverse VGLite matrix to transform back to camera frame coordinates (unflipped)
                float camera_x = inv_matrix[0][0] * lm_x + inv_matrix[0][1] * lm_y + inv_matrix[0][2];
                float camera_y = inv_matrix[1][0] * lm_x + inv_matrix[1][1] * lm_y + inv_matrix[1][2];

                /* Reapply the crop offset to get coordinates in camera frame space */
                camera_x += app_priv->last_hand_ldk_crop_params.left;
                camera_y += app_priv->last_hand_ldk_crop_params.top;

                // Re-apply the flip to match the labeled rectangle element's input image
#if (APP_SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
                camera_x = APP_CAMERA_WIDTH - camera_x - 1;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_VERTICAL)
                camera_y = APP_CAMERA_HEIGHT - camera_y - 1;
#elif (APP_SRC_DISPLAY_FLIP == FLIP_BOTH)
                camera_x = APP_CAMERA_WIDTH - camera_x - 1;
                camera_y = APP_CAMERA_HEIGHT - camera_y - 1;
#endif

                // Set the converted landmark coordinates
                landmark_color.rgb.B = 0xff;
                landmark_color.rgb.G = 0x00;
                app_priv->landmarks[landmark_idx].x = (int16_t)camera_x;
                app_priv->landmarks[landmark_idx].y = (int16_t)camera_y;
                app_priv->landmarks[landmark_idx].color = landmark_color;
                /* Set basic landmark properties */
                app_priv->landmarks[landmark_idx].clear = 0; /* don't clear landmark */
                app_priv->landmarks[landmark_idx].width = LANDMARK_POINT_SIZE; /* landmark point size */
                app_priv->landmarks[landmark_idx].stripe = false;
                /* Encode hand and landmark index in tag */
                app_priv->landmarks[landmark_idx].tag = (hand_idx << 8) | lm_idx;
            }

            uint32_t ldk_x = app_priv->landmarks[landmark_idx].x;
            uint32_t ldk_y = app_priv->landmarks[landmark_idx].y;
            uint32_t ldk_width = app_priv->landmarks[landmark_idx].width;
            uint32_t ldk_width_x2 = ldk_width * 2;
            uint32_t ldk_right_limit = APP_CAMERA_WIDTH - ldk_width_x2;
            uint32_t ldk_bottom_limit = APP_CAMERA_HEIGHT - ldk_width_x2;
            if ((ldk_y <= ldk_width) || (ldk_y >= ldk_bottom_limit) || (ldk_x <= ldk_width) || (ldk_x >= ldk_right_limit))
            {
                app_priv->label_rect_params.labels.detected_landmk = 1;
                app_priv->landmarks[0].clear = 1;
                app_priv->hand_data.has_hand = false;
                break;
            }
            landmark_idx++;
            app_priv->label_rect_params.labels.detected_landmk++;
        }
    }
    else
    {
#if !DISPLAY_BLAZE_PALM_RECTS_LDMKS
        /* detected_count contains at least the detection zone box */
        app_priv->label_rect_params.labels.detected_rect = 1;
        app_priv->label_rect_params.labels.max_rect = MAX_LABEL_RECTS;
        app_priv->label_rect_params.labels.rectangles = app_priv->rectangles;

        /* Clear landmarks array */
        memset(&app_priv->landmarks[0], 0, sizeof(app_priv->landmarks));
#endif
        /* params init */
        app_priv->label_rect_params.labels.max_landmk = MAX_LABEL_RECTS * MAX_LANDMARKS;
        app_priv->label_rect_params.labels.detected_landmk = 1;
        app_priv->label_rect_params.labels.landmarks = app_priv->landmarks;

        app_priv->landmarks[0].clear = 1;
    }
}

/**
 * Manage pipelines after hand landmark detection
 */
static void manage_pipelines_after_landmark(user_data_t *app_priv)
{
    if (app_priv->hand_data.has_hand)
    {
        if (mpp_is_running(app_priv->blaze_detector_mp) && mpp_stop(app_priv->blaze_detector_mp) != MPP_SUCCESS)
            PRINTF("Error: Failed to stop blaze palm detector pipeline\r\n");
    }
    else
    {
        if (mpp_is_running(app_priv->landmark_input_convert_mp))
        {
            if (mpp_stop(app_priv->landmark_input_convert_mp) != MPP_SUCCESS)
                PRINTF("Error: Failed to stop landmark input convert pipeline branch\r\n");
        }
        else
        {
            PRINTF("Error: Landmark input convert should not be already stopped\r\n");
        }
        if (!mpp_is_running(app_priv->blaze_detector_mp) && mpp_start(app_priv->blaze_detector_mp, 0, false) != MPP_SUCCESS)
            PRINTF("Error: Failed to start blaze palm detector pipeline\r\n");
        if (!mpp_is_running(app_priv->blaze_detector_input_convert_mp) && mpp_start(app_priv->blaze_detector_input_convert_mp, 0, false) != MPP_SUCCESS)
            PRINTF("Error: Failed to start blaze palm detector input pipeline\r\n");
    }
}

/**
 * Handle hand landmark inference output
 */
static void handle_hand_landmark_output(user_data_t *app_priv, const mpp_inference_cb_param_t *inf_output, uint32_t hand_idx)
{
    status_t ret;

    ret = HandLandmark_ProcessOutput(inf_output, &app_priv->hand_data);
    if (ret != kStatus_Success)
        PRINTF("mpp_event_listener: process output error for hand landmark model!\r\n");

    app_priv->hand_ldmk_inference_time_ms = inf_output->inference_time_ms;

    /* Update labeled rectangle element with detected faces and landmarks */
    if (app_priv->label_rect_elem != 0) {
        update_hand_landmarks(app_priv, hand_idx);

        /* prepare the output for the next model */
        float *gest_emb_inp = (float *) mpp_get_input_buff_address(app_priv->gesture_emb_elem, 0);
        if (gest_emb_inp == NULL)
        {
            PRINTF("mpp_event_listener: Failed to get gesture embedder input buffer\r\n");
            return;
        }
        for (int i = 0; i < MODEL_NUM_3D_LANDMARKS; i++) {
            gest_emb_inp[i * 3] = app_priv->hand_data.landmarks[i].x;
            gest_emb_inp[i * 3 + 1] = app_priv->hand_data.landmarks[i].y;
            gest_emb_inp[i * 3 + 2] = app_priv->hand_data.landmarks[i].z;
        }

        /* Update the element with both rectangles and landmarks */
        mpp_element_update(app_priv->mp, app_priv->label_rect_elem, &app_priv->label_rect_params, true);

        manage_pipelines_after_landmark(app_priv);

        app_priv->inference_frame_num++;
    }
}

/**
 * Handle gesture embedder inference output
 */
static void handle_gesture_embedder_output(user_data_t *app_priv, const mpp_inference_cb_param_t *inf_output)
{
    if (!app_priv->hand_data.has_hand) {
        return;
    }

    app_priv->gesture_emb_inference_time_ms = inf_output->inference_time_ms;

    /* Prepare gesture embedder output for gesture classifier input */
    float *gest_emb_output = (float *) inf_output->out_tensors[0]->data;
    float *gest_cls_inp = (float *) mpp_get_input_buff_address(app_priv->gesture_cls_elem, 0);

    for (int i = 0; i < GESTURE_EMBEDDER_OUTPUT_SIZE; i++) {
        gest_cls_inp[i] = gest_emb_output[i];
    }
}

/**
 * Handle gesture classifier inference output
 */
static void handle_gesture_classifier_output(user_data_t *app_priv, const mpp_inference_cb_param_t *inf_output)
{
    status_t ret;

    if (!app_priv->hand_data.has_hand) {
        return;
    }

    app_priv->gesture_cls_inference_time_ms = inf_output->inference_time_ms;

    /* Process gesture classifier output */
    ret = GestureClassifier_ProcessOutput(inf_output, &app_priv->gesture_data);
    if (ret != kStatus_Success)
        PRINTF("mpp_event_listener: process output error for gesture classifier model!\r\n");
}

/**
 * Handle inference output ready event
 */
static void handle_inference_output_ready(user_data_t *app_priv, const mpp_inference_cb_param_t *inf_output)
{
    static uint32_t hand_idx = 0;

    switch (inf_output->model_id) {
    case BLAZE_DETECTOR_MODEL_ID:
        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
            handle_blaze_detector_output(app_priv, inf_output, &hand_idx);
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }
        break;

    case HANDLANDMARK_MODEL_ID:
        /* process new hand data from inference */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
            handle_hand_landmark_output(app_priv, inf_output, hand_idx);
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }
        break;

    case GESTURE_EMBEDDER_MODEL_ID:
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
            handle_gesture_embedder_output(app_priv, inf_output);
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }
        break;

    case GESTURE_CLASSIFIER_MODEL_ID:
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
            handle_gesture_classifier_output(app_priv, inf_output);
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }
        break;

    default:
        PRINTF("mpp_event_listener: unknown model_id %d\r\n", inf_output->model_id);
        break;
    }
}

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data)
{
    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_CONVERT_PARAMS_READY:
        /* cast evt_data pointer to correct structure matching the event */
        handle_convert_params_ready(app_priv, (mpp_convert_cb_param_t *) evt_data);
        break;

    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        handle_inference_output_ready(app_priv, (const mpp_inference_cb_param_t *) evt_data);
        break;

    case MPP_EVENT_INVALID:
    default:
        /* nothing to do */
        break;
    }

    return 0;
}

/*******************************************************************************
 * Helper Functions for Pipeline Element Addition
 ******************************************************************************/

static int add_camera_element(mpp_t mp, bool stripe_mode, mpp_elem_handle_t *cam_elem)
{
    mpp_camera_params_t cam_params;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = APP_CAMERA_HEIGHT;
    cam_params.width =  APP_CAMERA_WIDTH;
    cam_params.format = APP_CAMERA_FORMAT;
    cam_params.fps    = 30;
    cam_params.stripe = stripe_mode;

    int ret = mpp_camera_add(mp, s_camera_name, &cam_params, cam_elem);
    if (ret) {
        PRINTF("Failed to add camera %s\r\n", s_camera_name);
        return ret;
    }
    return 0;
}

static int add_jpeg_decoder_element(mpp_t mp, mpp_stats_t *decoder_stats)
{
    if (APP_CAMERA_FORMAT != MPP_PIXEL_JPEG) {
        return 0; // Skip if not JPEG format
    }

    mpp_element_params_t elem_params_decoder;
    memset(&elem_params_decoder, 0, sizeof(mpp_element_params_t));
    elem_params_decoder.decode.dev_name = APP_DECODE_BACKEND_NAME;
    elem_params_decoder.decode.width = APP_CAMERA_WIDTH;
    elem_params_decoder.decode.height = APP_CAMERA_HEIGHT;
    elem_params_decoder.decode.out_format = MPP_PIXEL_YUYV;
    elem_params_decoder.stats = decoder_stats;

    int ret = mpp_element_add(mp, MPP_ELEMENT_IMG_DECODE, &elem_params_decoder, NULL);
    if (ret) {
        PRINTF("Failed to add element DECODE\r\n");
        return ret;
    }
    return 0;
}

static int add_blaze_detector_convert_element(mpp_t mp_split, bool stripe_mode, mpp_stats_t *convert_stats)
{
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));

    elem_params.convert.dev_name = NULL;
    elem_params.convert.out_buf.width = BLAZE_DETECTOR_WIDTH;
    elem_params.convert.out_buf.height = BLAZE_DETECTOR_HEIGHT;
    elem_params.convert.pixel_format = MPP_PIXEL_RGB;
    elem_params.convert.ops = MPP_CONVERT_COLOR;

    elem_params.convert.crop.top = CROP_TOP;
    elem_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    elem_params.convert.crop.left = CROP_LEFT;
    elem_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    elem_params.convert.ops |= MPP_CONVERT_CROP;

    elem_params.convert.scale.width = BLAZE_DETECTOR_WIDTH;
    elem_params.convert.scale.height = BLAZE_DETECTOR_HEIGHT;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    elem_params.convert.flip = APP_SRC_DISPLAY_FLIP;
    elem_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif

    elem_params.convert.stripe_in = stripe_mode;
    elem_params.convert.stripe_out = false;
    elem_params.stats = convert_stats;

    int ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT\r\n");
        return ret;
    }
    return 0;
}

static int add_blaze_detector_inference_element(mpp_t mp_bg, user_data_t *user_data, mpp_stats_t *blaze_stats)
{
    mpp_element_params_t blaze_detector_params;
    memset(&blaze_detector_params, 0 , sizeof(mpp_element_params_t));

    blaze_detector_params.ml_inference.model_data = blaze_detector_data;
    blaze_detector_params.ml_inference.model_size = blaze_detector_data_len;
    blaze_detector_params.ml_inference.model_id   = BLAZE_DETECTOR_MODEL_ID;
    blaze_detector_params.ml_inference.model_input_mean = BLAZE_DETECTOR_INPUT_MEAN;
    blaze_detector_params.ml_inference.model_input_std = BLAZE_DETECTOR_INPUT_STD;
    blaze_detector_params.ml_inference.inference_params.num_inputs = 1;
    blaze_detector_params.ml_inference.inference_params.num_outputs = 2;
    blaze_detector_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    blaze_detector_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    blaze_detector_params.stats = blaze_stats;

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &blaze_detector_params, &user_data->blaze_palm_elem);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE\r\n");
        return ret;
    }
    return 0;
}

static int add_landmark_convert_element(mpp_t mp_split, bool stripe_mode, user_data_t *user_data, mpp_stats_t *convert_stats)
{
    memset(&user_data->landmark_input_convert_params, 0, sizeof(user_data->landmark_input_convert_params));

    user_data->landmark_input_convert_params.convert.dev_name = NULL;
    user_data->landmark_input_convert_params.convert.elem_id = HAND_LANDMARK_CONVERT_ELEM_ID;
    user_data->landmark_input_convert_params.convert.out_buf.width = HAND_LANDMARK_WIDTH;
    user_data->landmark_input_convert_params.convert.out_buf.height = HAND_LANDMARK_HEIGHT;
    user_data->landmark_input_convert_params.convert.pixel_format = MPP_PIXEL_RGB;
    user_data->landmark_input_convert_params.convert.ops = MPP_CONVERT_COLOR;

    user_data->landmark_input_convert_params.convert.crop.top = CROP_TOP;
    user_data->landmark_input_convert_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    user_data->landmark_input_convert_params.convert.crop.left = CROP_LEFT;
    user_data->landmark_input_convert_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    user_data->landmark_input_convert_params.convert.ops |= MPP_CONVERT_CROP;

    user_data->landmark_input_convert_params.convert.scale.width = HAND_LANDMARK_WIDTH;
    user_data->landmark_input_convert_params.convert.scale.height = HAND_LANDMARK_HEIGHT;
    user_data->landmark_input_convert_params.convert.ops |= MPP_CONVERT_SCALE;

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    user_data->landmark_input_convert_params.convert.flip = APP_SRC_DISPLAY_FLIP;
    user_data->landmark_input_convert_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif

    user_data->landmark_input_convert_params.convert.stripe_in = stripe_mode;
    user_data->landmark_input_convert_params.convert.stripe_out = false;
    user_data->landmark_input_convert_params.stats = convert_stats;

    int ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &user_data->landmark_input_convert_params, &user_data->landmark_input_convert_elem);
    if (ret) {
        PRINTF("Failed to add element CONVERT\r\n");
        return ret;
    }
    return 0;
}

static int add_hand_landmark_inference_element(mpp_t mp_bg, user_data_t *user_data, mpp_stats_t *landmark_stats)
{
    mpp_element_params_t hand_landmark_params;
    memset(&hand_landmark_params, 0, sizeof(mpp_element_params_t));

    hand_landmark_params.ml_inference.model_data = hand_landmark_data;
    hand_landmark_params.ml_inference.model_size = hand_landmark_data_len;
    hand_landmark_params.ml_inference.model_id   = HANDLANDMARK_MODEL_ID;
    hand_landmark_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    hand_landmark_params.ml_inference.model_input_mean = HAND_LANDMARK_INPUT_MEAN;
    hand_landmark_params.ml_inference.model_input_std = HAND_LANDMARK_INPUT_STD;
    hand_landmark_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    hand_landmark_params.ml_inference.inference_params.num_inputs = 1;
    hand_landmark_params.ml_inference.inference_params.num_outputs = 4;
    hand_landmark_params.stats = landmark_stats;

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &hand_landmark_params, &user_data->hand_ldmk_elem);
    if (ret) {
        PRINTF("Failed to add element VALGO_TFLite\r\n");
        return ret;
    }
    return 0;
}

static int add_gesture_embedder_inference_element(mpp_t mp_bg, user_data_t *user_data, mpp_stats_t *embedder_stats)
{
    mpp_element_params_t gesture_embedder_params;
    memset(&gesture_embedder_params, 0, sizeof(mpp_element_params_t));

    gesture_embedder_params.ml_inference.model_data = gesture_embedder_data;
    gesture_embedder_params.ml_inference.model_size = gesture_embedder_data_len;
    gesture_embedder_params.ml_inference.model_id   = GESTURE_EMBEDDER_MODEL_ID;
    gesture_embedder_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    gesture_embedder_params.ml_inference.model_input_mean = 0;
    gesture_embedder_params.ml_inference.model_input_std = 1;
    gesture_embedder_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    gesture_embedder_params.ml_inference.inference_params.num_inputs = 1;
    gesture_embedder_params.ml_inference.inference_params.num_outputs = 1;
    gesture_embedder_params.stats = embedder_stats;

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &gesture_embedder_params, &user_data->gesture_emb_elem);
    if (ret) {
        PRINTF("Failed to add element VALGO_TFLite\r\n");
        return ret;
    }
    return 0;
}

static int add_gesture_classifier_inference_element(mpp_t mp_bg, user_data_t *user_data, mpp_stats_t *classifier_stats)
{
    mpp_element_params_t gesture_classifier_params;
    memset(&gesture_classifier_params, 0, sizeof(mpp_element_params_t));

    gesture_classifier_params.ml_inference.model_data = canned_gesture_classifier_data;
    gesture_classifier_params.ml_inference.model_size = canned_gesture_classifier_data_len;
    gesture_classifier_params.ml_inference.model_id   = GESTURE_CLASSIFIER_MODEL_ID;
    gesture_classifier_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    gesture_classifier_params.ml_inference.model_input_mean = 0;
    gesture_classifier_params.ml_inference.model_input_std = 1;
    gesture_classifier_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    gesture_classifier_params.ml_inference.inference_params.num_inputs = 1;
    gesture_classifier_params.ml_inference.inference_params.num_outputs = 1;
    gesture_classifier_params.stats = classifier_stats;

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &gesture_classifier_params, &user_data->gesture_cls_elem);
    if (ret) {
        PRINTF("Failed to add element VALGO_TFLite\r\n");
        return ret;
    }
    return 0;
}

static int add_display_convert_element(mpp_t mp, mpp_stats_t *convert_stats)
{
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
    return 0; // Skip if conversion not needed
#else
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));

    elem_params.convert.dev_name = NULL;
    elem_params.convert.out_buf.width = APP_CAMERA_WIDTH;
    elem_params.convert.out_buf.height = APP_CAMERA_HEIGHT;
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    elem_params.convert.ops = MPP_CONVERT_COLOR;
    elem_params.convert.flip = APP_SRC_DISPLAY_FLIP;
    elem_params.convert.ops |=  MPP_CONVERT_ROTATE;
    elem_params.stats = convert_stats;

    int ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT\r\n");
        return ret;
    }
    return 0;
#endif
}

static int add_labeled_rectangle_element(mpp_t mp, user_data_t *user_data, mpp_stats_t *labels_stats)
{
    memset(&user_data->label_rect_params, 0, sizeof(user_data->label_rect_params));
    memset(&user_data->rectangles, 0, sizeof(user_data->rectangles));

    user_data->label_rect_params.labels.max_rect = MAX_LABEL_RECTS;
    user_data->label_rect_params.labels.detected_rect = 1;
    user_data->label_rect_params.labels.rectangles = user_data->rectangles;
    user_data->label_rect_params.labels.max_landmk = MAX_LABEL_RECTS * MAX_LANDMARKS;

    /* Add detection zone box */
    user_data->rectangles[0].top    = DETECTION_ZONE_RECT_TOP;
    user_data->rectangles[0].left   = DETECTION_ZONE_RECT_LEFT;
    user_data->rectangles[0].bottom = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_HEIGHT;
    user_data->rectangles[0].right  = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_WIDTH;
    user_data->rectangles[0].line_width = RECT_LINE_WIDTH;
    user_data->rectangles[0].line_color.rgb.G = 0xff;
    strcpy((char *)user_data->rectangles[0].label, "Detection zone");

    user_data->label_rect_params.stats = labels_stats;

    int ret = mpp_element_add(mp, MPP_ELEMENT_LABELED_RECTANGLE, &user_data->label_rect_params, &user_data->label_rect_elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        return ret;
    }

    user_data->mp = mp;
    return 0;
}

#if !DISPLAY_TEXT
static int add_display_rotate_element(mpp_t mp, mpp_stats_t *rotate_stats)
{
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
    return 0;
#else
    if (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_0) {
        return 0; // No rotation needed
    }

    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));

    elem_params.convert.out_buf.width = APP_DISPLAY_WIDTH;
    elem_params.convert.out_buf.height = APP_DISPLAY_HEIGHT;
    elem_params.convert.angle = APP_DISPLAY_LANDSCAPE_ROTATE;
    elem_params.convert.ops = MPP_CONVERT_ROTATE;

#ifdef SCALED_VIEW
    elem_params.convert.scale.width =  SCALED_VIEW_WIDTH;
    elem_params.convert.scale.height = SCALED_VIEW_HEIGHT;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;
#endif

    if (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_270)
    {
        elem_params.convert.out_window.left = 0;
#ifdef SCALED_VIEW
        elem_params.convert.out_window.top = APP_DISPLAY_HEIGHT - SCALED_VIEW_HEIGHT - 1;
#else
        elem_params.convert.out_window.top = APP_DISPLAY_HEIGHT - (int)(APP_DISPLAY_WIDTH * ((float)APP_CAMERA_WIDTH/APP_CAMERA_HEIGHT));
#endif
        elem_params.convert.ops |= MPP_CONVERT_OUT_WINDOW;
    }

    elem_params.stats = rotate_stats;

    int ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT\r\n");
        return ret;
    }
    return 0;
#endif
}
#else
static int add_compose_element(mpp_t mp, user_data_t *user_data, text_info_t *txt_info, mpp_stats_t *compose_stats)
{
    static mpp_element_params_t elem_params_compose;
    memset(&elem_params_compose, 0, sizeof(mpp_element_params_t));
    elem_params_compose.stats = compose_stats;

    init_text_area(g_text_img, sizeof(g_text_img), txt_info);

    /* Output params */
    elem_params_compose.compose.out_angle = APP_DISPLAY_LANDSCAPE_ROTATE;
    elem_params_compose.compose.out_flip = FLIP_NONE;
    elem_params_compose.compose.out_format = APP_DISPLAY_FORMAT;
    elem_params_compose.compose.out_width = APP_DISPLAY_WIDTH;
    elem_params_compose.compose.out_height = APP_DISPLAY_HEIGHT;

    /* Set number of images to compose */
    elem_params_compose.compose.nb_images = COMPOSE_MAX_IMAGES;

    /* Allocate memory for image list */
    static mpp_img_compose_param_t compose_images[COMPOSE_MAX_IMAGES];
    memset(compose_images, 0, sizeof(compose_images));
    elem_params_compose.compose.image_list = compose_images;

    /* Input area - full frame */
    if (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_90) 
    {
        /* Configure text image */
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].width = TEXT_WIDTH;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].height = TEXT_HEIGHT;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].format = MPP_PIXEL_RGB565;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].buffer = g_text_img;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.left = TEXT_LEFT_POS_90;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.top = TEXT_TOP_POS_90;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.right = TEXT_RIGHT_POS_90;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.bottom = TEXT_BOTTOM_POS_90;

        /* Configure video input area */
        elem_params_compose.compose.input_area.left = 0;
        elem_params_compose.compose.input_area.top = 0;
#ifdef SCALED_VIEW
        elem_params_compose.compose.input_area.bottom = SCALED_VIEW_HEIGHT;
        elem_params_compose.compose.input_area.right = SCALED_VIEW_WIDTH - 1;
#else
        elem_params_compose.compose.input_area.bottom = (int)(APP_DISPLAY_WIDTH * ((float)APP_CAMERA_WIDTH/APP_CAMERA_HEIGHT));
        elem_params_compose.compose.input_area.right = APP_DISPLAY_WIDTH - 1;
#endif
    }
    else if (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_270)
    {
        /* Configure text image */
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].width = TEXT_WIDTH;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].height = TEXT_HEIGHT;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].format = MPP_PIXEL_RGB565;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].buffer = g_text_img;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.left = TEXT_LEFT_POS_270;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.top = TEXT_TOP_POS_270;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.right = TEXT_RIGHT_POS_270;
        elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.bottom = TEXT_BOTTOM_POS_270;

        /* Configure video input area */
        elem_params_compose.compose.input_area.left = 0;
        elem_params_compose.compose.input_area.bottom = APP_DISPLAY_HEIGHT - 1;
#ifdef SCALED_VIEW
        elem_params_compose.compose.input_area.top = APP_DISPLAY_HEIGHT - SCALED_VIEW_HEIGHT;
        elem_params_compose.compose.input_area.right = SCALED_VIEW_WIDTH - 1;
#else
        elem_params_compose.compose.input_area.top = APP_DISPLAY_HEIGHT - (int)(APP_DISPLAY_WIDTH * ((float)APP_CAMERA_WIDTH/APP_CAMERA_HEIGHT));
        elem_params_compose.compose.input_area.right = APP_DISPLAY_WIDTH - 1;
#endif
    }
    else
    {
        PRINTF("Unsupported rotation angle for compose element\r\n");
        return 1;
    }

    int ret = mpp_element_add(mp, MPP_ELEMENT_IMG_COMPOSE, &elem_params_compose, &user_data->compose_elem);
    if (ret) {
        PRINTF("Failed to add element IMG_COMPOSE (0x%x)\r\n", ret);
        return ret;
    }
    return 0;
}
#endif

static int add_display_element(mpp_t mp, bool stripe_mode)
{
    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = stripe_mode;
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
    disp_params.rotate = APP_DISPLAY_LANDSCAPE_ROTATE;
#endif

    int ret = mpp_display_add(mp, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\r\n", s_display_name);
        return ret;
    }
    return 0;
}

/*******************************************************************************
 * Helper Functions for Pipeline Initialization
 ******************************************************************************/

static void print_configuration_info(void)
{
    PRINTF("[%s]\r\n", mpp_get_version());
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    /* Print camera parameters */
    PRINTF("\r\n=== Camera Parameters ===\r\n");
    PRINTF("APP_CAMERA_WIDTH: %d\r\n", APP_CAMERA_WIDTH);
    PRINTF("APP_CAMERA_HEIGHT: %d\r\n", APP_CAMERA_HEIGHT);

    /* Print model input dimensions */
    PRINTF("\r\n=== Model Input Dimensions ===\r\n");
    PRINTF("BLAZE_DETECTOR_WIDTH: %d\r\n", BLAZE_DETECTOR_WIDTH);
    PRINTF("BLAZE_DETECTOR_HEIGHT: %d\r\n", BLAZE_DETECTOR_HEIGHT);
    PRINTF("\r\n");

    /* Print display dimensions */
    PRINTF("\r\n=== Display Dimensions ===\r\n");
    PRINTF("DISPLAY_SMALL_DIM: %d\r\n", (int)DISPLAY_SMALL_DIM);
    PRINTF("DISPLAY_LARGE_DIM: %d\r\n", (int)DISPLAY_LARGE_DIM);
    PRINTF("APP_DISPLAY_WIDTH: %d\r\n", (int)APP_DISPLAY_WIDTH);
    PRINTF("APP_DISPLAY_HEIGHT: %d\r\n", (int)APP_DISPLAY_HEIGHT);
    PRINTF("SWAP_DIMS: %d\r\n", SWAP_DIMS);

    /* Print detection zone parameters */
    PRINTF("\r\n=== Detection Zone Parameters ===\r\n");
    PRINTF("DETECTION_ZONE_RECT_WIDTH: %d\r\n", (int)DETECTION_ZONE_RECT_WIDTH);
    PRINTF("DETECTION_ZONE_RECT_HEIGHT: %d\r\n", (int)DETECTION_ZONE_RECT_HEIGHT);
    PRINTF("DETECTION_ZONE_RECT_LEFT: %d\r\n", (int)DETECTION_ZONE_RECT_LEFT);
    PRINTF("DETECTION_ZONE_RECT_TOP: %d\r\n", (int)DETECTION_ZONE_RECT_TOP);

    /* Print crop parameters */
    PRINTF("\r\n=== Crop Parameters ===\r\n");
    PRINTF("CROP_SIZE_LEFT: %d\r\n", (int)CROP_SIZE_LEFT);
    PRINTF("CROP_SIZE_TOP: %d\r\n", (int)CROP_SIZE_TOP);
    PRINTF("CROP_LEFT: %d\r\n", (int)CROP_LEFT);
    PRINTF("CROP_TOP: %d\r\n", (int)CROP_TOP);

    /* Print boxes offset parameters */
    PRINTF("\r\n=== Boxes Offset Parameters ===\r\n");
    PRINTF("BOXES_OFFSET_LEFT: %d\r\n", (int)BOXES_OFFSET_LEFT);
    PRINTF("BOXES_OFFSET_TOP: %d\r\n", (int)BOXES_OFFSET_TOP);
}

static int init_mpp_api(mpp_api_params_t *api_param, mpp_stats_t *api_stats)
{
    memset(api_param, 0, sizeof(mpp_api_params_t));
    memset(api_stats, 0, sizeof(mpp_stats_t));

#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param->rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param->rc_cycle_min = APP_RC_CYCLE_MIN;
#endif
    api_param->stats = api_stats;

    int ret = mpp_api_init(api_param);
    if (ret) {
        PRINTF("Failed to initialize MPP API\r\n");
    }
    return ret;
}

static mpp_t create_main_pipeline(user_data_t *user_data, mpp_stats_t *mpp_stats)
{
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    memset(mpp_stats, 0, sizeof(mpp_stats_t));

    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;
    mpp_params.stats = mpp_stats;

    int ret;
    mpp_t mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID) {
        PRINTF("Failed to create main pipeline\r\n");
    }
    return mp;
}

static int create_split_pipelines(mpp_t mp, user_data_t *user_data, mpp_t mp_split[2],
                                   mpp_stats_t mpp_split_stats[2])
{
    mpp_params_t mpp_params_split[2];

    memset(&mpp_split_stats[0], 0, sizeof(mpp_stats_t));
    memset(&mpp_split_stats[1], 0, sizeof(mpp_stats_t));

    mpp_params_split[0].evt_callback_f = &mpp_event_listener;
    mpp_params_split[0].mask = MPP_EVENT_ALL;
    mpp_params_split[0].cb_userdata = user_data;
    mpp_params_split[0].exec_flag = MPP_EXEC_RC;
    mpp_params_split[0].stats = &mpp_split_stats[0];

    mpp_params_split[1].evt_callback_f = &mpp_event_listener;
    mpp_params_split[1].mask = MPP_EVENT_ALL;
    mpp_params_split[1].cb_userdata = user_data;
    mpp_params_split[1].exec_flag = MPP_EXEC_RC;
    mpp_params_split[1].stats = &mpp_split_stats[1];

    int ret = mpp_split(mp, 2, &mpp_params_split[0], &mp_split[0]);
    if (ret) {
        PRINTF("Failed to split pipeline\r\n");
    }
    return ret;
}

static int create_blaze_detector_background_pipeline(mpp_t mp_split, user_data_t *user_data,
                                                      mpp_t *mp_bg, mpp_stats_t *mpp_stats_blaze)
{
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params_t));
    memset(mpp_stats_blaze, 0, sizeof(mpp_stats_t));

    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = user_data;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    mpp_params.stats = mpp_stats_blaze;

    int ret = mpp_background(mp_split, &mpp_params, mp_bg);
    if (ret) {
        PRINTF("Failed to create background pipeline for blaze detector\r\n");
    }
    return ret;
}

static int add_blaze_detector_nullsink(mpp_t mp_bg)
{
    int ret = mpp_nullsink_add(mp_bg);
    if (ret) {
        PRINTF("Failed to add NULL sink\r\n");
    }
    return ret;
}

static int create_landmark_background_pipeline(mpp_t mp_split, user_data_t *user_data,
                                                mpp_t *mp_bg_ldmk, mpp_stats_t *mpp_stats_bg_ldmk)
{
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params_t));
    memset(mpp_stats_bg_ldmk, 0, sizeof(mpp_stats_t));

    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = user_data;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    mpp_params.stats = mpp_stats_bg_ldmk;

    int ret = mpp_background(mp_split, &mpp_params, mp_bg_ldmk);
    if (ret) {
        PRINTF("Failed to create background pipeline for landmark detection\r\n");
    }
    return ret;
}

static int add_landmark_nullsink(mpp_t mp_bg_ldmk)
{
    int ret = mpp_nullsink_add(mp_bg_ldmk);
    if (ret) {
        PRINTF("Failed to add NULL sink\r\n");
    }
    return ret;
}

static void enable_stats(void)
{
    mpp_stats_enable(MPP_STATS_GRP_API);
    mpp_stats_enable(MPP_STATS_GRP_MPP);
    mpp_stats_enable(MPP_STATS_GRP_ELEMENT);
}

static int start_pipelines(mpp_t mp, mpp_t mp_split[2], mpp_t mp_bg, mpp_t mp_bg_ldmk)
{
    int ret;

    /* start preempt-able pipeline branch for blaze detector */
    ret = mpp_start(mp_bg, 0, false);
    if (ret) {
        PRINTF("Failed to start blaze detector preempt-able pipeline branch\r\n");
        return ret;
    }

    /* start preempt-able pipeline branch for landmark */
    ret = mpp_start(mp_bg_ldmk, 0, false);
    if (ret) {
        PRINTF("Failed to start landmark preempt-able pipeline branch\r\n");
        return ret;
    }

    /* start blaze detector input convert pipeline branch */
    ret = mpp_start(mp_split[0], 0, false);
    if (ret) {
        PRINTF("Failed to start blaze detector input pipeline branch\r\n");
        return ret;
    }

    /* start landmark input convert pipeline branch */
    ret = mpp_start(mp_split[1], 0, false);
    if (ret) {
        PRINTF("Failed to start landmark input pipeline branch\r\n");
        return ret;
    }

    /* Stop pipeline branch for hand landmark detection
     * It will be started only when hand is detected
     * It is still started above just to set the correct oper_status,
     * so it can be started later */
    ret = mpp_stop(mp_split[1]);
    if (ret) {
        PRINTF("Failed to stop landmark input pipeline branch\r\n");
        return ret;
    }

    ret = mpp_stop(mp_bg_ldmk);
    if (ret) {
        PRINTF("Failed to stop landmark background pipeline branch\r\n");
        return ret;
    }

    /* start main pipeline branch */
    ret = mpp_start(mp, 1, false);
    if (ret) {
        PRINTF("Failed to start main pipeline branch\r\n");
        return ret;
    }

    return 0;
}

static void print_inference_results(user_data_t *user_data, text_info_t *text_info)
{
#if defined(USE_USB_CAMERA) && PRINT_JPEG_VALIDATION_STATS
    static const uint32_t jpeg_stats_print_freq = 10000 / portTICK_PERIOD_MS;
    static TickType_t jpeg_stats_last_print_time = 0;
    TickType_t tick = 0;
#endif

    PRINTF("=======================================================\r\n");
    PRINTF("inference time blaze palm model: %u ms \r\n", user_data->blaze_detect_inference_time_ms);
    PRINTF("inference time hand landmark model: %u ms \r\n", user_data->hand_ldmk_inference_time_ms);
    PRINTF("inference time gesture embedder model: %u ms \r\n", user_data->gesture_emb_inference_time_ms);
    PRINTF("inference time gesture classifier model: %u ms \r\n", user_data->gesture_cls_inference_time_ms);

    text_info->blaze_detector_inference_time = user_data->blaze_detect_inference_time_ms;
    text_info->hand_landmark_inference_time =  user_data->hand_ldmk_inference_time_ms;
    text_info->gesture_embbeder_inference_time = user_data->gesture_emb_inference_time_ms;
    text_info->gesture_classifier_inference_time = user_data->gesture_cls_inference_time_ms;

    if (user_data->hand_data.has_hand)
    {
        PRINTF("Detected hand score: %d%%\r\n", (uint32_t)(user_data->hand_data.score * 100.0f));
        PRINTF("Left hand: %s\r\n", user_data->hand_data.left_hand ? "yes" : "no");
        PRINTF("Detected gesture: %s (%d%%)\r\n", user_data->gesture_data.gesture,
               (uint32_t)(user_data->gesture_data.score * 100.0f));
        strcpy(text_info->gesture, user_data->gesture_data.gesture);
        text_info->confidence = user_data->gesture_data.score * 100.0f;
        text_info->hand_confidence = user_data->hand_data.score * 100.0f;
#if DEBUG_VGLITE_MATRIX
        PRINTF("vglite_m = {\r\n");
        for (uint32_t i = 0; i < 3; i++) {
            PRINTF("   ");
            for (uint32_t j = 0; j < 3; j++) {
                PRINTF("\t%s%.2f", user_data->has_hand_matrix[i][j] < 0 ? "-":"" , user_data->has_hand_matrix[i][j]);
            }
            PRINTF("\r\n");
        }
        PRINTF("}\r\n");
        PRINTF("rotation_angle = %s%f\r\n", user_data->has_hand_rotation < 0 ? "-":"", user_data->has_hand_rotation);
        float inverse_m[3][3];
        inverse_matrix(user_data->has_hand_matrix, inverse_m);
        PRINTF("inverse_m = {\r\n");
        for (uint32_t i = 0; i < 3; i++) {
            PRINTF("   ");
            for (uint32_t j = 0; j < 3; j++) {
                PRINTF("\t%s%.2f", inverse_m[i][j] < 0 ? "-":"" , inverse_m[i][j]);
            }
            PRINTF("\r\n");
        }
        PRINTF("}\r\n");
#endif
    }
    else
    {
        strcpy(text_info->gesture, "NA");
        text_info->confidence = 0.0f;
        text_info->hand_confidence = 0.0f;
    }
    draw_text_area(text_info);
#if defined(USE_USB_CAMERA) && PRINT_JPEG_VALIDATION_STATS
    tick = xTaskGetTickCount();
    if (tick > jpeg_stats_last_print_time + jpeg_stats_print_freq) 
    {
        jpeg_stats_last_print_time = tick;
        HAL_JPEG_PrintValidationStats();
    }
#endif
    PRINTF("=======================================================\r\n");
}

#if PRINT_API_STATS
static void print_api_stats(mpp_stats_t *api_stats)
{
    mpp_stats_disable(MPP_STATS_GRP_API);
    PRINTF("\r\n=== API Stats ===\r\n");
    PRINTF("RC cycle: %d ms\r\n", api_stats->api.rc_cycle);
    PRINTF("RC cycle max: %d ms\r\n", api_stats->api.rc_cycle_max);
    PRINTF("PR slot: %d ms\r\n", api_stats->api.pr_slot);
    PRINTF("PR rounds: %d\r\n", api_stats->api.pr_rounds);
    PRINTF("App slot: %d ms\r\n", api_stats->api.app_slot);
    PRINTF("CPU load: %d %%\r\n", api_stats->api.cpu_load);
    mpp_stats_enable(MPP_STATS_GRP_API);
}
#endif

#if PRINT_MPP_STATS
static void print_mpp_stats(stats_collection_t *stats)
{
    mpp_stats_disable(MPP_STATS_GRP_MPP);
    PRINTF("\r\n=== MPP Stats ===\r\n");
    PRINTF("Main pipeline fps: %d\r\n", stats->mpp_stats->mpp.fps);
    PRINTF("Main pipeline exec_time: %d ms\r\n", stats->mpp_stats->mpp.mpp_exec_time);
    PRINTF("Split pipeline 0 fps: %d\r\n", stats->mpp_split_stats[0].mpp.fps);
    PRINTF("Split pipeline 0 exec_time: %d ms\r\n", stats->mpp_split_stats[0].mpp.mpp_exec_time);
    PRINTF("Split pipeline 1 fps: %d\r\n", stats->mpp_split_stats[1].mpp.fps);
    PRINTF("Split pipeline 1 exec_time: %d ms\r\n", stats->mpp_split_stats[1].mpp.mpp_exec_time);
    PRINTF("Background blaze detector pipeline fps: %d\r\n", stats->mpp_stats_blaze->mpp.fps);
    PRINTF("Background blaze detector pipeline exec_time: %d ms\r\n", stats->mpp_stats_blaze->mpp.mpp_exec_time);
    PRINTF("Background landmark pipeline fps: %d\r\n", stats->mpp_stats_bg_ldmk->mpp.fps);
    PRINTF("Background landmark pipeline exec_time: %d ms\r\n", stats->mpp_stats_bg_ldmk->mpp.mpp_exec_time);
    mpp_stats_enable(MPP_STATS_GRP_MPP);
}
#endif

#if PRINT_ELEM_STATS
static void print_element_stats(stats_collection_t *stats)
{
    mpp_stats_disable(MPP_STATS_GRP_ELEMENT);
    PRINTF("\r\n=== Element Stats ===\r\n");
    PRINTF("Decoder exec_time: %d ms\r\n", stats->decoder_stats->elem.elem_exec_time);
    PRINTF("Convert Blaze exec_time: %d ms\r\n", stats->convert_blaze_stats->elem.elem_exec_time);
    PRINTF("BlazeDetector exec_time: %d ms\r\n", stats->blaze_detector_stats->elem.elem_exec_time);
    PRINTF("Convert Hand Landmark exec_time: %d ms\r\n", stats->convert_hand_ldmk_stats->elem.elem_exec_time);
    PRINTF("Hand Landmark exec_time: %d ms\r\n", stats->hand_landmark_stats->elem.elem_exec_time);
    PRINTF("Gesture Embedder exec_time: %d ms\r\n", stats->gesture_embedder_stats->elem.elem_exec_time);
    PRINTF("Gesture Classifier exec_time: %d ms\r\n", stats->gesture_classifier_stats->elem.elem_exec_time);
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    PRINTF("Convert Display exec_time: %d ms\r\n", stats->convert_display_stats->elem.elem_exec_time);
#endif
    PRINTF("Labels exec_time: %d ms\r\n", stats->labels_stats->elem.elem_exec_time);
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
#if !DISPLAY_TEXT
    PRINTF("Rotate exec_time: %d ms\r\n", stats->rotate_stats->elem.elem_exec_time);
#endif
#endif
#if DISPLAY_TEXT
    PRINTF("Compose exec_time: %d ms\r\n", stats->compose_stats->elem.elem_exec_time);
#endif
    mpp_stats_enable(MPP_STATS_GRP_ELEMENT);
}
#endif

static void run_main_loop(user_data_t *user_data, stats_collection_t *stats, text_info_t *text_info)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    uint32_t last_inf_frame_num = user_data->inference_frame_num;

    for (;;) {
        xTaskDelayUntil(&xLastWakeTime, xFrequency);

        if (Atomic_CompareAndSwap_u32(&user_data->accessing, 1, 0)) {
            if (last_inf_frame_num != user_data->inference_frame_num) {
                print_inference_results(user_data, text_info);
                last_inf_frame_num = user_data->inference_frame_num;
            }
            __atomic_store_n(&user_data->accessing, 0, __ATOMIC_SEQ_CST);
        }

#if PRINT_API_STATS
        print_api_stats(stats->api_stats);
#endif

#if PRINT_MPP_STATS
        print_mpp_stats(stats);
#endif

#if PRINT_ELEM_STATS
        print_element_stats(stats);
#endif
#if (PRINT_API_STATS || PRINT_MPP_STATS || PRINT_ELEM_STATS)
        PRINTF("\r\n");
#endif
    }
}

static void app_task(void *params)
{
    static user_data_t user_data = {0};
    int ret;
    bool stripe_mode = (APP_STRIPE_MODE > 0) ? true : false;
    /* text info default configuration */
    text_info_t txt_info;
    strcpy(txt_info.gesture, "None");
    txt_info.hand_confidence = 0.0f;
    txt_info.confidence = 0.0f;
    txt_info.blaze_detector_inference_time = 0;
    txt_info.hand_landmark_inference_time = 0;
    txt_info.gesture_embbeder_inference_time = 0;
    txt_info.gesture_classifier_inference_time = 0;

    /* Print configuration information */
    print_configuration_info();

    /* Initialize MPP API */
    static mpp_api_params_t api_param;
    static mpp_stats_t api_stats;
    ret = init_mpp_api(&api_param, &api_stats);
    if (ret)
        goto err;

    /* Create main pipeline */
    static mpp_stats_t mpp_stats;
    mpp_t mp = create_main_pipeline(&user_data, &mpp_stats);
    if (mp == MPP_INVALID)
        goto err;

    /* Add camera element */
    ret = add_camera_element(mp, stripe_mode, NULL);
    if (ret)
        goto err;

    /* Add JPEG decoder if needed */
    static mpp_stats_t decoder_stats;
    ret = add_jpeg_decoder_element(mp, &decoder_stats);
    if (ret)
        goto err;

    /* Create split pipelines */
    mpp_t mp_split[2];
    static mpp_stats_t mpp_split_stats[2];
    ret = create_split_pipelines(mp, &user_data, mp_split, mpp_split_stats);
    if (ret)
        goto err;

    /* Add blaze detector convert element */
    static mpp_stats_t convert_blaze_stats;
    ret = add_blaze_detector_convert_element(mp_split[0], stripe_mode, &convert_blaze_stats);
    if (ret)
        goto err;

    user_data.blaze_detector_input_convert_mp = mp_split[0];

    /* Create background pipeline for blaze detector */
    mpp_t mp_bg;
    static mpp_stats_t mpp_stats_blaze;
    ret = create_blaze_detector_background_pipeline(mp_split[0], &user_data, &mp_bg, &mpp_stats_blaze);
    if (ret)
        goto err;

    /* Add blaze detector inference element */
    static mpp_stats_t blaze_detector_stats;
    ret = add_blaze_detector_inference_element(mp_bg, &user_data, &blaze_detector_stats);
    if (ret)
        goto err;

    /* Add null sink for blaze detector */
    ret = add_blaze_detector_nullsink(mp_bg);
    if (ret)
        goto err;

    user_data.blaze_detector_mp = mp_bg;

    /* Add landmark convert element */
    static mpp_stats_t convert_hand_ldmk_stats;
    ret = add_landmark_convert_element(mp_split[1], stripe_mode, &user_data, &convert_hand_ldmk_stats);
    if (ret)
        goto err;

    user_data.landmark_input_convert_mp = mp_split[1];

    /* Create background pipeline for landmark detection */
    mpp_t mp_bg_ldmk;
    static mpp_stats_t mpp_stats_bg_ldmk;
    ret = create_landmark_background_pipeline(mp_split[1], &user_data, &mp_bg_ldmk, &mpp_stats_bg_ldmk);
    if (ret)
        goto err;

    /* Add hand landmark inference element */
    static mpp_stats_t hand_landmark_stats;
    ret = add_hand_landmark_inference_element(mp_bg_ldmk, &user_data, &hand_landmark_stats);
    if (ret)
        goto err;

    user_data.landmark_mp = mp_bg_ldmk;

    /* Add gesture embedder inference element */
    static mpp_stats_t gesture_embedder_stats;
    ret = add_gesture_embedder_inference_element(mp_bg_ldmk, &user_data, &gesture_embedder_stats);
    if (ret)
        goto err;

    /* Add gesture classifier inference element */
    static mpp_stats_t gesture_classifier_stats;
    ret = add_gesture_classifier_inference_element(mp_bg_ldmk, &user_data, &gesture_classifier_stats);
    if (ret)
        goto err;

    /* Add null sink for landmark detection */
    ret = add_landmark_nullsink(mp_bg_ldmk);
    if (ret)
        goto err;

    /* Add display convert element */
    static mpp_stats_t convert_display_stats;
    ret = add_display_convert_element(mp, &convert_display_stats);
    if (ret)
        goto err;

    /* Add labeled rectangle element */
    static mpp_stats_t labels_stats;
    ret = add_labeled_rectangle_element(mp, &user_data, &labels_stats);
    if (ret)
        goto err;

#if !DISPLAY_TEXT
    /* Add display rotate element */
    static mpp_stats_t rotate_stats;
    ret = add_display_rotate_element(mp, &rotate_stats);
    if (ret)
        goto err;
#else
    /* Add compose element */
    static mpp_stats_t compose_stats;
    ret = add_compose_element(mp, &user_data, &txt_info, &compose_stats);
    if (ret)
        goto err;
#endif

    /* Add display element */
    ret = add_display_element(mp, stripe_mode);
    if (ret)
        goto err;

    /* Enable statistics */
    enable_stats();

    /* Start all pipelines */
    ret = start_pipelines(mp, mp_split, mp_bg, mp_bg_ldmk);
    if (ret)
        goto err;

    /* Run main loop */
    stats_collection_t stats = {
        .api_stats = &api_stats,
        .mpp_stats = &mpp_stats,
        .mpp_split_stats = mpp_split_stats,
        .mpp_stats_blaze = &mpp_stats_blaze,
        .mpp_stats_bg_ldmk = &mpp_stats_bg_ldmk,
        .decoder_stats = &decoder_stats,
        .convert_blaze_stats = &convert_blaze_stats,
        .blaze_detector_stats = &blaze_detector_stats,
        .convert_hand_ldmk_stats = &convert_hand_ldmk_stats,
        .hand_landmark_stats = &hand_landmark_stats,
        .gesture_embedder_stats = &gesture_embedder_stats,
        .gesture_classifier_stats = &gesture_classifier_stats,
        .convert_display_stats = &convert_display_stats,
        .labels_stats = &labels_stats,
#if !DISPLAY_TEXT
        .rotate_stats = &rotate_stats
#else
        .compose_stats = &compose_stats
#endif
    };
    run_main_loop(&user_data, &stats, &txt_info);

err:
    for (;;) {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}
