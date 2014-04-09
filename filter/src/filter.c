#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Pipes.h>
#include <pipeHandler.h>
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


//// DIVISION ALGORITHM /////
uint64_t divideUnsigned(uint64_t dividend, uint32_t divisor)
{
        uint64_t quotient = 0;
        if (divisor != 0)
        {
                while (dividend >= divisor)
                {
                        uint64_t curr_quotient = 1;
                        uint64_t shifted_divisor = divisor;
                        uint64_t reduced_dividend_by_2 = (dividend >> 1);
                        while (shifted_divisor < reduced_dividend_by_2)
                        {
                                        shifted_divisor = (shifted_divisor << 1);
                                        curr_quotient = (curr_quotient << 1);
                        }
                        quotient += curr_quotient;
                        dividend -= shifted_divisor;
                }
        }
        return (quotient);
}

int64_t divideSigned(int64_t dividend, int32_t divisor)
{
        int8_t sign = 1; 
        uint64_t udividend;
        uint32_t udivisor;
	dividend = (dividend < 0) ? (-dividend) : dividend;
	sign = (dividend < 0) ? (-sign) : sign;
	divisor = (divisor < 0) ? (-divisor) : divisor;
	sign = (divisor < 0) ? (-sign) : sign;
	udividend = (uint64_t) dividend;
	udivisor = (uint32_t) divisor;

        int64_t quotient = 0;
        if (udivisor != 0)
        {
                quotient = divideUnsigned(udividend, udivisor);
		quotient = (sign < 0) ? -quotient : quotient;
        }
        return (quotient);
}





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
//	if (halfPtr < 0)
//		halfPtr += LPbuff_size;
	halfPtr = (halfPtr < 0) ? halfPtr + LPbuff_size : halfPtr;
	LPy0 = (LPy1 << 1) - LPy2 + datum - (LPbuff[halfPtr] << 1) + LPbuff[ptr];		
	LPy2 = LPy1;
	LPy1 = LPy0;
	output = divideSigned (LPy0, LPbuff_halfSize * LPbuff_halfSize);
//	output = LPy0 / (LPbuff_halfSize*LPbuff_halfSize);  
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
	halfPtr = (halfPtr < 0) ? halfPtr + HPbuff_size : halfPtr;
//	if (halfPtr < 0)
//		halfPtr += HPbuff_size;

	HPy0 = HPy1 + datum - HPbuff[ptr];
	HPy1 = HPy0;
	output = HPbuff[halfPtr] - divideSigned(HPy0, HPbuff_size);
//	output = HPbuff[halfPtr] - (HPy0/HPbuff_size);	

	HPbuff[ptr] = datum;
	HPbuff_ptr = circUpdateFilt(ptr, HPbuff_size);
//	if (ptr == HP_maxptr)
//		ptr = 0;
//	else
//		ptr = ptr + 1;
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
		output = divideUnsigned(WINsum, WINbuff_size); 	// also, doesn't saturate sum. Sum is stored as it is without clipping but output is clipped
//	output = (WINsum > WINsum_saturation) ? WINout_saturation : WINsum/WINbuff_size;

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
//#ifdef SW
//			printf("first iteration done\n");
//#endif
		}
//		int output = read_uint32("WINout_pipe");
//		write_uint32("output_pipe", output); // both can be clubbed by writing output pipe directly in mvwin()
//	}
}




