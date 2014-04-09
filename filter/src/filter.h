//200 samples per heartbeat
//constants for different filter stages
//check readme for the source of constant values used in the program

#define sample_rate 200
#define ms_per_sample ((double)1000/(double)sample_rate)

//#define LPfilt_range ((int)(25/ms_per_sample + 0.5)) // why 25, buff size should be upto 48ms, hence, should use 24 here ?? 
						     // however, with ms_per_sample 5, it doesn't make difference

/********************************
** FILTER STUFF **
********************************/

#define LPbuff_size 10//((int)(50/ms_per_sample + 0.5)) 
#define HPbuff_size 25//((int)(125/ms_per_sample + 0.5))
#define DERIVbuff_size 2//((int)(10/ms_per_sample + 0.5))
#define WINbuff_size 16//((int)(80/ms_per_sample + 0.5))

#define LPbuff_halfSize LPbuff_size/2
#define HPbuff_halfSize HPbuff_size/2

#define WINout_saturation 32000
#define WINsum_saturation WINout_saturation*WINbuff_size





void QRSFilt(uint8_t initialize);

