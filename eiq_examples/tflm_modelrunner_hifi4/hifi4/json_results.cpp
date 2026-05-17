/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>

#include "fsl_debug_console.h"
#include "modelrunner.h"
#include "stdio.h"
#ifdef MODELRUNNER_HTTP
#include <lwip/sockets.h>
#endif

static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode_block(const uint8_t in[3], char out[4], int len) {
  out[0] = base64_table[in[0] >> 2];
  out[1] = base64_table[((in[0] & 0x03) << 4) | (len > 1 ? (in[1] >> 4) : 0)];
  out[2] =
      (len > 1)
          ? base64_table[((in[1] & 0x0F) << 2) | (len > 2 ? (in[2] >> 6) : 0)]
          : '=';
  out[3] = (len > 2) ? base64_table[in[2] & 0x3F] : '=';
}

int write_chunk(int sock, const char* data, size_t len) {
#ifdef MODELRUNNER_HTTP
  char header[16];
  int header_len = snprintf(header, sizeof(header), "%X\r\n", len);

  if (write(sock, header, header_len) < 0) return -1;
  if (write(sock, data, len) < 0) return -1;
  if (write(sock, "\r\n", 2) < 0) return -1;
#else
  PRINTF(data);
#endif
  return 0;
}

int base64_encode_chunks(int sock, uint8_t* data, size_t data_len) {
  char encoded[30];
  const size_t chunk_size = 18;
  size_t offset = 0;

  while (offset < data_len) {
    size_t current_chunk_size =
        (data_len - offset) < chunk_size ? (data_len - offset) : chunk_size;

    int out_pos = 0;
    for (size_t i = 0; i < current_chunk_size; i += 3) {
      size_t remaining = current_chunk_size - i;
      size_t block_len = (remaining < 3) ? remaining : 3;
      base64_encode_block(data + offset + i, encoded + out_pos, (int)block_len);
      out_pos += 4;
    }
    encoded[out_pos] = '\0';
    if (write_chunk(sock, encoded, strlen(encoded)) < 0) return -1;
    offset += current_chunk_size;
  }
  return 0;
}

int inference_results(int sock, NNServer* server, int outputs_idx[],
                      int n_outputs) {
  char data[1500];
  size_t data_len = 0;

#ifdef MODELRUNNER_HTTP
  data_len = snprintf(data, sizeof(data),
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: application/json\r\n"
                      "Transfer-Encoding: chunked\r\n"
                      "\r\n");
  if (write(sock, data, data_len) < 0) return -1;
#endif

  data_len = snprintf(data, sizeof(data), "{");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  if (n_outputs > 0) {
    data_len = snprintf(data, sizeof(data), "\"outputs\":[");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
    for (int i = 0; i < n_outputs; i++) {
      if (i != 0) {
        data_len = snprintf(data, sizeof(data), ",");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
      }
      if (server->output.data[outputs_idx[i]]) {
        data_len = snprintf(data, sizeof(data), "{");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;

        data_len = snprintf(data, sizeof(data), "\"name\":\"%s\",",
                            server->output.name[outputs_idx[i]]);
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;

        data_len = snprintf(data, sizeof(data), "\"datatype\":\"%s\",",
                            server->output.data_type[outputs_idx[i]]);
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;

        data_len = snprintf(data, sizeof(data), "\"shape\":[");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
        for (int dim = 0; dim < server->output.shape_size[outputs_idx[i]];
             ++dim) {
          if (dim != 0) {
            data_len = snprintf(data, sizeof(data), ",");
            if (data_len < 0 || data_len >= sizeof(data)) return -1;
            if (write_chunk(sock, data, data_len) < 0) return -1;
          }
          data_len = snprintf(data, sizeof(data), "%d",
                              server->output.shape_data[outputs_idx[i]][dim]);
          if (data_len < 0 || data_len >= sizeof(data)) return -1;
          if (write_chunk(sock, data, data_len) < 0) return -1;
        }
        data_len = snprintf(data, sizeof(data), "],");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;

        data_len = snprintf(data, sizeof(data), "\"data\":\"");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;

        size_t data_size = server->output.bytes[outputs_idx[i]];
        uint8_t* data_output = (uint8_t*)server->output.data[outputs_idx[i]];

        base64_encode_chunks(sock, data_output, data_size);
        data_len = snprintf(data, sizeof(data), "\"}");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
      }
    }
    data_len = snprintf(data, sizeof(data), "],");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
  }

  data_len = snprintf(data, sizeof(data), "\"timing\":%lld}", server->run_ns);
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

#ifdef MODELRUNNER_HTTP
  const char* end_chunk = "0\r\n\r\n";
  write(sock, end_chunk, strlen(end_chunk));
#endif

  return 0;
}

int model_info(int sock, NNServer* server) {
  char data[1500];
  size_t data_len = 0;
#ifdef MODELRUNNER_HTTP
  data_len = snprintf(data, sizeof(data),
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: application/json\r\n"
                      "Transfer-Encoding: chunked\r\n"
                      "\r\n");
  if (write(sock, data, data_len) < 0) return -1;
#endif

  data_len = snprintf(data, sizeof(data), "{");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), "\"timing\": %lld", server->run_ns);
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len =
      snprintf(data, sizeof(data), ",\"model_size\": %d", server->model_size);
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), ",\"ktensor_arena_size\": %d",
                      server->kTensorArenaSize);
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), ",\"inputs\": [");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  for (int i = 0; i < server->input.inputs_size; i++) {
    if (i != 0) {
      data_len = snprintf(data, sizeof(data), ",");
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }
    data_len = snprintf(data, sizeof(data), "{");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len =
        snprintf(data, sizeof(data), "\"name\": \"%s\"", server->input.name[i]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"scale\": %f",
                        (double)server->input.scale[i]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"zero_points\": %d",
                        server->input.zero_point[i]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"data_type\": \"%s\"",
                        server->input.data_type[i]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"shape\": [");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    for (int dim = 0; dim < server->input.shape_size[i]; ++dim) {
      if (dim != 0) {
        data_len = snprintf(data, sizeof(data), ",");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
      }
      data_len =
          snprintf(data, sizeof(data), "%d", server->input.shape_data[i][dim]);
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }

    data_len = snprintf(data, sizeof(data), "]}");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
  }

  data_len = snprintf(data, sizeof(data), "]");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), ",\"outputs\": [");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  for (int i = 0; i < server->output.outputs_size; i++) {
    if (i != 0) {
      data_len = snprintf(data, sizeof(data), ",");
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }
    data_len = snprintf(data, sizeof(data), "{");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    int idx = server->output.index[i];

    data_len = snprintf(data, sizeof(data), "\"name\": \"%s\"",
                        server->output.name[idx]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"scale\": %f",
                        (double)server->output.scale[idx]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"zero_points\": %d",
                        server->output.zero_point[idx]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"data_type\": \"%s\"",
                        server->output.data_type[idx]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"shape\": [");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    for (int dim = 0; dim < server->output.shape_size[idx]; ++dim) {
      if (dim != 0) {
        data_len = snprintf(data, sizeof(data), ",");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
      }
      data_len = snprintf(data, sizeof(data), "%d",
                          server->output.shape_data[idx][dim]);
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }
    data_len = snprintf(data, sizeof(data), "]}");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
  }
  data_len = snprintf(data, sizeof(data), "]");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), ",\"layer_count\": %d",
                      server->output.num_outputs);
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  data_len = snprintf(data, sizeof(data), ",\"layers\": [");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

  for (int i = 0; i < server->layers.num_layers; i++) {
    if (i != 0) {
      data_len = snprintf(data, sizeof(data), ",");
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }
    data_len = snprintf(data, sizeof(data), "{");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), "\"name\": \"%s\"",
                        server->output.name[server->layers.output_idx[i][0]]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"type\": \"%s\"",
                        server->layers.type[i]);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"timing\": %lld",
                        server->layers.timing[i] / server->inference_count);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    const char* process_unit =
        (strcmp(server->layers.type[i], "NeutronGraph") == 0) ? "NPU" : "CPU";
    data_len =
        snprintf(data, sizeof(data), ",\"process_unit\": \"%s\"", process_unit);
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;

    data_len = snprintf(data, sizeof(data), ",\"shape\": [");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
    for (int dim = 0; dim < server->layers.shape_size[i]; ++dim) {
      if (dim != 0) {
        data_len = snprintf(data, sizeof(data), ",");
        if (data_len < 0 || data_len >= sizeof(data)) return -1;
        if (write_chunk(sock, data, data_len) < 0) return -1;
      }
      data_len =
          snprintf(data, sizeof(data), "%d", server->layers.shape_data[i][dim]);
      if (data_len < 0 || data_len >= sizeof(data)) return -1;
      if (write_chunk(sock, data, data_len) < 0) return -1;
    }
    data_len = snprintf(data, sizeof(data), "]");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
    data_len = snprintf(data, sizeof(data), "}");
    if (data_len < 0 || data_len >= sizeof(data)) return -1;
    if (write_chunk(sock, data, data_len) < 0) return -1;
  }
  data_len = snprintf(data, sizeof(data), "]}");
  if (data_len < 0 || data_len >= sizeof(data)) return -1;
  if (write_chunk(sock, data, data_len) < 0) return -1;

#ifdef MODELRUNNER_HTTP
  const char* end_chunk = "0\r\n\r\n";
  write(sock, end_chunk, strlen(end_chunk));
#endif
  return 0;
}
