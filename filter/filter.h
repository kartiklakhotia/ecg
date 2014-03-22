//200 samples per heartbeat
//constants for different filter stages
#define sample_rate 200
#define ms_per_sample ((double)1000/(double)sample_rate)

#define LPfilt_range ((int)(25/ms_per_sample + 0.5))
#define HPfilt_range ((int)(125/ms_per_sample + 0.5))

#define LPbuff_size 2*LPfilt_range
#define HPbuff_size HPfilt_range
#define window_width ((int)(80/ms_per_sample + 0.5))

#define LP_maxptr LPbuff_size-1 				
#define HP_maxptr HPbuff_size-1 //max permissible indices for LP and HP circular buffers

#define LPbuff_halfSize LPbuff_size/2
#define HPbuff_halfSize HPbuff_size/2

//global memory spaces (can be replaced with static if it works with ahir)

int LPbuff[LPbuff_size];
int HPbuff[HPbuff_size];


