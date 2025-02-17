/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lc3_codec.h"

#include <string.h>

static int lc3_resolve_bitrate(int dt_us, int nbytes)
{
    if( !( (dt_us == 10000) || (dt_us == 7500) ) )
    {
        return LC3_CODEC_ERR;
    }

    if( (nbytes < 20) || (400 < nbytes))
    {
        return LC3_CODEC_ERR;
    }

    int bitrate = ((unsigned)nbytes * (1000*1000*8) + dt_us/2) / dt_us;

    return bitrate;
}

int lc3_encoder_init(lc3_encoder_t *encoder, int sample_rate, int duration_us, int target_bytes, int sample_bits)
{
	WORD32 handle_size;
	WORD32 scratch_size;
	XA_ERRORCODE err;
	WORD16 frame_dms;

    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

	encoder->cfg.sampleRate = sample_rate;
	encoder->cfg.nChannels = 1;
	encoder->cfg.bipsIn = sample_bits;
	encoder->cfg.bitrate = lc3_resolve_bitrate(duration_us, target_bytes);

	memset(encoder->handle_buff, 0, sizeof(encoder->handle_buff));
	memset(encoder->scratch_buff, 0, sizeof(encoder->scratch_buff));

	handle_size = xa_lc3_enc_get_handle_byte_size(&encoder->cfg);

	if(handle_size > sizeof(encoder->handle_buff))
	{
		return LC3_CODEC_ERR;
	}

	encoder->handle = (xa_codec_handle_t)encoder->handle_buff;

	scratch_size = xa_lc3_enc_get_scratch_byte_size(&encoder->cfg);

	if(scratch_size > sizeof(encoder->scratch_buff))
	{
		return LC3_CODEC_ERR;
	}

	encoder->scratch_size = scratch_size;

	err = xa_lc3_enc_init(encoder->handle, encoder->scratch_buff, &encoder->cfg);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

	frame_dms = duration_us / 100;
	err = xa_lc3_enc_set_param(encoder->handle, XA_LC3_ENC_PARAM_FRAME_DMS, &frame_dms);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

	WORD32 bitrate = encoder->cfg.bitrate;
	err = xa_lc3_enc_set_param(encoder->handle, XA_LC3_ENC_PARAM_BITRATE, &bitrate);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

    encoder->sample_rate = sample_rate;
    encoder->duration_us = duration_us;
    encoder->enc_bytes   = target_bytes;
    encoder->sample_bits = sample_bits;

    return 0;
}

int lc3_encoder(lc3_encoder_t *encoder, void *input, uint8_t *output)
{
	XA_ERRORCODE err;
	UWORD32 in_samples = LC3_SAMPLES_PER_FRAME(encoder);
	UWORD32 out_bytes = LC3_ENC_BYTES_PER_FRAME(encoder);

    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

    memcpy(encoder->audio_buff, input, LC3_SAMPLES_BYTES_PER_FRAME(encoder));

	err = xa_lc3_enc_process(encoder->handle, encoder->audio_buff, encoder->sdu_buff, &in_samples, &out_bytes, encoder->scratch_size);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

	memcpy(output, encoder->sdu_buff, LC3_ENC_BYTES_PER_FRAME(encoder));

    return 0;
}

int lc3_encoder_deinit(lc3_encoder_t *encoder)
{
    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

    memset(encoder, 0, sizeof(lc3_encoder_t));

    return 0;
}

int lc3_decoder_init(lc3_decoder_t *decoder, int sample_rate, int duration_us, int input_bytes, int sample_bits)
{
	WORD32 handle_size;
	WORD32 scratch_size;
	XA_ERRORCODE err;
	WORD16 frame_dms;

    if(!decoder)
    {
        return  LC3_CODEC_ERR;;
    }

	memset(decoder->handle_buff, 0, sizeof(decoder->handle_buff));
	memset(decoder->scratch_buff, 0, sizeof(decoder->scratch_buff));

	decoder->cfg.sampleRate = sample_rate;
	decoder->cfg.nChannels = 1;
	decoder->cfg.bipsOut = sample_bits;

	handle_size = xa_lc3_dec_get_handle_byte_size(&decoder->cfg);

	if(handle_size > sizeof(decoder->handle_buff))
	{
		return LC3_CODEC_ERR;
	}

	decoder->handle = (xa_codec_handle_t)decoder->handle_buff;

	scratch_size = xa_lc3_dec_get_scratch_byte_size(&decoder->cfg);

	if(scratch_size > sizeof(decoder->scratch_buff))
	{
		return LC3_CODEC_ERR;
	}

	decoder->scratch_size = scratch_size;

	err = xa_lc3_dec_init(decoder->handle, decoder->scratch_buff, &decoder->cfg);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

	frame_dms = duration_us / 100;
	err = xa_lc3_dec_set_param(decoder->handle, XA_LC3_DEC_PARAM_FRAME_DMS, &frame_dms);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

    decoder->sample_rate = sample_rate;
    decoder->duration_us = duration_us;
    decoder->enc_bytes   = input_bytes;
    decoder->sample_bits = sample_bits;

    return 0;
}

int lc3_decoder(lc3_decoder_t *decoder, uint8_t *input, int frame_flag, void *output)
{
	XA_ERRORCODE err;
	UWORD32 in_bytes = LC3_ENC_BYTES_PER_FRAME(decoder);
	UWORD32 out_samples = 0;
	WORD32 bfi_ext = frame_flag;

    if(!decoder)
    {
        return LC3_CODEC_ERR;
    }

	memcpy(decoder->sdu_buff, input, LC3_ENC_BYTES_PER_FRAME(decoder));

	err = xa_lc3_dec_process(decoder->handle, decoder->sdu_buff, decoder->audio_buff, &in_bytes, &out_samples, decoder->scratch_size, bfi_ext);
	if(err)
	{
		return LC3_CODEC_ERR;
	}

    memcpy(output, decoder->audio_buff, LC3_SAMPLES_BYTES_PER_FRAME(decoder));

    return 0;
}

int lc3_decoder_deinit(lc3_decoder_t *decoder)
{
    if(!decoder)
    {
        return LC3_CODEC_ERR;
    }

    memset(decoder, 0, sizeof(lc3_decoder_t));

    return 0;
}
