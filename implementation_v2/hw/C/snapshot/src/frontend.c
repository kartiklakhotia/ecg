#include <stdlib.h>
#include <Pipes.h>
#include <pipeHandler.h>

#include "data_structs.h"
#include "utils.h"
#include "best_fit.h"
#include "daemons.h"
#include "beat.h"

extern HermiteFit best_hermite_fit;
extern ResultBuffer filtered_result_buffer;
int32_t inserted_qrs_delay = 0;

int32_t corrected_qrs_peak = 0;
uint8_t  corrected_qrs_peak_valid = 0;

uint8_t  fetch_beat_done = 0;
uint8_t  best_fit_done   = 0;
	

	
void initializer()
{
#ifdef SW
	fprintf(stderr,"Info: started initializer\n");
#endif

	// coefficients fixed to 2,1,-1,-2
	initDerivativeFilter();
#ifdef SW
	fprintf(stderr,"Info:initializer: initialized derivative filter.\n");
#endif


	
	// read the filter orders.
	uint8_t band_pass_filter_order = getUint8();
#ifdef READBACK
	sendUint8(band_pass_filter_order);
#endif 
#ifdef SW
	fprintf(stderr,"Info:initializer: received band_pass_filter_order=%d.\n",
			band_pass_filter_order);
#endif

	// reads the filter coefficients from the input pipe.
	initBandPassFilter(band_pass_filter_order);
#ifdef SW
	fprintf(stderr,"Info:initializer: received band_pass_filter coeffs.\n");
#endif

	uint8_t moving_average_filter_order = getUint8();

	inserted_qrs_delay = (moving_average_filter_order >> 1) + 1;
#ifdef READBACK
	sendUint8(moving_average_filter_order);
#endif
#ifdef SW
	fprintf(stderr,"Info:initializer: received moving_average_filter_order=%d.\n",
			moving_average_filter_order);
#endif

	// initialize the moving average filter.
	initMovingAverageFilter(moving_average_filter_order);
#ifdef SW
	fprintf(stderr,"Info:initializer: initialized moving_average_filter.\n");
#endif

	// init qrs detector
	initQrsDetector();
#ifdef SW
	fprintf(stderr,"Info:initializer: initialized qrs peak detector.\n");
#endif

	// filtered data result buffer.
	initFilteredResultBuffer();
#ifdef SW
	fprintf(stderr,"Info:initializer: initialized filtered result buffer.\n");
#endif

#ifndef FRONTEND
	//
	// number of hermite polynomials is 6.
	//  	read from input.
	//
	initializeHermitePolynomials();
	best_hermite_fit.valid_flag = 0;

#ifdef SW
	fprintf(stderr,"Info:initializer: initialize hermite polynomials completed.\n");
#endif
#endif
}

void sendBestFitToOutput()
{
	sendUint32(best_hermite_fit.beat_index);
	sendUint32((uint32_t) best_hermite_fit.qrs_peak);
	sendUint32(best_hermite_fit.best_sigma_index);
	
	sendReal(best_hermite_fit.best_mse);
	int I;
	for(I = 0; I < NHERMITE; I++)
	{
		sendReal(best_hermite_fit.hermite_coefficients[I]);
	}
	best_hermite_fit.valid_flag = 0;
}

void controllerDaemon ()
{
   
	initializer();
#ifndef FRONTEND
	write_uint8("start_hermite_fitter",1);
#endif

	uint32_t  sample_index = 0;
	while(1) 
	{

		int16_t isample =  getUint16();
		int32_t sample  = isample;

#ifdef SW
		fprintf(stderr,"IN: %d %d\n", isample, sample);
#endif

		int32_t filtered_sample = applyBandPassFilter(sample);
#ifdef SW
		fprintf(stderr,"BPF: %d\n", filtered_sample);
#endif
	
		//
		// filtered results are pushed. into the buffer for recovery 
		// once a beat has been detected.
		//
		pushIntoFilteredResultBuffer(sample_index, filtered_sample);

		int32_t derivative_sample = applyDerivativeFilter(filtered_sample);
#ifdef SW
		fprintf(stderr,"DER: %d\n", derivative_sample);
#endif

		int32_t moving_average_sample = applyMovingAverageFilter(derivative_sample);
#ifdef SW
		fprintf(stderr,"MAR: %d\n", moving_average_sample);
#endif

		// returns -ve number if no peak found.
		int32_t qrs_peak = applyQrsDetector(sample_index, moving_average_sample);

		if(qrs_peak >= 0)
		{
#ifdef SW
			fprintf(stderr,"QRS: %d\n", qrs_peak);
#endif

			corrected_qrs_peak = qrs_peak - inserted_qrs_delay;
			corrected_qrs_peak_valid = 1;

#ifdef FRONTEND
			sendUint32(corrected_qrs_peak);
#else
			write_uint32("qrs_detect_to_beat_daemon", corrected_qrs_peak);
#endif
		}

		sample_index++;
	}
}
