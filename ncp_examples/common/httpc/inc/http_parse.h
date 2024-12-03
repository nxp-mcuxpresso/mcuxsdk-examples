/*
 * Copyright (c) 2004, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * \Adapted: C Michael Sundius, cozybit Inc.
 *     took the http handling code from this server and integrated it
 *     into cozybit's simple http server.  This module now only handles an http
 *     request contained in the input buffer and returns a reply buffer.
 */

/*
 *  Copyright 2008-2020 NXP
 */

/*! \file http_parse.h
 *  \brief Common HTTP functions
 *
 */
#ifndef __HTTP_PARSE_H__
#define __HTTP_PARSE_H__

#include <httpd.h>

/** Parse tag/value form elements present in HTTP POST body
 *
 * Given a tag this function will retrieve its value from the buffer and return
 * it to the caller.
 * \param[in] inbuf Pointer to NULL-terminated buffer that holds POST data
 * \param[in] tag The tag to look for
 * \param[out] val Buffer where the value will be copied to
 * \param[in] val_len The length of the val buffer
 *
 *
 * \return WM_SUCCESS when a valid tag is found, error otherwise
 */
int httpd_get_tag_from_post_data(char *inbuf, const char *tag, char *val, unsigned val_len);

/** Parse tag/value form elements present in HTTP GET URL
 *
 * Given a tag this function will retrieve its value from the HTTP URL and
 * return it to the caller.
 * \param[in] req_p pointer to \ref httpd_request_t object
 * \param[in] tag The tag to look for
 * \param[out] val Buffer where the value will be copied to
 * \param[in] val_len The length of the val buffer
 *
 * \return WM_SUCCESS when a valid tag is found, error otherwise
 */
int httpd_get_tag_from_url(httpd_request_t *req_p, const char *tag, char *val, unsigned val_len);

int htsys_getln_soc(int sd, char *data_p, int buflen);

#endif /* __HTTP_PARSE_H__ */
