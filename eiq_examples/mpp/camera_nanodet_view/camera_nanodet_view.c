/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to build a simple graph:
 * 2D camera -> split -> image converter -> draw labeled rectangles -> display
 *                   +-> image converter -> inference engine (model: Nanodet-M)
 * The camera view finder is displayed on screen
 * The model performs object detection among a list of 80 object types (see nanodet_labels.h),
 * the model output is displayed on UART console by application */

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

/* utility functions */
#include "utils.h"

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

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

/* Model data input */
#include APP_TFLITE_NANODET_DATA
#include APP_TFLITE_NANODET_INFO

#include "nanodet_labels.h"
#include "nanodet_m_output_postproc.h"

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

/* if display_aspect_ratio > view_aspect_ratio?*/
#if (DISPLAY_LARGE_DIM * VIEW_SMALL_DIM > VIEW_LARGE_DIM * DISPLAY_SMALL_DIM)
#define SCALED_VIEW_WIDTH APP_DISPLAY_WIDTH
#define SCALED_VIEW_HEIGHT (APP_DISPLAY_WIDTH * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT )
#else /*?if display_aspect_ratio < view_aspect_ratio */
#define SCALED_VIEW_WIDTH (APP_DISPLAY_HEIGHT * APP_CAMERA_HEIGHT / APP_CAMERA_WIDTH )
#define SCALED_VIEW_HEIGHT APP_DISPLAY_HEIGHT
#endif


/* label rect line width */
#define RECT_LINE_WIDTH 2

#define MODEL_ASPECT_RATIO   (1.0f * NANODET_WIDTH / NANODET_HEIGHT)
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

#define STATS_PRINT_PERIOD_MS 1000

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] =  APP_CAMERA_NAME;

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LABEL_RECTS 10
#define NUM_BOXES_MAX   MIN(APP_MAX_BOXES, NANODET_MAX_POINTS)  /* Nanodet max bounding boxes */

typedef struct _user_data_t {
    int inference_frame_num;
    uint32_t inference_time_ms;
    mpp_t mp;
    mpp_elem_handle_t elem;
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    /* detected boxes */
    box_data  boxes[NUM_BOXES_MAX];
    /* detected boxes count */
    int detected_count;
    uint32_t accessing; /* boolean protecting access */
} user_data_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);

/*******************************************************************************
 * Code
 ******************************************************************************/
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

    if (pdPASS != ret)
    {
        PRINTF("Failed to create app_task task");
        while (1);
    }

    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);
    return 0;
}

/* Translate boxes into labeled rectangles using display characteristics */
void boxes_to_rects(box_data boxes[], uint32_t num_boxes, uint32_t max_boxes, mpp_labeled_rect_t *rects) {

    uint32_t box_counter = 1;

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes && box_counter < max_boxes; i++) {
        if (boxes[i].area == 0)
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes[i].left * DETECTION_ZONE_RECT_WIDTH)/ NANODET_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].right = (int)((boxes[i].right * DETECTION_ZONE_RECT_WIDTH)/ NANODET_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].bottom = (int)((boxes[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/NANODET_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].top = (int)((boxes[i].top * DETECTION_ZONE_RECT_HEIGHT)/NANODET_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;
        uint8_t label_size = sizeof(rects[box_counter].label);
        strncpy((char *) rects[box_counter].label, nanodet_labels[boxes[i].label], label_size-1);
        rects[box_counter].label[label_size-1] = '\0';  /* in case label has been truncated */

        box_counter++;
    }
}

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data) {
    status_t ret;
    const mpp_inference_cb_param_t *inf_output;

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;

        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
        {
            ret = NANODET_ProcessOutput(inf_output, app_priv->boxes);
            if (ret != kStatus_Success)
                PRINTF("mpp_event_listener: process output error!");
            app_priv->detected_count = 0;
            /* count valid results */
            for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
            {
                if (app_priv->boxes[i].score > 0)
                    app_priv->detected_count++;
            }
            app_priv->inference_time_ms = inf_output->inference_time_ms;
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }

        if ( (app_priv->mp != NULL) && (app_priv->elem != 0) ){
            mpp_element_params_t params;
            memset(&params, 0, sizeof(params));
            /* detected_count contains at least the detection zone box */
            params.labels.detected_rect = app_priv->detected_count + 1;
            params.labels.max_rect = MAX_LABEL_RECTS;
            params.labels.rectangles = app_priv->labels;
            boxes_to_rects(app_priv->boxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);
            mpp_element_update(app_priv->mp, app_priv->elem, &params, true);
        }

        app_priv->inference_frame_num++;
        break;
    case MPP_EVENT_INVALID:
    default:
        /* nothing to do */
        break;
    }

    return 0;
}

static void app_task(void *params)
{
    static user_data_t user_data = {0};
    int ret;

    bool stripe_mode = (APP_STRIPE_MODE > 0)? true : false;

    PRINTF("[%s]\r\n", mpp_get_version());
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    /* Print camera parameters */
    PRINTF("\r\n=== Camera Parameters ===\r\n");
    PRINTF("APP_CAMERA_WIDTH: %d\r\n", APP_CAMERA_WIDTH);
    PRINTF("APP_CAMERA_HEIGHT: %d\r\n", APP_CAMERA_HEIGHT);

    /* Print model input dimensions */
    PRINTF("\r\n=== Model Input Dimensions ===\r\n");
    PRINTF("NANODET_WIDTH: %d\r\n", NANODET_WIDTH);
    PRINTF("NANODET_HEIGHT: %d\r\n", NANODET_HEIGHT);
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

    /* init API */
    mpp_api_params_t api_param = {0};
    static mpp_stats_t api_stats = {0};
#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param.rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param.rc_cycle_min = APP_RC_CYCLE_MIN;
#endif
    api_param.stats = &api_stats;
    ret = mpp_api_init(&api_param);
    if (ret)
        goto err;

    mpp_t mp;
    mpp_params_t mpp_params;
    static mpp_stats_t mpp_stats = {0};
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;
    mpp_params.stats = &mpp_stats;

    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;

    mpp_camera_params_t cam_params;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = APP_CAMERA_HEIGHT;
    cam_params.width =  APP_CAMERA_WIDTH;
    cam_params.format = APP_CAMERA_FORMAT;
    cam_params.fps    = 30;
    cam_params.stripe = stripe_mode;
#ifdef USE_USB_CAMERA
    cam_params.in_advance_enqueue = true;
#endif
    ret = mpp_camera_add(mp, s_camera_name, &cam_params, NULL);
    if (ret) {
        PRINTF("Failed to add camera %s\n", s_camera_name);
        goto err;
    }

    static mpp_stats_t decoder_stats = {0};
    if (APP_CAMERA_FORMAT == MPP_PIXEL_JPEG)
    {
        /* Add element jpeg decode */
        mpp_element_params_t elem_params_decoder;
        memset(&elem_params_decoder, 0, sizeof(mpp_element_params_t));
#ifdef APP_DECODE_BACKEND_NAME
        elem_params_decoder.decode.dev_name = APP_DECODE_BACKEND_NAME;
#else
        elem_params_decoder.decode.dev_name = NULL;
#endif
        elem_params_decoder.decode.width = APP_CAMERA_WIDTH;
        elem_params_decoder.decode.height = APP_CAMERA_HEIGHT;

        elem_params_decoder.decode.out_format = MPP_PIXEL_YUYV;

        elem_params_decoder.stats = &decoder_stats;

        ret = mpp_element_add(mp, MPP_ELEMENT_IMG_DECODE, &elem_params_decoder, NULL);
        if (ret)
        {
            PRINTF("Failed to add element DECODE\n");
            goto err;
        }
        else{
            PRINTF("Added HW jpeg decoder to the branch mp !\r\n");
        }
    }

    /* split the pipeline into 2 branches:
     * - first for the conversion to model
     * - second for the label-rect draw & display
     * this order is needed to avoid running inference on an image containing label-rect */
    mpp_t mp_split;
    static mpp_stats_t mpp_split_stats = {0};
    mpp_params.exec_flag = MPP_EXEC_RC;
    mpp_params.stats = &mpp_split_stats;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* On the preempt-able branch run the ML Inference (using a nanodet-m TF-Lite/Glow model) */
    /* First do crop + resize + color convert */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    static mpp_stats_t convert_stats = {0};
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width = NANODET_WIDTH;
    elem_params.convert.out_buf.height = NANODET_HEIGHT;
    /* color convert */
    elem_params.convert.pixel_format = MPP_PIXEL_RGB;
    elem_params.convert.ops = MPP_CONVERT_COLOR;
    /* crop center of image */
    elem_params.convert.crop.top = CROP_TOP;
    elem_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    elem_params.convert.crop.left = CROP_LEFT;
    elem_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    elem_params.convert.ops |= MPP_CONVERT_CROP;
    /* resize: scaling parameters */
    elem_params.convert.scale.width = NANODET_WIDTH;
    elem_params.convert.scale.height = NANODET_HEIGHT;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;
    /* then add a flip */
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    elem_params.convert.flip = APP_SRC_DISPLAY_FLIP;
    elem_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif
    elem_params.convert.stripe_in = stripe_mode;
    elem_params.convert.stripe_out = false; /* model takes full frames */

    elem_params.stats = &convert_stats;

    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* create a background mpp (preempt-able branch) for the ML Inference
     * because it may take longer than capture period.
     * Inference runs an persondetect TF-Lite model */
    mpp_t mp_bg;
    static mpp_stats_t mpp_stats_bg = {0};
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    mpp_params.stats = &mpp_stats_bg;

    ret = mpp_background(mp, &mpp_params, &mp_bg);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* configure TFlite element with model */
    mpp_element_params_t nanodet_params;
    static mpp_stats_t nanodet_stats = {0};
    memset(&nanodet_params, 0 , sizeof(mpp_element_params_t));

    nanodet_params.ml_inference.model_data = nanodet_m_0_5x_nhwc_nopermute_tflite;
    nanodet_params.ml_inference.model_size = nanodet_m_0_5x_nhwc_nopermute_tflite_len;
    nanodet_params.ml_inference.model_input_mean = NANODET_INPUT_MEAN;
    nanodet_params.ml_inference.model_input_std = NANODET_INPUT_STD;
    nanodet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    nanodet_params.ml_inference.inference_params.num_inputs = 1;
    nanodet_params.ml_inference.inference_params.num_outputs = 2;
    nanodet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    nanodet_params.stats = &nanodet_stats;

    ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &nanodet_params, NULL);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE");
        goto err;
    }
    /* close the pipeline with a null sink */
    ret = mpp_nullsink_add(mp_bg);
    if (ret) {
        PRINTF("Failed to add NULL sink\n");
        goto err;
    }

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    /* On the secondary branch of the pipeline, send the frame to the display */
    /* First do color-convert + flip */
    memset(&elem_params, 0, sizeof(elem_params));
    static mpp_stats_t convert_display_stats = {0};
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width = APP_CAMERA_WIDTH;
    elem_params.convert.out_buf.height = APP_CAMERA_HEIGHT;
    /* Add color convert */
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    elem_params.convert.ops = MPP_CONVERT_COLOR;
    /* then add a flip */
    elem_params.convert.flip = APP_SRC_DISPLAY_FLIP;
    elem_params.convert.ops |=  MPP_CONVERT_ROTATE;

    elem_params.stats = &convert_display_stats;

    ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);

    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }
#endif

    /* add one label rectangle */
    memset(&elem_params, 0, sizeof(elem_params));
    memset(&user_data.labels, 0, sizeof(user_data.labels));
    static mpp_stats_t labels_stats = {0};

    /* params init */
    elem_params.labels.max_rect = MAX_LABEL_RECTS;
    elem_params.labels.detected_rect = 1;
    elem_params.labels.rectangles = user_data.labels;

    /* first add detection zone box */
    user_data.labels[0].top    = DETECTION_ZONE_RECT_TOP;
    user_data.labels[0].left   = DETECTION_ZONE_RECT_LEFT;
    user_data.labels[0].bottom = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_HEIGHT;
    user_data.labels[0].right  = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_WIDTH;
    user_data.labels[0].line_width = RECT_LINE_WIDTH;
    user_data.labels[0].line_color.rgb.G = 0xff;
    strcpy((char *)user_data.labels[0].label, "Detection zone");

    elem_params.stats = &labels_stats;

    /* retrieve the element handle while add api */
    ret = mpp_element_add(mp_split, MPP_ELEMENT_LABELED_RECTANGLE, &elem_params, &user_data.elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        goto err;
    }
    /* pass the mpp of the element 'label rectangle' to callback */
    user_data.mp = mp_split;

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    static mpp_stats_t rotate_stats = {0};
    /* then rotate if needed */
    if (APP_DISPLAY_LANDSCAPE_ROTATE != ROTATE_0) {
        memset(&elem_params, 0, sizeof(elem_params));
        /* set output buffer dims */
        elem_params.convert.out_buf.width = APP_DISPLAY_WIDTH;
        elem_params.convert.out_buf.height = APP_DISPLAY_HEIGHT;
        /* Add rotate */
        elem_params.convert.angle = APP_DISPLAY_LANDSCAPE_ROTATE;
        elem_params.convert.ops = MPP_CONVERT_ROTATE;
#ifdef SCALED_VIEW
        /* Add scale */
        elem_params.convert.scale.width =  SCALED_VIEW_WIDTH;
        elem_params.convert.scale.height = SCALED_VIEW_HEIGHT;
        elem_params.convert.ops |= MPP_CONVERT_SCALE;
#endif
        elem_params.stats = &rotate_stats;
        ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);

        if (ret) {
            PRINTF("Failed to add element CONVERT\r\n");
            goto err;
        }
    }
#endif

    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = stripe_mode;
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
    disp_params.rotate = APP_DISPLAY_LANDSCAPE_ROTATE;
#endif
    ret = mpp_display_add(mp_split, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", s_display_name);
        goto err;
    }

    mpp_stats_enable(MPP_STATS_GRP_API);
    mpp_stats_enable(MPP_STATS_GRP_MPP);
    mpp_stats_enable(MPP_STATS_GRP_ELEMENT);

    /* start preempt-able pipeline branch */
    ret = mpp_start(mp_bg, 0, false);
    if (ret) {
        PRINTF("Failed to start preempt-able pipeline branch");
        goto err;
    }
    /* start secondary pipeline branch */
    ret = mpp_start(mp_split, 0, false);
    if (ret) {
        PRINTF("Failed to start secondary pipeline branch");
        goto err;
    }
    /* start main pipeline branch */
    ret = mpp_start(mp, 1, false);
    if (ret) {
        PRINTF("Failed to start main pipeline branch");
        goto err;
    }

    TickType_t xLastWakeTime;
    int i;
    const TickType_t xFrequency = STATS_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    uint32_t last_inf_frame_num = user_data.inference_frame_num;
    bool detected = false;
    for (;;) {
        xTaskDelayUntil( &xLastWakeTime, xFrequency );
        if (last_inf_frame_num != user_data.inference_frame_num)
        {
            if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
            {
                detected = false;
                for (i = 0; i < NUM_BOXES_MAX; i++)
                {
                    if (user_data.boxes[i].area > 0)
                    {
                        PRINTF("nanodet : box %d label %s score %d(%%)\r\n", i,
                                nanodet_labels[user_data.boxes[i].label], (int)(user_data.boxes[i].score * 100.0f));
                        detected = true;
                    }
                }
                if (!detected)
                    PRINTF("nanodet : no detection\r\n");
                PRINTF("inference time %u ms\r\n", user_data.inference_time_ms);
                __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
            }
            last_inf_frame_num = user_data.inference_frame_num;
        }

#if PRINT_API_STATS
        /* Print API stats */
        mpp_stats_disable(MPP_STATS_GRP_API);
        PRINTF("\r\n=== API Stats ===\r\n");
        PRINTF("RC cycle: %d ms\r\n", api_stats.api.rc_cycle);
        PRINTF("RC cycle max: %d ms\r\n", api_stats.api.rc_cycle_max);
        PRINTF("PR slot: %d ms\r\n", api_stats.api.pr_slot);
        PRINTF("PR rounds: %d\r\n", api_stats.api.pr_rounds);
        PRINTF("App slot: %d ms\r\n", api_stats.api.app_slot);
        PRINTF("CPU load: %d %%\r\n", api_stats.api.cpu_load);
        mpp_stats_enable(MPP_STATS_GRP_API);
#endif

#if PRINT_MPP_STATS
        /* Print MPP stats */
        mpp_stats_disable(MPP_STATS_GRP_MPP);
        PRINTF("\r\n=== MPP Stats ===\r\n");
        PRINTF("Main pipeline fps: %d\r\n", mpp_stats.mpp.fps);
        PRINTF("Main pipeline exec_time: %d ms\r\n", mpp_stats.mpp.mpp_exec_time);
        PRINTF("Split pipeline fps: %d\r\n", mpp_split_stats.mpp.fps);
        PRINTF("Split pipeline exec_time: %d ms\r\n", mpp_split_stats.mpp.mpp_exec_time);
        PRINTF("Background pipeline fps: %d\r\n", mpp_stats_bg.mpp.fps);
        PRINTF("Background pipeline exec_time: %d ms\r\n", mpp_stats_bg.mpp.mpp_exec_time);
        mpp_stats_enable(MPP_STATS_GRP_MPP);
#endif

#if PRINT_ELEM_STATS
        /* Print Element stats */
        mpp_stats_disable(MPP_STATS_GRP_ELEMENT);
        PRINTF("\r\n=== Element Stats ===\r\n");
        PRINTF("Decoder exec_time: %d ms\r\n", decoder_stats.elem.elem_exec_time);
        PRINTF("Convert exec_time: %d ms\r\n", convert_stats.elem.elem_exec_time);
        PRINTF("Nanodet exec_time: %d ms\r\n", nanodet_stats.elem.elem_exec_time);
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
        PRINTF("Convert Display exec_time: %d ms\r\n", convert_display_stats.elem.elem_exec_time);
#endif
        PRINTF("Labels exec_time: %d ms\r\n", labels_stats.elem.elem_exec_time);
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
        PRINTF("Rotate exec_time: %d ms\r\n", rotate_stats.elem.elem_exec_time);
#endif
        mpp_stats_enable(MPP_STATS_GRP_ELEMENT);
#endif

#if (PRINT_API_STATS || PRINT_MPP_STATS || PRINT_ELEM_STATS)
        PRINTF("\r\n");
#endif
    }

err:
    for (;;)
    {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

