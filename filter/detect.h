
//200 samples per heartbeat
//constants for different filter stages
//check readme for the source of constant values used in the program


/********************************
** DELAYS IN SAMPLE UNITS **
********************************/
#define MS1500	((int) (1500/ms_per_sample + 0.5))
#define MS1000	((int) (1000/ms_per_sample + 0.5))
#define MS360	((int) (360/ms_per_sample + 0.5))
#define MS220	((int) (220/ms_per_sample + 0.5))
#define MS150	((int) (150/ms_per_sample + 0.5))
#define MS95	((int) (95/ms_per_sample + 0.5))

/********************************
** DETECTOR STUFF **
********************************/

#define PRE_BLANK 	((int) (195/ms_per_sample + 0.5)) //look 200ms before current peak arrival
			               			  //samples in previous 195ms & 5ms of current sample

#define MIN_PEAK_AMP	7 //prevent detection of peaks below 150 uV

#define FILTER_DELAY 	(int)(((double)DERIVBUFF_size/2) + ((double)LPbuff_size/2 - 1) + (((double)HPbuff_size-1)/2) + PRE_BLANK)
#define DDbuff_size	 WINbuff_size + FILTER_DELAY + ((int)(100/ms_per_sample + 0.5)) 
#define DDCALC_maxptr	DERIV_maxptr



// global memory spaces //

int DDbuff[DDbuff_size];
int DDCALCbuff[DERIVbuff_size];
int NOISEbuff[8], RRbuff[8], QRSbuff[8];

