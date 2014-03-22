#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pipes.h>
#include "filter.h"

//declare loop pipelining as a NOP function for SW execution
#ifdef SW
void __loop_pipelining_on__(int A, int B, int C) {}
#else
void __loop_pipelining_on__(int, int, int);
#endif

int beatSample;
int LPy1, LPy2, LPbuff_ptr;
int HPy1, HPbuff_ptr;


//initialize to a reset state
void init()
{
	int index;
	for (index = 0; index < LPbuff_size; index++)
	{
		__loop_pipelining_on__(7,2,0);
		LPbuff[index]=0;
	} 
	for (index = 0; index < HPbuff_size; index++)
	{
		__loop_pipelining_on__(7,2,0);
		HPbuff[index]=0;
	} 
	LPy1 = 0;
	LPy2 = 0;
	HPy1 = 0;
	LPbuff_ptr = 0;
	HPbuff_ptr = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Digital Low-pass Filter
// Difference equation is as follows :
/*
 	y[n] = 2*y[n-1] - y[n-2] + x[n] - 2*x[n-24ms(LPbuff_size/2)] + x[n-48ms(LPbuff_size)]
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void lpfilt()
{
	long LPy0; //check if long is required, what about normalization
		//also, the further Hermite coeffs calculator is working on double precision FP data
	int output, halfPtr; //halfPtr represents the sample that occurred 24ms ago

	int datum = dataSample; //avoid reading global variables at too many points
	int ptr = LPbuff_ptr;

	halfPtr = ptr-LPbuff_halfSize;
	if (halfPtr < 0)
		halfPtr += LPbuff_size;
	LPy0 = (LPy1 << 1) - LPy2 + datum - (LPbuff[halfPtr] << 1) + LPbuff[ptr];		
	LPy2 = LPy1;
	LPy1 = LPy0;
	output = LPy0 / (LPbuff_halfSize*LPbuff_halfSize); // what is the reason for this 
							// particular scaling coefficient
	LPbuff[ptr] = datum;
	if (ptr == LP_maxptr)
		ptr = 0;
	else
		ptr = ptr + 1;
	LPbuff_ptr = ptr;		
	write_uint32("LPout_pipe", output); // send this output for high pass filtering	
					   // no need to pass argument to and from main function
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Digital High-pass Filter
// Difference equation is as follows :
/*
 	y[n] = y[n-1] + x[n] - x[n-128ms(HPbuff_size)]
	z[n] = x[n-64ms(HPbuff_halfSize)] - y[n]
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void hpfilt()
{
	long HPy0;
	int output, halfPtr;
	
	int datum = read_uint32("LPout_pipe");
	int ptr = HPbuff_ptr;

	halfPtr = ptr-HPbuff_halfSize;
	if (halfPtr < 0)
		halfPtr += HPbuff_size;

	HPy0 = HPy1 + datum - HPbuff[ptr];
	HPy1 = HPy0;
	output = HPbuff[halfPtr] - HPy0;	

	HPbuff[ptr] = datum;
	if (ptr == HP_maxptr)
		ptr = 0;
	else
		ptr = ptr + 1;
	HPbuff_ptr = ptr;
	write_uint32("HPout_pipe", output);
}		
