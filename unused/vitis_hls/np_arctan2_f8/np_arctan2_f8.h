/**
 *  @file    np_arctan2_f8.h
 *  @author  Gerbrand De Laender
 *  @date    07/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise arc tangent of x1/x2 choosing the quadrant correctly.
 *  Double precision (f8) implementation.
 *
 */

#pragma once

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define TYPE_DOUBLE
typedef double np_t;
typedef unsigned int len_t;
typedef ap_axiu<8 * sizeof(np_t), 1, 1, 1> channel_t;
typedef hls::stream<channel_t> stream_t;

void np_arctan2_f8_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len);
void np_arctan2_f8_sw(np_t *x1, np_t *x2, np_t *out, len_t len);
