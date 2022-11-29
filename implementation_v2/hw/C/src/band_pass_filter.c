#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "app_defines.h"
#include "data_structs.h"
#include "utils.h"
#include "best_fit.h"

FilterState128  	band_pass_filter;

void initBandPassFilter(uint8_t filter_order)
{
	band_pass_filter.filter_order = filter_order;
	band_pass_filter.write_pointer = 0;

	// get the band-pass filter coefficients.
	uint8_t I;
	for(I=0; I < 128; I++)
	{
		band_pass_filter.filter_samples[I] = 0;
	}

	for(I=0; I < filter_order; I++)
	{
		uint32_t fc = getUint32();
		band_pass_filter.filter_coefficients[I] = (int32_t) fc;
#ifdef SW
		fprintf(stderr,"BPFFC: %d %d\n", I, band_pass_filter.filter_coefficients[I]);
#endif
		
#ifdef READBACK
		//
		// send it back to the one who sent it...
		//
		sendUint32(fc);
#endif
	}

}

int32_t applyBandPassFilter(int32_t sample)
{
	uint16_t wp = band_pass_filter.write_pointer;
	band_pass_filter.filter_samples[wp] = sample;

	int32_t ret_val = 0;

	// start index is (wp - order) + 1.
	uint16_t start_index = ((wp + 129) - band_pass_filter.filter_order) & 0x7f;
	
	uint16_t J = 0;
	uint16_t I = start_index;
	for(J = 0; J < band_pass_filter.filter_order; J++)
	{
		int32_t X = band_pass_filter.filter_samples[I];
		int32_t U = X * band_pass_filter.filter_coefficients[J];
		ret_val += U;
		I = (I + 1) & 0x7f;
	}


	// add 1 to the write pointer..
	wp = (wp + 1) & 0x7f;
	band_pass_filter.write_pointer = wp;

	return(ret_val);
}
