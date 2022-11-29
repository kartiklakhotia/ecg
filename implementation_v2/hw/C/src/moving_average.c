#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "data_structs.h"

MovingAverageFilter64 moving_average_filter;

void initMovingAverageFilter(uint8_t filter_order)
{
	moving_average_filter.filter_order = filter_order;
	moving_average_filter.write_pointer = 0;

	// set the moving average filter coefficients.
	int I;
	for(I=0; I < 64; I++)
	{
		moving_average_filter.filter_samples[I] = 0;
	}
	moving_average_filter.last_value = 0;
}
		
int32_t applyMovingAverageFilter(int32_t sample)
{
	uint16_t wp = moving_average_filter.write_pointer;
	moving_average_filter.filter_samples[wp] = sample;

	int32_t ret_val = moving_average_filter.last_value;

	// outgoing index at (wp - ORDER).
	uint16_t I = ((wp + 64) - moving_average_filter.filter_order) & 0x3f;
	int32_t outgoing_value = moving_average_filter.filter_samples[I];

	ret_val += (sample - outgoing_value);
	moving_average_filter.last_value = ret_val;

	wp = (wp + 1) & 0x3f;
	moving_average_filter.write_pointer = wp;

	return(ret_val);
}


