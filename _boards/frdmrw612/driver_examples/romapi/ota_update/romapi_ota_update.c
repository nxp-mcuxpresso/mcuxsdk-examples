/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_romapi_iap.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * @brief Helper to assert function return status.
 *
 * @details Print finished message and trap forever upon failure.
 */
#define APP_ASSERT(expected, actual, ...) \
    do                                    \
    {                                     \
        if (expected != actual)           \
        {                                 \
            PRINTF("Example failed: ");   \
            PRINTF(__VA_ARGS__);          \
            while (1)                     \
                ;                         \
        }                                 \
    } while (0);

#define APP_ASSERT_TRUE(condition, ...) \
    do                                  \
    {                                   \
        if (!(condition))               \
        {                               \
            PRINTF("Example failed: "); \
            PRINTF(__VA_ARGS__);        \
            while (1)                   \
                ;                       \
        }                               \
    } while (0);

#define TAG_SB_V3 (0x33766273) // "sbv3"  sb3.1 maigc number

#define MIN_IMAGE_OFFSET_BYTES (500u * 1024u)
#define OTA_IMAGE_OFFSET_BYTES (256u * 1024u)

#define IMAGE_VERSION_MASK           (0x0000FFFFu)
#define NON_PROGRAMMED_IMAGE_VERSION (0xFFFFu)

#define OTP_FLEXSPI_IMAGE_OFFSET_FUSE_IDX   (17u)
#define OTP_FLEXSPI_IMAGE_OFFSET_FUSE_SHIFT (11u)
#define OTP_FLEXSPI_IMAGE_OFFSET_FUSE_MASK  (0x1FF800u)
#define OTP_FLEXSPI_IMAGE_OFFSET_FUSE_WIDTH (10u)
#define OTP_FLEXSPI_IMAGE_OFFSET_FUSE_VALUE()                                                       \
    ((OCOTP->OTP_SHADOW[OTP_FLEXSPI_IMAGE_OFFSET_FUSE_IDX] & OTP_FLEXSPI_IMAGE_OFFSET_FUSE_MASK) >> \
     OTP_FLEXSPI_IMAGE_OFFSET_FUSE_SHIFT)

#define OTP_RKTH_383_352_FUSE_IDX     (104u)
#define OTP_RKTH_383_352_FUSE_VALUE() (OCOTP->OTP_SHADOW[OTP_RKTH_383_352_FUSE_IDX])

#define OTP_CUST_SK_MK_31_0_FUSE_IDX     (92u)
#define OTP_CUST_SK_MK_31_0_FUSE_VALUE() (OCOTP->OTP_SHADOW[OTP_CUST_SK_MK_31_0_FUSE_IDX])

enum _ospi_constants
{
    kOspiMem_BaseAddr               = 0x08000000,
    kOspiMem_ConfigBlockOffset      = 0x400,
    kOspiMem_ConfigBlockSize        = 0x200,
    kOspiMem_BootImageVersionOffset = 0x600,
    kOspiMem_ImageStartOffset       = 0x1000,
};

typedef struct _image_header
{
    uint32_t reserved0[8];       //!< [0x00-0x1f]
    uint32_t image_length;       //!< [0x20-0x23] image length
    uint32_t image_type;         //!< [0x24-0x27] image type
    uint32_t auth_block_offset;  //!< [0x28-0x2b] authenticate block offset/crc checksum
    uint32_t reserved1[2];       //!< [0x2c-0x33] reserved
    uint32_t image_load_address; //!< [0x34-0x37] image load address
    uint32_t reserved2[2];       //!< [0x38-0x3f]
} image_header_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t iap_api_context_buffer[0x6000];

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t get_remap_offset()
{
    uint32_t offset = FLEXSPI->HADDROFFSET;
    return offset;
}

static uint32_t get_image_offset_from_otp()
{
    uint32_t image_offset_fuse_value = OTP_FLEXSPI_IMAGE_OFFSET_FUSE_VALUE();
    uint32_t image_offset            = image_offset_fuse_value * 256 * 1024;
    return image_offset;
}

static uint32_t read_image_version(uint32_t addr)
{
    uint32_t full_version = *((uint32_t *)addr);
    uint32_t version      = full_version & IMAGE_VERSION_MASK;
    return version;
}

static uint32_t increment_image_version(uint32_t version)
{
    uint32_t incremented_version = 0;
    if (version == NON_PROGRAMMED_IMAGE_VERSION)
    {
        incremented_version = 1;
    }
    else
    {
        incremented_version = version + 1;
    }
    return incremented_version;
}

static uint32_t encode_image_version(uint32_t version)
{
    return version | (((~version) & IMAGE_VERSION_MASK) << 16);
}

static void write_incremented_image_version(api_core_context_t *api_core_context)
{
    status_t status = kStatus_Fail;

    uint32_t image_offset_otp   = get_image_offset_from_otp();
    uint32_t remap_offset       = get_remap_offset();
    uint32_t image_index        = remap_offset == 0u ? 0u : 1u;
    uint32_t base_address       = kOspiMem_BaseAddr + (image_index == 0 ? 0 : image_offset_otp);
    uint32_t other_base_address = kOspiMem_BaseAddr + (image_index == 0 ? image_offset_otp : 0);

    uint32_t fcb_address = base_address + kOspiMem_ConfigBlockOffset;

    uint32_t image_version                     = read_image_version(base_address + kOspiMem_BootImageVersionOffset);
    uint32_t incremented_image_version         = increment_image_version(image_version);
    uint32_t encoded_incremented_image_version = encode_image_version(incremented_image_version);

    PRINTF("INFO: Programming incremented image version.\r\n");

    status = iap_mem_config(api_core_context, (uint32_t *)fcb_address, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_config returned with code [0x%X]\r\n", status);

    status = iap_mem_write(api_core_context, other_base_address + kOspiMem_BootImageVersionOffset, sizeof(uint32_t),
                           (uint8_t *)&encoded_incremented_image_version, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_write returned with code [0x%X]\r\n", status);

    status = iap_mem_flush(api_core_context);
    APP_ASSERT(kStatus_Success, status, "iap_mem_flush returned with code [0x%X]\r\n", status);
}

static void ota_update_plain(api_core_context_t *api_core_context, uint32_t ota_image_address, uint32_t ota_image_size)
{
    status_t status = kStatus_Fail;

    uint32_t image_offset_otp   = get_image_offset_from_otp();
    uint32_t remap_offset       = get_remap_offset();
    uint32_t image_index        = remap_offset == 0u ? 0u : 1u;
    uint32_t base_address       = kOspiMem_BaseAddr + (image_index == 0 ? 0 : image_offset_otp);
    uint32_t other_base_address = kOspiMem_BaseAddr + (image_index == 0 ? image_offset_otp : 0);

    uint32_t fcb_address = base_address + kOspiMem_ConfigBlockOffset;

    PRINTF("INFO: Programming plain OTA image to address 0x%x\r\n", other_base_address);

    status = iap_mem_config(api_core_context, (uint32_t *)fcb_address, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_config returned with code [0x%X]\r\n", status);

    status = iap_mem_erase(api_core_context, other_base_address, kOspiMem_ImageStartOffset + ota_image_size,
                           kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_erase returned with code [0x%X]\r\n", status);

    status = iap_mem_write(api_core_context, other_base_address + kOspiMem_ImageStartOffset, ota_image_size,
                           (uint8_t *)ota_image_address, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_write returned with code [0x%X]\r\n", status);

    status = iap_mem_write(api_core_context, other_base_address + kOspiMem_ConfigBlockOffset, kOspiMem_ConfigBlockSize,
                           (uint8_t *)fcb_address, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_write returned with code [0x%X]\r\n", status);

    status = iap_mem_flush(api_core_context);
    APP_ASSERT(kStatus_Success, status, "iap_mem_flush returned with code [0x%X]\r\n", status);
}

static void ota_update_sb3(api_core_context_t *api_core_context, uint32_t ota_image_addr, size_t ota_image_size)
{
    status_t status = kStatus_Fail;
    APP_ASSERT_TRUE(OTP_RKTH_383_352_FUSE_VALUE() != 0, "For processing SB3, RKTH has to be provisioned.\r\n");
    APP_ASSERT_TRUE(OTP_CUST_SK_MK_31_0_FUSE_VALUE() != 0, "For processing SB3, CUST_SK_MK has to be provisioned\r\n");

    PRINTF("INFO: Starting updating from SB3 file at location 0x%08x, size: 0x%08x\r\n", ota_image_addr,
           ota_image_size);

    status = iap_sbloader_init(api_core_context);
    APP_ASSERT(kStatus_Success, status, "iap_sbloader_init returned with code [0x%X]\r\n", status);

    // In this example the whole image is immediately available and it would be possible to call
    // the sbloader_pump once, giving it the whole image. However, for illustration purpose, the image
    // is processed in chunks (simulating reception over interface).
    uint32_t remaining = ota_image_size;
    const uint8_t *pos = (const uint8_t *)ota_image_addr;
    while (remaining > 0)
    {
        uint32_t rx_block_size = MAX(remaining, 12 * 1024);

        status = iap_sbloader_pump(api_core_context, pos, rx_block_size);
        APP_ASSERT_TRUE(status == kStatusRomLdrDataUnderrun || status == kStatus_Success,
                        "iap_sbloader_pump returned with code [0x%X]\r\n", status);
        pos += rx_block_size;
        remaining -= rx_block_size;
    }

    // This would be the alternative of passing in the whole image at once.
    // status = iap_sbloader_pump(api_core_context, (uint8_t*) ota_image_addr, ota_image_size);
    // APP_ASSERT(kStatus_Success, status, "iap_sbloader_pump returned with code [0x%X]\r\n", status);

    status = iap_sbloader_finalize(api_core_context);
    APP_ASSERT(kStatus_Success, status, "iap_sbloader_finalize returned with code [0x%X]\r\n", status);
}

static uint32_t ota_update(api_core_context_t *api_core_context, uint32_t ota_image_address)
{
    if (*((uint32_t *)ota_image_address) == 0xFFFFFFFFu)
    {
        PRINTF("ERROR: No OTA image was found at location 0x%08x (memory is erased).\r\n", ota_image_address);
        PRINTF("       Please download an image and reset the board.\r\n");
        return 0;
    }

    const nboot_sb3_header_t *sb_header = (nboot_sb3_header_t *)ota_image_address;
    const image_header_t *header        = (image_header_t *)ota_image_address;
    uint32_t image_size                 = 0;

    if (sb_header->magic == TAG_SB_V3)
    {
        image_size = sb_header->imageTotalLength + sb_header->blockCount * sb_header->blockSize;
        PRINTF("INFO: Found sb3 image with size 0x%08x\r\n", image_size);
        ota_update_sb3(api_core_context, ota_image_address, image_size);
    }
    else
    {
        image_size = header->image_length;
        PRINTF("INFO: Found masterboot image with size 0x%08x\r\n", image_size);
        ota_update_plain(api_core_context, ota_image_address, image_size);
    }
    return image_size;
}

static void erase_ota_image(api_core_context_t *api_core_context, uint32_t ota_image_address, uint32_t ota_image_size)
{
    status_t status = kStatus_Fail;

    uint32_t image_offset_otp = get_image_offset_from_otp();
    uint32_t remap_offset     = get_remap_offset();
    uint32_t image_index      = remap_offset == 0u ? 0u : 1u;
    uint32_t base_address     = kOspiMem_BaseAddr + (image_index == 0 ? 0 : image_offset_otp);

    uint32_t fcb_address = base_address + kOspiMem_ConfigBlockOffset;

    PRINTF("INFO: Erasing OTA image\r\n");

    status = iap_mem_config(api_core_context, (uint32_t *)fcb_address, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_config returned with code [0x%X]\r\n", status);

    status = iap_mem_erase(api_core_context, ota_image_address, ota_image_size, kMemoryID_FlexspiNor);
    APP_ASSERT(kStatus_Success, status, "iap_mem_erase returned with code [0x%X]\r\n", status);
}

int main()
{
    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("\r\n");
    PRINTF("INFO: OTA update example\r\n");

    uint32_t image_offset_otp = get_image_offset_from_otp();
    if (image_offset_otp >= MIN_IMAGE_OFFSET_BYTES)
    {
        status_t status = kStatus_Fail;

        uint32_t remap_offset  = get_remap_offset();
        uint32_t image_index   = remap_offset == 0u ? 0u : 1u;
        uint32_t base_address  = kOspiMem_BaseAddr + (image_index == 0 ? 0 : image_offset_otp);
        uint32_t image_version = read_image_version(base_address + kOspiMem_BootImageVersionOffset);

        uint32_t ota_image_address = base_address + OTA_IMAGE_OFFSET_BYTES;

        PRINTF("\r\n");
        PRINTF("\r\n");
        PRINTF("INFO: Running from image %d (remapped to 0x%08x).\r\n", image_index, remap_offset);
        PRINTF("INFO: Image version: %d.\r\n", image_version);
        PRINTF("\r\n");
        PRINTF("The source of the OTA update is typically from a network. In order to \r\n");
        PRINTF("keep the example simpler and decouple from the dependency on a network \r\n");
        PRINTF("connection, it takes the update image from a Flash location.\r\n");
        PRINTF("An update image is expected at location 0x%08x.\r\n", ota_image_address);
        PRINTF("Please download an image (e.g. using JLINK) to that location before \r\n");
        PRINTF("running this example.\r\n");
        PRINTF("\r\n");

        api_core_context_t apiCoreCtx;
        const kp_api_init_param_t apiInitParam = {.allocStart = (uint32_t)&iap_api_context_buffer[0],
                                                  .allocSize  = sizeof(iap_api_context_buffer)};

        status = iap_api_init(&apiCoreCtx, &apiInitParam);
        APP_ASSERT(kStatus_Success, status, "iap_api_init returned with code [0x%X]\r\n", status);

        uint32_t image_size = ota_update(&apiCoreCtx, ota_image_address);
        if (image_size > 0)
        {
            write_incremented_image_version(&apiCoreCtx);
            erase_ota_image(&apiCoreCtx, ota_image_address, image_size);
            PRINTF("INFO: Image was updated, rebooting...\r\n");
            PRINTF("---------------------------------------------\r\n");

            NVIC_SystemReset();
        }
    }
    else
    {
        PRINTF(
            "INFO: Skipping ota example: Dual image boot of bootloader has to be enabled and the image offset has to "
            "be at least 0x%08x for this example to work.",
            MIN_IMAGE_OFFSET_BYTES);
    }

    while (1)
    {
    }
}
