#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "app_defines.h"


typedef enum _FsmState {
	RESET,
	LOOKINGFORPEAK,
	PROVISIONALPEAKFOUND,
	HALFPEAKFOUND
} FsmState;

typedef struct _QrsState {

	FsmState fsm_state;
	int32_t   peak_value;
	int32_t  peak_index;
	int32_t  half_peak_counter;
} QrsState;

QrsState qrs_state;

void initQrsDetector ()
{
	qrs_state.fsm_state = RESET;
	qrs_state.peak_value = -1000.0;
	qrs_state.peak_index = -1;
	qrs_state.half_peak_counter = 72;
}

#ifdef SW
void stateToString(FsmState f, char* buf)
{
	switch(f) {
		case RESET : sprintf(buf, "RESET"); break;
		case LOOKINGFORPEAK : sprintf(buf, "LOOKINGFORPEAK"); break;
		case PROVISIONALPEAKFOUND : sprintf(buf, "PROVISIONALPEAKFOUND"); break;
		case HALFPEAKFOUND : sprintf(buf, "HALFPEAKFOUND"); break;
		default: break;
	}
}
char sprint_buffer[256];

#endif


		
int32_t applyQrsDetector(uint32_t time_step, int32_t sample)
{
	int ret_val = -1;
#ifdef SW
#ifdef DEBUGPRINT
	stateToString(qrs_state.fsm_state, sprint_buffer);
	fprintf(stderr,"Info: entered processSample state=%s peak=%d peak_index=%d, half-peak-count=%d sample=%d, time_step=%d\n", 
				sprint_buffer, qrs_state.peak_value, qrs_state.peak_index,
				qrs_state.half_peak_counter,
				sample, time_step);
#endif
#endif
	uint8_t pflag = 0;

	switch(qrs_state.fsm_state) {
		case RESET:
			qrs_state.fsm_state  = LOOKINGFORPEAK;
			qrs_state.peak_value = -1.0;
			qrs_state.peak_index  = -1;
			break;
		case LOOKINGFORPEAK:
			pflag = (qrs_state.peak_value > QRS_PEAK_THRESHOLD);
			if(pflag && (sample < (qrs_state.peak_value - QRS_EPSILON)))
			// a decrease from the peak?
			// could be a provisional peak.
			{
				qrs_state.fsm_state = PROVISIONALPEAKFOUND;
			}
			else if(sample > qrs_state.peak_value)
			{
				qrs_state.peak_value = sample;
				qrs_state.peak_index  = time_step;
			}
			break;
		//
		// if we get a peak, we wait for a window of PEAKWINDOW samples to check if
		// we get a higher peak.
		//
		case PROVISIONALPEAKFOUND:
			//
			// provisional peak.. if we find a higher peak than the
			// most recently recorded one....
			//
			if((sample > qrs_state.peak_value) && 
					((time_step - qrs_state.peak_index)  < 72))
			{
				qrs_state.peak_value = sample;
				qrs_state.peak_index = time_step;
			}
			else if(sample < (qrs_state.peak_value >> 1))
			// lost half the value of the peak?
			{
				ret_val = qrs_state.peak_index;
				qrs_state.fsm_state = HALFPEAKFOUND;
				qrs_state.half_peak_counter = 72 - (time_step - qrs_state.peak_index);

			}
			break;
		case HALFPEAKFOUND: // wait for 200ms, ie 72 samples from the last peak.
			if(qrs_state.half_peak_counter == 0)
			{
				qrs_state.fsm_state = RESET;
			}
			else
			{
				qrs_state.half_peak_counter--;
			}
			break;
		default:
			break;
			
	}

	return(ret_val);
}

