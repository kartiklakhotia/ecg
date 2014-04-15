#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <riffa.h>
#include "hermite.h"
#include "bestFit.h"
#include "timer.h"

#define MIN_SIGMA  0.003*(1000.0/360.0)
#define MAX_SIGMA  0.004*(1000.0/360.0)


fpga_t * fpga;


double _hF0[MEM_SIZE];
double _hF1[MEM_SIZE];
double _hF2[MEM_SIZE];
double _hF3[MEM_SIZE];
double _hF4[MEM_SIZE];
double _hF5[MEM_SIZE];
double samples[NSAMPLES];


void  HF(int N, int SI, double* hhf) {
		int idx;
		int hermiteSent;
		double hF[NSAMPLES];
		double sigma = MIN_SIGMA + SI*((MAX_SIGMA-MIN_SIGMA)/NSIGMAS);
		normalizedHermiteBasisFunction(N,sigma,hF);
		hermiteSent = fpga_send(fpga,1,hF,NSAMPLES*2,0,1,2000);
		fprintf(stderr, "num samples sent is %d\n", hermiteSent);
		for(idx	= 0; idx < NSAMPLES; idx++)
		{
			double oF = hF[idx];
			hhf[idx + (SI*NSAMPLES)] = oF;
		}
}

void Sender()
{
	double sigma;
	int SI, idx;
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(0,SI,_hF0);
	}
	fprintf(stderr," Sent hF0.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(1,SI,_hF1);
	}
	fprintf(stderr," Sent hF1.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(2,SI,_hF2);
	}
	fprintf(stderr," Sent hF2.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(3,SI,_hF3);
	}
	fprintf(stderr," Sent hF3.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(4,SI,_hF4);
	}
	fprintf(stderr," Sent hF4.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		HF(5,SI,_hF5);
	}
	fprintf(stderr," Sent hF5.\n");
}



int main(int argc, char* argv[])
{
	float result;
	int I;
	int sampleSent;
	double * pcoeffs;
	pcoeffs = (double *)malloc(60*8);
	double * best_sigma_index;
	best_sigma_index = (double *)malloc(8);
	*best_sigma_index = 0;
	GET_TIME_INIT(2);
	if(argc < 2)
	{
		fprintf(stderr,"Supply data set file.\n");
		return(1);
	}

	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		fprintf(stderr,"Could not open data set file %s.\n", argv[1]);
		return(1);
	}

//	signal(SIGINT,  Exit);
//	signal(SIGTERM, Exit);
	fpga = fpga_open(0);
	fpga_reset(fpga);

	Sender();
	for(I = 0; I < NSAMPLES; I++)
	{
		double X;
		fscanf(fp, "%le", &X);
		samples[I] = X;
	}

	fclose(fp);

	for (I=0; I<10000000; I++){
		GET_TIME_VAL(0);
		sampleSent = fpga_send(fpga,0,samples,NSAMPLES*2,0,1,2000);//on channel 1
//		fprintf(stderr," Sent samples.\n");
//		fprintf(stderr,"num data samples sent %d \n", sampleSent);

//	calculateReferenceFit();

		int recv = fpga_recv(fpga,0,best_sigma_index,2,5000);
		GET_TIME_VAL(1);	
		recv = fpga_recv(fpga,1,pcoeffs,12,5000);
		fprintf(stdout, " Best sigma= %f (index = %f).\n", (MIN_SIGMA + (best_sigma_index[0]*(MAX_SIGMA - MIN_SIGMA)/NSIGMAS)), best_sigma_index[0]);
//	double p0 = read_float64("fit_coefficient_pipe");
//	double p1 = read_float64("fit_coefficient_pipe");
//	double p2 = read_float64("fit_coefficient_pipe");
//	double p3 = read_float64("fit_coefficient_pipe");
//	double p4 = read_float64("fit_coefficient_pipe");
//	double p5 = read_float64("fit_coefficient_pipe");
//	for (I=0; I<NSIGMAS; I++){
		fprintf(stdout, "Fit coefficients = %le, %le, %le, %le, %le, %le.\n", pcoeffs[0],pcoeffs[1],pcoeffs[2],pcoeffs[3],pcoeffs[4],pcoeffs[5]);
		printf("time taken = %f\n", (TIME_VAL_TO_MS(1)-TIME_VAL_TO_MS(0)));
//	}
	}
	fpga_close(fpga);
//	uint32_t elapsed_time = read_uint32("elapsed_time_pipe");
//	fprintf(stdout,"Elapsed time = %d.\n", elapsed_time);

	return(0);
}
