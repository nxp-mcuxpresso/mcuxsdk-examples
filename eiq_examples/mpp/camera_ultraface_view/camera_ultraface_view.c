/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to build a simple graph with 2 branches:
 * 2D camera -> split +-> image converter -> inference engine (model: Ultraface-slim)
 *                    +-> image converter(*) -> draw labeled rectangles -> display
 * The camera view finder branch: 
 * displays captured content on screen;
 * (*) image conversion may be skipped for cameras supporting same format as display;
 * the pipeline may deal with stripes of images instead of full-frame to reduce memory footprint;
 * this stripe mode can only be enabled when camera and display devices support it.
 * The inference engine branch:
 * runs a face detection model using TF-Lite micro inference engine;
 * the pipeline is configured to run inference engine in a background task.
 * The model output is displayed on UART console and on screen using labeled rectangles. */

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
#include "models/utils.h"

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

#ifndef APP_STRIPE_MODE
#define APP_STRIPE_MODE 0
#endif

/* Model data input */
#include APP_TFLITE_ULTRAFACE_DATA
/* Model info */
#include APP_TFLITE_ULTRAFACE_INFO

#if defined(APP_USE_NEUTRON16_MODEL) && (HAL_TFLM_TENSOR_ARENA_SIZE_KB < 254)
#error "Must set HAL_TFLM_TENSOR_ARENA_SIZE_KB >= 254"
#elif !defined(APP_USE_NEUTRON16_MODEL) && (HAL_TFLM_TENSOR_ARENA_SIZE_KB < 1536)
#error "Must set HAL_TFLM_TENSOR_ARENA_SIZE_KB >= 1536"
#endif

#include "ultraface_output_postproc.h"

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

/* label rect line width */
#define RECT_LINE_WIDTH 2

#define MODEL_ASPECT_RATIO   (1.0f * ULTRAFACE_WIDTH / ULTRAFACE_HEIGHT)
/* output is displayed in landscape mode */
#define DISPLAY_ASPECT_RATIO (1.0f * DISPLAY_LARGE_DIM / DISPLAY_SMALL_DIM)
/* camera aspect ratio */
#define CAMERA_ASPECT_RATIO  (1.0f * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT)

/*
 * The detection zone is a rectangle centered on the display. It has the same shape as the model input.
 * The rectangle dimensions are calculated based on the display small dim and respecting the model aspect ratio
 * The detection zone width and height depend on the display_aspect_ratio compared to the model aspect_ratio:
 * if the display_aspect_ratio >= model_aspect_ratio then :
 *                  (width, height) = (display_small_dim * model_aspect_ratio, display_small_dim)
 * if the display_aspect_ratio < model_aspect_ratio then :
 *                  (width, height) = (display_small_dim, display_small_dim / model_aspect_ratio)
 *
 * */
#define DETECTION_ZONE_RECT_HEIGHT ((DISPLAY_ASPECT_RATIO >= MODEL_ASPECT_RATIO) ? \
		DISPLAY_SMALL_DIM : (DISPLAY_SMALL_DIM / MODEL_ASPECT_RATIO))
#define DETECTION_ZONE_RECT_WIDTH  ((DISPLAY_ASPECT_RATIO >= MODEL_ASPECT_RATIO) ? \
		(DISPLAY_SMALL_DIM * MODEL_ASPECT_RATIO) : DISPLAY_SMALL_DIM)

/* detection zone top/left offsets */
#define DETECTION_ZONE_RECT_TOP  (DISPLAY_SMALL_DIM - DETECTION_ZONE_RECT_HEIGHT)/2
#define DETECTION_ZONE_RECT_LEFT (DISPLAY_LARGE_DIM - DETECTION_ZONE_RECT_WIDTH)/2

/*
 *  The computation of the crop size(width and height) and the crop top/left depends on the detection
 *  zone dims and offsets and on the camera-display scaling factor SF which is calculated differently
 *  depending on 2 constraints:
 *           * Constraint 1: display aspect ratio compared to the camera aspect ratio.
 *           * Constraint 2: SWAP_DIMS value.
 * if the display_aspect_ratio < camera_aspect_ratio :
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_WIDTH / APP_CAMERA_WIDTH
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_HEIGHT / APP_CAMERA_HEIGHT
 * if the display_aspect_ratio >= camera_aspect_ratio:
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_HEIGHT / APP_CAMERA_HEIGHT
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_WIDTH / APP_CAMERA_WIDTH
 * the crop dims and offsets are calculated in the following way:
 * CROP_SIZE_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_SIZE_LEFT = DETECTION_ZONE_RECT_WIDTH / SF
 * CROP_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_LEFT = DETECTION_ZONE_RECT_LEFT / SF
 * */
#if ((DISPLAY_LARGE_DIM * APP_CAMERA_HEIGHT) < (DISPLAY_SMALL_DIM * APP_CAMERA_WIDTH))
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * APP_CAMERA_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * APP_CAMERA_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * APP_CAMERA_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * APP_CAMERA_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#else   /* DISPLAY_ASPECT_RATIO() >= CAMERA_ASPECT_RATIO() */
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * APP_CAMERA_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * APP_CAMERA_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * APP_CAMERA_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * APP_CAMERA_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#endif  /* DISPLAY_ASPECT_RATIO() < CAMERA_ASPECT_RATIO() */

/* Detected boxes offsets */
#define BOXES_OFFSET_LEFT DETECTION_ZONE_RECT_LEFT
#define BOXES_OFFSET_TOP  DETECTION_ZONE_RECT_TOP

#define OUTPUT_PRINT_PERIOD_MS 1000

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] =  APP_CAMERA_NAME;

#define ULTRAFACE_DETECTION_LABEL "face"

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LABEL_RECTS     10
#define NUM_BOXES_MAX       MIN(APP_MAX_BOXES, ULTRAFACE_MAX_POINTS) /* max nb of boxes to filter */

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_elem_handle_t elem;
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    box_data boxes[NUM_BOXES_MAX];
    uint32_t accessing; /* boolean protecting access */
    int detected_count;          /* number of detected boxes */
    int inference_time_ms;
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

    if (pdPASS != ret) {
        PRINTF("Failed to create app_task task");
        while (1);
    }

    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);
    return 0;
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
        rects[box_counter].left = (int)((boxes[i].left * DETECTION_ZONE_RECT_WIDTH)/ ULTRAFACE_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].right = (int)((boxes[i].right * DETECTION_ZONE_RECT_WIDTH)/ ULTRAFACE_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].bottom = (int)((boxes[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/ULTRAFACE_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].top = (int)((boxes[i].top * DETECTION_ZONE_RECT_HEIGHT)/ULTRAFACE_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;
        uint8_t label_size = sizeof(rects[box_counter].label);
        strncpy((char *) rects[box_counter].label, ULTRAFACE_DETECTION_LABEL, label_size-1);
        rects[box_counter].label[label_size-1] = '\0';  /* in case label has been truncated */

        box_counter++;
    }
}

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data)
{
    status_t ret;
    const mpp_inference_cb_param_t *inf_output;

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;

        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
            ret = ULTRAFACE_ProcessOutput(
                    inf_output,
                    app_priv->boxes,
                    NUM_BOXES_MAX);
            if (ret != kStatus_Success)
                PRINTF("mpp_event_listener: process output error!");

            app_priv->detected_count = 0;
            app_priv->inference_time_ms = inf_output->inference_time_ms;

            /* count valid results */
            for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
            {
                if (app_priv->boxes[i].score > 0)
                    app_priv->detected_count++;
            }
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }

        /* update labeled rectangle */
        if ( (app_priv->mp != NULL) && (app_priv->elem != 0) ) {
            mpp_element_params_t params;
            /* detected_count contains at least the detection zone box */
            params.labels.detected_count = app_priv->detected_count + 1;
            params.labels.max_count = MAX_LABEL_RECTS;
            params.labels.rectangles = app_priv->labels;
            boxes_to_rects(app_priv->boxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);

            mpp_element_update(app_priv->mp, app_priv->elem, &params);
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

    /* init API */
    mpp_api_params_t api_param = {0};
#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param.rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param.rc_cycle_min = APP_RC_CYCLE_MIN;
#endif
    ret = mpp_api_init(&api_param);
    if (ret)
        goto err;

    mpp_t mp;
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;

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
    ret = mpp_camera_add(mp, s_camera_name, &cam_params);
    if (ret) {
    	PRINTF("Failed to add camera %s\n", s_camera_name);
    	goto err;
    }

    /* split the pipeline into 2 branches:
     * - first for the conversion to model
     * - second for the label-rect draw & display
     * this order is needed to avoid running inference on an image containing label-rect */
    mpp_t mp_split;
    mpp_params.exec_flag = MPP_EXEC_RC;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* First do crop + resize + color convert */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width = ULTRAFACE_WIDTH;
    elem_params.convert.out_buf.height = ULTRAFACE_HEIGHT;
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
    elem_params.convert.scale.width = ULTRAFACE_WIDTH;
    elem_params.convert.scale.height = ULTRAFACE_HEIGHT;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;
    /* then add a flip */
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    elem_params.convert.flip = FLIP_HORIZONTAL;
    elem_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif
    elem_params.convert.stripe_in = stripe_mode;
    elem_params.convert.stripe_out = false; /* model takes full frames */

    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* create a background mpp (preempt-able branch) for the ML Inference
     * because it may take longer than capture period.
     * Inference runs an ultraface TF-Lite model */
    mpp_t mp_bg;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_background(mp, &mpp_params, &mp_bg);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* configure TFlite element with model */
    mpp_element_params_t ultraface_params;
    memset(&ultraface_params, 0 , sizeof(mpp_element_params_t));

    ultraface_params.ml_inference.model_data = ultraface_data;
    ultraface_params.ml_inference.model_size = ultraface_data_len;
    ultraface_params.ml_inference.model_input_mean = ULTRAFACE_INPUT_MEAN;
    ultraface_params.ml_inference.model_input_std = ULTRAFACE_INPUT_STD;
    ultraface_params.ml_inference.inference_params.num_inputs = 1;
    ultraface_params.ml_inference.inference_params.num_outputs = 1;
    ultraface_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    ultraface_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &ultraface_params, NULL);
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
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width = APP_CAMERA_WIDTH;
    elem_params.convert.out_buf.height = APP_CAMERA_HEIGHT;
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;

    /* then add a flip */
    elem_params.convert.flip = FLIP_HORIZONTAL;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE;

    ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);

    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }
#endif

    /* add one label rectangle */
    memset(&elem_params, 0, sizeof(elem_params));
    memset(&user_data.labels, 0, sizeof(user_data.labels));

    /* params init */
    elem_params.labels.max_count = MAX_LABEL_RECTS;
    elem_params.labels.detected_count = 1;
    elem_params.labels.rectangles = user_data.labels;

    /* first add detection zone box */
    user_data.labels[0].top    = DETECTION_ZONE_RECT_TOP;
    user_data.labels[0].left   = DETECTION_ZONE_RECT_LEFT;
    user_data.labels[0].bottom = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_HEIGHT;
    user_data.labels[0].right  = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_WIDTH;
    user_data.labels[0].line_width = RECT_LINE_WIDTH;
    user_data.labels[0].line_color.rgb.G = 0xff;
    strcpy((char *)user_data.labels[0].label, "Detection zone");

    /* retrieve the element handle while add api */
    ret = mpp_element_add(mp_split, MPP_ELEMENT_LABELED_RECTANGLE, &elem_params, &user_data.elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        goto err;
    }
    /* pass the mpp of the element 'label rectangle' to callback */
    user_data.mp = mp_split;

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    /* then rotate if needed */
    if (APP_DISPLAY_LANDSCAPE_ROTATE != ROTATE_0) {
        memset(&elem_params, 0, sizeof(elem_params));
        /* set output buffer dims */
        elem_params.convert.out_buf.width = APP_DISPLAY_WIDTH;
        elem_params.convert.out_buf.height = APP_DISPLAY_HEIGHT;
        elem_params.convert.angle = APP_DISPLAY_LANDSCAPE_ROTATE;
        elem_params.convert.ops = MPP_CONVERT_ROTATE;
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

    /* start preempt-able pipeline branch */
    ret = mpp_start(mp_bg, 0);
    if (ret) {
        PRINTF("Failed to start preempt-able pipeline branch");
        goto err;
    }
    /* start secondary pipeline branch */
    ret = mpp_start(mp_split, 0);
    if (ret) {
        PRINTF("Failed to start secondary pipeline branch");
        goto err;
    }
    /* start main pipeline branch */
    ret = mpp_start(mp, 1);
    if (ret) {
        PRINTF("Failed to start main pipeline branch");
        goto err;
    }

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        xTaskDelayUntil( &xLastWakeTime, xFrequency );
        if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0)) {
            PRINTF("inference time %d ms \r\n", user_data.inference_time_ms);
            if (user_data.detected_count <= 0) {
                PRINTF("ultraface : no face detected\r\n");
            } else {
                for (int i = 0; i < NUM_BOXES_MAX; i++)
                {
                    if (user_data.boxes[i].area > 0)
                    {
                        PRINTF("ultraface : box %d label %s score %d(%%)\r\n", i,
                                ULTRAFACE_DETECTION_LABEL, (int)(user_data.boxes[i].score * 100.0f));
                    }
                }
            }

            __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
        }
    }

    /* pause application task */
    vTaskSuspend(NULL);

err:
    for (;;) {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}
