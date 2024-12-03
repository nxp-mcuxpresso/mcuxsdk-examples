/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to build a simple graph with 2 branches:
 * 2D camera -> split +-> image converter -> inference engine (model: MOBILEFACENET)
 *                    +-> image converter(*) -> draw labeled rectangles -> display
 * The camera view finder branch:
 * displays captured content on screen;
 * (*) image conversion may be skipped for cameras supporting same format as display;
 * The inference engine branch:
 * runs a face recognition model using TF-Lite micro inference engine;
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
#include "board_init.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"

/* utility functions */
#include "models/utils.h"

/* Model data input */
#include APP_TFLITE_MOBILEFACENET_DATA

/* Persons database */
#include APP_DATABASE_NAME

/* Model output post-processing */
#include "models/mobilefacenet/mobilefacenet_output_postproc_quantized.h"

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/
/* label rect line width */
#define RECT_LINE_WIDTH 2

#define APP_GFX_BACKEND_NAME "gfx_PXP"

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

#define MODEL_ASPECT_RATIO   (1.0f * MOBILEFACENET_WIDTH / MOBILEFACENET_HEIGHT)
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

#define OUTPUT_PRINT_PERIOD_MS 1100

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] =  APP_CAMERA_NAME;

/** Default priority for application tasks
   Tasks created by the application have a lower priority than pipeline tasks by default.
   Pipeline_task_max_prio in mpp_api_params_t structure should be adjusted with other application tasks.*/
#define APP_DEFAULT_PRIO        1

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _user_data_t {
	int inference_frame_num;
	mpp_t mp;
	mpp_elem_handle_t elem;
	mpp_labeled_rect_t labels[1];
	recognition_result result;
	uint32_t accessing; /* boolean protecting access to user data */
	int inference_time_ms;
} user_data_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);

/*******************************************************************************
 * Code
 ******************************************************************************/

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

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data) {
	const mpp_inference_cb_param_t *inf_output;
	recognition_result result;

	/* user_data handle contains application private data */
	user_data_t *app_priv = (user_data_t *)user_data;

	switch(evt) {
	case MPP_EVENT_INFERENCE_OUTPUT_READY:
		/* cast evt_data pointer to correct structure matching the event */
		inf_output = (const mpp_inference_cb_param_t *) evt_data;
		MOBILEFACENET_ProcessOutput(
                inf_output,
                Embedding_database,
                DATABASE_MAX_PEOPLE,
                &result);
		/* check that we can modify the user data (not accessed by other task) */
		if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
		{
			app_priv->inference_time_ms = inf_output->inference_time_ms;
			/* copy recognition results */
			app_priv->result = result;
			__atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
		}

		mpp_element_params_t params;
		uint8_t label_size = sizeof(params.labels.rectangles[0].label);

		const char* label = "\0";
		// Update the label in the first rectangle
		params.labels.detected_count = 1;
		params.labels.max_count = 1;
		params.labels.rectangles = app_priv->labels;
		strncpy((char *)params.labels.rectangles[0].label, label, label_size);
		params.labels.rectangles[0].label[label_size - 1] = '\0';
        if ( (app_priv->elem != 0) && ( app_priv->mp != NULL ) )
        {
            mpp_element_update(app_priv->mp, app_priv->elem, &params);
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
	mpp_element_params_t elem_params;
	memset(&elem_params, 0, sizeof(elem_params));
	/* pick default device from the first listed and supported by Hw */
	elem_params.convert.dev_name = APP_GFX_BACKEND_NAME;
	/* set output buffer dims */
	elem_params.convert.out_buf.width = MOBILEFACENET_WIDTH;
	elem_params.convert.out_buf.height = MOBILEFACENET_HEIGHT;
	// crop center of image
	elem_params.convert.crop.top = CROP_TOP;
	elem_params.convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
	elem_params.convert.crop.left = CROP_LEFT;
	elem_params.convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
	elem_params.convert.ops = MPP_CONVERT_CROP;
	// resize: scaling parameters
	elem_params.convert.scale.width = MOBILEFACENET_WIDTH;
	elem_params.convert.scale.height = MOBILEFACENET_HEIGHT;
	elem_params.convert.ops |= MPP_CONVERT_SCALE;
	//converting image pixel format
	elem_params.convert.pixel_format = MOBILEFACENET_FORMAT;
	elem_params.convert.ops |= MPP_CONVERT_COLOR;
	/* then add a flip */
	elem_params.convert.flip = FLIP_HORIZONTAL;
	elem_params.convert.ops |=  MPP_CONVERT_ROTATE;
	ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);
	if (ret ) {
		PRINTF("Failed to add element CONVERT\n");
		goto err;
	}

	// configure TFlite element with model
	mpp_element_params_t mobilefacenet_params;
	static mpp_stats_t mobilefacenet_stats;
	memset(&mobilefacenet_params, 0 , sizeof(mpp_element_params_t));

	mobilefacenet_params.ml_inference.model_data = mobilefacenet_data;
	mobilefacenet_params.ml_inference.model_size = mobilefacenet_data_len;
	mobilefacenet_params.ml_inference.model_input_mean = MOBILEFACENET_INPUT_MEAN;
	mobilefacenet_params.ml_inference.model_input_std = MOBILEFACENET_INPUT_STD;
	mobilefacenet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
	mobilefacenet_params.ml_inference.inference_params.num_inputs = 1;
	mobilefacenet_params.ml_inference.inference_params.num_outputs = 1;
	mobilefacenet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
	mobilefacenet_params.stats = &mobilefacenet_stats;

	ret = mpp_element_add(mp_split, MPP_ELEMENT_INFERENCE, &mobilefacenet_params, NULL);
	if (ret) {
		PRINTF("Failed to add element VALGO_TFLite");
		goto err;
	}

	// close the pipeline with a null sink
	ret = mpp_nullsink_add(mp_split);
	if (ret) {
		PRINTF("Failed to add NULL sink\n");
		goto err;
	}

	/* On the main branch of the pipeline, send the frame to the display */
	/* First do color-convert + flip */
	memset(&elem_params, 0, sizeof(elem_params));
	/* pick default device from the first listed and supported by Hw */
	elem_params.convert.dev_name = APP_GFX_BACKEND_NAME;
	/* set output buffer dims */
	elem_params.convert.out_buf.width = APP_CAMERA_WIDTH;
	elem_params.convert.out_buf.height = APP_CAMERA_HEIGHT;
	elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;

	/* then add a flip */
	elem_params.convert.flip = FLIP_HORIZONTAL;
	elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE ;

	ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

	if (ret) {
		PRINTF("Failed to add element CONVERT\n");
		goto err;
	}

	// add one label rectangle
	memset(&elem_params, 0, sizeof(elem_params));
	memset(&user_data.labels, 0, sizeof(user_data.labels));

	// params init
	elem_params.labels.max_count = 1;
	elem_params.labels.detected_count = 1;
	elem_params.labels.rectangles = user_data.labels;

	// first add detection zone box
	user_data.labels[0].top    = DETECTION_ZONE_RECT_TOP;
	user_data.labels[0].left   = DETECTION_ZONE_RECT_LEFT;
	user_data.labels[0].bottom = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_HEIGHT;
	user_data.labels[0].right  = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_WIDTH;
	user_data.labels[0].line_width = RECT_LINE_WIDTH;
	user_data.labels[0].line_color.rgb.B = 0xff;
	strcpy((char *)user_data.labels[0].label, "Person not recognized");

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

	// start preempt-able pipeline branch
	ret = mpp_start(mp_split, 0);
	if (ret) {
		PRINTF("Failed to start pipeline");
		goto err;
	}

	// start main pipeline branch
	ret = mpp_start(mp, 1);
	if (ret) {
		PRINTF("Failed to start pipeline");
		goto err;
	}

	TickType_t x_last_awake_time;
	const TickType_t x_frequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
	x_last_awake_time = xTaskGetTickCount();
	for (;;) {
		xTaskDelayUntil( &x_last_awake_time, x_frequency );
		mpp_stats_disable(MPP_STATS_GRP_ELEMENT);
		PRINTF("Element stats --------------------------\r\n");
		PRINTF("mobilefacenet : exec_time %u (ms)\r\n", mobilefacenet_stats.elem.elem_exec_time);
		mpp_stats_enable(MPP_STATS_GRP_ELEMENT);

		if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
		{
			PRINTF("inference time %d (ms) \r\n", user_data.inference_time_ms);

			if (user_data.result.recognized_name[0]=='\0')
			{
				PRINTF("Face not recognized. \r\n");
			}
			else {
				PRINTF("Recognized face: %s with similarity percentage: %d%%\r\n", user_data.result.recognized_name, user_data.result.similarity_percentage);
			}
			/* after reading, inference output should be cleared */
			strcpy(user_data.result.recognized_name,"\0");
			user_data.result.similarity_percentage = 0;
			__atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
		}
	}

	/* pause application task */
	vTaskSuspend(NULL);

	err:
	for (;;)
	{
		PRINTF("Error building application pipeline : ret %d\r\n", ret);
		vTaskSuspend(NULL);
	}
}
