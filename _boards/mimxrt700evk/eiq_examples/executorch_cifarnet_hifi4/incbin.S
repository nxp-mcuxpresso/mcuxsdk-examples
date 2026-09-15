/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSP_IMAGE_COPY_TO_RAM
#define DSP_IMAGE_COPY_TO_RAM 1
#endif

#if DSP_IMAGE_COPY_TO_RAM

#if defined(__GNUC__) || defined(__ARMCC_VERSION)

        .section .dspcode , "ax" @progbits @preinit_array
        .global dsp_literal_image_start
        .type dsp_literal_image_start, %object
        .align 2
dsp_literal_image_start:
#ifdef DEBUG
        .incbin "dsp_literal_debug.bin"
#else
        .incbin "dsp_literal_release.bin"
#endif
        .global dsp_literal_image_end
        .type dsp_literal_image_end, %object
dsp_literal_image_end:
        .global dsp_text_image_start
        .type dsp_text_image_start, %object
        .align 2
dsp_text_image_start:
#ifdef DEBUG
        .incbin "dsp_text_debug.bin"
#else
        .incbin "dsp_text_release.bin"
#endif
        .global dsp_text_image_end
        .type dsp_text_image_end, %object
dsp_text_image_end:
        .global dsp_data_image_start
        .type dsp_data_image_start, %object
        .align 2
dsp_data_image_start:
#ifdef DEBUG
        .incbin "dsp_data_debug.bin"
#else
        .incbin "dsp_data_release.bin"
#endif
        .global dsp_data_image_end
        .type dsp_data_image_end, %object
dsp_data_image_end:
        .global dsp_literal_image_size
        .type dsp_literal_image_size, %object
        .align 2
dsp_literal_image_size:
        .int  dsp_literal_image_end - dsp_literal_image_start
        .global dsp_text_image_size
        .type dsp_text_image_size, %object
        .align 2
dsp_text_image_size:
        .int  dsp_text_image_end - dsp_text_image_start
        .global dsp_data_image_size
        .type dsp_data_image_size, %object
        .align 2
dsp_data_image_size:
        .int  dsp_data_image_end - dsp_data_image_start
        .end

#endif

#endif
