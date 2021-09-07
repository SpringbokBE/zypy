/**
 * Implementation of many unary and binary operations that resemble NumPy's
 * ufuncs (universal functions). Floating point (f4) implementation.
 */

////////////////////////////////////////////////////////////////////////////////

#include <hls_math.h>

#include "ufuncs_f4.h"

////////////////////////////////////////////////////////////////////////////////

// Add arguments element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.add.html

float2 add(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = in.first + in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Multiply arguments element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.multiply.html

float2 multiply(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = in.first * in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the element-wise square of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.square.html

float2 square(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = in.first * in.first;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the exponentiation of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.exp.html

float2 exp(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::expf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the natural logarithm of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.log.html

float2 log(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::logf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the absolute difference between the input arguments.

float2 absdiff(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::absf(in.first - in.second);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Subtract element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.subtract.html

float2 subtract(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = in.first - in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// True division of the inputs, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.true_divide.html

float2 true_divide(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::divide(in.first, in.second);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Trigonometric sine, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sin.html

float2 sin(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::sinf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Non-negative square-root, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sqrt.html

float2 sqrt(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::sqrtf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Trigonometric inverse tangent, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.arctan.html

float2 arctan(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::atanf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Hyperbolic sine, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sinh.html

float2 sinh(float2 in)
{
	#pragma HLS inline

	float2 out;

	out.first = hls::sinhf(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////
