#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "app_defines.h"
#include "hermite.h"

int current_beat_id = 0;
REAL current_beat[144];

void  __nHF__(int N, int SI, REAL* hf) 
{
	int idx;

	REAL sigma = MIN_SIGMA + SI*((MAX_SIGMA-MIN_SIGMA)/NSIGMAS);
	normalizedHermiteBasisFunction(N,sigma,hf);
}

			
REAL dotProductReduce(REAL* hF, REAL* residual_beat)
{
	int I;
	REAL dp = 0;
	for(I = 0; I < NSAMPLES; I++)
	{
		dp += hF[I] * residual_beat[I];
	}
	for(I = 0; I < NSAMPLES; I++)
	{
		residual_beat[I] -= dp*hF[I];
	}
	return(dp);
}

REAL bestFitForSigma(int N, int SI, int print_flag)
{
	if(print_flag)
		fprintf(stderr,"B:%d:%d %d %d %d ", N, SI, current_beat_id, current_beat_id*144, SI);

	REAL residual_beat[NSAMPLES];


	int I;
	for(I=0; I< NSAMPLES; I++)
	{
		residual_beat[I] = current_beat[I];
	}

	int J = 0;
	for(J = 0; J < N; J++)
	{
		REAL hF[NSAMPLES];
		__nHF__(J, SI, hF);

		REAL dp = dotProductReduce(hF, residual_beat);
		if(print_flag)
			fprintf(stderr," %le ", dp);
	}

	REAL err = 0;
	for(J = 0; J < NSAMPLES; J++)
	{
		err += (residual_beat[J] * residual_beat[J]);
	}
	if(print_flag)
		fprintf(stderr," %le\n", err);

	return(err);
}

void bestFit(int N)
{

	REAL best_error = 1.0e+50;
	int best_SI  = -1;


	int I;
	REAL be = 0;
	for(I=0; I< NSAMPLES; I++)
	{
		be += (current_beat[I]*current_beat[I]);
	}

	int SI;
	for(SI=0; SI < NSIGMAS; SI++)
	{
		REAL err = bestFitForSigma(N, SI, 0);

		if(err < best_error)
		{
			best_SI = SI;
			best_error = err;
		}
	}

	bestFitForSigma(N, best_SI, 1);
	fprintf(stdout,"R: %d %d %le %le\n", N, best_SI, best_error, best_error/be);
}


int readCurrentBeat()
{

	int I;
	for(I = 0; I < NSAMPLES; I++)
	{
		int idx;
		char s[16];
		REAL x;
#ifndef ONEBEAT
#ifdef USE_DOUBLE
		int n = fscanf(stdin,"%s %d %lf",s,&idx, &x);
#else
		int n = fscanf(stdin,"%s %d %f",s,&idx, &x);
#endif
		if(n < 3)
			return(1);
#else
#ifdef USE_DOUBLE
		int n = fscanf(stdin,"%lf",&x);
#else
		int n = fscanf(stdin,"%f",&x);
#endif
		if(n < 1)
			return(1);
#endif


		if(n < 1)
		  return(1);

		if((I < 16) || (I > 128))
			current_beat[I] = 0;
		else
			current_beat[I] = x;
	}
	current_beat_id++;
	return(0);
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s <number-of-hermites> < beat-file\n", argv[0]);
		return(1);
	}

	int nhermites = atoi(argv[1]);


	while(1)
	{
		int status = readCurrentBeat();
		if(status)
			break;

		bestFit(nhermites);
	}

	return(0);
}
