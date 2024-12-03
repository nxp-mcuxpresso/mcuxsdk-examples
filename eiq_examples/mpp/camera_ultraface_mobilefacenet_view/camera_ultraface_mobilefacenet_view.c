/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to recognize a face:
 * 2D camera -> split -> image converter -> draw labeled rectangles -> display
 *                   +-> image converter -> inference engine (model: mobilefacenet/ultraface)
 * The camera view finder is displayed on screen
 * The model performs face detection then recognition using TF-Lite micro inference engine
 * the model output is displayed on UART console by application */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "atomic.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "string.h"
#include "stdbool.h"
#include "stdio.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board_init.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"
#include "hal_utils.h"

/* utility functions */
#include "models/utils.h"

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* Tensorflow lite model data input */
#include APP_TFLITE_MOBILEFACENET_DATA
#include APP_TFLITE_ULTRAFACE_DATA

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] =  APP_CAMERA_NAME;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#include "app_constants.h"
#include "app_types.h"
#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_ULTRAFACE_INFO

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);
int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);

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
            configMINIMAL_STACK_SIZE + 2400,
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

void print_periodic(user_data_t *user_data)
{
    if (user_data->last_model == MODEL_ULTRAFACE)
    {
        if (user_data->detected_count <= 0)
        {
            PRINTF("%s : no detection\r\n", ULTRAFACE_NAME);
        }
        else
        {
            for (int i = 0; i < NUM_BOXES_MAX; i++)
            {
                if (user_data->final_boxes[i].area > 0)
                {
                    PRINTF("%s : box %d score %d(%%)\r\n", ULTRAFACE_NAME, i,
                            (int)(user_data->final_boxes[i].score * 100.0f));
                }
            }
        }
    }
    else // MODEL_MOBILEFACENET
    {
        PRINTF("%s : name %s score %d(%%)\r\n", MOBILEFACENET_NAME,
                user_data->result.recognized_name, user_data->result.similarity_percentage);
    }
    PRINTF("inference time %d ms \r\n", user_data->inference_time_ms);
    return;
}

static void app_task(void *params)
{
    static user_data_t user_data = {0};
    user_data.cur_model = MODEL_ULTRAFACE;
    user_data.state = STATE_DETECTING;
    int ret = 0;

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
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* First do crop + resize + color convert */
    mpp_element_params_t infer_conv_params;
    memset(&infer_conv_params, 0, sizeof(infer_conv_params));
    /* pick default device from the first listed and supported by Hw */
    infer_conv_params.convert.dev_name = APP_GFX_BACKEND_NAME;
    /* set output buffer dims */
    infer_conv_params.convert.out_buf.width = ULTRAFACE_WIDTH;
    infer_conv_params.convert.out_buf.height = ULTRAFACE_HEIGHT;
    /* color convert */
    infer_conv_params.convert.pixel_format = MPP_PIXEL_RGB;
    infer_conv_params.convert.ops = MPP_CONVERT_COLOR;
    /* crop center of image */
    infer_conv_params.convert.crop.top = CROP_TOP;
    infer_conv_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    infer_conv_params.convert.crop.left = CROP_LEFT;
    infer_conv_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    infer_conv_params.convert.ops |= MPP_CONVERT_CROP;
    /* resize: scaling parameters */
    infer_conv_params.convert.scale.width = ULTRAFACE_WIDTH;
    infer_conv_params.convert.scale.height = ULTRAFACE_HEIGHT;
    infer_conv_params.convert.ops |= MPP_CONVERT_SCALE;
    /* then add a flip */
#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
    infer_conv_params.convert.flip = SRC_DISPLAY_FLIP;
    infer_conv_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif
    infer_conv_params.convert.stripe_in = false;
    infer_conv_params.convert.stripe_out = false; /* model takes full frames */

    mpp_elem_handle_t infer_conv_h;
    ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &infer_conv_params, &infer_conv_h);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* create a background mpp (preempt-able branch) for the ML Inference
     * because it may take longer than capture period.
     * Inference runs an persondetect TF-Lite model */
    mpp_t mp_bg;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_background(mp_split, &mpp_params, &mp_bg);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* prepare the mobilefacenet model params */
    mpp_element_params_t mobilefacenet_params;
    memset(&mobilefacenet_params, 0 , sizeof(mpp_element_params_t));

    mobilefacenet_params.ml_inference.model_data = mobilefacenet_data;
    mobilefacenet_params.ml_inference.model_size = mobilefacenet_data_len;
    mobilefacenet_params.ml_inference.model_input_mean = MOBILEFACENET_INPUT_MEAN;
    mobilefacenet_params.ml_inference.model_input_std = MOBILEFACENET_INPUT_STD;
    mobilefacenet_params.ml_inference.inference_params.num_inputs = 1;
    mobilefacenet_params.ml_inference.inference_params.num_outputs = 1;
    mobilefacenet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    mobilefacenet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    /* prepare the ultraface model params */
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

    /* configure TFlite element with ultraface model */
    ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &ultraface_params, &user_data.infer_elem);

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
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    /* pick default device from the first listed and supported by Hw */
    elem_params.convert.dev_name = NULL;
    /* set output buffer dims */
    elem_params.convert.out_buf.width =  (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH);
    elem_params.convert.out_buf.height = (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT);
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    /* scaling parameters */
    if ((DISPLAY_LARGE_DIM * SRC_HEIGHT) < (DISPLAY_SMALL_DIM * SRC_WIDTH)) {
        elem_params.convert.scale.width =  (SWAP_DIMS ? APP_DISPLAY_HEIGHT : APP_DISPLAY_WIDTH);
        elem_params.convert.scale.height = (SWAP_DIMS ? (APP_DISPLAY_HEIGHT * SRC_HEIGHT / SRC_WIDTH) :
                (APP_DISPLAY_WIDTH * SRC_HEIGHT / SRC_WIDTH));
    } else {
        elem_params.convert.scale.height = (SWAP_DIMS ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT);
        elem_params.convert.scale.width  = (SWAP_DIMS ? (APP_DISPLAY_WIDTH * SRC_WIDTH / SRC_HEIGHT) :
                (APP_DISPLAY_HEIGHT * SRC_WIDTH / SRC_HEIGHT));
    }

    elem_params.convert.flip = SRC_DISPLAY_FLIP;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE | MPP_CONVERT_SCALE;

    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

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

    /* first add recognition zone box */
    user_data.labels[0].top    = RECO_ZONE_RECT_TOP;
    user_data.labels[0].left   = RECO_ZONE_RECT_LEFT;
    user_data.labels[0].bottom = RECO_ZONE_RECT_TOP + RECO_ZONE_RECT_HEIGHT;
    user_data.labels[0].right  = RECO_ZONE_RECT_LEFT + RECO_ZONE_RECT_WIDTH;
    user_data.labels[0].line_width = RECT_LINE_WIDTH;
    user_data.labels[0].line_color.rgb.G = 0xff;
    strcpy((char *)user_data.labels[0].label, ZONE_LABEL_RECO);

    /* add element and retrieve its handle in user data */
    ret = mpp_element_add(mp, MPP_ELEMENT_LABELED_RECTANGLE, &elem_params, &user_data.labrect_elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        goto err;
    }
    /* pass the mpp of the element 'label rectangle' to callback */
    user_data.mp = mp;

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
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
#endif

    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = false;
#ifdef APP_SKIP_CONVERT_FOR_DISPLAY
    disp_params.rotate = APP_DISPLAY_LANDSCAPE_ROTATE;
#endif
    ret = mpp_display_add(mp, s_display_name, &disp_params);
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

    TickType_t xLastPrintTime = 0, tick = 0, notifyTime = 0;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    const TickType_t notifyDelay = OUTPUT_NOTIFY_PERIOD_MS / portTICK_PERIOD_MS;
    for (;;) {

        /* manage periodic print */
        tick = xTaskGetTickCount();
        if ((tick > (xLastPrintTime + xFrequency)) && Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
        {
            xLastPrintTime = tick;
            print_periodic(&user_data);
        }
        __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);

        /* manage timed state transition */
        if (user_data.state == STATE_RECOGNIZED || user_data.state == STATE_REGISTERED)
        {
            /* start timer */
            notifyTime = tick;
            user_data.state = STATE_NOTIFYING_USER;

            /* stop branches with convert and inference elements */
            /* until the user is notified */
            mpp_stop(mp_bg);
            mpp_stop(mp_split);
        }

        if ((user_data.state == STATE_NOTIFYING_USER) && (tick > notifyTime + notifyDelay))
        {
            /* stop timer */
            notifyTime = 0;
            user_data.state = STATE_USER_NOTIFIED;
        }

        /* manage face recognition state machine */
        if (user_data.state == STATE_DETECTED)
        {
            /* stop branches with convert and inference elements */
            mpp_stop(mp_bg);
            mpp_stop(mp_split);
            PRINTF("Switching to MobileFaceNet \r\n");
            /* update convert params for mobilefacenet */
            infer_conv_params.convert.crop.top = RECO_CROP_TOP;
            infer_conv_params.convert.crop.bottom = RECO_CROP_TOP + RECO_CROP_SIZE_TOP - 1;
            infer_conv_params.convert.crop.left = RECO_CROP_LEFT;
            infer_conv_params.convert.crop.right = RECO_CROP_LEFT + RECO_CROP_SIZE_LEFT - 1;
            infer_conv_params.convert.out_buf.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.out_buf.height = MOBILEFACENET_HEIGHT;
            infer_conv_params.convert.scale.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.scale.height = MOBILEFACENET_HEIGHT;
            ret = mpp_element_update(mp_split, infer_conv_h, &infer_conv_params);
            if (ret) {
                PRINTF("Failed to update element convert for mobilefacenet");
                goto err;
            }

            /* switch to mobilefacenet model */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &mobilefacenet_params);
            if (ret) {
                PRINTF("Failed to update element inference for mobilefacenet");
                goto err;
            }

            /* update zone label */
            /* detected_count contains at least the detection zone box */
            if (user_data.labrect_elem != 0)
            {
                elem_params.labels.detected_count = user_data.detected_count + 1;
                elem_params.labels.max_count = MAX_LABEL_RECTS;
                elem_params.labels.rectangles = user_data.labels;
                strcpy((char *)user_data.labels[0].label, ZONE_LABEL_REGISTERING);
                mpp_element_update(mp, user_data.labrect_elem, &elem_params);
            }

            user_data.cur_model = MODEL_MOBILEFACENET;
            user_data.state = STATE_RECOGNIZING;
            mpp_start(mp_split, 0);
            mpp_start(mp_bg, 0);
        }
        else if (user_data.state == STATE_USER_NOTIFIED)
        {
            PRINTF("Switching to Ultraface \r\n");
            /* update convert params for ultraface */
            infer_conv_params.convert.crop.top = CROP_TOP;
            infer_conv_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
            infer_conv_params.convert.crop.left = CROP_LEFT;
            infer_conv_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
            infer_conv_params.convert.out_buf.width = ULTRAFACE_WIDTH;
            infer_conv_params.convert.out_buf.height = ULTRAFACE_HEIGHT;
            infer_conv_params.convert.scale.width = ULTRAFACE_WIDTH;
            infer_conv_params.convert.scale.height = ULTRAFACE_HEIGHT;
            ret = mpp_element_update(mp_split, infer_conv_h, &infer_conv_params);
            if (ret) {
                PRINTF("Failed to update element convert for ultraface");
                goto err;
            }

            /* switch to ULTRAFACE */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &ultraface_params);
            if (ret) {
                PRINTF("Failed to update element inference for ultraface");
                goto err;
            }

            user_data.cur_model = MODEL_ULTRAFACE;
            strcpy((char *)user_data.labels[0].label, ZONE_LABEL_RECO);
            user_data.state = STATE_DETECTING;
            mpp_start(mp_split, 0);
            mpp_start(mp_bg, 0);
        }
        else
        {
            /* empty */
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
