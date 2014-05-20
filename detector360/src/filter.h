//200 samples per heartbeat
//constants for different filter stages
//check readme for the source of constant values used in the program

#define SAMPLE_RATE 360
#define ms_per_sample ((double)1000/(double)sample_rate)

#define MS10	(int)((SAMPLE_RATE*10+500)/1000)//	((int) (10/ MS_PER_SAMPLE + 0.5))
#define MS20	(int)((SAMPLE_RATE*20+500)/1000)//	((int) (10/ MS_PER_SAMPLE + 0.5))
#define MS25	(int)((SAMPLE_RATE*25+500)/1000)//	((int) (25/MS_PER_SAMPLE + 0.5))
#define MS30	(int)((SAMPLE_RATE*30+500)/1000)//	((int) (30/MS_PER_SAMPLE + 0.5))
#define MS40	(int)((SAMPLE_RATE*40+500)/1000)//	((int) (30/MS_PER_SAMPLE + 0.5))
#define MS80	(int)((SAMPLE_RATE*80+500)/1000)//	((int) (80/MS_PER_SAMPLE + 0.5))
#define MS95	(int)((SAMPLE_RATE*95+500)/1000)//	((int) (95/MS_PER_SAMPLE + 0.5))
#define MS100	(int)((SAMPLE_RATE*100+500)/1000)//	((int) (100/MS_PER_SAMPLE + 0.5))
#define MS125	(int)((SAMPLE_RATE*125+500)/1000)//	((int) (125/MS_PER_SAMPLE + 0.5))
#define MS150	(int)((SAMPLE_RATE*150+500)/1000)//	((int) (150/MS_PER_SAMPLE + 0.5))
#define MS160	(int)((SAMPLE_RATE*160+500)/1000)//	((int) (160/MS_PER_SAMPLE + 0.5))
#define MS175	(int)((SAMPLE_RATE*175+500)/1000)//	((int) (175/MS_PER_SAMPLE + 0.5))
#define MS195	(int)((SAMPLE_RATE*195+500)/1000)//	((int) (195/MS_PER_SAMPLE + 0.5))
#define MS200	(int)((SAMPLE_RATE*200+500)/1000)//	((int) (200/MS_PER_SAMPLE + 0.5))
#define MS220	(int)((SAMPLE_RATE*220+500)/1000)//	((int) (220/MS_PER_SAMPLE + 0.5))
#define MS250	(int)((SAMPLE_RATE*250+500)/1000)//	((int) (250/MS_PER_SAMPLE + 0.5))
#define MS300	(int)((SAMPLE_RATE*300+500)/1000)//	((int) (300/MS_PER_SAMPLE + 0.5))
#define MS360	(int)((SAMPLE_RATE*360+500)/1000)//	((int) (360/MS_PER_SAMPLE + 0.5))
#define MS450	(int)((SAMPLE_RATE*450+500)/1000)//	((int) (450/MS_PER_SAMPLE + 0.5))
#define MS1000	(int)(SAMPLE_RATE)
#define MS1500	(int)((SAMPLE_RATE*1500+500)/1000)//	((int) (1500/MS_PER_SAMPLE))
/********************************
** FILTER STUFF **
********************************/

#define LPbuff_size (int)(2*MS25) 
#define HPbuff_size (int)(MS125)
#define DERIVbuff_size (int)(MS10)
#define WINbuff_size (int)(MS80)

#define LPbuff_halfSize (int)(LPbuff_size/2)
#define HPbuff_halfSize (int)(HPbuff_size/2)

#define WINout_saturation 32000
#define WINsum_saturation WINout_saturation*WINbuff_size

#define PRE_BLANK 	MS200  //200ms min gap between consecutive QRS peaks
#define LHPFILT_DELAY (int)((LPbuff_size + HPbuff_size - 3)/2) 
#define FILTER_DELAY 	(DERIVbuff_size)/2 + PRE_BLANK
#define FILTERbuff_size  MS1500 + WINbuff_size - MS360 + FILTER_DELAY + MS200
		// 1.5 times r-r interval in case peak is detected in search back
		// search back location is atleast 360ms from previous qrs, to remove T-waves from being considered as search back peaks
		// window width
		// delay incurred in filter, all are linear phase and hence, constant delay for all frequencies
		// 200 ms delay for holding the peak before passing it for evaluation

int FILTERbuff_ptr;
float FILTERbuff[FILTERbuff_size];

void QRSFilt();
void initFilt();
