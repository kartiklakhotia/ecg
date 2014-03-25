#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pipes.h>
#include "filter.h"
#include "detect.h"

//declare loop pipelining as a NOP function for SW execution
#ifdef SW
void __loop_pipelining_on__(int A, int B, int C) {}
#else
void __loop_pipelining_on__(int, int, int);
#endif

//global variables and pointers
double threshold = 0.3125;
int DDbuff_ptr, DDCALCbuff_ptr;
int qpk_count, maxder, lastmax, count, sbpeak;
int initBlank, initMax, preBlankCnt, sbCnt;
int maxPeak, timeSinceMaxPeak; //variables for peak function
int lastDatum;

void initDet()
{
	int index;
	for (index=0; index<8; index++)
	{
	__loop_pipelining_on__(7,2,0);
		NOISEbuff[i] = 0;
		RRbuff[i] = MS1000; 	
	}
	for (index = 0; index < DERIVbuff_size; index++)
	{
		__loop_pipelining_on__(7,2,0);
		DDCALCbuff[index]=0;
	} 
	qpk_count = maxder = lastmax = count = sbpeak = 0;
	initBlank = initMax = preBlankCnt = DDbuff_ptr = DDCALCbuff_ptr = 0;
	sbCnt = MS1500;
	maxPeak = timeSinceMaxPeak = 0;
	lastDatum = 0;
//	initfilt();
}

void QRSdet()
{
	initDet();
	QRSfilt(1); //initiate filter buffers, pointers and variables also to reset state;
	int aPeak, newPeak, tempPeak;
	while(1)
	{
		int prefilt_datum = read_uint32("input_pipe");  //clubbed in the lpfilt() function of Filter part
		write_uint32("filt_input_pipe", prefilt_datum);	
		QRSfilt(0);

		aPeak = peak();	
		int preBlank_count = preBlankCnt;

		uint8_t prelim_cond0 = (aPeak > 0);
		uint8_t prelim_cond1 = (preBlank_count > 0); 
		uint8_t prelim_cond2 = (aPeak > tempPeak);
		uint8_t prelim_cond3 = (preBlank_count == 1);

		uint8_t peakDet_cond0 = prelim_cond0 && (!prelim_cond1);
		uint8_t peakDet_cond1 = (!prelim_cond0) && prelim_cond1;
		uint8_t peakDet_cond2 = prelim_cond0 && prelim_cond3;

		tempPeak = (peakDet_cond0 || prelim_cond2) ? aPeak : tempPeak;
		preBlank_count = (peakDet_cond1 || peakDet_cond2) ? preBlank_count-1 : preBlank_count;
		preBlank_count = (peakDet_cond0 || prelim_cond2) ? PRE_BLANK : preBlank_count;
		newPeak = ((peakDet_cond1 && prelim_cond3) || (peakDet_cond2 && !prelim_cond2)) ? tempPeak : newPeak;
		
		preBlankCnt = preBlank_count;
		
		DDbuff[DDbuff_ptr] = ddcalc(prefilt_datum);
		//...//
	}
}

int peak()
{
	int pk = 0;
	int timeSinceMax = timeSinceMaxPeak;
	int max = maxPeak;
	int datum = read_uint32("filt_output_pipe");

	timeSinceMax = (timeSinceMax > 0) ? (timeSinceMax+1) : timeSinceMax;	
	uint8_t newPeakCondition = ((datum > lastDatum) && (datum > max));
	uint8_t peakConfirmCondition = ((!newPeakCondition) && ((datum < (max >> 1)) || (timeSinceMax > MS95)));
	
	max = (newPeakCondition) ? datum : max;
	timeSinceMax = (newPeakCondition && (max > 2)) ? 1 : timeSinceMax;
	pk = (peakConfirmCondition) ? max : pk;
	max = (peakConfirmCondition) ? 0 : max;
	timeSinceMax = (peakConfirmCondition) ? 0 : timeSinceMax;
	pk = (aPeak < MIN_PEAK_AMP) ? 0 : pk; 
	lastDatum = datum;
	return (pk);
}



/**************************************************
** DERIVATIVE CALCULATOR FOR RAW DATA
**************************************************/
int ddcalc(int datum)
{
	int ptr = DDCALCbuff_ptr;

	int output = datum - DDCALCbuff[ptr];
	DDCALCbuff[ptr] = datum;
	if (ptr == DDCALC_maxptr)
		ptr = 0;
	else
		ptr = ptr + 1;
	DDCALCbuff_ptr = ptr;
	return output;
}
