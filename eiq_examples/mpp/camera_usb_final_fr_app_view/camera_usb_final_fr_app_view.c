/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to recognize a face:
 * USB/Virtual Camera (IR:VGA & RGB:QVGA) -> split -> JPEG HW -> image converter -> draw labeled rectangles -> display
 *                   +-> JPEG HW -> image converter -> inference engine (model: scrfd_kps/antispoofing/mobilefacenet)
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
#include "math.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"
#include "hal_utils.h"
#include "hal_os.h"
#include "hal_debug.h"
#include "common/draw_text.h"

/* utility functions */
#include "utils.h"
#include "face_box_utils.h"
#include "database_utils.h"

#include "app_constants.h"
#include "app_types.h"
#include "app_msg.h"

#include "mpp_app_task_notify.h"

/* Tensorflow lite model data input */
#include APP_TFLITE_MOBILEFACENET_DATA
#include APP_TFLITE_SCRFD_KPS_DATA
#include APP_TFLITE_ANTISPOOFING_DATA

/* Persons database */
#include APP_DATABASE_NAME

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
#define APP_SKIP_CONVERT_FOR_DISPLAY 0
#endif

#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_SCRFD_KPS_INFO
#include APP_TFLITE_ANTISPOOFING_INFO

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* image buffer for the text area */
uint8_t g_text_img[TEXT_WIDTH*TEXT_HEIGHT*TEXT_BPP] __attribute__((section(".noinit.$cam_buff_sh_mem"), aligned(64)));
/* image buffer for the inference preview area */
#ifdef DEBUG_PREVIEW_RECOGNITION
__ALIGNED(64) uint8_t  g_inf_img[INFPVW_WIDTH * INFPVW_HEIGHT * INFPVW_BPP];
#endif

/* logo image array */
#include "images/NXP_Logo_RGB_Colour_320.h"

#ifndef APP_STRIPE_MODE
#define APP_STRIPE_MODE 0
#endif

/* pick default backend if not specified. */
#ifndef APP_GFX_BACKEND_NAME
#define APP_GFX_BACKEND_NAME NULL
#endif

static user_data_t user_data = {0};

typedef struct _args_t {
    char camera_name[32];
    char display_name[32];
    mpp_pixel_format_t src_format;
    mpp_pixel_format_t display_format;
} args_t;

/* info to be dislayed in text area */
typedef struct {
	char name[MAX_WORD_SIZE];
	float confidence;
	int detection_inference_time;
    int antispoofing_inference_time;
    int recognition_inference_time;
	char state[MAX_WORD_SIZE];
} text_info_t;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);
int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
extern void ctrl_task_notify(uint8_t cmd, msg_t* para);
static void registration_timeout_callback(TimerHandle_t xTimer);

static struct rpmsg_lite_instance *rpmsg_inst = NULL;
volatile uint16_t *mcmgr_event_data = NULL;
static QueueHandle_t s_mpp_task_queue = NULL;

/* registration timer vaiables */
static TickType_t registration_start_time = 0;
static TickType_t registration_end_time = 0;
static TimerHandle_t registration_timer = NULL;
static bool registration_timer_expired = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

 /*!
 * @brief drawing of text by cpu into a buffer.
 */
static void draw_text_area(void* buf, uint32_t size, text_info_t* text)
{
    int space4items  = get_font_height() + 6;
    int start_y_middle = get_font_height()/2 + 2;
    int row_indentation = get_font_width()/2;
    char tmp[MAX_STRING_SIZE];
    text_context_t ctx;

    /*Clear the buffer. */
    memset(buf,0,size);
    init_text_buf(&ctx, buf, TEXT_WIDTH, TEXT_HEIGHT, TEXT_WIDTH);

    draw_text_line(&ctx, "User name:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"   %s",text->name);
    draw_text_line(&ctx, tmp, row_indentation, start_y_middle );

    start_y_middle += space4items;
    draw_text_line(&ctx, "Confidence:", row_indentation, start_y_middle);
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"    %2d%%",(int)(text->confidence));
    draw_text_line(&ctx, tmp, row_indentation, start_y_middle );

    start_y_middle += space4items;
    draw_text_line(&ctx, "Inf times:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"    detect:%dms",text->detection_inference_time);
	draw_text_line(&ctx, tmp, row_indentation, start_y_middle );
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"    recog:%dms",text->recognition_inference_time);
    draw_text_line(&ctx, tmp, row_indentation, start_y_middle );

#if USE_ANTISPOOFING
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"   spoof:%dms",text->antispoofing_inference_time);
    draw_text_line(&ctx, tmp, row_indentation, start_y_middle );

    start_y_middle += space4items;
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%s",text->state);
	draw_text_line(&ctx, tmp, row_indentation, start_y_middle );
#endif

	//make sure all data are pushed to the memory.
	XCACHE_CleanCacheByRange((uint32_t)buf,size);
}

void mpp_app_start(volatile uint16_t *mcmgr_event_data_p, struct rpmsg_lite_instance *rpmsg_inst_p)
{
    BaseType_t ret = pdFAIL;
    TaskHandle_t handle = NULL;

    args_t *args = pvPortMalloc(sizeof(args_t));
    if (!args) {
        PRINTF("Allocation failed\n\rCannot start MPP task\r\n");
        return;
    }

    strcpy(args->display_name, APP_DISPLAY_NAME);
    strcpy(args->camera_name, APP_CAMERA_NAME);
    args->src_format = APP_CAMERA_FORMAT;
    args->display_format = APP_DISPLAY_FORMAT;

    mcmgr_event_data = mcmgr_event_data_p;
    rpmsg_inst = rpmsg_inst_p;

    ret = xTaskCreate(
          app_task,
          "app_task",
          configMINIMAL_STACK_SIZE + 2400,
          (void *) args,
          APP_DEFAULT_PRIO,
          &handle);

    if (ret != pdTRUE)
    {
        PRINTF("Failed to create MPP task\r\n");
        vPortFree(args);
        return;
    }

    PRINTF("mpp start\r\n");
}

// Timer callback function
static void registration_timeout_callback(TimerHandle_t xTimer)
{
    registration_timer_expired = true;
}

void print_conditional(user_data_t *user_data)
{
    if (user_data->state != STATE_REGISTERING)
    {
        if (user_data->last_model == MODEL_SCRFD_KPS)
        {
            if (user_data->detected_count > 0)
            {
                for (int i = 0; i < NUM_BOXES_MAX; i++)
                {
                    if (user_data->final_boxes[i].area > 0)
                    {
                        PRINTF("%s : box %d score %d(%%)\r\n", SCRFD_KPS_NAME, i,
                                (int)(user_data->final_boxes[i].score * 100.0f));
                        PRINTF("SCRFD inference time %d ms \r\n", user_data->inference_time_ms);
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
                PRINTF("Mobilefacenet inference time %d ms \r\n", user_data->inference_time_ms);
            }
        }
        else // MODEL_ANTISPOOFING
        {
            PRINTF("The antispoofing inference time %d ms \r\n", user_data->inference_time_ms);
            if(user_data->liveness.result[1] > SPOOFING_THRESHOLD)
            {
                PRINTF("%s : Real face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[1]);
            }
            else
            {
                PRINTF("%s : Fake face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[0]);
            }
        
        }

#ifdef DISPLAY_CPU_LOAD
        mpp_stats_disable(MPP_STATS_GRP_API);
        PRINTF("CPU Load: %u(%%)\n\r", user_data->api_stats->api.cpu_load);
        mpp_stats_enable(MPP_STATS_GRP_API);
#endif
    }

    return;
}

static void app_task(void *params)
{
    args_t *args = (args_t *) params;
    bool stripe_mode = false;
    user_data.cur_model = MODEL_SCRFD_KPS;
    user_data.state = STATE_DETECTING;
    user_data.db = g_embedding_db;
    user_data.db_max = DATABASE_MAX_SIZE;
#ifdef DEBUG_PREVIEW_RECOGNITION
    user_data.inference_view = g_inf_img;
#else
    user_data.inference_view = NULL;
#endif
    int ret = 0;
    /* text info default configuration */
    text_info_t txt_info;
    strcpy(txt_info.name, "");
    txt_info.confidence = 0.0f;
    txt_info.detection_inference_time = 0;
    txt_info.recognition_inference_time = 0;

    int detection_inference_time = 0;
    int recognition_inference_time = 0;
#if USE_ANTISPOOFING
    txt_info.antispoofing_inference_time = 0;
    int antispoofing_inference_time = 0;
#endif
    PRINTF("[%s]\r\n", mpp_get_version());

    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    /* show constants */
    PRINTF("VIEW_SRC_WIDTH = %d\r\n", VIEW_SRC_WIDTH);
    PRINTF("VIEW_SRC_HEIGHT = %d\r\n", VIEW_SRC_HEIGHT);
    PRINTF("INF_SRC_WIDTH = %d\r\n", INF_SRC_WIDTH);
    PRINTF("INF_SRC_HEIGHT = %d\r\n", INF_SRC_HEIGHT);
    PRINTF("CROP_LEFT = %d\r\n", CROP_LEFT);
    PRINTF("CROP_TOP = %d\r\n", CROP_TOP);
    PRINTF("CROP_SIZE_LEFT = %d\r\n", CROP_SIZE_LEFT);
    PRINTF("CROP_SIZE_TOP = %d\r\n", CROP_SIZE_TOP);
    PRINTF("RECO_CROP_LEFT = %d\r\n", RECO_CROP_LEFT);
    PRINTF("RECO_CROP_TOP = %d\r\n", RECO_CROP_TOP);
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

    init_database(g_embedding_db);

    /* init API */
    static mpp_api_params_t api_param = {0};
#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param.rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param.rc_cycle_min = APP_RC_CYCLE_MIN;
#endif

    /* fix max pipeline task priority. */
    api_param.pipeline_task_max_prio = APP_PIPELINE_TASK_MAX_PRIO;
    api_param.pipeline_rc_task_prio = APP_RC_TASK_MAX_PRIO;
    api_param.pipeline_pr_task_prio = APP_PR_TASK_MAX_PRIO;

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
    PRINTF("Debug: MPP created \r\n");
    static mpp_camera_params_t cam_params;
    mpp_elem_handle_t cam_elem;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = RGB_CAMERA_HEIGHT;
    cam_params.width  = RGB_CAMERA_WIDTH;
    cam_params.format = args->src_format;
    cam_params.fps    = 30;
    cam_params.stripe = stripe_mode;
    cam_params.rpmsg_inst = rpmsg_inst;
    cam_params.mcmgr_event_data = mcmgr_event_data;
    cam_params.n_streams = 2;
    cam_params.stream[0].type = RGB_STREAM;
    cam_params.stream[0].active = true;
    cam_params.stream[0].height = RGB_CAMERA_HEIGHT;
    cam_params.stream[0].width = RGB_CAMERA_WIDTH;
    cam_params.stream[1].type = IR_STREAM;
    cam_params.stream[1].active = true;
    cam_params.stream[1].height = IR_CAMERA_HEIGHT;
    cam_params.stream[1].width = IR_CAMERA_WIDTH;
    cam_params.in_advance_enqueue = false;
    PRINTF("Debug: About to add a USB camera\r\n");
    ret = mpp_camera_add(mp, args->camera_name, &cam_params, &cam_elem);
    if (ret) {
        PRINTF("Failed to add camera %s\n", args->camera_name);
        goto err;
    }
    PRINTF("Debug: Added an USB camera \r\n");
    /* split the pipeline into 2 branches:
     * - first for the conversion to model
     * - second for the label-rect draw & display
     * this order is needed to avoid running inference on an image containing label-rect */
    static mpp_t mp_split;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    user_data.mp_split = mp_split;

    if (args->src_format == MPP_PIXEL_JPEG)
    {
        /* Add element jpeg decode */
        mpp_element_params_t elem_params_decoder;
        memset(&elem_params_decoder, 0, sizeof(mpp_element_params_t));
        elem_params_decoder.decode.dev_name = IMG_DECODE_DEV_NAME;
        elem_params_decoder.decode.width = IR_CAMERA_WIDTH;
        elem_params_decoder.decode.height = IR_CAMERA_HEIGHT;

        if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_CPU") == 0)
            elem_params_decoder.decode.out_format = MPP_PIXEL_BGR; /* TODO auto detect */
        else if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_HW") == 0)
            elem_params_decoder.decode.out_format = MPP_PIXEL_YUYV; /* TODO auto detect */

        ret = mpp_element_add(mp_split, MPP_ELEMENT_IMG_DECODE, &elem_params_decoder, NULL);
        if (ret)
        {
            PRINTF("Failed to add element DECODE\n");
            goto err;
        }
        else{
            PRINTF("Added HW jpeg decoder to the branch mp_split !\r\n");
        }
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

    /* First do crop + resize + color convert */
    static mpp_element_params_t infer_conv_params;
    memset(&infer_conv_params, 0, sizeof(infer_conv_params));
    /* pick GFX device */
    infer_conv_params.convert.dev_name = APP_GFX_BACKEND_INFER_NAME;
    /* set output buffer dims */
    infer_conv_params.convert.out_buf.width = SCRFD_KPS_WIDTH;
    infer_conv_params.convert.out_buf.height = SCRFD_KPS_HEIGHT;
    /* color convert */
    infer_conv_params.convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
    infer_conv_params.convert.ops = MPP_CONVERT_COLOR;
    /* crop center of image */
    infer_conv_params.convert.crop.top = CROP_TOP;
    infer_conv_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    infer_conv_params.convert.crop.left = CROP_LEFT;
    infer_conv_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    infer_conv_params.convert.ops |= MPP_CONVERT_CROP;
    /* resize: scaling parameters */
    infer_conv_params.convert.scale.width = SCRFD_KPS_WIDTH;
    infer_conv_params.convert.scale.height = SCRFD_KPS_HEIGHT;
    infer_conv_params.convert.ops |= MPP_CONVERT_SCALE;
    /* then add a flip */
    if (SRC_DISPLAY_FLIP != FLIP_NONE)
    {
        infer_conv_params.convert.flip = SRC_DISPLAY_FLIP;
        infer_conv_params.convert.ops |=  MPP_CONVERT_ROTATE;
    }

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
    ret = mpp_element_add(mp_bg, MPP_ELEMENT_CONVERT, &infer_conv_params, &infer_conv_h);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
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

    /* prepare the scrfd_kps model params */
    static mpp_element_params_t scrfd_kps_params;
    memset(&scrfd_kps_params, 0 , sizeof(mpp_element_params_t));
    scrfd_kps_params.ml_inference.model_data = scrfd_kps_data;
    scrfd_kps_params.ml_inference.model_size = scrfd_kps_data_len;
    scrfd_kps_params.ml_inference.model_input_mean = SCRFD_KPS_INPUT_MEAN;
    scrfd_kps_params.ml_inference.model_input_std = SCRFD_KPS_INPUT_STD;
    scrfd_kps_params.ml_inference.inference_params.num_inputs = 1;
    scrfd_kps_params.ml_inference.inference_params.num_outputs = 9;
    scrfd_kps_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    scrfd_kps_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    /* configure TFlite element with scrfd_kps model */
    ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, &scrfd_kps_params, &user_data.infer_elem);
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
    if (args->src_format == MPP_PIXEL_JPEG)
    {
        /* Add element jpeg decode */
        mpp_element_params_t elem_params_decoder;
        memset(&elem_params_decoder, 0, sizeof(mpp_element_params_t));
        elem_params_decoder.decode.dev_name = IMG_DECODE_DEV_NAME;
        elem_params_decoder.decode.width = RGB_CAMERA_WIDTH;
        elem_params_decoder.decode.height = RGB_CAMERA_HEIGHT;

        if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_CPU") == 0)
            elem_params_decoder.decode.out_format = MPP_PIXEL_BGR; /* TODO auto detect */
        else if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_HW") == 0)
            elem_params_decoder.decode.out_format = MPP_PIXEL_YUYV; /* TODO auto detect */

        ret = mpp_element_add(mp, MPP_ELEMENT_IMG_DECODE, &elem_params_decoder, NULL);
        if (ret)
        {
            PRINTF("Failed to add element DECODE\n");
            goto err;
        }
    }
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

    if (SRC_DISPLAY_FLIP != FLIP_NONE)
        elem_params.convert.flip = SRC_DISPLAY_FLIP;

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
    elem_params.labels.max_landmk = MAX_LABEL_RECTS * SCRFD_NUM_LANDMARKS;

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

    /* compose element params */
    static mpp_element_params_t elem_params_compose;
    memset(&elem_params_compose, 0, sizeof(mpp_element_params_t));
    mpp_stats_t compose_stats = {0};
    elem_params_compose.stats = &compose_stats;

    draw_text_area(g_text_img, sizeof(g_text_img), &txt_info);

    /* Output params */
    elem_params_compose.compose.out_angle = ROTATE_270;
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

    /* Configure logo image */
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].width = LOGO_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].height = LOGO_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].format = MPP_PIXEL_RGB;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].buffer = (void *) NXP_Logo_RGB888_Colour_320_map;

    /* Logo area */
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.left = LOGO_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.top = LOGO_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.right = LOGO_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.bottom = LOGO_BOTTOM_POS;

    /* Configure text image */
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].width = TEXT_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].height = TEXT_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].format = MPP_PIXEL_RGB565;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].buffer = g_text_img;
    
    /* Text area */
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.left = TEXT_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.top = TEXT_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.right = TEXT_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.bottom = TEXT_BOTTOM_POS;

    /* Configure Inference preview image */
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].width = INFPVW_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].height = INFPVW_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].format = MPP_PIXEL_RGB;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].buffer = NULL;

    /* Inference preview area position */
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.left = INFPVW_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.top = INFPVW_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.right = INFPVW_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.bottom = INFPVW_BOTTOM_POS;
    user_data.p_params_compose = &elem_params_compose;

    /* Input area - full frame */
    elem_params_compose.compose.input_area.left = 0;
    elem_params_compose.compose.input_area.top = APP_DISPLAY_HEIGHT - (int)(APP_DISPLAY_WIDTH * FRAME_ASP_RATIO);
    elem_params_compose.compose.input_area.bottom = APP_DISPLAY_HEIGHT - 1;
    elem_params_compose.compose.input_area.right = APP_DISPLAY_WIDTH - 1;

    /* retrieve the element handle while add api */
    ret = mpp_element_add(mp, MPP_ELEMENT_IMG_COMPOSE, &elem_params_compose, &user_data.compose_elem);
    if (ret) {
        PRINTF("Failed to add element IMG_COMPOSE (0x%x)\r\n", ret);
        goto err;
    }

    static mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = false;
    ret = mpp_display_add(mp, args->display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", args->display_name);
        goto err;
    }

    mpp_stats_enable(MPP_STATS_GRP_API);

    user_data.api_stats = &api_stats;

    TickType_t xLastPrintTime = 0, tick = 0, notifyTime = 0;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    const TickType_t notifyDelay = OUTPUT_NOTIFY_PERIOD_MS / portTICK_PERIOD_MS;
    uint32_t last_inf_frame_num = user_data.inference_frame_num;
    bool first_mpp_start = true;

    s_mpp_task_queue = xQueueCreate(MPP_CMD_QUEUE_SIZE, sizeof(msg_t *));
    if (s_mpp_task_queue == NULL) {
        PRINTF("Failed to create mpp receive queue\r\n");
        goto err;
    }
    mpp_task_set_queue(s_mpp_task_queue);

    // create registration timeout timer
    registration_timer = xTimerCreate(
    		"RegTimer",
			pdMS_TO_TICKS(5000), // Default period, will be changed when needed
			pdFALSE, // One-shot timer
			NULL,
			registration_timeout_callback
    );

    if (registration_timer == NULL) {
    	PRINTF("Failed to create registration timer\r\n");
    	goto err;
    }

    int ctrl_task_msg = -1;

    for (;;) {

        /* communication with control task parameters */
        void *ctrl_task_data = NULL;

        // Check for incoming messages from control task
        msg_t *received_msg;

        if (xQueueReceive(s_mpp_task_queue, &received_msg, pdMS_TO_TICKS(5)) == pdPASS) // Non-blocking receive
        {
            /* Handle message received from control task */
            switch(received_msg->msgID)
            {
                case MPP_TASK_FACE_REG_REQ:
                {
                    // Extract the name from the message parameter
                    face_reg_req_msg_payload_t *reg_params = &received_msg->reg_payload;
                    char *registration_name = reg_params->name;
                    int registration_timeout = reg_params->timeout * 1000; // convert to milliseconds
                
                     // Store the name for registration (you might need to add this to user_data)
                    if (registration_name != NULL && registration_name[0] != '\0')
                    {
                        // Store the name somewhere accessible (add to user_data_t if needed)
                        strcpy(user_data.registration_name, registration_name);
                        user_data.registration_timeout = registration_timeout;
                    
                        // Change state to registering
                        user_data.state = STATE_REGISTRATION_MODE;
                        user_data.registration_mode_on = 1; /* activate registraion mode */

                        if (first_mpp_start == true)
                        {
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
                            ret = mpp_start(mp, first_mpp_start, false);
                            if (ret) {
                                PRINTF("Failed to start main pipeline branch");
                                goto err;
                            }

                            first_mpp_start = false;

                            /* small delay to allow pipelines to initialize */
                            vTaskDelay(pdMS_TO_TICKS(100));

                            /* stop the inference pippelines after initialization */
                            mpp_stop(mp_bg);
                            mpp_stop(mp_split);
                        }
                        else
                        {
                            /* stop the inference pippelines */
                            if (mpp_is_running(mp_bg))
                                mpp_stop(mp_bg);
                            if (mpp_is_running(mp_split))
                                mpp_stop(mp_split);

                            /* Start main pipeline if not running */
                            if (!mpp_is_running(mp))
                                mpp_start(mp, 0, false);
                        }

                        /* start registration timer */
                        registration_start_time = xTaskGetTickCount();
                        registration_end_time = 0; // Reset end time

                        // Configure and start existing timer
                        if (registration_timer != NULL) {
                        	xTimerChangePeriod(registration_timer, pdMS_TO_TICKS(user_data.registration_timeout), 0);
                        	registration_timer_expired = false;
                        	xTimerStart(registration_timer, 0);
                        	PRINTF("Registration timer started for %d ms\r\n", user_data.registration_timeout);
                        }
                    }

                    break;
                }

                case MPP_TASK_FR_ENABLE_LCD_ON:
                    /* Check if this is the first time we start the pipeline */
                    if (!first_mpp_start)
                    {
                        /* If it's not, stop pipelines if they are running */
                        if (mpp_is_running(mp_bg))
                            mpp_stop(mp_bg);
                        if (mpp_is_running(mp_split))
                            mpp_stop(mp_split);
                        if (mpp_is_running(mp))
                            mpp_stop(mp);

                        vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops
                        
                        /* Start with the face detection model */
                        /* PRINTF("Switching to SCRFD \r\n"); */
                        /* update convert params for SCRFD */
                        infer_conv_params.convert.crop.top = CROP_TOP;
                        infer_conv_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
                        infer_conv_params.convert.crop.left = CROP_LEFT;
                        infer_conv_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
                        infer_conv_params.convert.out_buf.width = SCRFD_KPS_WIDTH;
                        infer_conv_params.convert.out_buf.height = SCRFD_KPS_HEIGHT;
                        infer_conv_params.convert.scale.width = SCRFD_KPS_WIDTH;
                        infer_conv_params.convert.scale.height = SCRFD_KPS_HEIGHT;
                        infer_conv_params.convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
                        ret = mpp_element_update(mp_bg, infer_conv_h, &infer_conv_params, true);
                        if (ret) {
                            PRINTF("Failed to update element convert for scrfd_kps\r\n");
                            goto err;
                        }

                        /* switch to SCRFD_KPS */
                        ret = mpp_element_update(mp_bg, user_data.infer_elem, &scrfd_kps_params, true);
                        if (ret) {
                            PRINTF("Failed to update element inference for scrfd_kps\r\n");
                            goto err;
                        }

                        user_data.cur_model = MODEL_SCRFD_KPS;
                        user_data.state = STATE_DETECTING;
                    }

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
                    ret = mpp_start(mp, first_mpp_start, false);
                    if (ret) {
                        PRINTF("Failed to start main pipeline branch");
                        goto err;
                    }

                    first_mpp_start = false;

                    break;

                case MPP_TASK_FR_DISABLE_LCD_ON:
                    /* Stop the inference branches, but keep display on */
                    if (mpp_is_running(mp_bg))
                        mpp_stop(mp_bg);
                    if (mpp_is_running(mp_split))
                        mpp_stop(mp_split);

                    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops

                    /* update labeled rectangle */
                    mpp_element_params_t rect_params;
                    memset(&rect_params, 0, sizeof(rect_params));
                    /* detected_count contains only the detection zone box */
                    rect_params.labels.detected_rect = 1;
                    rect_params.labels.max_rect = MAX_LABEL_RECTS;
                    rect_params.labels.rectangles = user_data.labels;
                    mpp_element_update(mp, user_data.labrect_elem, &rect_params, true);

                    /* Start display if not started already */
                    if (!mpp_is_running(mp))
                    {
                        if (first_mpp_start)
                        {
                            /* Hack to set the state of the seconday branhes to stopped instead of not started */
                            mpp_start(mp_bg, 0, false);
                            mpp_start(mp_split, 0, false);
                            mpp_stop(mp_bg);
                            mpp_stop(mp_split);
                        }

                        /* In case this is the first mpp start, we need to allocated the memory for the pipelines */
                        ret = mpp_start(mp, first_mpp_start, false);
                        if (ret) {
                            PRINTF("Failed to start main pipeline branch");
                            goto err;
                        }
                        first_mpp_start = false;
                    }

                    /* Clear the user data state to avoid any residual state */
                    user_data.state = STATE_DETECTING;

                    break;

                case MPP_TASK_FR_DISABLE_LCD_OFF:
                    if (mpp_is_running(mp_bg))
                        mpp_stop(mp_bg);
                    if (mpp_is_running(mp_split))
                        mpp_stop(mp_split);
                    if (mpp_is_running(mp))
                        mpp_stop(mp);

                    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops

                    /* Clear the user data state to avoid any residual state */
                    user_data.state = STATE_DETECTING;

                    break;

                default:
                    PRINTF("Command %d not supported\r\n", received_msg->msgID);
                    break;
            }

            if (received_msg != NULL)
                vPortFree(received_msg);
        }

        // check for timeout
        if (user_data.registration_mode_on == 1 && registration_timer_expired) {
        	registration_end_time = xTaskGetTickCount();
        	if (user_data.state != STATE_REGISTERED && user_data.state != STATE_REGISTRATION_CANCELLED) {
        		PRINTF("Registration timeout - forcing cancellation after %d ms\r\n",
        				(registration_end_time - registration_start_time) * portTICK_PERIOD_MS);
        		user_data.state = STATE_REGISTRATION_CANCELLED;
        		ctrl_task_msg = CTRL_TASK_FACE_REG_TIMEOUT;

        		// Stop and cleanup timer
				if (registration_timer != NULL) {
					xTimerStop(registration_timer, 0);
				}
				registration_timer_expired = false;
        	}
        }

        /* manage periodic print */
        tick = xTaskGetTickCount();
        if ((tick > (xLastPrintTime + xFrequency)) && Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
        {
            if (last_inf_frame_num != user_data.inference_frame_num)
            {
                xLastPrintTime = tick;
                // Store separately the detection , antispoofing and recognition inference times
                if (user_data.last_model == MODEL_SCRFD_KPS)
                {
                    detection_inference_time = user_data.inference_time_ms;
                 if (user_data.state == STATE_DETECTING && user_data.detected_count == 0)
                    {
                        strcpy(txt_info.name, "");
                        txt_info.confidence = 0.0f;
                        txt_info.detection_inference_time = detection_inference_time;
                        txt_info.recognition_inference_time = 0;
                    #if USE_ANTISPOOFING
                        txt_info.antispoofing_inference_time = 0;
                        strcpy(txt_info.state, "");
                    #endif
                        draw_text_area(g_text_img, sizeof(g_text_img), &txt_info);
                    }
                
                }
                else if (user_data.last_model == MODEL_MOBILEFACENET)
                {
                    recognition_inference_time = user_data.inference_time_ms;
                }
            #if USE_ANTISPOOFING
                else // MODEL_ANTISPOOFING
                {
                    antispoofing_inference_time = user_data.inference_time_ms;
                }
            #endif
#ifdef PRINT_INFERENCE_RESULTS
                print_conditional(&user_data);
#endif /* PRINT_INFERENCE_RESULTS */
                last_inf_frame_num = user_data.inference_frame_num;
            }
        }
        __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);

        if (user_data.state == STATE_RECOGNIZED)
        {
            // create and fill recognition result structure from user data result
        	msg_t *ctrl_task_notify_msg = (msg_t *) pvPortMalloc(sizeof(msg_t));
            strcpy(ctrl_task_notify_msg->fr_payload.name, user_data.result.recognized_name);
            ctrl_task_notify_msg->fr_payload.similarity = user_data.result.similarity_percentage;

            ctrl_task_msg = CTRL_TASK_FACE_RECOGNIZED;
            ctrl_task_data = ctrl_task_notify_msg;
            /* update text area */
            strcpy(txt_info.name, user_data.result.recognized_name);
            txt_info.confidence = user_data.result.similarity_percentage;
            txt_info.detection_inference_time = detection_inference_time;
            txt_info.recognition_inference_time = recognition_inference_time;
        #if USE_ANTISPOOFING   
            txt_info.antispoofing_inference_time = antispoofing_inference_time;
            strcpy(txt_info.state, "Real face");
        #endif
            draw_text_area(g_text_img, sizeof(g_text_img), &txt_info);
        }
        else if (user_data.state == STATE_NOT_RECOGNIZED)
        {
            if (user_data.registration_mode_on == 1)
                user_data.state = STATE_REGISTERING;
            else
            {
                ctrl_task_msg = CTRL_TASK_FACE_NOT_RECOGNIZED;
                ctrl_task_notify(ctrl_task_msg, NULL);
                ctrl_task_msg = -1;

                if (user_data.labrect_elem != 0)
                {
                	elem_params.labels.detected_rect = user_data.detected_count + 1;
                	elem_params.labels.max_rect = MAX_LABEL_RECTS;
                	elem_params.labels.rectangles = user_data.labels;
                	mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
                }
                strcpy(txt_info.name, "Unknown");
                txt_info.confidence = 0.0f;
                txt_info.detection_inference_time = detection_inference_time;
                txt_info.recognition_inference_time = recognition_inference_time;
            #if USE_ANTISPOOFING    
                txt_info.antispoofing_inference_time = antispoofing_inference_time;
                strcpy(txt_info.state, "Real face");
            #endif
                draw_text_area(g_text_img, sizeof(g_text_img), &txt_info);
                /* mpp_element_update(mp, user_data.compose_elem, &elem_params_compose); */

                user_data.state = STATE_NOT_REGISTERING;
            }
        }

        /* manage timed state transition */
        if (user_data.state == STATE_RECOGNIZED || user_data.state == STATE_REGISTERED || user_data.state == STATE_SPOOF ||
                user_data.state == STATE_REGISTRATION_CANCELLED || user_data.state == STATE_NOT_REGISTERING || user_data.state == STATE_REGISTRATION_MODE)
        {
        	/* communicate updates to ctrl task if needed*/
        	if (ctrl_task_msg != -1)
        	{
                if ((ctrl_task_msg == CTRL_TASK_FACE_REG_TIMEOUT) ||
                    (ctrl_task_msg == CTRL_TASK_FACE_REG_COMPLETE))
                {
                    mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_ON, NULL);
                }
        		ctrl_task_notify(ctrl_task_msg, ctrl_task_data);
                ctrl_task_msg = -1;
        	}
        #if USE_ANTISPOOFING
            if(user_data.state == STATE_SPOOF)
            {
                txt_info.detection_inference_time = detection_inference_time;
                txt_info.antispoofing_inference_time = antispoofing_inference_time;
                strcpy(txt_info.state, "Fake face");
                txt_info.recognition_inference_time = 0;
                txt_info.confidence = 0.0f;
                draw_text_area(g_text_img, sizeof(g_text_img), &txt_info);
            }
        #endif
            /* start timer */
            notifyTime = tick;
            user_data.state = STATE_NOTIFYING_USER;

            /* stop branches with convert and inference elements */
            /* until the user is notified */
            mpp_stop(mp_bg);
        }

        if ((user_data.state == STATE_NOTIFYING_USER) && (tick > notifyTime + notifyDelay))
        {
            /* stop timer */
            notifyTime = 0;
            user_data.state = STATE_USER_NOTIFIED;
        }

        if (user_data.state == STATE_REGISTERING)
        {
            static char label[64];
            /* update zone label */
            /* detected_count contains at least the detection zone box */
            if (user_data.labrect_elem != 0)
            {
                elem_params.labels.detected_rect = user_data.detected_count + 1;
                elem_params.labels.max_rect = MAX_LABEL_RECTS;
                elem_params.labels.rectangles = user_data.labels;
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }

            if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
            {
            	/* register new face embedding */
            	set_new_face_embeddings((const float *)user_data.result.embedding);
            	if (!database_add(user_data.registration_name))
            	{
            		registration_end_time = xTaskGetTickCount();
            		if (user_data.state == STATE_REGISTERING)
            		{
            			user_data.state = STATE_REGISTRATION_CANCELLED;
            			ctrl_task_msg = CTRL_TASK_FACE_REG_TIMEOUT;
            		}
            	}
            	else
            	{
            		registration_end_time = xTaskGetTickCount(); // Update end time on success
            		///strcpy(label, ZONE_LABEL_REGISTERED);
            		///strcat(label, user_data.registration_name);

            		if (user_data.state == STATE_REGISTERING)
            		{
            			user_data.state = STATE_REGISTERED;
            			ctrl_task_msg = CTRL_TASK_FACE_REG_COMPLETE;
            			PRINTF("Registration completed successfully in %d ms\r\n",
            					(registration_end_time - registration_start_time) * portTICK_PERIOD_MS);
            		}
            	}

            	// Stop the timer when registration completes (success or failure)
            	if (registration_timer != NULL)
            	{
            		xTimerStop(registration_timer, 0);
            	}
            	registration_timer_expired = false;

            	/* registration should be cleared */
            	strcpy(user_data.registration_name, "\0");
            	user_data.registration_mode_on = 0;

            	if (user_data.labrect_elem != 0)
            	{
            		elem_params.labels.detected_rect = user_data.detected_count + 1;
            		elem_params.labels.max_rect = MAX_LABEL_RECTS;
            		elem_params.labels.rectangles = user_data.labels;
            		strcpy((char *)user_data.labels[0].label, label);
            		mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            	}

            	__atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
            }
        }
        /* manage face recognition state machine */
        else if (user_data.state == STATE_DETECTED)
        {
            ctrl_task_msg = CTRL_TASK_FACE_DETECTED;
            ctrl_task_notify(ctrl_task_msg, NULL);
            ctrl_task_msg = -1;
            /* stop branches with convert and inference elements */
            mpp_stop(mp_bg);
#if USE_ANTISPOOFING
            /* update convert params for the antispoofing model */
            if (get_face_crop_area(&user_data.final_boxes[0], &infer_conv_params.convert.crop))
            {
                PRINTF("Failed to get face crop area\r\n");
                goto err;
            }
            if (SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
            {
                uint32_t temp_width = infer_conv_params.convert.crop.right - infer_conv_params.convert.crop.left;
                infer_conv_params.convert.crop.left = INF_SRC_WIDTH - infer_conv_params.convert.crop.right - 1;
                infer_conv_params.convert.crop.right = infer_conv_params.convert.crop.left + temp_width;
            }
            infer_conv_params.convert.out_buf.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.out_buf.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.scale.width = ANTISPOOFING_WIDTH;
            infer_conv_params.convert.scale.height = ANTISPOOFING_HEIGHT;
            infer_conv_params.convert.pixel_format = ANTISPOOFING_PIXEL_FORMAT;
            ret = mpp_element_update(mp_bg, infer_conv_h, &infer_conv_params, true);
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
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }
            user_data.cur_model = MODEL_ANTISPOOFING;
            user_data.state = STATE_CHECKING_ANTISPOOFING;
            mpp_start(mp_bg, 0, true);
#else
            /* Skipping Antispoofing, going directly to MobileFaceNet */
            user_data.state = STATE_REAL;
#endif
        }
        else if (user_data.state == STATE_REAL)
        {
            /* PRINTF("Switching to MobileFaceNet \r\n"); */
            /* update convert params for mobilefacenet */
#if USE_ANTISPOOFING
            mpp_stop(mp_bg);
#endif
            if (get_face_crop_area(&user_data.final_boxes[0], &infer_conv_params.convert.crop))
            {
                PRINTF("Failed to get face crop area\r\n");
                goto err;
            }
            if (SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
            {
                uint32_t temp_width = infer_conv_params.convert.crop.right - infer_conv_params.convert.crop.left;
                infer_conv_params.convert.crop.left = INF_SRC_WIDTH - infer_conv_params.convert.crop.right - 1;
                infer_conv_params.convert.crop.right = infer_conv_params.convert.crop.left + temp_width;
            }

            infer_conv_params.convert.out_buf.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.out_buf.height = MOBILEFACENET_HEIGHT;
            infer_conv_params.convert.scale.width = MOBILEFACENET_WIDTH;
            infer_conv_params.convert.scale.height = MOBILEFACENET_HEIGHT;
            infer_conv_params.convert.pixel_format = MOBILEFACENET_PIXEL_FORMAT;
            ret = mpp_element_update(mp_bg, infer_conv_h, &infer_conv_params, true);
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
                mpp_element_update(mp, user_data.labrect_elem, &elem_params, true);
            }
            user_data.cur_model = MODEL_MOBILEFACENET;
            user_data.state = STATE_RECOGNIZING;
            mpp_start(mp_bg, 0, true);
        }
        else if (user_data.state == STATE_USER_NOTIFIED)
        {
            /* PRINTF("Switching to SCRFD KPS \r\n"); */
            /* update convert params for scrfd_kps */
            infer_conv_params.convert.crop.top = CROP_TOP;
            infer_conv_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
            infer_conv_params.convert.crop.left = CROP_LEFT;
            infer_conv_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
            infer_conv_params.convert.out_buf.width = SCRFD_KPS_WIDTH;
            infer_conv_params.convert.out_buf.height = SCRFD_KPS_HEIGHT;
            infer_conv_params.convert.scale.width = SCRFD_KPS_WIDTH;
            infer_conv_params.convert.scale.height = SCRFD_KPS_HEIGHT;
            infer_conv_params.convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
            ret = mpp_element_update(mp_bg, infer_conv_h, &infer_conv_params, false);
            if (ret) {
                PRINTF("Failed to update element convert for scrfd_kps\r\n");
                goto err;
            }

            /* switch to SCRFD_KPS */
            ret = mpp_element_update(mp_bg, user_data.infer_elem, &scrfd_kps_params, false);
            if (ret) {
                PRINTF("Failed to update element inference for scrfd_kps\r\n");
                goto err;
            }

            user_data.cur_model = MODEL_SCRFD_KPS;
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