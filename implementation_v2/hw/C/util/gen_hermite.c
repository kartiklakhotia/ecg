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
	if(argc < 3)
	{
		fprintf(stderr,"Usage: %s hermite-order sigma-index\n", argv[0]);
		return(1);
	}

	int hermite_order = atoi(argv[1]);
	int sigma_index   = atoi(argv[2]);

	REAL hF[MEM_SIZE_PER_HF];
	__HF__(hermite_order, sigma_index, hF);

	int I;
	int offset  = NSAMPLES*sigma_index;
	for(I =0; I < NSAMPLES; I++)
	{
		fprintf(stdout,"%le\n", hF[I + offset]);
	}

	return(0);
}
