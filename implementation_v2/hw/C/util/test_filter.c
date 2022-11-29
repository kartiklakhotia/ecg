#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct __FilterState128  {

	int32_t  filter_coefficients[128];
	int32_t  filter_samples[128];
	int32_t  filter_order;
	uint16_t write_pointer;
} FilterState128;

FilterState128 band_pass_filter;

void print_usage(char* a0)
{
	fprintf(stderr,"Usage: %s filter-coeff-file  input-data-file\n", a0);
}

void initBandPassFilter()
{
	band_pass_filter.write_pointer = 0;
	int I;
	for(I = 0; I < 128; I++)
	{
		band_pass_filter.filter_samples[I] = 0;
		band_pass_filter.filter_coefficients[I] = 0;
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



int main (int argc, char* argv[])
{
	if(argc < 3)
	{
		print_usage(argv[0]);
		return(1);
	}

	int32_t filter_coeffs[1024];
	FILE* fc = fopen(argv[1],"r");
	if(fc == NULL)
	{
		fprintf(stderr,"Error: filter coefficient file %s could not be opened\n", argv[1]);
		return(1);
	}

	FILE* id = fopen(argv[2],"r");
	if(id == NULL)
	{
		fprintf(stderr,"Error: input data file %s could not be opened\n", argv[2]);
		return(1);
	}

	// read filter coefficients.
	int I=0;
	while (!feof(fc))
	{
		char line_buffer[256];
		char* fs = fgets(line_buffer,255, fc);
		if(fs == NULL)
			break;

		if(line_buffer[0] == '!')
			continue;

		int32_t x;
		int n = sscanf(line_buffer,"%d", &x);

		if((n == 0) || (n == EOF))
			break;

		band_pass_filter.filter_coefficients[I] = x;
		fprintf(stderr,"BPFFC: %d %d\n", I, x);
		I++;
	}
	fclose(fc);

	band_pass_filter.write_pointer = 0;
	band_pass_filter.filter_order  = I;

	fprintf(stderr,"Info: filter order=%d\n", I);

	while(!feof(id))
	{
		int32_t sample;
		int n = fscanf(id, "%d", &sample);

		if((n==0) || (n==EOF))
			break;
		int32_t f = applyBandPassFilter(sample);
		fprintf(stderr,"BPF: %d\n", f);
	}

	fclose(id);
	return(0);
}

