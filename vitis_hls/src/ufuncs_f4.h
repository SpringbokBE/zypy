/**
 * Header file for ufuncs_f4.cc.
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

// Definition of all supported operations.

#define NONE 0
#define ADD 1
#define MULTIPLY 2
#define SQUARE 3
#define EXP 4
#define LOG 5
#define ABSDIFF 6
#define SUBTRACT 7
#define TRUE_DIVIDE 8
#define SIN 9
#define SQRT 10
#define ARCTAN 11
#define SINH 12

// Is the supported operation binary?

const static bool is_binary[] = {
	false, 	// NONE
	true, 	// ADD
	true, 	// MULTIPLY
	false, 	// SQUARE
	false, 	// EXP
	false, 	// LOG
	true, 	// ABSDIF
	true,	// SUBTRACT
	true,	// TRUE_DIVIDE
	false,	// SIN
	false,	// SQRT
	false,	// ARCTAN
	false	// SINH
};

// Does the supported operation return two values?

const static bool returns_pair[] = {
	false, 	// NONE
	false, 	// ADD
	false, 	// MULTIPLY
	false, 	// SQUARE
	false, 	// EXP
	false, 	// LOG
	false, 	// ABSDIF
	false,	// SUBTRACT
	false,	// TRUE_DIVIDE
	false,	// SIN
	false,	// SQRT
	false,	// ARCTAN
	false,	// SINH
	true	// Prevent optimizing away output ports
};

////////////////////////////////////////////////////////////////////////////////

// Custom type to hold two floats, used as input and output of the ufuncs.

typedef struct {
	float first;
	float second;
} float2;

////////////////////////////////////////////////////////////////////////////////

float2 add(float2 in);
float2 multiply(float2 in);
float2 square(float2 in);
float2 exp(float2 in);
float2 log(float2 in);
float2 absdiff(float2 in);
float2 subtract(float2 in);
float2 true_divide(float2 in);
float2 sin(float2 in);
float2 sqrt(float2 in);
float2 arctan(float2 in);
float2 sinh(float2 in);

////////////////////////////////////////////////////////////////////////////////
