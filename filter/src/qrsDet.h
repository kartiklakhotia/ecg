
//200 samples per heartbeat
//constants for different filter stages
//check readme for the source of constant values used in the program



/********************************
** DETECTOR STUFF **
********************************/


#define MIN_PEAK_AMP	7 //prevent detection of peaks below 150 uV

#define DDbuff_size	 WINbuff_size + FILTER_DELAY + MS100 




// function avaialble at output interface //
void qrsDet();
