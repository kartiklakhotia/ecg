
//200 samples per heartbeat
//constants for different filter stages
//check readme for the source of constant values used in the program



/********************************
** DETECTOR STUFF **
********************************/

#define PRE_BLANK 	MS200  //200ms min gap between consecutive QRS peaks

#define MIN_PEAK_AMP	7 //prevent detection of peaks below 150 uV

#define FILTER_DELAY 	(DERIVbuff_size + LPbuff_size + HPbuff_size -3)/2 + PRE_BLANK
#define DDbuff_size	 WINbuff_size + FILTER_DELAY + MS100 




// function avaialble at output interface //
void qrsDet();
