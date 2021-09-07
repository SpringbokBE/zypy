/**
 *  @file    np_sin_f4.h
 *  @author  Gerbrand De Laender
 *  @date    04/05/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Single precision (f4) implementation.
 *
 */

#pragma once

#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef unsigned int len_t;

typedef ap_axiu<8 * sizeof(float), 1, 1, 1> channel_t;
typedef ap_axiu<2 * 8 * sizeof(float), 1, 1, 1> channel2_t;
typedef ap_axiu<4 * 8 * sizeof(float), 1, 1, 1> channel4_t;
typedef ap_axiu<8 * 8 * sizeof(float), 1, 1, 1> channel8_t;

typedef hls::stream<channel_t> stream_t;
typedef hls::stream<channel2_t> stream2_t;
typedef hls::stream<channel4_t> stream4_t;
typedef hls::stream<channel8_t> stream8_t;

void np_sin_f4_hw(stream_t &ins, stream_t &outs);
void np_sin_2f4_hw(stream2_t &ins, stream2_t &outs);
void np_sin_4f4_hw(stream4_t &ins, stream4_t &outs);
void np_sin_8f4_hw(stream8_t &ins, stream8_t &outs);

void np_sin_f4_sw(float *ina, float *outa, len_t len);
