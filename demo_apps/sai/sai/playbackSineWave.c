/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static float32_t do_fft(
    uint32_t sampleRate, uint32_t bitWidth, const uint8_t *buffer, float32_t *fftData, float32_t *fftResult);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static float32_t ffData[2 * BUFFER_SIZE];
static float32_t ffResult[BUFFER_SIZE];
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;
extern uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
extern volatile bool istxFinished;
extern volatile bool isrxFinished;
extern volatile uint32_t beginCount;
extern volatile uint32_t sendCount;
extern volatile uint32_t receiveCount;
extern bool sdcard;
extern volatile uint32_t fullBlock;
extern volatile uint32_t emptyBlock;
/*******************************************************************************
 * Code
 ******************************************************************************/
static float32_t do_fft(
    uint32_t sampleRate, uint32_t bitWidth, const uint8_t *buffer, float32_t *fftData, float32_t *fftResult)
{
    /* Counter variable for navigating buffers */
    uint32_t counter;

    /* Return value for wav frequency in hertz */
    float32_t wavFreqHz = 0.0f;

    /* CMSIS status & FFT instance */
    arm_status status;                /* ARM status variable */
    arm_cfft_radix2_instance_f32 fft; /* ARM FFT instance */

    /* Frequency analysis variables */
    float32_t maxValue       = 0.0f;        /* max value for greatest FFT bin amplitude */
    uint32_t testIndex       = 0U;          /* value for storing the bin location with maxValue */
    uint32_t complexBuffSize = BUFFER_SIZE * 2U;
    uint32_t fftSize         = BUFFER_SIZE; /* FFT bin size */
    uint32_t ifftFlag        = 0U;          /* Flag for the selection of CFFT/CIFFT */
    uint32_t doBitReverse    = 1U;          /* Flag for selection of normal order or bit reversed order */

    /* Validate input parameters */
    if ((buffer == NULL) || (fftData == NULL) || (fftResult == NULL) || (sampleRate == 0U) || (fftSize == 0U))
    {
        return 0.0f;
    }

    /* Check for potential overflow in multiplication and division */
    if ((sampleRate > (UINT32_MAX / 2U)) || (fftSize == 0U))
    {
        return 0.0f;
    }

    float32_t hzPerBin = 2.0f * ((float32_t)sampleRate / (float32_t)fftSize); /* Calculate hz per FFT bin */

    const uint8_t *temp8;                                                     /* Point to data for 8 bit samples */
    uint8_t temp8Data;

    const uint16_t *temp16; /* Point to data for 16 bit samples */
    int16_t temp16Data;

    const uint32_t *temp32; /* Point to data for 32 bit samples */
    int32_t temp32Data;

    /* Set status as success */
    status = ARM_MATH_SUCCESS;

    /* Wav data variables */
    switch (bitWidth)
    {
        case 8U:
        {
            /* Validate buffer size for 8-bit data */
            size_t elementSize = sizeof(uint8_t);
            if (complexBuffSize > (SIZE_MAX / elementSize))
            {
                return 0.0f;
            }

            temp8     = buffer;
            temp8Data = 0U;

            /* Copy wav data to fft input array */
            for (counter = 0U; counter < complexBuffSize; counter++)
            {
                if ((counter % 2U) == 0U)
                {
                    /* Check bounds before accessing buffer */
                    size_t byteIndex = counter / 2U;
                    if (byteIndex < (BUFFER_SIZE * BUFFER_NUM))
                    {
                        temp8Data        = temp8[byteIndex];
                        fftData[counter] = (float32_t)temp8Data;
                    }
                    else
                    {
                        fftData[counter] = 0.0f;
                    }
                }
                else
                {
                    fftData[counter] = 0.0f;
                }
            }

            /* Set instance for Real FFT */
            status = arm_cfft_radix2_init_f32(&fft, fftSize, ifftFlag, doBitReverse);

            if (status == ARM_MATH_SUCCESS)
            {
                /* Perform Real FFT on fftData */
                arm_cfft_radix2_f32(&fft, fftData);

                /* Populate FFT bins */
                arm_cmplx_mag_f32(fftData, fftResult, fftSize);

                /* Zero out non-audible, low-frequency noise from FFT Results. */
                fftResult[0] = 0.0f;

                /* Find max bin and location of max (first half of bins as this is the only valid section) */
                arm_max_f32(fftResult, fftSize, &maxValue, &testIndex);
            }

            break;
        }

        case 16U:
            /* Validate buffer size for 16-bit data */
            if ((complexBuffSize > (SIZE_MAX / sizeof(uint16_t))) ||
                ((complexBuffSize / 2U) > (SIZE_MAX / sizeof(uint16_t))))
            {
                return 0.0f;
            }

            temp16     = (const uint16_t *)buffer;
            temp16Data = 0;

            /* Copy wav data to fft input array */
            for (counter = 0U; counter < complexBuffSize; counter++)
            {
                if ((counter % 2U) == 0U)
                {
                    /* Check bounds before accessing buffer */
                    size_t sampleIndex = counter / 2U;
                    if (sampleIndex < ((BUFFER_SIZE * BUFFER_NUM) / sizeof(uint16_t)))
                    {
                        temp16Data       = (int16_t)temp16[sampleIndex];
                        fftData[counter] = (float32_t)temp16Data;
                    }
                    else
                    {
                        fftData[counter] = 0.0f;
                    }
                }
                else
                {
                    fftData[counter] = 0.0f;
                }
            }

            /* Set instance for Real FFT */
            status = arm_cfft_radix2_init_f32(&fft, fftSize, ifftFlag, doBitReverse);

            if (status == ARM_MATH_SUCCESS)
            {
                /* Perform Real FFT on fftData */
                arm_cfft_radix2_f32(&fft, fftData);

                /* Populate FFT bins */
                arm_cmplx_mag_f32(fftData, fftResult, fftSize);

                /* Zero out non-audible, low-frequency noise from FFT Results. */
                fftResult[0] = 0.0f;

                /* Find max bin and location of max (first half of bins as this is the only valid section) */
                arm_max_f32(fftResult, fftSize, &maxValue, &testIndex);
            }

            break;

        case 32U:
            /* Validate buffer size for 32-bit data */
            if ((complexBuffSize > (SIZE_MAX / sizeof(uint32_t))) ||
                ((complexBuffSize / 2U) > (SIZE_MAX / sizeof(uint32_t))))
            {
                return 0.0f;
            }

            temp32     = (const uint32_t *)buffer;
            temp32Data = 0;

            /* Copy wav data to fft input array */
            for (counter = 0U; counter < complexBuffSize; counter++)
            {
                if ((counter % 2U) == 0U)
                {
                    /* Check bounds before accessing buffer */
                    size_t sampleIndex = counter / 2U;
                    if (sampleIndex < ((BUFFER_SIZE * BUFFER_NUM) / sizeof(uint32_t)))
                    {
                        temp32Data       = (int32_t)temp32[sampleIndex];
                        fftData[counter] = (float32_t)temp32Data;
                    }
                    else
                    {
                        fftData[counter] = 0.0f;
                    }
                }
                else
                {
                    fftData[counter] = 0.0f;
                }
            }

            /* Set instance for Real FFT */
            status = arm_cfft_radix2_init_f32(&fft, fftSize, ifftFlag, doBitReverse);

            if (status == ARM_MATH_SUCCESS)
            {
                /* Perform Real FFT on fftData */
                arm_cfft_radix2_f32(&fft, fftData);

                /* Populate FFT bins */
                arm_cmplx_mag_f32(fftData, fftResult, fftSize);

                /* Zero out non-audible, low-frequency noise from FFT Results. */
                fftResult[0] = 0.0f;

                /* Find max bin and location of max (first half of bins as this is the only valid section) */
                arm_max_f32(fftResult, fftSize, &maxValue, &testIndex);
            }

            break;

        default:
            /* Invalid bit width - return error */
            status = ARM_MATH_ARGUMENT_ERROR;
            break;
    }

    if (status != ARM_MATH_SUCCESS)
    {
        wavFreqHz = 0.0f; /* If an error has occurred set frequency of wav data to 0Hz */
        PRINTF("\r\nFFT computation error.\r\n");
    }
    else
    {
        /* Check for potential overflow in multiplication */
        if (testIndex <= (FLT_MAX / hzPerBin))
        {
            /* Set wavFreqHz to bin location of max amplitude multiplied by the hz per bin */
            wavFreqHz = (float32_t)testIndex * hzPerBin;
        }
        else
        {
            wavFreqHz = 0.0f; /* Overflow would occur, set to 0 */
        }
    }

    return wavFreqHz;
}

void PlaybackSine(I2S_Type *base, uint32_t SineWaveFreqHz, uint32_t time_s)
{
    /* Validate input parameters */
    if ((base == NULL) || (SineWaveFreqHz == 0U) || (time_s == 0U))
    {
        return;
    }

    /* Check for potential overflow and underflow in division */
    if (SineWaveFreqHz > DEMO_AUDIO_SAMPLE_RATE)
    {
        return;
    }

    uint32_t count      = DEMO_AUDIO_SAMPLE_RATE / SineWaveFreqHz;
    uint32_t i          = 0U;
    int16_t sinVal      = 0;
    uint32_t val        = 0U;
    sai_transfer_t xfer = {0};
    float32_t freq      = 0.0f;
    uint32_t totalNum   = 0U;
    uint32_t index      = 0U;

    /* Validate count to prevent buffer overflow */
    if ((count == 0U) || (count > (BUFFER_SIZE / 4U)))
    {
        return;
    }

    /* Clear the status */
    istxFinished = false;
    sendCount    = 0U;
    emptyBlock   = BUFFER_NUM;

    /* Generate the sine wave data */
    for (i = 0U; i < count; i++)
    {
        /* Check for potential overflow in multiplication before division */
        if (i <= (UINT32_MAX / 0x8000U))
        {
            uint32_t angle = (0x8000U * i) / count;
            if (angle <= (uint32_t)INT16_MAX)
            {
                sinVal = arm_sin_q15((q15_t)angle);
                if (sinVal >= 0)
                {
                    val = (uint32_t)sinVal;
                }
                else
                {
                    /* Convert negative int16_t to unsigned without signed overflow */
                    val = (uint32_t)((uint16_t)sinVal);
                }
            }
            else
            {
                val = 0U;
            }
        }
        else
        {
            val = 0U;
        }

        /* Check bounds before array access */
        if (((4U * i) + 3U) < (BUFFER_SIZE * BUFFER_NUM))
        {
            audioBuff[4U * i]        = (uint8_t)(val & 0xFFU);
            audioBuff[(4U * i) + 1U] = (uint8_t)((val >> 8U) & 0xFFU);
            audioBuff[(4U * i) + 2U] = (uint8_t)(val & 0xFFU);
            audioBuff[(4U * i) + 3U] = (uint8_t)((val >> 8U) & 0xFFU);
        }
        else
        {
            break; /* Prevent buffer overflow */
        }
    }

    /* Calculate maximum safe iterations to prevent buffer overflow */
    uint32_t maxIterations = 0U;
    if ((4U * count) != 0U)
    {
        maxIterations = (BUFFER_SIZE * BUFFER_NUM) / (4U * count);
    }

    /* Repeat the cycle */
    for (i = 1U; i < maxIterations; i++)
    {
        /* Check for potential overflow in multiplication */
        if ((i <= (SIZE_MAX / (4U * count))) && (4U <= (SIZE_MAX / count)) && (count <= (SIZE_MAX / 4U)))
        {
            size_t destOffset = i * 4U * count;
            size_t copySize   = 4U * count;

            /* Ensure we don't exceed buffer bounds */
            if ((destOffset < (BUFFER_SIZE * BUFFER_NUM)) && ((destOffset + copySize) <= (BUFFER_SIZE * BUFFER_NUM)))
            {
                (void)memcpy(&audioBuff[destOffset], audioBuff, copySize);
            }
            else
            {
                break;
            }
        }
        else
        {
            break; /* Prevent overflow */
        }
    }

    /* Send times according to the time need to playback */
    /* Check for potential overflow in multiple steps */
    if ((DEMO_AUDIO_SAMPLE_RATE <= (UINT32_MAX / time_s)) && (time_s <= (UINT32_MAX / DEMO_AUDIO_SAMPLE_RATE)))
    {
        uint32_t temp = DEMO_AUDIO_SAMPLE_RATE * time_s;
        if ((temp <= (UINT32_MAX / 4U)) && (BUFFER_SIZE != 0U))
        {
            beginCount = (temp * 4U) / BUFFER_SIZE;
        }
        else
        {
            beginCount = 0U;
            return;
        }
    }
    else
    {
        beginCount = 0U;
        return;
    }

    /* Compute the frequency of the data using FFT */
    freq = do_fft(DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH, audioBuff, ffData, ffResult);

    PRINTF("\r\n Data frequency is %f\r\n", freq);

    /* Reset SAI Tx internal logic */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);

    /* Do the playback */
    xfer.data     = audioBuff;
    xfer.dataSize = BUFFER_SIZE;

    while (totalNum < beginCount)
    {
        /* Transfer data already prepared, so while there is any empty slot, just transfer */
        if (emptyBlock > 0U)
        {
            /* Check bounds before calculating buffer offset */
            if ((index < BUFFER_NUM) && ((index * BUFFER_SIZE) < (BUFFER_SIZE * BUFFER_NUM)))
            {
                xfer.data = &audioBuff[index * BUFFER_SIZE];

                /* Shall make sure the sai buffer queue is not full */
                if (SAI_TransferSendEDMA(base, &txHandle, &xfer) == kStatus_Success)
                {
                    index = (index + 1U) % BUFFER_NUM;
                    totalNum++;
                    emptyBlock--;
                }
            }
            else
            {
                break; /* Prevent buffer overflow */
            }
        }
    }

    /* Wait for the send finished */
    while (istxFinished == false)
    {
        /* Empty loop body - waiting for completion */
    }
}
