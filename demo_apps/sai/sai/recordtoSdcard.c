/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ff.h"
#include "diskio.h"
#include "fsl_sd.h"
#include "sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define WAV_HEADER_SIZE     44U
#define WAV_FMT_CHUNK_SIZE  16U
#define WAV_PCM_FORMAT      1U
#define WAV_BITS_PER_SAMPLE 16U
#define WAV_CHANNELS        2U
#define BYTES_PER_SAMPLE    2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint8_t channels, uint32_t fileSize);
/*******************************************************************************
 * Variables
 ******************************************************************************/
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
extern FIL g_fileObject;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void write_uint32_le(uint8_t *buffer, size_t offset, uint32_t value)
{
    if ((buffer == NULL) || (offset > (WAV_HEADER_SIZE - 4U)))
    {
        return;
    }
    buffer[offset]      = (uint8_t)(value & 0xFFU);
    buffer[offset + 1U] = (uint8_t)((value >> 8U) & 0xFFU);
    buffer[offset + 2U] = (uint8_t)((value >> 16U) & 0xFFU);
    buffer[offset + 3U] = (uint8_t)((value >> 24U) & 0xFFU);
}

static void write_uint16_le(uint8_t *buffer, size_t offset, uint16_t value)
{
    if ((buffer == NULL) || (offset > (WAV_HEADER_SIZE - 2U)))
    {
        return;
    }
    buffer[offset]      = (uint8_t)(value & 0xFFU);
    buffer[offset + 1U] = (uint8_t)((value >> 8U) & 0xFFU);
}

void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint8_t channels, uint32_t fileSize)
{
    if (header == NULL)
    {
        return;
    }

    /* Check for potential overflow */
    if (fileSize < WAV_HEADER_SIZE)
    {
        return;
    }

    uint32_t totalDataLen = fileSize - 8U;
    uint32_t audioDataLen = fileSize - WAV_HEADER_SIZE;

    /* Check for overflow in byteRate calculation */
    if ((sampleRate > 0U) && (bitsPerFrame > 0U) && (channels > 0U))
    {
        uint32_t bytesPerSample = bitsPerFrame / 8U;
        if (bytesPerSample == 0U)
        {
            return;
        }

        /* Check for multiplication overflow */
        if ((sampleRate > (UINT32_MAX / bytesPerSample)) || ((sampleRate * bytesPerSample) > (UINT32_MAX / channels)))
        {
            return;
        }

        uint32_t byteRate   = sampleRate * bytesPerSample * channels;
        uint16_t blockAlign = (uint16_t)(channels * bytesPerSample);

        /* RIFF header */
        header[0] = 'R';
        header[1] = 'I';
        header[2] = 'F';
        header[3] = 'F';
        write_uint32_le(header, 4U, totalDataLen);

        /* WAVE header */
        header[8]  = 'W';
        header[9]  = 'A';
        header[10] = 'V';
        header[11] = 'E';

        /* fmt chunk */
        header[12] = 'f';
        header[13] = 'm';
        header[14] = 't';
        header[15] = ' ';
        write_uint32_le(header, 16U, WAV_FMT_CHUNK_SIZE);
        write_uint16_le(header, 20U, WAV_PCM_FORMAT);
        write_uint16_le(header, 22U, channels);
        write_uint32_le(header, 24U, sampleRate);
        write_uint32_le(header, 28U, byteRate);
        write_uint16_le(header, 32U, blockAlign);
        write_uint16_le(header, 34U, (uint16_t)bitsPerFrame);

        /* data chunk */
        header[36] = 'd';
        header[37] = 'a';
        header[38] = 't';
        header[39] = 'a';
        write_uint32_le(header, 40U, audioDataLen);
    }
}

void RecordSDCard(I2S_Type *base, uint32_t time_s)
{
    if (base == NULL)
    {
        PRINTF("Invalid base pointer\r\n");
        return;
    }

    /* Check for potential overflow in file size calculation */
    if (time_s > (UINT32_MAX / DEMO_AUDIO_SAMPLE_RATE / BYTES_PER_SAMPLE / WAV_CHANNELS))
    {
        PRINTF("Recording time too large\r\n");
        return;
    }

    uint32_t i                      = 0U;
    uint32_t bytesWritten           = 0U;
    uint32_t bytesRead              = 0U;
    uint32_t txindex                = 0U;
    uint32_t rxindex                = 0U;
    uint32_t sdReadCount            = 0U;
    uint8_t header[WAV_HEADER_SIZE] = {0};

    /* Safe calculation with overflow check */
    uint32_t audioDataSize = time_s * DEMO_AUDIO_SAMPLE_RATE * BYTES_PER_SAMPLE * WAV_CHANNELS;
    uint32_t fileSize      = audioDataSize + WAV_HEADER_SIZE;

    /* Check for overflow */
    if (fileSize < audioDataSize)
    {
        PRINTF("File size calculation overflow\r\n");
        return;
    }

    FRESULT error;
    sai_transfer_t xfer                = {0};
    static const TCHAR wavpathBuffer[] = DEMO_RECORD_WAV_PATH;

    /* Clear the status */
    isrxFinished = false;
    receiveCount = 0U;
    istxFinished = false;
    sendCount    = 0U;
    sdcard       = true;

    PRINTF("\r\nBegin to record......\r\n");
    PRINTF("\r\nFile path is record/music1.wav\r\n");
    error = f_open(&g_fileObject, (char const *)wavpathBuffer, (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error != FR_OK)
    {
        if (error == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            return;
        }
    }

    /* Write the data into the sdcard */
    wav_header(header, DEMO_AUDIO_SAMPLE_RATE, WAV_BITS_PER_SAMPLE, WAV_CHANNELS, fileSize);

    /* Write the wav header */
    error = f_write(&g_fileObject, (void *)header, WAV_HEADER_SIZE, (UINT *)&bytesWritten);
    if ((error != FR_OK) || (bytesWritten != WAV_HEADER_SIZE))
    {
        PRINTF("Write file failed. \r\n");
        (void)f_close(&g_fileObject);
        return;
    }

    /* Reset SAI internal logic */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
    SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

    /* Start to record */
    if (BUFFER_SIZE > 0U)
    {
        beginCount = audioDataSize / BUFFER_SIZE;
    }
    else
    {
        PRINTF("Invalid buffer size\r\n");
        (void)f_close(&g_fileObject);
        return;
    }

    /* Start record first */
    (void)memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);
    xfer.dataSize = BUFFER_SIZE;
    for (i = 0U; i < BUFFER_NUM; i++)
    {
        xfer.data = &audioBuff[i * BUFFER_SIZE];
        if (SAI_TransferReceiveEDMA(base, &rxHandle, &xfer) != kStatus_Success)
        {
            PRINTF("Failed to start audio receive\r\n");
            (void)f_close(&g_fileObject);
            return;
        }
    }

    emptyBlock = 0U;
    while ((isrxFinished != true) || (fullBlock != 0U))
    {
        if (fullBlock > 0U)
        {
            error = f_write(&g_fileObject, &audioBuff[txindex * BUFFER_SIZE], BUFFER_SIZE, (UINT *)&bytesWritten);
            if ((error != FR_OK) || (bytesWritten != BUFFER_SIZE))
            {
                PRINTF("Write file failed. \r\n");
                (void)f_close(&g_fileObject);
                return;
            }

            txindex = (txindex + 1U) % BUFFER_NUM;
            fullBlock--;
            emptyBlock++;
        }

        if ((emptyBlock > 0U) && (isrxFinished == false))
        {
            xfer.data = &audioBuff[rxindex * BUFFER_SIZE];
            rxindex   = (rxindex + 1U) % BUFFER_NUM;
            if (SAI_TransferReceiveEDMA(base, &rxHandle, &xfer) != kStatus_Success)
            {
                PRINTF("Failed to continue audio receive\r\n");
                (void)f_close(&g_fileObject);
                return;
            }
            emptyBlock--;
        }
    }

    error = f_close(&g_fileObject);
    if (error != FR_OK)
    {
        PRINTF("Failed to close file after recording\r\n");
        return;
    }

    PRINTF("\r\nRecord is finished!\r\n");

    /* Playback the record file */
    PRINTF("\r\nPlayback the recorded file...");
    txindex    = 0U;
    rxindex    = 0U;
    emptyBlock = 0U;
    fullBlock  = 0U;
    (void)memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);

    error = f_open(&g_fileObject, (char const *)wavpathBuffer, FA_READ);
    if (error != FR_OK)
    {
        PRINTF("Failed to open file for playback\r\n");
        return;
    }

    error = f_lseek(&g_fileObject, WAV_HEADER_SIZE);
    if (error != FR_OK)
    {
        PRINTF("Set file pointer position failed. \r\n");
        (void)f_close(&g_fileObject);
        return;
    }

    for (i = 0U; i < BUFFER_NUM; i++)
    {
        error = f_read(&g_fileObject, (void *)(&audioBuff[i * BUFFER_SIZE]), BUFFER_SIZE, (UINT *)&bytesRead);
        if (error != FR_OK)
        {
            PRINTF("Read file failed. \r\n");
            (void)f_close(&g_fileObject);
            return;
        }

        if (bytesRead != BUFFER_SIZE)
        {
            /* End of file reached */
            break;
        }

        sdReadCount++;
        fullBlock++;
    }

    /* Wait for playback finished */
    while (istxFinished != true)
    {
        if ((emptyBlock > 0U) && (sdReadCount < beginCount))
        {
            error = f_read(&g_fileObject, (void *)(&audioBuff[rxindex * BUFFER_SIZE]), BUFFER_SIZE, (UINT *)&bytesRead);
            if (error != FR_OK)
            {
                PRINTF("Read file failed. \r\n");
                (void)f_close(&g_fileObject);
                return;
            }

            if (bytesRead != BUFFER_SIZE)
            {
                /* End of file reached */
                break;
            }

            rxindex = (rxindex + 1U) % BUFFER_NUM;
            emptyBlock--;
            fullBlock++;
            sdReadCount++;
        }

        if (fullBlock > 0U)
        {
            xfer.data = &audioBuff[txindex * BUFFER_SIZE];
            txindex   = (txindex + 1U) % BUFFER_NUM;
            if (SAI_TransferSendEDMA(base, &txHandle, &xfer) != kStatus_Success)
            {
                PRINTF("Failed to send audio data\r\n");
                (void)f_close(&g_fileObject);
                return;
            }
            fullBlock--;
        }
    }

    error = f_close(&g_fileObject);
    if (error != FR_OK)
    {
        PRINTF("Failed to close file after playback\r\n");
    }

    PRINTF("\r\nPlayback is finished!\r\n");
}
