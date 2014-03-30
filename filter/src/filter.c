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

//global variables and pointers (could be converted to static if ahir permits and if its better to do so)
long LPy1, LPy2, HPy1;
int LPbuff_ptr, HPbuff_ptr, DERIVbuff_ptr, WINbuff_ptr;
long WINsum;


//initialize all buffers and pointers
void initFilt()
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
	for (index = 0; index < DERIVbuff_size; index++)
	{
		__loop_pipelining_on__(7,2,0);
		DERIVbuff[index]=0;
	} 
	LPy1 = 0;
	LPy2 = 0;
	HPy1 = 0;
	WINsum = 0;
	LPbuff_ptr = 0;
	HPbuff_ptr = 0;
	DERIVbuff_ptr = 0;
	WINbuff_ptr = 0;
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
	if (halfPtr < 0)
		halfPtr += LPbuff_size;
	LPy0 = (LPy1 << 1) - LPy2 + datum - (LPbuff[halfPtr] << 1) + LPbuff[ptr];		
	LPy2 = LPy1;
	LPy1 = LPy0;
	output = LPy0 / (LPbuff_halfSize*LPbuff_halfSize); // what is the reason for this 
							// particular scaling coefficient
	LPbuff[ptr] = datum;
	LPbuff_ptr = circUpdateFilt(ptr, LPbuff_size);
//	if (ptr == LP_maxptr)
//		ptr = 0;
//	else
//		ptr = ptr + 1;
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
	if (halfPtr < 0)
		halfPtr += HPbuff_size;

	HPy0 = HPy1 + datum - HPbuff[ptr];
	HPy1 = HPy0;
	output = HPbuff[halfPtr] - (HPy0/HPbuff_size);	

	HPbuff[ptr] = datum;
	HPbuff_ptr = circUpdateFilt(ptr, HPbuff_size);
//	if (ptr == HP_maxptr)
//		ptr = 0;
//	else
//		ptr = ptr + 1;
	HPbuff_ptr = ptr;
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
	DERIVbuff[ptr] = datum;
	DERIVbuff_ptr = circUpdateFilt(ptr, DERIVbuff_size);
//	if (ptr == DERIV_maxptr)
//		ptr = 0;
//	else
//		ptr = ptr + 1;
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
	if (WINsum > WINsum_saturation)
		output = WINout_saturation;
	else					// why this saturation value
		output = WINsum/WINbuff_size; 	// also, doesn't saturate sum. Sum is stored as it is without clipping but output is clipped

	WINbuff[ptr] = datum;
	WINbuff_ptr = circUpdateFilt(ptr, WINbuff_size);
//	if (ptr == WIN_maxptr)
//		ptr = 0;
//	else
//		ptr = ptr + 1;
	write_uint32("filt_output_pipe", output);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Parent Function
// Will call all above defined functions in correct sequence
//////////////////////////////////////////////////////////////////////////////////////////////
void QRSFilt(uint8_t initialize)
{
//	initfilt();
//	while(1){
//		int beatSample = read_uint32("input_pipe");
//		write_uint32("sample_pipe", beatSample); //both can be clubbed by reading input pipe directly in the lpfilt()
		
		if (initialize)
			initFilt();
		else
		{	
			lpFilt();	
			hpFilt();
			deriv();
			mvWin();
		}
//		int output = read_uint32("WINout_pipe");
//		write_uint32("output_pipe", output); // both can be clubbed by writing output pipe directly in mvwin()
//	}
}




