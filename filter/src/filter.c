#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "divide.h"
#include "filter.h"

//declare loop pipelining as a NOP function for SW execution
#ifdef SW
void __loop_pipelining_on__(int A, int B, int C) {}
#else
void __loop_pipelining_on__(int, int, int);
#endif

//global variables and pointers (could be converted to static if ahir permits and if its better to do so)
long LPy1, LPy2, HPy1;
int LPbuff_ptr, HPbuff_ptr, DERIVbuff_ptr, WINbuff_ptr;
long WINsum;


//global memory spaces
int LPbuff[LPbuff_size];
int HPbuff[HPbuff_size];
int DERIVbuff[DERIVbuff_size];
int WINbuff[WINbuff_size];


//initialize all buffers and pointers
void initFilt()
{
	int index;
	for (index = 0; index < LPbuff_size; index++)
	{
		LPbuff[index]=0;
	} 
	for (index = 0; index < HPbuff_size; index++)
	{
		HPbuff[index]=0;
	} 
	for (index = 0; index < DERIVbuff_size; index++)
	{
		DERIVbuff[index]=0;
	} 
	for (index = 0; index < WINbuff_size; index++)
	{
		WINbuff[index] = 0;
	}
	for (index = 0; index < FILTERbuff_size; index++)
	{
		FILTERbuff[index] = 0;
	}
	LPy1 = LPy2 = HPy1 = 0;
	WINsum = 0;
	LPbuff_ptr = HPbuff_ptr = DERIVbuff_ptr = WINbuff_ptr = FILTERbuff_ptr = 0;
}


/**************************************************
** POINTER UPDATING FOR CIRCULAR BUFFERS
**************************************************/
int circUpdateFilt(int ptr, int size)
{
	int nextval = ptr + 1;
	nextval = (nextval == size) ? 0 : nextval;
	return nextval;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Digital Low-pass Filter
// Difference equation is given by :
/*
 	y[n] = 2*y[n-1] - y[n-2] + x[n] - 2*x[n-24ms(LPbuff_size/2)] + x[n-48ms(LPbuff_size)]
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void lpFilt()
{
	long LPy0; //check if long is required, what about normalization
		//also, the further Hermite coeffs calculator is working on double precision FP data
	int output, halfPtr; //halfPtr represents the sample that occurred 24ms ago

	int datum = read_uint32("filt_input_pipe"); //main input pipe to filter.
	int ptr = LPbuff_ptr; //avoid reading global pointer value at multiple places

	halfPtr = ptr-LPbuff_halfSize;
	halfPtr = (halfPtr < 0) ? halfPtr + LPbuff_size : halfPtr;
	LPy0 = (LPy1 << 1) - LPy2 + datum - (LPbuff[halfPtr] << 1) + LPbuff[ptr];		
	LPy2 = LPy1;
	LPy1 = LPy0;
	output = divideSigned (LPy0, LPbuff_halfSize * LPbuff_halfSize);
	LPbuff[ptr] = datum;
	LPbuff_ptr = circUpdateFilt(ptr, LPbuff_size);
	write_uint32("LPout_pipe", output); // send this output for high pass filtering	
					   // no need to pass argument to and from main function
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Digital High-pass Filter
// Difference equation is given by :
/*
 	y[n] = y[n-1] + x[n] - x[n-128ms(HPbuff_size)]
	z[n] = x[n-64ms(HPbuff_halfSize)] - y[n]
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void hpFilt()
{
	long HPy0;
	int output, halfPtr;
	
	int datum = read_uint32("LPout_pipe");
	int ptr = HPbuff_ptr;

	halfPtr = ptr-HPbuff_halfSize;
	halfPtr = (halfPtr < 0) ? halfPtr + HPbuff_size : halfPtr;

	HPy0 = HPy1 + datum - HPbuff[ptr];
	HPy1 = HPy0;
	output = HPbuff[halfPtr] - divideSigned(HPy0, HPbuff_size);

	HPbuff[ptr] = datum;
	HPbuff_ptr = circUpdateFilt(ptr, HPbuff_size);
	write_uint32("HPout_pipe", output);
}		



//////////////////////////////////////////////////////////////////////////////////////////////
// Digital derivative approximation
// Difference equations is given by :
/*
	y[n] = x[n] - x[n-10ms(DERIVbuff_size)]
*/
//////////////////////////////////////////////////////////////////////////////////////////////
void deriv()
{
	int datum = read_uint32("HPout_pipe");
	int ptr = DERIVbuff_ptr;

	int output = abs(datum - DERIVbuff[ptr]);
	DERIVbuff[ptr] = FILTERbuff[FILTERbuff_ptr] = datum;
	DERIVbuff_ptr = circUpdateFilt(ptr, DERIVbuff_size);
	FILTERbuff_ptr = circUpdateFilt(FILTERbuff_ptr, FILTERbuff_size);
	write_uint32("DERIVout_pipe", output);
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Moving window integrator
// Returns average of all samples over window width
/* Sum is calculated as given in following difference equation :
	sum[n] = sum[n-1] + x[n] - x[n-80ms(WINbuff_size)] 
*/
//////////////////////////////////////////////////////////////////////////////////////////////
void mvWin()
{
	int datum = read_uint32("DERIVout_pipe");
	int ptr = WINbuff_ptr; 
	int output;

	WINsum = WINsum + datum - WINbuff[ptr];	
	output = divideSigned(WINsum, WINbuff_size); 	
	output = (output > WINout_saturation) ? WINout_saturation : output;

	WINbuff[ptr] = datum;
	WINbuff_ptr = circUpdateFilt(ptr, WINbuff_size);
	write_uint32("filt_output_pipe", output);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Parent Function
// Will call all above defined functions in correct sequence
//////////////////////////////////////////////////////////////////////////////////////////////
void QRSFilt()
{
		
	lpFilt();	
	hpFilt();
	deriv();
	mvWin();
}




