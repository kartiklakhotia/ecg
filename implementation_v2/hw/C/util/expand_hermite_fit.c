#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "app_defines.h"
#include "hermite.h"

REAL hF0[MEM_SIZE_PER_HF];
REAL hF1[MEM_SIZE_PER_HF];
REAL hF2[MEM_SIZE_PER_HF];
REAL hF3[MEM_SIZE_PER_HF];
REAL hF4[MEM_SIZE_PER_HF];
REAL hF5[MEM_SIZE_PER_HF];

void  __HF__(int N, int SI, REAL* hhf);

void initHermitePolys()
{
	int SI;
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(0,SI,hF0);
		__HF__(1,SI,hF1);
		__HF__(2,SI,hF2);
		__HF__(3,SI,hF3);
		__HF__(4,SI,hF4);
		__HF__(5,SI,hF5);
	}
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s fit-file\n", argv[0]);
		return(1);
	}

	FILE* fp = fopen (argv[1], "r");
	if(fp == NULL)
	{
		fprintf(stderr,"Error: could not open fit file %s\n", argv[1]);
		return(1);
	}

	initHermitePolys();

	while(!feof(fp))
	{
		char line_buffer[1024];
		char* fs = fgets(line_buffer,1023, fp);
		if(fs == NULL)
			break;

		char fit_str[16];
		int32_t beat_id, peak_index, SI;
		REAL err, c0, c1, c2, c3, c4, c5;		

#ifdef USE_DOUBLE
		int n = sscanf(line_buffer,"%s %d %d %d %le %le %le %le %le %le %le",
					fit_str, &beat_id, &peak_index, &SI,
					&c0, &c1, &c2, &c3, &c4, &c5, &err);
#else
		int n = sscanf(line_buffer,"%s %d %d %d %e %e %e %e %e %e %e",
					fit_str, &beat_id, &peak_index, &SI,
					&c0, &c1, &c2, &c3, &c4, &c5, &err);
#endif
		if(n < 11)
			break;


		int32_t beat_start = peak_index - 72;
		int32_t offset = NSAMPLES*SI;
		int32_t I;
		for(I = 0; I < NSAMPLES; I++)
		{
			REAL current_beat = 0;
			
			current_beat += c0 * hF0[I + offset];
			current_beat += c1 * hF1[I + offset];
			current_beat += c2 * hF2[I + offset];
			current_beat += c3 * hF3[I + offset];
			current_beat += c4 * hF4[I + offset];
			current_beat += c5 * hF5[I + offset];

#ifdef USE_DOUBLE
			fprintf(stdout, "%d %le\n", beat_start + I, current_beat);
#else
			fprintf(stdout, "%d %e\n", beat_start + I, current_beat);
#endif
		}
			
	}
	fclose(fp);

	return(0);
}
