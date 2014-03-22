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
int HPy1, HPy2, HPbuff_ptr;


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
	HPy2 = 0;
	LPbuff_ptr = 0;
	HPbuff_ptr = 0;
}


// Digital Low-pass Filter
// Difference equation is as follows :
/*
 	y[n] = 2*y[n-1] - y[n-2] + x[n] - 2*x[t-24ms(LPbuff_size/2)] + x[t-48ms(LPbuff_size)]
*/
void lpfilt()
{
	long y0; //check if long is required, what about normalization
		//also, the further Hermite coeffs calculator is working on double precision FP data
	int output, halfPtr; //halfPtr represents the sample that occurred 24ms ago
	int datum = dataSample; //avoid reading global variable at too many places
	halfPtr = ptr-LPbuff_halfSize;
	if (halfPtr < 0)
		halfPtr += LPbuff_size;
	y0 = (LPy1 << 1) - LPy2 + datum - (LPbuff[halfPtr] << 1) + LPbuff[ptr];		
	LPy2 = LPy1;
	LPy1 = y0;
	output = y0 / (LPbuff_halfSize*LPbuff_halfSize); // what is the reason for this 
							// particular scaling coefficient
	LPbuff[ptr] = datum;
	if (ptr == LP_maxptr)
		ptr = 0;
	else
		ptr = ptr + 1;
		
	write_uint32("LPout_pipe", output); // send this output for high pass filtering	
					   // no need to pass argument to and from main function
}



	
	 
