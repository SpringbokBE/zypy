/**
 * Implementation of many unary and binary operations that resemble NumPy's
 * ufuncs (universal functions). Integer (i4) implementation.
 */

////////////////////////////////////////////////////////////////////////////////

#include <hls_math.h>

#include "ufuncs_i4.h"

////////////////////////////////////////////////////////////////////////////////

// Add arguments element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.add.html

int2 add(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = in.first + in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Multiply arguments element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.multiply.html

int2 multiply(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = in.first * in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the element-wise square of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.square.html

int2 square(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = in.first * in.first;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the exponentiation of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.exp.html

int2 exp(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::exp(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the natural logarithm of the input. See
// https://numpy.org/doc/stable/reference/generated/numpy.log.html

int2 log(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::log(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Return the absolute difference between the input arguments.

int2 absdiff(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::abs(in.first - in.second);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Subtract element-wise. See
// https://numpy.org/doc/stable/reference/generated/numpy.subtract.html

int2 subtract(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = in.first - in.second;

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// True division of the inputs, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.true_divide.html

int2 true_divide(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::divide(in.first, in.second);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Trigonometric sine, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sin.html

int2 sin(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::sin(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Non-negative square-root, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sqrt.html

int2 sqrt(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::sqrt(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Trigonometric inverse tangent, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.arctan.html

int2 arctan(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::atan(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

// Hyperbolic sine, element-wise.
// https://numpy.org/doc/stable/reference/generated/numpy.sinh.html

int2 sinh(int2 in)
{
	#pragma HLS inline

	int2 out;

	out.first = hls::sinh(in.first);

	return out;
}

////////////////////////////////////////////////////////////////////////////////
