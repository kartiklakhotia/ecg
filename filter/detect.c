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
int QRSbuff_ptr, RRbuff_ptr, NOISEbuff_ptr;
int qpk_count, maxDer, lastMax, count, sbPeak;
int initBlank, initMax, preBlank_count, sb_count;
int maxPeak, timeSinceMaxPeak; //variables for peak function
int lastDatum;
int DDCALCbuff[DERIVbuff_size];
uint8_t init8Done;

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
	qpk_count = maxDer = lastMax = count = sbPeak = 0;
	initMax = preBlankCnt = DDbuff_ptr = DDCALCbuff_ptr = QRSbuff_ptr = RRbuff_ptr = NOISEbuff_ptr = 0;
	initBlank = 1;
	sb_count = MS1500 + MS150;
	maxPeak = timeSinceMaxPeak = 0;
	lastDatum = 0;
	init8Done = 0;
//	initfilt();
}

void QRSdet()
{
	initDet();
	QRSfilt(1); //initiate filter buffers, pointers and variables also to reset state;
	int aPeak, newPeak, tempPeak;
	int qmean, rrmean, nmean;
	int det_thresh;
	while(1)
	{
		int prefilt_datum = read_uint32("input_pipe");  //clubbed in the lpfilt() function of Filter part
		write_uint32("filt_input_pipe", prefilt_datum);	
		QRSfilt(0);


	///////////////// PEAK DETECTION AND VERIFICATION OF POINT OF OCCURRENCE ////////////
		aPeak = peak();	

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
		
		
		DDbuff[DDbuff_ptr] = ddCalc(prefilt_datum);
		DDbuff_ptr = circUpdateDet(DDbuff_ptr, DDbuff_size);
		
			
	//////////// QRS DETECTION ///////////////
	
	/* Initializing buffers with first 8 beats */
		if (!init8Done)
		{
			count = (newPeak > 0) ? WINbuff_size : count + 1;
			uint8_t onesec_cond = (initBlank == MS1000);
			initBlank = (onesec_cond) ? 1 : initBlank + 1;
			qrsbuf[qpkcnt] = initMax;
			initMax = (onesec_cond) ? 0 : initMax;
			qpk_count = (onesec_cond) ? qpk_count + 1 : qpk_count;
			init8Done = (onesec_cond && (qpk_count == 8));			
		//	qmean = (init8Done) ? meancalc(QRSbuff) : qmean;
			nmean = 0;
			rrmean = rrmean;
			if (init8Done){
				qmean = meanCalc(QRSbuff);
				det_thresh = threshCalc(qmean, nmean);
			}
		} 

		else
		{
			count = count + 1;
			uint8_t cond_QRSdetect = ((newPeak > det_thresh) && !(	
		//...//
	
		}
		
	}
}


/**************************************************
** PEAK DETECTOR (DOESN'T CONFIRM QRS)
**************************************************/
int peak()
{
	int pk;
	int timeSinceMax = timeSinceMaxPeak;
	int max = maxPeak;
	int datum = read_uint32("filt_output_pipe");

	timeSinceMax = (timeSinceMax > 0) ? (timeSinceMax+1) : timeSinceMax;	
	uint8_t newPeak_cond = ((datum > lastDatum) && (datum > max));
	uint8_t peakConfirm_cond = ((datum < (max >> 1)) || (timeSinceMax > MS95));
	
	pk = (peakConfirm_cond) ? max : pk;
	max = (peakConfirm_cond) ? 0 : max;
	timeSinceMax = (peakConfirm_cond) ? 0 : timeSinceMax;
	max = (newPeak_cond) ? datum : max;
	timeSinceMax = (newPeak_cond && (max > 2)) ? 1 : timeSinceMax;
	pk = ((pk < MIN_PEAK_AMP) || newPeak_cond)  ? 0 : pk; 

	timeSinceMaxPeak = timeSinceMax;
	maxPeak = max;
	lastDatum = datum;
	return (pk);
}



/**************************************************
** DERIVATIVE CALCULATOR FOR RAW DATA
**************************************************/
int ddCalc(int datum)
{
	int ptr = DDCALCbuff_ptr;

	int output = datum - DDCALCbuff[ptr];
	DDCALCbuff[ptr] = datum;
	DDCALCbuff_ptr = circupdatefilt(ptr, DERIVbuff_size);
	return output;
}


/**************************************************
** POINTER UPDATING FOR CIRCULAR BUFFERS
**************************************************/
int circUpdateDet(int ptr, int size)
{
	int nextval = ptr + 1;
	nextval = (nextval == size) ? 0 : nextval;
	return nextval;
}


/*************************************************
** MEAN CALCULATOR FOR QRS, RR AND NOISE BUFFERS
*************************************************/
int meanCalc(int *buffer)
{
	int sum = 0;
	for (int i=0; i<4; i++)
	{
		__loop_pipelining_on__(4,1,0);
		sum += (buffer[i] + buffer[i+1]);	
	}
	return (sum >> 3);
}


/*************************************************
** CALCULATE THRESHOLD DYNAMICALLY
*************************************************/
int threshCalc(int qmean, int nmean)
{
	double temp = (qmean - nmean);
	int threshval = nmean + (int)(threshold*temp);
	return threshval;
}
			 
/*************************************************
** BASELINE SHIFT CHECK
** CHECK +ve & -ve SLOPES IN 220ms WINDOW
*************************************************/
uint8_t blsCheck()
{
	int max, min, maxt, mint, t, x;
	max = 0;
	min = 0;
	int ptr = DDbuff_ptr;
	for (t=0; t<MS220; t++)
	{
	__loop_pipelining_on__(7,2,0);
		x = DDbuff[ptr];
		uint8_t max_cond = (x > max);
		uint8_t min_cond = (x < min);
		maxt = (max_cond) ? t : maxt;
		max = (max_cond) ? x : max;
		mint = (min_cond) ? t : mint;
		min = (min_cond) ? x : min;
		ptr = ptr + 1; //no need for circular implementation since buffer is much larger than 220ms.
	}

		maxDer = max;
		min = -min;


		uint8_t output = ((max > (min >> 3)) && (min > (max >> 3)) && (abs(maxt-mint) < MS150))	? 0 : 1;
		return output;
}
