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
int initBlank, initMax, preBlank_count, sb_count, set_count;
int sbLoc;
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
	qpk_count = maxDer = lastMax = count = rset_count = sbPeak = 0;
	initMax = preBlankCnt = DDbuff_ptr = DDCALCbuff_ptr = QRSbuff_ptr = RRbuff_ptr = NOISEbuff_ptr = 0;
	initBlank = 1;
	sb_count = sbLoc = MS1500;
	maxPeak = timeSinceMaxPeak = 0;
	lastDatum = 0;
	init8Done = 0;
}

void QRSDet()
{
	initDet();
	QRSFilt(1); //initiate filter buffers, pointers and variables also to reset state;
	int aPeak, newPeak, tempPeak;
	int qmean, rrmean, nmean;
	int RSETbuff[8];
	int det_thresh;
	int qrsVal, rrVal, noiseVal;
	uint8_t init8Done_next, onesec_cond;
	while(1)
	{
		int QRSdelay = 0;
		int prefilt_datum = read_uint32("det_input_pipe");  //clubbed in the lpfilt() function of Filter part
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
	
		onesec_cond = (initBlank == MS1000);
	/* Initializing buffers with first 8 beats */
		if (!init8Done)
		{
			count = (newPeak > 0) ? WINbuff_size : count + 1;
			initBlank = (onesec_cond) ? 1 : initBlank + 1;
			qrsbuf[qpk_count] = initMax;
			initMax = (onesec_cond) ? 0 : initMax;
			qpk_count = (onesec_cond) ? qpk_count + 1 : qpk_count;
			init8Done_next = (onesec_cond && (qpk_count == 8));			
		//	qmean = (init8Done) ? meancalc(QRSbuff) : qmean;
			nmean = 0;
			rrmean = MS1000;
			sb_count = MS1500 + MS150;
			qmean = (init8Done_next) ? meanCalc(QRSbuff) : qmean;
			det_thresh = (init8Done_next) ? threshCalc(qmean, nmean) : det_thresh;
			initMax = (newPeak > initMax) ? newPeak : initMax;
		//	if (init8Done){
		//		qmean = meanCalc(QRSbuff);
		//		det_thresh = threshCalc(qmean, nmean);
		//	}
		} 

	/* Check if peak is QRS peak or NOISE peak. If noise and exceeds previous peak in search back, update search back location*/
		else
		{
			count = count + 1;
			uint8_t bls_cond = blsCheck();
			uint8_t QRSdet_cond0 = ((newPeak > det_thresh) && (!bls_cond));
			uint8_t NOISEdet_cond0 = ((newPeak > 0) && (newPeak < det_thresh) && (!bls_cond));
			uint8_t sbUpdate_cond = (NOISEdet_cond0 && (newPeak > sbPeak) && (count >= (MS360 + WINDOW_WIDTH)));

			noiseVal = (NOISEdet_cond0) ? newPeak : noiseVal;
			count = (QRSdet_cond0) ? WINDOW_WIDTH : count;	
		
			sbPeak = (QRSdet_cond0) ? 0 : sbPeak; 
			sbPeak = (sbUpdate_cond) ? newPeak : sbPeak; 
			sbLoc = (sbUpdate_cond) ? (count - WINbuff_size) : sbLoc;	

			if (NOISEdet_cond0){
				noiseUpdate(noiseVal);
				det_thresh = threshCalc(qmean, nmean);
			}
			nmean = (NOISEdet_cond0) ? meanCalc(NOISEbuff) : nmean;	

			uint8_t QRSdet_cond1 = ((count > sb_count) && (sbPeak > (det_thresh >> 1)) && (!QRSdet_cond0));
			uint8_t QRSdet_final = (QRSdet_cond0 || QRSdet_cond1);
			qrsVal = (QRSdet_cond0) ? newPeak : qrsVal;
			rrVal = (QRSdet_cond0) ? (count - WINDOW_WIDTH) : rrVal;	
			qrsVal = (QRSdet_cond1) ? sbPeak : qrsVal;
			rrVal = (QRSdet_cond1) ? sbLoc : rrVal;
			if (QRSdet_final){
				qrsUpdate(qrsVal, rrVal);
				qmean = meanCalc(QRSbuff);
				rrmean = meanCalc(RRbuff);
				det_thresh = threshCalc(qmean,nmean);
			}
			lastMax = (QRSdet_final) ? maxDer : lastMax;
			sb_count = (QRSdet_cond1) ? (rrmean + (rrmean >> 1) + WINbuff_size ) : sb_count; 
			count = (QRSdet_cond1) ? (count - sbLoc) : count;
			QRSdelay = (QRSdet_cond0) ? (WINbuff_size + FILTER_DELAY) : QRSdelay;
			QRSdelay = (QRSdet_cond1) ? (count + FILTER_DELAY) : QRSdelay;
			sbPeak = (QRSdet_cond1) ? 0 : sbPeak;
			maxDer = (QRSdet_final) ? 0 : maxDer;
			initBlank = (QRSdet_final) ? 1 : initBlank;
			initMax = (QRSdet_final) ? 0 : initMax;
			rset_count = (QRSdet_final) ? 0 : rset_count;
			//...//
	
		}
		init8Done = init8Done_next;
		onesec_cond = (initBlank == MS1000);
	
/* In background, check for threshold change if there is no peak for 8 consecutive seconds */			
		if(init8Done)
		{
			initBlank = (onesec_cond) ? 1 : initBlank + 1;	 	
			RSETbuff[rset_count] = initMax;
			initMax = (onesec_cond) ? 0 : initMax;
			rset_count = (onesec_cond) ? (rset_count + 1) : rset_count;
			uint8_t timeout_cond = (rset_count == 8);
			nmean = (timeout_cond) ? 0 : nmean;
			rrmean = (timeout_cond) ? MS1000 : rrmean;
			sb_count = (timeout_cond) ? (MS1500 + MS150) : sb_count;
			initBlank = (timeout_cond) ? 1 : initBlank;
			rset_count = (timeout_cond) ? 0 : rset_count;
			if (timeout_cond)
			{
				for (int i=0; i<8; i++)
				{
				__loop_pipelining_on__(3, 1, 0);
					QRSbuff[i] = RSETbuff[i];
					NOISEbuff[i] = 0;
				}
				qmean = meanCalc(QRSbuff);
				det_thresh = threshCalc(qmean, nmean);	
			}
			initMax = (timeout_cond) ? 0 : initMax;
			initMax = (newPeak > initMax) ? newPeak : initMax;
		}	
		
		write uint32_t("det_output_pipe", QRSdelay);	
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
		ptr = circUpdateDet(ptr, DDbuff_size); //don't push this value in DDbuff_ptr
	}

		maxDer = max;
		min = -min;
		

		uint8_t output = ((max > (min >> 3)) && (min > (max >> 3)) && (abs(maxt-mint) < MS150))	? 0 : 1;
		return output;
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
	QRSbuff_ptr = circUpdateDet(ptr0, 8);
	RRbuff_ptr = circUpdateDet(ptr1, 8);
}	


/*************************************************
**UPDATE NOISE BUFFER ON NOISE PEAK DETECTION
*************************************************/
void noiseUpdate(int noiseVal)
{
	int ptr2 = NOISEbuff_ptr;
	NOISEbuff[ptr2] = noiseVal;
	NOISEbuff_ptr = circUpdateDet(ptr2, 8);
}	
