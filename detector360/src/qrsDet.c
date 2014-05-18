#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "filter.h"
#include "qrsDet.h"
#include "ptrFunc.h"

//declare loop pipelining as a NOP function for SW execution
//#ifdef SW
//void __loop_pipelining_on__(int A, int B, int C) {}
//#else
//void __loop_pipelining_on__(int, int, int);
//#endif

//global variables and pointers
double threshold = 0.3125;
int DDbuff_ptr, DDCALCbuff_ptr;
int QRSbuff_ptr, RRbuff_ptr, NOISEbuff_ptr;
int maxDer;
int maxPeak, timeSinceMaxPeak; //variables for peak function
int lastDatum;

// global memory spaces //
int DDbuff[DDbuff_size];
int DDCALCbuff[DERIVbuff_size];
int NOISEbuff[8], RRbuff[8], QRSbuff[8];

void initDet()
{
	int index;
	for (index=0; index<8; index++)
	{
		NOISEbuff[index] = 0;
		RRbuff[index] = MS1000; 	
		QRSbuff[index] = 0;
	}
	for (index = 0; index < DERIVbuff_size; index++)
	{
		DDCALCbuff[index]=0;
	} 
	maxDer = 0;
	DDbuff_ptr = DDCALCbuff_ptr = 0;
	QRSbuff_ptr = RRbuff_ptr = NOISEbuff_ptr = 0;
	maxPeak = timeSinceMaxPeak = 0;
	lastDatum = 0;
}



/**************************************************
** PEAK DETECTOR (DOESN'T CONFIRM QRS)
**************************************************/
//int peak()
int peak()
{
	int pk;
	int timeSinceMax = timeSinceMaxPeak;
	int max = maxPeak;
	int datum = read_uint32("filt_output_pipe");
	
	timeSinceMax = (timeSinceMax > 0) ? (timeSinceMax+1) : timeSinceMax;	
	uint8_t newPeak_cond = ((datum > lastDatum) && (datum > max));
	uint8_t peakConfirm_cond = (((datum < (max >> 1)) || (timeSinceMax > MS95)) && (!newPeak_cond)) ;
	
	pk = (peakConfirm_cond && (max >= MIN_PEAK_AMP)) ? max : 0;
	max = (peakConfirm_cond) ? 0 : max;
	timeSinceMax = (peakConfirm_cond) ? 0 : timeSinceMax;
	max = (newPeak_cond) ? datum : max;
	timeSinceMax = (newPeak_cond && (max > 2)) ? 1 : timeSinceMax;

	timeSinceMaxPeak = timeSinceMax;
	maxPeak = max;
	lastDatum = datum;
	return pk;
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



/**************************************************
** POINTER UPDATING FOR REVERSE CIRCULAR BUFFERS
**************************************************/
int revUpdateDet(int ptr, int max)
{
	int nextval = (ptr == 0) ? max : ptr - 1;
	return nextval;
}



/**************************************************
** DERIVATIVE CALCULATOR FOR RAW DATA
**************************************************/
int ddCalc(int datum)
{
	int ptr = DDCALCbuff_ptr;

	int output = datum - DDCALCbuff[ptr];
	DDCALCbuff[ptr] = datum;
	DDCALCbuff_ptr = circUpdateDet(ptr, DERIVbuff_size);
	return output;
}




/*************************************************
** CALCULATE THRESHOLD DYNAMICALLY
*************************************************/
int threshCalc(int qmean, int nmean)
{
//	double temp = (qmean - nmean);
//	int threshval = nmean + (int)(threshold*temp);
	int temp = qmean - nmean;
	int threshval = nmean + (temp >> 4) + (temp >> 2);
	return threshval;
}
			 
/*************************************************
** BASELINE SHIFT CHECK
** CHECK +ve & -ve SLOPES IN 220ms WINDOW
*************************************************/
uint8_t blsCheck()
{
	int max, min, maxt, mint, t, x;
	max = min = 0;
	maxt = mint = 0;
	int ptr = DDbuff_ptr;
	for (t=0; t<MS220; ++t)
	{
		x = DDbuff[ptr];
		uint8_t max_cond = (x > max);
		uint8_t min_cond = (x < min);
		maxt = (max_cond) ? t : maxt;
		max = (max_cond) ? x : max;
		mint = (min_cond) ? t : mint;
		min = (min_cond) ? x : min;
		ptr = circUpdateDet(ptr, DDbuff_size); //don't push this value in DDbuff_ptr
	}

		maxDer = max;
		min = -min;
		

		uint8_t output = ((max > (min >> 3)) && (min > (max >> 3)) && (abs(maxt-mint) < MS150));
		return (!output);
}


/*************************************************
**UPDATE QRS & RR BUFFERS ON QRS PEAK DETECTION
*************************************************/
void qrsUpdate(int qrsVal, int rrVal)
{
	int ptr0 = QRSbuff_ptr;
	int ptr1 = RRbuff_ptr;
	QRSbuff[ptr0] = qrsVal;
	RRbuff[ptr1] = rrVal;
	QRSbuff_ptr = revUpdateDet(ptr0, 7);
	RRbuff_ptr = revUpdateDet(ptr1, 7);
}	


/*************************************************
**UPDATE NOISE BUFFER ON NOISE PEAK DETECTION
*************************************************/
void noiseUpdate(int noiseVal)
{
	int ptr2 = NOISEbuff_ptr;
	NOISEbuff[ptr2] = noiseVal;
	NOISEbuff_ptr = revUpdateDet(ptr2, 7);
}	




// PARENT FUNCTION //

void qrsDet()
{
	initDet();
	initFilt(); //initiate filter buffers, pointers and variables also to reset state;
	int aPeak, newPeak, tempPeak, sbPeak;
	int qpk_count, count, initBlank, preBlank_count, rset_count;
	int initMax, lastMax, det_thresh;
	int sb_count, sbLoc;
	initMax = det_thresh = lastMax = 0;
	aPeak = newPeak = tempPeak = sbPeak = 0;
	qpk_count = count = initBlank = preBlank_count = rset_count = 0;
	sb_count = sbLoc = MS1500;
	int qmean, rrmean, nmean;
	qmean = 0;
	nmean = 0;
	rrmean = MS1000; 
	int RSETbuff[8];
	int qrsVal, rrVal, noiseVal;
	qrsVal = rrVal = noiseVal = 0;
	uint8_t init8Done_next, onesec_cond, init8Done;
	init8Done = 0;
	uint8_t prelim_cond0, prelim_cond1, prelim_cond2, prelim_cond3;
	uint8_t peakDet_cond0, peakDet_cond1, peakDet_cond2;
	uint8_t bls_cond, QRSdet_cond0, QRSdet_cond1, QRSdet_final, NOISEdet_cond0, sbUpdate_cond;
	while(1)
	{
		int QRSdelay = 0;
		float data_in = (float)(read_float64("det_input_pipe"));  
//		printf("%f\n", data_in);
		int prefilt_datum = (int)data_in;
		write_float32("filt_input_pipe", data_in);	
		QRSFilt();
	///////////////// PEAK DETECTION AND VERIFICATION OF POINT OF OCCURRENCE ////////////
	//	int postfilt_datum = read_uint32("filt_output_pipe");

		aPeak = peak();	
		prelim_cond0 = (aPeak > 0);
		prelim_cond1 = (preBlank_count > 0); 
		prelim_cond2 = (aPeak > tempPeak);
		prelim_cond3 = (--preBlank_count == 0);

		peakDet_cond0 = prelim_cond0 && (!prelim_cond1);
		peakDet_cond1 = (!prelim_cond0) && prelim_cond1;
		peakDet_cond2 = prelim_cond0 && (!prelim_cond2) && prelim_cond3;

		tempPeak = (peakDet_cond0 || prelim_cond2) ? aPeak : tempPeak;
		preBlank_count = (peakDet_cond0 || prelim_cond2) ? PRE_BLANK : preBlank_count;
		newPeak = ((peakDet_cond1 && prelim_cond3) || (peakDet_cond2)) ? tempPeak : 0;
		
		
		DDbuff[DDbuff_ptr] = ddCalc(prefilt_datum);
		DDbuff_ptr = circUpdateDet(DDbuff_ptr, DDbuff_size);
		
			
	//////////// QRS DETECTION ///////////////
		count = count + 1;
	
	// Initializing buffers with first 8 beats //
		if (!init8Done)
		{
			onesec_cond = (++initBlank == MS1000);
			count = (newPeak > 0) ? WINbuff_size : count;
			initBlank = (onesec_cond) ? 0 : initBlank;
			QRSbuff[qpk_count] = initMax;
			initMax = (onesec_cond) ? 0 : initMax;
			qpk_count = (onesec_cond) ? qpk_count + 1 : qpk_count;
			init8Done_next = (onesec_cond && (qpk_count == 8));			
			nmean = 0;
			rrmean = MS1000;
			sb_count = MS1500 + MS150;
			initMax = (newPeak > initMax) ? newPeak : initMax;
			if (init8Done_next){
				qmean = meanCalc(QRSbuff);
				det_thresh = threshCalc(qmean, nmean);
			}
		} 

	// Check if peak is QRS peak or NOISE peak. If noise and exceeds previous peak in search back, update search back location//
		else
		{
			bls_cond = blsCheck();
			QRSdet_cond0 = ((newPeak > det_thresh) && (!bls_cond));
			NOISEdet_cond0 = ((newPeak > 0) && (newPeak <= det_thresh) && (!bls_cond));
			sbUpdate_cond = (NOISEdet_cond0 && (newPeak > sbPeak) && (count >= (MS360 + WINbuff_size)));

			noiseVal = (NOISEdet_cond0) ? newPeak : noiseVal;
			if (NOISEdet_cond0){
				noiseUpdate(noiseVal);
				nmean = meanCalc(NOISEbuff);
				det_thresh = threshCalc(qmean, nmean);
			}
		
			sbPeak = (sbUpdate_cond) ? newPeak : sbPeak; 
			sbLoc = (sbUpdate_cond) ? (count - WINbuff_size) : sbLoc;	


			QRSdet_cond1 = ((count > sb_count) && (sbPeak > (det_thresh >> 1)) && (!QRSdet_cond0));
			QRSdet_final = (QRSdet_cond0 || QRSdet_cond1);
			qrsVal = (QRSdet_cond0) ? newPeak : qrsVal;
			rrVal = (QRSdet_cond0) ? (count - WINbuff_size) : rrVal;	
			count = (QRSdet_cond0) ? WINbuff_size : count;	
			qrsVal = (QRSdet_cond1) ? sbPeak : qrsVal;
			rrVal = (QRSdet_cond1) ? sbLoc : rrVal;
			if (QRSdet_final){
				qrsUpdate(qrsVal, rrVal);
				qmean = meanCalc(QRSbuff);
				rrmean = meanCalc(RRbuff);
				det_thresh = threshCalc(qmean,nmean);
			}
			lastMax = (QRSdet_final) ? maxDer : lastMax;
			sb_count = (QRSdet_final) ? (rrmean + (rrmean >> 1) + WINbuff_size ) : sb_count; 
			count = (QRSdet_cond1) ? (count - sbLoc) : count;
			QRSdelay = (QRSdet_cond0) ? (WINbuff_size + FILTER_DELAY) : QRSdelay;
			QRSdelay = (QRSdet_cond1) ? (count + FILTER_DELAY) : QRSdelay;
			sbPeak = (QRSdet_final) ? 0 : sbPeak;
			maxDer = (QRSdet_final) ? 0 : maxDer;
			initBlank = (QRSdet_final) ? 0 : initBlank;
			initMax = (QRSdet_final) ? 0 : initMax;
			rset_count = (QRSdet_final) ? 0 : rset_count;
	
		}
		init8Done = init8Done_next;
	
// In background, check for threshold change if there is no peak for 8 consecutive seconds //			
		if(init8Done)
		{
			onesec_cond = (++initBlank == MS1000);
			initBlank = (onesec_cond) ? 0 : initBlank;	 	
			RSETbuff[rset_count] = initMax;
			initMax = (onesec_cond) ? 0 : initMax;
			rset_count = (onesec_cond) ? (rset_count + 1) : rset_count;
			uint8_t timeout_cond = (rset_count == 8);
			nmean = (timeout_cond) ? 0 : nmean;
			rrmean = (timeout_cond) ? MS1000 : rrmean;
			sb_count = (timeout_cond) ? (MS1500 + MS150) : sb_count;
			initBlank = (timeout_cond) ? 0 : initBlank;
			rset_count = (timeout_cond) ? 0 : rset_count;
			if (timeout_cond)
			{
				int i;
				for (i=0; i<8; i++)
				{
					QRSbuff[i] = RSETbuff[i];
					NOISEbuff[i] = 0;
				}
				qmean = meanCalc(QRSbuff);
				det_thresh = threshCalc(qmean, nmean);	
			}
			initMax = (newPeak > initMax) ? newPeak : initMax;
		}	


	/// CENTERING, remove the shift in peak detection ///
		int sweepIndex = (QRSdelay > 0) ? FILTERbuff_ptr + LHPFILT_DELAY - QRSdelay  - MS20 : 0;
		int sweepCount = -MS20;
		float sweepMaxVal = 0;
		int sweepMaxIndex = 0;
		sweepIndex = (sweepIndex < 0) ? sweepIndex + FILTERbuff_size : sweepIndex;
		while (sweepCount < MS20)
		{
			uint8_t maxCond = (FILTERbuff[sweepIndex] > sweepMaxVal);
			sweepMaxVal = maxCond ? FILTERbuff[sweepIndex] : sweepMaxVal;
			sweepMaxIndex = maxCond ? sweepCount : sweepMaxIndex;
			sweepIndex = circUpdateDet(sweepIndex, FILTERbuff_size);
			sweepCount++;
		}
		int64_t data_out = (QRSdelay > 0) ? (QRSdelay - sweepMaxIndex) : 0; 

		write_uint64("det_output_pipe", data_out);	
	}
}

