/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//#include "board_init.h"
#include "demo_config.h"
#include "demo_info.h"
#include "fsl_debug_console.h"
#include "image.h"
#include "image_utils.h"
#include "model.h"
#include "output_postproc.h"
#include "timer.h"
#include "app.h"

void prepare_model()
{
    if (MODEL_Init() != kStatus_Success)
    {
        PRINTF("Failed initializing model" EOL);
        for (;;) {}
    }
}

void delay()
{

  for(int i=0;i<1000000;i++)
	  asm("nop");

}

void tflm_label_image(void)
{
/*  
    BOARD_Init();
    TIMER_Init();

    DEMO_PrintInfo();

    if (MODEL_Init() != kStatus_Success)
    {
        PRINTF("Failed initializing model" EOL);
        for (;;) {}
    }
*/
    TIMER_Init();
    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t* inputData = MODEL_GetInputTensorData(&inputDims, &inputType);
    //uint8_t* inputData2 = MODEL_GetInputTensorData(&inputDims, &inputType);

    tensor_dims_t outputDims;
    tensor_type_t outputType;
    uint8_t* outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);

    //while (1)
    //{
        /* Expected tensor dimensions: [batches, height, width, channels] */
//      CLKCTL0->PSCCTL5_CLR |= (1UL << CLKCTL0_PSCCTL5_NPU0_SHIFT); //Turn off NPU clock

     /*   if (IMAGE_GetImage(inputData, inputDims.data[2], inputDims.data[1], inputDims.data[3]) != kStatus_Success)
        {
            PRINTF("Failed retrieving input image" EOL);
            for (;;) {}
        }
     */
        MODEL_ConvertInput(inputData, &inputDims, inputType);
#if TIMING
        auto startTime = TIMER_GetTimeInUS();
        DEMO_LOG("#### before run inference \r\n ");
#endif

#if DELAY
        delay();
       // CLKCTL0->PSCCTL5_SET |= (1UL << CLKCTL0_PSCCTL5_NPU0_SHIFT); //Turn on NPU clock
#endif       
	MODEL_RunInference();


#if TIMING
        auto endTime = TIMER_GetTimeInUS();
        DEMO_LOG("#### after run inference \r\n");
#endif

#if DELAY
//CLKCTL0->PSCCTL5_CLR |= (1UL << CLKCTL0_PSCCTL5_NPU0_SHIFT); //Turn off NPU clock
        delay();    
#endif

#if TIMING
        MODEL_ProcessOutput(outputData, &outputDims, outputType, endTime - startTime);
#else
	MODEL_ProcessOutput(outputData, &outputDims, outputType);
#endif
    //}
}
