#include <stdio.h>
#include <stdint.h>
#include <math.h>

uint8_t write_pointer;
int32_t samples[4];
int32_t filter_coeffs[] = {2, 1, -1, -2};


void initDerivativeFilter()
{
	uint8_t I;
	write_pointer = 0;
	for(I=0; I<4; I++)
	{
		samples[I] = 0;
	}
	filter_coeffs[0] = 2;
	filter_coeffs[1] = 1;
	filter_coeffs[2] = -1;
	filter_coeffs[3] = -2;
}

int32_t applyDerivativeFilter(int32_t sample)
{
	samples[write_pointer] = sample;
	
	int32_t ret_val = 0;
	
	uint8_t J =  0;
	uint8_t I = (write_pointer + 1) & 0x3;

	for(J = 0; J < 4; J++)
	{
		ret_val += filter_coeffs[J] * samples[I];
		I = (I + 1) & 0x3;	
	}
	
	write_pointer = (write_pointer + 1) & 0x3;
	ret_val = (ret_val < 0) ? -ret_val : ret_val;

	return(ret_val);
}




