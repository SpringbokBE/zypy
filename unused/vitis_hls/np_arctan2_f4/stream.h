/**
 *  @file    stream.h
 *  @author  Gerbrand De Laender
 *  @date    24/01/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  AXI Stream helper functions. Template parameters are:
 *  T = Data type excluding AXI4-Stream side channels
 *  T_SIDE = Data type including AXI4-Stream side channels
 *
 */

#pragma once

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "np_arctan2_f4.h"

// Pops an item from the stream. Side channel information is discarded.
template <typename T, typename T_SIDE>
T pop_stream(const T_SIDE &val)
{
	#pragma HLS inline

	union {
		#if defined(TYPE_DOUBLE)
			unsigned long long int input;
		#else
			unsigned int input;
		#endif
		T output;
	} converter;

	converter.input = val.data;
	T ret = converter.output;

	return converter.output;
	return val.data;
}

// Pushes an item from the stream.
template <typename T, typename T_SIDE>
T_SIDE push_stream(const T &in, bool last = false)
{
	#pragma HLS inline

	T_SIDE out;

	union {
		T input;
		#if defined(TYPE_DOUBLE)
			unsigned long long int output;
		#else
			unsigned int output;
		#endif
	} converter;

	converter.input = in;

	out.data = converter.output;
	out.keep = 0xf;  // A one for each byte of data
	out.strb = 0xf;  // A one for each byte of data
	out.user = 0;
	out.last = last ? 1 : 0;
	out.id = 0;
	out.dest = 0;

	return out;
}
