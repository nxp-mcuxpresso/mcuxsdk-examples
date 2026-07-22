/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* @brief This example application shows usage of MultiMedia Pipeline to build a pipeline graph:
  * static image -> split -> image converter -> draw labeled rectangles -> display
  *                      +-> image converter -> inference engine (model: Nanodet-M)
  *                      +-> image converter -> inference engine (model: PersonDetect)
  * The static image with detection results from both models is displayed on screen.
  * The Nanodet-M model performs object detection among 80 object types (see nanodet_labels.h).
  * The PersonDetect model performs person detection.
  * Both model outputs are displayed on UART console and as labeled bounding boxes on screen. */
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
#include "nanodet_labels.h"
#include "nanodet_m_output_postproc.h"
#include "persondetect_output_postprocess.h"

/* Model info */
#include APP_TFLITE_NANODET_INFO
#include APP_TFLITE_PERSONDETECT_INFO


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LABEL_RECTS 10
#define NUM_BOXES_MAX_NANODET           MIN(APP_MAX_BOXES, NANODET_MAX_POINTS)  /* Nanodet max bounding boxes */
#define NUM_BOXES_MAX_PERSONDETECT      80 /* Persondetect max bounding boxes */

typedef struct _user_data_t {
    int inference_frame_num_nanodet;
    int inference_frame_num_persondetect;
    uint32_t inference_time_nanodet;
    uint32_t inference_time_persondetect;
    mpp_t mp;
    mpp_elem_handle_t elem;
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    /* detected boxes */
    box_data  boxes_nanodet[NUM_BOXES_MAX_NANODET];
    box_data  boxes_persondetect[NUM_BOXES_MAX_PERSONDETECT];
    /* detected boxes count */
    int detected_count_nanodet;
    int detected_count_persondetect;
    uint32_t accessing; /* boolean protecting access */
} user_data_t;

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* Model data input */
#include APP_TFLITE_NANODET_DATA
#include APP_TFLITE_PERSONDETECT_DATA

#include "skigirl_COCO_320_320_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_WIDTH
void *image_data = (void *)skigirl_COCO_320_320_bgra_data;

/*
 * SWAP_DIMS = 1 if source/display dims are reversed
 * SWAP_DIMS = 0 if source/display have the same orientation
 */
#define SWAP_DIMS (((APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_90) || (APP_DISPLAY_LANDSCAPE_ROTATE == ROTATE_270)) ? 1 : 0)

/* display small and large dims */
#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM MAX(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

#define MODEL_ASPECT_RATIO   (1.0f * NANODET_WIDTH / NANODET_HEIGHT)
/* output is displayed in landscape mode */
#define DISPLAY_ASPECT_RATIO (1.0f * DISPLAY_LARGE_DIM / DISPLAY_SMALL_DIM)
/* camera aspect ratio */
#define CAMERA_ASPECT_RATIO  (1.0f * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT)

/* label rect line width */
#define RECT_LINE_WIDTH 2

/*
 * The detection zone is a rectangle that has the same shape as the model input.
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
#define DETECTION_ZONE_RECT_LEFT 0

/*
 *  The computation of the crop size(width and height) and the crop top/left depends on the detection
 *  zone dims and offsets and on the source-display scaling factor SF which is calculated differently
 *  depending on 2 constraints:
 *           * Constraint 1: display aspect ratio compared to the source aspect ratio.
 *           * Constraint 2: SWAP_DIMS value.
 * if the display_aspect_ratio < source_aspect_ratio :
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_WIDTH / SRC_IMAGE_WIDTH
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_HEIGHT / SRC_IMAGE_HEIGHT
 * if the display_aspect_ratio >= source_aspect_ratio:
 *            - SWAP_DIMS = 0: SF = APP_DISPLAY_HEIGHT / SRC_IMAGE_HEIGHT
 *            - SWAP_DIMS = 1: SF = APP_DISPLAY_WIDTH / SRC_IMAGE_WIDTH
 * the crop dims and offsets are calculated in the following way:
 * CROP_SIZE_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_SIZE_LEFT = DETECTION_ZONE_RECT_WIDTH / SF
 * CROP_TOP = DETECTION_ZONE_RECT_HEIGHT / SF
 * CROP_LEFT = DETECTION_ZONE_RECT_LEFT / SF
 * */
#if ((DISPLAY_LARGE_DIM * SRC_IMAGE_HEIGHT) < (DISPLAY_SMALL_DIM * SRC_IMAGE_WIDTH))
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * SRC_IMAGE_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * SRC_IMAGE_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * SRC_IMAGE_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * SRC_IMAGE_WIDTH) / (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH))
#else   /* DISPLAY_ASPECT_RATIO() >= SOURCE_ASPECT_RATIO() */
#define CROP_SIZE_TOP   ((DETECTION_ZONE_RECT_HEIGHT * SRC_IMAGE_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_SIZE_LEFT  ((DETECTION_ZONE_RECT_WIDTH * SRC_IMAGE_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))

#define CROP_TOP  ((DETECTION_ZONE_RECT_TOP * SRC_IMAGE_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#define CROP_LEFT ((DETECTION_ZONE_RECT_LEFT * SRC_IMAGE_HEIGHT) / (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT))
#endif  /* DISPLAY_ASPECT_RATIO() < SOURCE_ASPECT_RATIO() */

/* Detected boxes offsets */
#define BOXES_OFFSET_LEFT DETECTION_ZONE_RECT_LEFT
#define BOXES_OFFSET_TOP  DETECTION_ZONE_RECT_TOP

#define STATS_PRINT_PERIOD_MS 1000

static const char s_display_name[] = APP_DISPLAY_NAME;

#define PERSONDETECT_DETECTION_LABEL "person"

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

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
void boxes_to_rects(box_data boxes_nanodet[], uint32_t num_boxes_nanodet,
                    box_data boxes_persondetect[], uint32_t num_boxes_persondetect,
                    uint32_t max_boxes, mpp_labeled_rect_t *rects)
{
    uint32_t box_counter = 1;

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes_nanodet && box_counter < max_boxes; i++) {
        if (boxes_nanodet[i].area == 0)
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes_nanodet[i].left * DETECTION_ZONE_RECT_WIDTH)/ NANODET_WIDTH) + BOXES_OFFSET_TOP;
        rects[box_counter].right = (int)((boxes_nanodet[i].right * DETECTION_ZONE_RECT_WIDTH)/ NANODET_WIDTH) + BOXES_OFFSET_TOP;
        rects[box_counter].bottom = (int)((boxes_nanodet[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/NANODET_HEIGHT) + BOXES_OFFSET_LEFT;
        rects[box_counter].top = (int)((boxes_nanodet[i].top * DETECTION_ZONE_RECT_HEIGHT)/NANODET_HEIGHT) + BOXES_OFFSET_LEFT;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;
        rects[box_counter].line_color.rgb.G = 0x00;
        rects[box_counter].line_color.rgb.R = 0x00;
        uint8_t label_size = sizeof(rects[box_counter].label);
        snprintf((char *) rects[box_counter].label, label_size, "nanodet: %s", nanodet_labels[boxes_nanodet[i].label]);
        rects[box_counter].label[label_size-1] = '\0';  /* in case label has been truncated */

        box_counter++;
    }

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes_persondetect && box_counter < max_boxes; i++) {
        if (boxes_persondetect[i].area == 0)
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes_persondetect[i].left * DETECTION_ZONE_RECT_WIDTH)/ PERSONDETECT_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].right = (int)((boxes_persondetect[i].right * DETECTION_ZONE_RECT_WIDTH)/ PERSONDETECT_WIDTH) + BOXES_OFFSET_LEFT;
        rects[box_counter].bottom = (int)((boxes_persondetect[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/PERSONDETECT_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].top = (int)((boxes_persondetect[i].top * DETECTION_ZONE_RECT_HEIGHT)/PERSONDETECT_HEIGHT) + BOXES_OFFSET_TOP;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.R = 0xff;
        rects[box_counter].line_color.rgb.G = 0x00;
        rects[box_counter].line_color.rgb.B = 0x00;
        uint8_t label_size = sizeof(rects[box_counter].label);
        snprintf((char *) rects[box_counter].label, label_size, "persondet: %s", PERSONDETECT_DETECTION_LABEL);
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

        switch (inf_output->model_id) {
            case NANODET_MODEL_ID:
                /* check that we can modify the user data (not accessed by other task) */
                if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
                {
                    ret = NANODET_ProcessOutput(inf_output, app_priv->boxes_nanodet);
                    if (ret != kStatus_Success)
                        PRINTF("mpp_event_listener: process output error!");

                    app_priv->detected_count_nanodet = 0;
                    app_priv->inference_time_nanodet = inf_output->inference_time_ms;

                    /* count valid results */
                    for (uint32_t i = 0; i < NUM_BOXES_MAX_NANODET; i++)
                    {
                        if (app_priv->boxes_nanodet[i].score > 0)
                            app_priv->detected_count_nanodet++;
                    }
                    app_priv->inference_frame_num_nanodet++;
                    /* end of modification of user data */
                    __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
                }
                break;

            case PERSONDETECT_MODEL_ID:
                /* check that we can modify the user data (not accessed by other task) */
                if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {
                    ret = Persondetect_Output_postprocessing(
                            inf_output,
                            app_priv->boxes_persondetect,
                            NUM_BOXES_MAX_PERSONDETECT);
                    if (ret != kStatus_Success)
                        PRINTF("mpp_event_listener: process output error!");

                    app_priv->detected_count_persondetect = 0;
                    app_priv->inference_time_persondetect = inf_output->inference_time_ms;

                    /* count valid results */
                    for (uint32_t i = 0; i < NUM_BOXES_MAX_PERSONDETECT; i++)
                    {
                        if (app_priv->boxes_persondetect[i].score > 0)
                            app_priv->detected_count_persondetect++;
                    }
                    app_priv->inference_frame_num_persondetect++;
                    /* end of modification of user data */
                    __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
                }
                break;

            default:
                PRINTF("mpp_event_listener: unknown model_id %d!\r\n", inf_output->model_id);
                break;
        }

        if ( (app_priv->mp != NULL) && (app_priv->elem != 0) )
        {
            mpp_element_params_t params;
            memset(&params, 0, sizeof(params));
            /* detected_count contains at least the detection zone box */
            params.labels.detected_rect = app_priv->detected_count_nanodet + app_priv->detected_count_persondetect + 1;
            params.labels.max_rect = MAX_LABEL_RECTS;
            params.labels.rectangles = app_priv->labels;
            boxes_to_rects(app_priv->boxes_nanodet, NUM_BOXES_MAX_NANODET,
                    app_priv->boxes_persondetect, NUM_BOXES_MAX_PERSONDETECT,
                    MAX_LABEL_RECTS, params.labels.rectangles);
            mpp_element_update(app_priv->mp, app_priv->elem, &params, true);
        }
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

    PRINTF("[%s]\r\n", mpp_get_version());
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    ret = mpp_api_init(NULL);
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

    user_data.mp = mp;

    mpp_img_params_t img_params;
    memset(&img_params, 0, sizeof (mpp_img_params_t));
    img_params.format = SRC_IMAGE_FORMAT;
    img_params.width = SRC_IMAGE_WIDTH;
    img_params.height = SRC_IMAGE_HEIGHT;
    mpp_static_img_add(mp, &img_params, (void *)image_data, NULL);

    /* split the pipeline into 2 branches */
    mpp_t mp_split[2];
    mpp_params_t mpp_params_split[2];
    memcpy(&mpp_params_split[0], &mpp_params, sizeof(mpp_params_t));
    memcpy(&mpp_params_split[1], &mpp_params, sizeof(mpp_params_t));
    mpp_params_split[0].exec_flag = MPP_EXEC_PREEMPT;
    mpp_params_split[1].exec_flag = MPP_EXEC_RC;

    ret = mpp_split(mp, 2 , &mpp_params_split[0], &mp_split[0]);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* On the preempt-able branch run the ML Inference (using a nanodet-m TF-Lite/Glow model) */
    /* First do crop + resize + color convert */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
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

    ret = mpp_element_add(mp_split[0], MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* configure TFlite/Glow element with model */
    mpp_element_params_t nanodet_params;
    memset(&nanodet_params, 0 , sizeof(mpp_element_params_t));

    nanodet_params.ml_inference.model_data = nanodet_m_0_5x_nhwc_nopermute_tflite;
    nanodet_params.ml_inference.model_size = nanodet_m_0_5x_nhwc_nopermute_tflite_len;
    nanodet_params.ml_inference.model_id = NANODET_MODEL_ID;
    nanodet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    nanodet_params.ml_inference.model_input_mean = NANODET_INPUT_MEAN;
    nanodet_params.ml_inference.model_input_std = NANODET_INPUT_STD;
    nanodet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
    nanodet_params.ml_inference.inference_params.num_inputs = 1;
    nanodet_params.ml_inference.inference_params.num_outputs = 2;
    nanodet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;

    ret = mpp_element_add(mp_split[0], MPP_ELEMENT_INFERENCE, &nanodet_params, NULL);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE");
        goto err;
    }
    /* close the pipeline with a null sink */
    ret = mpp_nullsink_add(mp_split[0]);
    if (ret) {
        PRINTF("Failed to add NULL sink\n");
        goto err;
    }

    /* Add persondetect model */
    /* First do crop + resize + color convert */
    memset(&elem_params, 0, sizeof(elem_params));
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width = PERSONDETECT_WIDTH;
    elem_params.convert.out_buf.height = PERSONDETECT_HEIGHT;
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
    elem_params.convert.scale.width = PERSONDETECT_WIDTH;
    elem_params.convert.scale.height = PERSONDETECT_HEIGHT;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;

    ret = mpp_element_add(mp_split[1], MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* configure TFlite element with model */
    mpp_element_params_t persondetect_params;
    memset(&persondetect_params, 0 , sizeof(mpp_element_params_t));

    persondetect_params.ml_inference.model_data = persondetect_data;
    persondetect_params.ml_inference.model_size = persondetect_data_len;
    persondetect_params.ml_inference.model_id = PERSONDETECT_MODEL_ID;
    persondetect_params.ml_inference.model_input_mean = PERSONDETECT_INPUT_MEAN;
    persondetect_params.ml_inference.model_input_std = PERSONDETECT_INPUT_STD;
    persondetect_params.ml_inference.inference_params.num_inputs = 1;
    persondetect_params.ml_inference.inference_params.num_outputs = 1;
    persondetect_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    persondetect_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    ret = mpp_element_add(mp_split[1], MPP_ELEMENT_INFERENCE, &persondetect_params, NULL);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE");
        goto err;
    }
    /* close the pipeline with a null sink */
    ret = mpp_nullsink_add(mp_split[1]);
    if (ret) {
        PRINTF("Failed to add NULL sink\n");
        goto err;
    }

    /* On the main branch of the pipeline, send the frame to the display */
    /* First do color-convert */
    memset(&elem_params, 0, sizeof(elem_params));
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width =  (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH);
    elem_params.convert.out_buf.height = (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT);
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    /* scaling parameters */
    if ((DISPLAY_LARGE_DIM * SRC_IMAGE_HEIGHT) < (DISPLAY_SMALL_DIM * SRC_IMAGE_WIDTH)) {
        elem_params.convert.scale.width =  (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH);
        elem_params.convert.scale.height = (SWAP_DIMS ? (APP_DISPLAY_HEIGHT * SRC_IMAGE_HEIGHT / SRC_IMAGE_WIDTH) :
                (APP_DISPLAY_WIDTH * SRC_IMAGE_HEIGHT / SRC_IMAGE_WIDTH));
    } else {
        elem_params.convert.scale.height = (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT);
        elem_params.convert.scale.width  = (SWAP_DIMS ? (APP_DISPLAY_WIDTH * SRC_IMAGE_WIDTH / SRC_IMAGE_HEIGHT) :
                (APP_DISPLAY_HEIGHT * SRC_IMAGE_WIDTH / SRC_IMAGE_HEIGHT));
    }

    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_SCALE;
    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* add one label rectangle */
    memset(&elem_params, 0, sizeof(elem_params));
    memset(&user_data.labels, 0, sizeof(user_data.labels));

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

    /* retrieve the element handle while add api */
    ret = mpp_element_add(mp, MPP_ELEMENT_LABELED_RECTANGLE, &elem_params, &user_data.elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        goto err;
    }

    /* then rotate if needed */
    if (APP_DISPLAY_LANDSCAPE_ROTATE != ROTATE_0) {
        memset(&elem_params, 0, sizeof(elem_params));
        /* set output buffer dims */
        elem_params.convert.out_buf.width = APP_DISPLAY_WIDTH;
        elem_params.convert.out_buf.height = APP_DISPLAY_HEIGHT;
        elem_params.convert.angle = APP_DISPLAY_LANDSCAPE_ROTATE;
        elem_params.convert.ops = MPP_CONVERT_ROTATE;
        ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

        if (ret) {
            PRINTF("Failed to add element CONVERT\r\n");
            goto err;
        }
    }

    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    ret = mpp_display_add(mp, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", s_display_name);
        goto err;
    }

    /* start preempt-able pipeline branch */
    ret = mpp_start(mp_split[0], 0, false);
    if (ret) {
        PRINTF("Failed to start pipeline");
        goto err;
    }
    ret = mpp_start(mp_split[1], 0, false);
    if (ret) {
        PRINTF("Failed to start pipeline");
        goto err;
    }
    /* start main pipeline branch */
    ret = mpp_start(mp, 1, false);
    if (ret) {
        PRINTF("Failed to start pipeline");
        goto err;
    }

    TickType_t xLastWakeTime;
    int i;
    const TickType_t xFrequency = STATS_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    uint32_t last_inf_frame_num_nanodet = user_data.inference_frame_num_nanodet;
    uint32_t last_inf_frame_num_persondetect = user_data.inference_frame_num_persondetect;
    for (;;) {
        xTaskDelayUntil( &xLastWakeTime, xFrequency );
        if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
        {
            PRINTF("===========================================================\r\n");
            if (last_inf_frame_num_nanodet != user_data.inference_frame_num_nanodet)
            {
                PRINTF("nanodet : inference time %u ms (frame num %d)\r\n", user_data.inference_time_nanodet, user_data.inference_frame_num_nanodet);
                for (i = 0; i < NUM_BOXES_MAX_NANODET; i++)
                {
                    if (user_data.boxes_nanodet[i].area > 0)
                    {
                        PRINTF("nanodet : box %d label %s score %d(%%)\r\n", i,
                                nanodet_labels[user_data.boxes_nanodet[i].label], (int)(user_data.boxes_nanodet[i].score * 100.0f));
                    }
                }
                last_inf_frame_num_nanodet = user_data.inference_frame_num_nanodet;
            }
            if (last_inf_frame_num_persondetect != user_data.inference_frame_num_persondetect)
            {
                PRINTF("presondet : inference time %u ms (frame_num %d)\r\n", user_data.inference_time_persondetect, user_data.inference_frame_num_persondetect);
                if (user_data.detected_count_persondetect <= 0)
                {
                    PRINTF("persondet : no person detected\r\n");
                }
                else
                {
                    for (int i = 0; i < NUM_BOXES_MAX_PERSONDETECT; i++)
                    {
                        if (user_data.boxes_persondetect[i].area > 0)
                        {
                            PRINTF("persondet : box %d label %s score %d(%%)\r\n", i,
                                    PERSONDETECT_DETECTION_LABEL, (int)(user_data.boxes_persondetect[i].score * 100.0f));
                        }
                    }
                }
                last_inf_frame_num_persondetect = user_data.inference_time_persondetect;
            }
            PRINTF("===========================================================\r\n");
            __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
        }
    }

err:
    for (;;)
    {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

