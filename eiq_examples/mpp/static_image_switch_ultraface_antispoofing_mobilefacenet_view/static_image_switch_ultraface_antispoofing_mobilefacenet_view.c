/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to recognize a face:
 * Static images switch -> split -> image converter -> draw labeled rectangles -> display
 *                   +-> image converter -> inference engine (model: ultraface/antispoofing/mobilefacenet)
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
#include "app.h"

/* Shell includes */
#include "fsl_shell.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"
#include "hal_utils.h"
#include "hal_os.h"
#include "hal_debug.h"

/* utility functions */
#include "models/utils.h"
#include "models/shell_database_utils.h"
#include "face_box_utils.h"

#include "app_constants.h"
#include "app_types.h"

/* Tensorflow lite model data input */
#include APP_TFLITE_MOBILEFACENET_DATA
#include APP_TFLITE_ULTRAFACE_DATA
#include APP_TFLITE_ANTISPOOFING_DATA

/* Persons database */
#include APP_DATABASE_NAME

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
#define APP_SKIP_CONVERT_FOR_DISPLAY 0
#endif

#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_ULTRAFACE_INFO
#include APP_TFLITE_ANTISPOOFING_INFO

/* Include src images */
#include APP_STATIC_IMAGE_1
#include APP_STATIC_IMAGE_2

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* Input images */
#define SRC1_IMAGE_NAME "jianfeng_qin_rgb"
#define SRC2_IMAGE_NAME "antispoofing_fake_rgb"
#define SRC1_IMAGE_FORMAT SRC_IMAGE_JIANFENG_QIN_RGB_FORMAT
#define SRC1_IMAGE_HEIGHT SRC_IMAGE_JIANFENG_QIN_RGB_HEIGHT
#define SRC1_IMAGE_WIDTH SRC_IMAGE_JIANFENG_QIN_RGB_WIDTH
#define SRC2_IMAGE_FORMAT SRC_IMAGE_ANTISPOOFING_FAKE_RGB_FORMAT
#define SRC2_IMAGE_HEIGHT SRC_IMAGE_ANTISPOOFING_FAKE_RGB_HEIGHT
#define SRC2_IMAGE_WIDTH SRC_IMAGE_ANTISPOOFING_FAKE_RGB_WIDTH

/* shell & serial handlers */
static shell_handle_t s_shellHandle;
extern serial_handle_t g_serialHandle;

/* shell task status. */
static int task_status;

#ifndef REGISTRATION_DELAY_MS
#define REGISTRATION_DELAY_MS 200
#endif

/* Frequency of changing source image in ms */
#define IMG_CHANGE_FREQ 1000

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* shell functions */
/* The handle should be 4 byte aligned, because unaligned access isn't be supported on some devices.*/
SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);

static TaskHandle_t shell_task_handle = NULL;

void *image1_data = (void *)jianfeng_qin_rgb_data;
void *image2_data = (void *)antispoofing_fake_rgb_data;

static const char s_display_name[] = APP_DISPLAY_NAME;

static user_data_t user_data = {0};

/* define shell commands */
SHELL_COMMAND_DEFINE(Add,
		"\r\n\"Add  arg \": Name \r\n Usage: name of person you want to add to database \r\n          ",
		database_add,
		1);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);
int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
static void shell_database_init(void *params);

/*******************************************************************************
 * Code
 ******************************************************************************/
/* @brief shell and database init function */
static void shell_database_init(void *params)
{
    /* Init SHELL */
    s_shellHandle = (shell_handle_t)&s_shellHandleBuffer[0];

    char *propmt_text = "SHELL DATABASE CONTROL MODE >";

    if (kStatus_SHELL_Success != SHELL_Init(s_shellHandle, g_serialHandle, propmt_text))
    {
        PRINTF("Shell initialization failed!\r\n");
        return;
    }

    init_database(g_embedding_db);

    /* Add new command to commands list */
    /* Definition of shell commands */

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(Add));

    while (1)
    {
        SHELL_TASK_DELAY;
    }
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
			shell_database_init,
			"shell_database_init",
			configMINIMAL_STACK_SIZE + 1000,
			NULL,
			APP_DEFAULT_PRIO + 1,
			&shell_task_handle);

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

void print_conditional(user_data_t *user_data)
{
	if (user_data->state != STATE_REGISTERING)
	{
		if (task_status == -1) /* task suspended */
		{
			vTaskSuspend(shell_task_handle);
			task_status = -1;
		}

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
						PRINTF("Ultraface inference time %u ms \r\n", user_data->inference_time_ms);
					}
				}
			}
		}
		else if(user_data->last_model == MODEL_MOBILEFACENET)
		{
            if (strcmp(user_data->result.recognized_name, "") != 0)
            {
                PRINTF("%s : name %s score %d(%%)\r\n", MOBILEFACENET_NAME,
                                                    user_data->result.recognized_name, user_data->result.similarity_percentage);
                PRINTF("Mobilefacenet inference time %u ms \r\n", user_data->inference_time_ms);
            }
        }
        else // MODEL_ANTISPOOFING
        {  
            PRINTF("The antispoofing inference time %u ms \r\n", user_data->inference_time_ms);
            if(user_data->liveness.result[1] > SPOOFING_THRESHOLD)
            {
                PRINTF("%s : Real face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[1]);
            }
            else
            {
                PRINTF("%s : Fake face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[0]);
            }
        
		}

		mpp_stats_disable(MPP_STATS_GRP_API);
		PRINTF("CPU Load: %u(%%)\n\r", user_data->api_stats->api.cpu_load);
		mpp_stats_enable(MPP_STATS_GRP_API);
	}

	return;
}

static void app_task(void *params)
{
    user_data.cur_model = MODEL_ULTRAFACE;
    user_data.state = STATE_DETECTING;
    user_data.db = g_embedding_db;
    user_data.db_max = DATABASE_MAX_SIZE;
    void *crt_image_data;
    int ret = 0;
    static int registration_start_time = 0;
    static int registration_end_time = 0;
    static int face_registered = 1;
    /* registration status. */
    static int registering = 0;

    PRINTF("[%s]\r\n", mpp_get_version());

    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    /* show constants */
    PRINTF("SRC_WIDTH = %d\r\n", SRC_WIDTH);
    PRINTF("SRC_HEIGHT = %d\r\n", SRC_HEIGHT);
    PRINTF("CROP_LEFT = %d\r\n", CROP_LEFT);
    PRINTF("CROP_TOP = %d\r\n", CROP_TOP);
    PRINTF("CROP_SIZE_LEFT = %d\r\n", CROP_SIZE_LEFT);
    PRINTF("CROP_SIZE_TOP = %d\r\n", CROP_SIZE_TOP);
    PRINTF("RECO_CROP_SIZE_LEFT = %d\r\n", RECO_CROP_SIZE_LEFT);
    PRINTF("RECO_CROP_SIZE_TOP = %d\r\n", RECO_CROP_SIZE_TOP);
    PRINTF("\r\n");
    PRINTF("DISPLAY_LARGE_DIM = %d\r\n", DISPLAY_LARGE_DIM);
    PRINTF("DISPLAY_SMALL_DIM = %d\r\n", DISPLAY_SMALL_DIM);
    PRINTF("VIEW_WIDTH = %d\r\n", VIEW_WIDTH);
    PRINTF("VIEW_HEIGHT = %d\r\n", VIEW_HEIGHT);
    PRINTF("DETECTION_ZONE_RECT_TOP = %d\r\n", DETECTION_ZONE_RECT_TOP);
    PRINTF("DETECTION_ZONE_RECT_LEFT = %d\r\n", DETECTION_ZONE_RECT_LEFT);
    PRINTF("DETECTION_ZONE_RECT_WIDTH = %d\r\n", DETECTION_ZONE_RECT_WIDTH);
    PRINTF("DETECTION_ZONE_RECT_HEIGHT = %d\r\n", DETECTION_ZONE_RECT_HEIGHT);
#ifndef APP_DYNAMIC_RECO_ZONE
    PRINTF("RECO_ZONE_RECT_WIDTH = %d\r\n", RECO_ZONE_RECT_WIDTH);
    PRINTF("RECO_ZONE_RECT_HEIGHT = %d\r\n", RECO_ZONE_RECT_HEIGHT);
#endif

    /* init API */
    static mpp_api_params_t api_param = {0};
#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param.rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param.rc_cycle_min = APP_RC_CYCLE_MIN;
#endif

	/* fix max pipeline task priority. */
    api_param.pipeline_task_max_prio = APP_PIPELINE_TASK_MAX_PRIO;

    mpp_stats_t api_stats;
    memset(&api_stats, 0, sizeof(api_stats));
    api_param.stats = &api_stats;

    ret = mpp_api_init(&api_param);
    if (ret)
        goto err;

    static mpp_t mp;
    static mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;

    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;

    static mpp_img_params_t img_params;
    memset(&img_params, 0 , sizeof(img_params));
    img_params.height = SRC1_IMAGE_HEIGHT;
    img_params.width =  SRC1_IMAGE_WIDTH;
    img_params.format = SRC1_IMAGE_FORMAT;
    crt_image_data = image1_data;
    mpp_elem_handle_t static_img_elem;
    mpp_static_img_add(mp, &img_params, crt_image_data, &static_img_elem);

    /* split the pipeline into 2 branches:
     * - first for the conversion to model
     * - second for the label-rect draw & display
     * this order is needed to avoid running inference on an image containing label-rect */
    static mpp_t mp_split;
    mpp_params.exec_flag = MPP_EXEC_RC;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* First do crop + resize + color convert */
    static mpp_element_params_t infer_conv_params;
    memset(&infer_conv_params, 0, sizeof(infer_conv_params));
    /* pick GFX device */
    infer_conv_params.convert.dev_name = APP_GFX_BACKEND_INFER_NAME;
    /* set output buffer dims */
    infer_conv_params.convert.out_buf.width = ULTRAFACE_WIDTH;
    infer_conv_params.convert.out_buf.height = ULTRAFACE_HEIGHT;
    /* color convert */
    infer_conv_params.convert.pixel_format = ULTRAFACE_PIXEL_FORMAT;
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
#if (SRC_DISPLAY_FLIP != FLIP_NONE)
    infer_conv_params.convert.flip = SRC_DISPLAY_FLIP;
    infer_conv_params.convert.ops |=  MPP_CONVERT_ROTATE;
#endif
    infer_conv_params.convert.stripe_in = false;
    infer_conv_params.convert.stripe_out = false; /* model takes full frames */

    HAL_LOGI("parameters of first image conversion for inference branch: \r\n");
    HAL_LOGI("infer_conv_params.convert.crop.top = %d\r\n", infer_conv_params.convert.crop.top);
    HAL_LOGI("infer_conv_params.convert.crop.bottom = %d\r\n", infer_conv_params.convert.crop.bottom);
    HAL_LOGI("infer_conv_params.convert.crop.left = %d\r\n", infer_conv_params.convert.crop.left);
    HAL_LOGI("infer_conv_params.convert.crop.right = %d\r\n", infer_conv_params.convert.crop.right);
    HAL_LOGI("infer_conv_params.convert.scale.width = %d\r\n", infer_conv_params.convert.scale.width);
    HAL_LOGI("infer_conv_params.convert.scale.height = %d\r\n", infer_conv_params.convert.scale.height);

    static mpp_elem_handle_t infer_conv_h;
    ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &infer_conv_params, &infer_conv_h);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* create a background mpp (preempt-able branch) for the ML Inference
     * because it may take longer than capture period.
     * Inference runs an persondetect TF-Lite model */
    static mpp_t mp_bg;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_background(mp_split, &mpp_params, &mp_bg);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* prepare the antispoofing model params */
    static mpp_element_params_t antispoofing_params;
    memset(&antispoofing_params, 0 , sizeof(mpp_element_params_t));
    antispoofing_params.ml_inference.model_data = antispoofing_data;
    antispoofing_params.ml_inference.model_size = antispoofing_data_len;
    antispoofing_params.ml_inference.model_input_mean = ANTISPOOFING_INPUT_MEAN;
    antispoofing_params.ml_inference.model_input_std = ANTISPOOFING_INPUT_STD;
    antispoofing_params.ml_inference.inference_params.num_inputs = 1;
    antispoofing_params.ml_inference.inference_params.num_outputs = 1;
    antispoofing_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    antispoofing_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    /* prepare the mobilefacenet model params */
    static mpp_element_params_t mobilefacenet_params;
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
    static mpp_element_params_t ultraface_params;
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

    /* On the secondary branch of the pipeline, send the frame to the display */
    /* First do color-convert + flip */
    static mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    /* pick GFX device */
    elem_params.convert.dev_name = APP_GFX_BACKEND_NAME;
    /* set output buffer dims */
    elem_params.convert.out_buf.width =  VIEW_WIDTH;
    elem_params.convert.out_buf.height = VIEW_HEIGHT;
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    /* scaling parameters */
    elem_params.convert.scale.width =  VIEW_WIDTH;
    elem_params.convert.scale.height = VIEW_HEIGHT;

    HAL_LOGI("parameters of first image conversion for display branch: \r\n");
    HAL_LOGI("elem_params.convert.scale.width = %d\r\n", elem_params.convert.scale.width);
    HAL_LOGI("elem_params.convert.scale.height = %d\r\n", elem_params.convert.scale.height);
    HAL_LOGI("elem_params.convert.out_buf.width = %d\r\n", elem_params.convert.out_buf.width);
    HAL_LOGI("elem_params.convert.out_buf.height = %d\r\n", elem_params.convert.out_buf.height);

#if (SRC_DISPLAY_FLIP != FLIP_NONE)
    elem_params.convert.flip = SRC_DISPLAY_FLIP;
#endif
    elem_params.convert.ops |= MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE | MPP_CONVERT_SCALE;

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

#if (APP_SKIP_CONVERT_FOR_DISPLAY == 0)
    /* then rotate if needed */
    if (APP_DISPLAY_LANDSCAPE_ROTATE != ROTATE_0) {
        memset(&elem_params, 0, sizeof(elem_params));
        elem_params.convert.dev_name = APP_GFX_BACKEND_NAME;
        /* set output buffer dims */
        elem_params.convert.out_buf.width = APP_DISPLAY_WIDTH;
        elem_params.convert.out_buf.height = APP_DISPLAY_HEIGHT;
        elem_params.convert.angle = APP_DISPLAY_LANDSCAPE_ROTATE;
        elem_params.convert.scale.width =  SCALED_VIEW_WIDTH;
        elem_params.convert.scale.height = SCALED_VIEW_HEIGHT;

        HAL_LOGI("parameters of second image conversion for display branch: \r\n");
        HAL_LOGI("elem_params.convert.scale.width = %d\r\n", elem_params.convert.scale.width);
        HAL_LOGI("elem_params.convert.scale.height = %d\r\n", elem_params.convert.scale.height);
        HAL_LOGI("elem_params.convert.out_buf.width = %d\r\n", elem_params.convert.out_buf.width);
        HAL_LOGI("elem_params.convert.out_buf.height = %d\r\n", elem_params.convert.out_buf.height);

        elem_params.convert.ops = MPP_CONVERT_ROTATE | MPP_CONVERT_SCALE;
        ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

        if (ret) {
            PRINTF("Failed to add element CONVERT\r\n");
            goto err;
        }
    }
#endif

    static mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = false;
    ret = mpp_display_add(mp, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", s_display_name);
        goto err;
    }

    mpp_stats_enable(MPP_STATS_GRP_API);

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

    user_data.api_stats = &api_stats;

    TickType_t xLastPrintTime = 0, tick = 0, notifyTime = 0;
    TickType_t x_last_awake_time;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    const TickType_t notifyDelay = OUTPUT_NOTIFY_PERIOD_MS / portTICK_PERIOD_MS;
    const uint32_t image_change_freq = IMG_CHANGE_FREQ / portTICK_PERIOD_MS;
    uint32_t last_inf_frame_num = user_data.inference_frame_num;
    uint32_t crt_time = 0, start_time = 0;
    for (;;) {

        /* manage periodic print */
        tick = xTaskGetTickCount();
        if ((tick > (xLastPrintTime + xFrequency)) || Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
        {
            if (last_inf_frame_num != user_data.inference_frame_num)
            {
                xLastPrintTime = tick;
                print_conditional(&user_data);
                last_inf_frame_num = user_data.inference_frame_num;
            }
        }
        __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);

        /* manage timed state transition */
        if (user_data.state == STATE_RECOGNIZED || user_data.state == STATE_REGISTERED || user_data.state == STATE_SPOOF ||
                user_data.state == STATE_REGISTRATION_CANCELLED)
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

        if (user_data.state == STATE_REGISTERING)
        {
        	/* update zone label */
        	/* detected_count contains at least the detection zone box */
            if (user_data.labrect_elem != 0)
            {
                elem_params.labels.detected_rect = user_data.detected_count + 1;
                elem_params.labels.max_rect = MAX_LABEL_RECTS;
                elem_params.labels.rectangles = user_data.labels;
                strcpy((char *)user_data.labels[0].label, ZONE_LABEL_REGISTERING);
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }

            if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
            {
                /* set new person embeddings */
                set_new_face_embeddings((const float *)user_data.result.embedding);

                registration_start_time = hal_get_exec_time();
                /* wait for user to finish registration */
                while(registering != 1)
                {
                    /* check if user entered the new face name */
                    registering = registration_state();
                    /* cancel registration, if delay is expired */
                    if (registration_end_time >= registration_start_time + REGISTRATION_DELAY_MS)
                    {
                        PRINTF("*** Registration time expired! ***\r\n");
                        if (user_data.labrect_elem != 0)
                        {
                            elem_params.labels.detected_rect = user_data.detected_count + 1;
                            elem_params.labels.max_rect = MAX_LABEL_RECTS;
                            elem_params.labels.rectangles = user_data.labels;
                            strcpy((char *)user_data.labels[0].label, ZONE_LABEL_REGISTRATION_END);
                            mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
                        }
                        face_registered = 0; /* person will not be added to database */
                        break;
                    }
                    registration_end_time = hal_get_exec_time();
                }

                /* reset registration state to 0 */
                registering = reset_registration_state();

                if (face_registered == 1) /* registration done */
                {
                    PRINTF("*** Face registered! ***\r\n");

                    if (user_data.state == STATE_REGISTERING)
                        user_data.state = STATE_REGISTERED;
                }
                else /* registration cancelled */
                {
                    if (user_data.state == STATE_REGISTERING)
                        user_data.state = STATE_REGISTRATION_CANCELLED;
                }

                __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
            }
        }

        /* manage face recognition state machine */
        if (user_data.state == STATE_DETECTED)
        {
            /* stop branches with convert and inference elements */
            mpp_stop(mp_bg);
            mpp_stop(mp_split);
            PRINTF("Switching to Antispoofing \r\n");
            /* update convert params for the antispoofing model */
#ifdef APP_DYNAMIC_RECO_ZONE
            /* adapt source crop area to detected box */
            if (get_face_crop_area(&user_data.final_boxes[0], &infer_conv_params.convert.crop))
            {
                PRINTF("Failed to get face crop area\r\n");
                goto err;
            }
            infer_conv_params.convert.out_buf.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.out_buf.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.scale.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.scale.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.pixel_format = ANTISPOOFING_PIXEL_FORMAT;

            HAL_LOGI("updated parameters of image conversion for inference branch: \r\n");
            HAL_LOGI("infer_conv_params.convert.crop.top = %d\r\n", infer_conv_params.convert.crop.top);
            HAL_LOGI("infer_conv_params.convert.crop.bottom = %d\r\n", infer_conv_params.convert.crop.bottom);
            HAL_LOGI("infer_conv_params.convert.crop.left = %d\r\n", infer_conv_params.convert.crop.left);
            HAL_LOGI("infer_conv_params.convert.crop.right = %d\r\n", infer_conv_params.convert.crop.right);
#else
            infer_conv_params.convert.crop.top = RECO_CROP_TOP;
            infer_conv_params.convert.crop.bottom = RECO_CROP_TOP + RECO_CROP_SIZE_TOP - 1;
            infer_conv_params.convert.crop.left = RECO_CROP_LEFT;
            infer_conv_params.convert.crop.right = RECO_CROP_LEFT + RECO_CROP_SIZE_LEFT - 1;
            infer_conv_params.convert.out_buf.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.out_buf.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.scale.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.scale.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.pixel_format = ANTISPOOFING_PIXEL_FORMAT;
#endif
            ret = mpp_element_update(mp_split, infer_conv_h, &infer_conv_params, true);
            if (ret) 
            {
                PRINTF("Failed to update element convert for the antispoofing model\r\n");
                goto err;
            }

            /* switch to antispoofing model */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &antispoofing_params, true);
            if (ret) 
            {
                PRINTF("Failed to update element inference for the antispoofing model\r\n");
                goto err;
            }
            if (user_data.labrect_elem != 0)
            {
                elem_params.labels.detected_rect = user_data.detected_count + 1;
                elem_params.labels.max_rect = MAX_LABEL_RECTS;
                elem_params.labels.rectangles = user_data.labels;
                strcpy((char *)user_data.labels[0].label, ZONE_LABEL_ANTISPOOFING);
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }
            user_data.cur_model = MODEL_ANTISPOOFING;
            user_data.state = STATE_CHECKING_ANTISPOOFING;
            mpp_start(mp_split, 0, false);
            mpp_start(mp_bg, 0, false);
        }
        else if (user_data.state == STATE_REAL)
        {
            PRINTF("Switching to MobileFaceNet \r\n");
            mpp_stop(mp_bg);
            mpp_stop(mp_split);
            /* update convert params for mobilefacenet */
            infer_conv_params.convert.out_buf.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.out_buf.height = MOBILEFACENET_HEIGHT;
            infer_conv_params.convert.scale.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.scale.height = MOBILEFACENET_HEIGHT;
            infer_conv_params.convert.pixel_format = MOBILEFACENET_PIXEL_FORMAT;
            ret = mpp_element_update(mp_split, infer_conv_h, &infer_conv_params, true);
            if (ret) {
                PRINTF("Failed to update element convert for mobilefacenet\r\n");
                goto err;
            }

            /* switch to mobilefacenet model */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &mobilefacenet_params, true);
            if (ret) {
                PRINTF("Failed to update element inference for mobilefacenet\r\n");
                goto err;
            }

            /* update zone label */
            /* detected_count contains at least the detection zone box */
            if (user_data.labrect_elem != 0)
            {
                elem_params.labels.detected_rect = user_data.detected_count + 1;
                elem_params.labels.max_rect = MAX_LABEL_RECTS;
                elem_params.labels.rectangles = user_data.labels;
                strcpy((char *)user_data.labels[0].label, ZONE_LABEL_RECOGNIZING);
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }

            user_data.cur_model = MODEL_MOBILEFACENET;
            user_data.state = STATE_RECOGNIZING;
            mpp_start(mp_split, 0, false);
            mpp_start(mp_bg, 0, false);
        }
        else if (user_data.state == STATE_USER_NOTIFIED)
        {
            crt_time = tick;
            if ((crt_time - start_time) >= image_change_freq)
            {
                mpp_element_params_t img_params;
                memset(&img_params, 0, sizeof(img_params));

                if (crt_image_data == image1_data)
                {
                    img_params.static_image.img_params.format = SRC2_IMAGE_FORMAT;
                    img_params.static_image.img_params.width  = SRC2_IMAGE_WIDTH;
                    img_params.static_image.img_params.height = SRC2_IMAGE_HEIGHT;
                    crt_image_data = image2_data;
                    PRINTF("Change image to %s\r\n", SRC2_IMAGE_NAME);
                }
                else
                {
                    img_params.static_image.img_params.format = SRC1_IMAGE_FORMAT;
                    img_params.static_image.img_params.width  = SRC1_IMAGE_WIDTH;
                    img_params.static_image.img_params.height = SRC1_IMAGE_HEIGHT;
                    crt_image_data = image1_data;
                    PRINTF("Change image to %s\r\n", SRC1_IMAGE_NAME);
                }
                img_params.static_image.img_buffer = crt_image_data;
                ret = mpp_element_update(mp, static_img_elem, &img_params, true);
                start_time = tick;
            } 
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
            infer_conv_params.convert.pixel_format = ULTRAFACE_PIXEL_FORMAT;
            ret = mpp_element_update(mp_split, infer_conv_h, &infer_conv_params, true);
            if (ret) {
                PRINTF("Failed to update element convert for ultraface\r\n");
                goto err;
            }

            /* switch to ULTRAFACE */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &ultraface_params, true);
            if (ret) {
                PRINTF("Failed to update element inference for ultraface\r\n");
                goto err;
            }

            user_data.cur_model = MODEL_ULTRAFACE;
            strcpy((char *)user_data.labels[0].label, ZONE_LABEL_RECO);
            user_data.state = STATE_DETECTING;
            mpp_start(mp_split, 0, false);
            mpp_start(mp_bg, 0, false);
        }
        else
        {
        	/* empty */
        }
    }

    err:
    for (;;) {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}
