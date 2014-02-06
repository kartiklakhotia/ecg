#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "hermite.h"
#include "bestFit.h"
#ifndef SW
#include "vhdlCStubs.h"
#endif

#define MIN_SIGMA  0.003*(1000.0/360.0)
#define MAX_SIGMA  0.004*(1000.0/360.0)

double _hF0[MEM_SIZE];
double _hF1[MEM_SIZE];
double _hF2[MEM_SIZE];
double _hF3[MEM_SIZE];
double _hF4[MEM_SIZE];
double _hF5[MEM_SIZE];
double samples[NSAMPLES];



void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}


#define __HF__(N,SI,hhf) {\
		int idx;\
		double hF[NSAMPLES];\
		double sigma = MIN_SIGMA + SI*((MAX_SIGMA-MIN_SIGMA)/NSIGMAS);\
		normalizedHermiteBasisFunction(N,sigma,hF);\
		for(idx	= 0; idx < NSAMPLES; idx++)\
		{\
			double oF = hF[idx];\
			write_float64("in1_data",oF);\
			hhf[idx + (SI*NSAMPLES)] = oF;\
		}}
	

void Sender()
{
	double sigma;
	int SI, idx;
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(0,SI,_hF0);
	}
	fprintf(stderr," Sent hF0.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(1,SI,_hF1);
	}
	fprintf(stderr," Sent hF1.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(2,SI,_hF2);
	}
	fprintf(stderr," Sent hF2.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(3,SI,_hF3);
	}
	fprintf(stderr," Sent hF3.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(4,SI,_hF4);
	}
	fprintf(stderr," Sent hF4.\n");
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__HF__(5,SI,_hF5);
	}
	fprintf(stderr," Sent hF5.\n");
}

#ifdef SW
DEFINE_THREAD(bestFit)
#endif

int main(int argc, char* argv[])
{
	float result;
	int I, J;

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

	signal(SIGINT,  Exit);
	signal(SIGTERM, Exit);

#ifdef SW
	init_pipe_handler();
	PTHREAD_DECL(bestFit);
	PTHREAD_CREATE(bestFit);
#endif

	Sender();
	for(I = 0; I < NSAMPLES; I++)
	{
		double X;
		fscanf(fp, "%le", &X);
		write_float64("in0_data", X);
		samples[I] = X;
	}
	fprintf(stderr," Sent samples.\n");
	fclose(fp);

//	calculateReferenceFit();
//	for (I=0; I<NSIGMAS; I++){
		double best_sigma_index = read_float64("out0_data");
	
	fprintf(stdout, " Best sigma= %f (index = %le).\n", (MIN_SIGMA + (best_sigma_index*(MAX_SIGMA - MIN_SIGMA)/NSIGMAS)), best_sigma_index);
//		fprintf(stdout, "this sigma with number %d, ms error is %le\n", I, best_sigma_index);
//	}
//	double fp_product = read_float64("out1_data");
//	fprintf(stdout, "product is %le\n", fp_product);
//	fprintf(stdout,"Hermite coeffs are \n\n");	
/*	for (I=0; I<NSIGMAS; I++){
//		for (J=0; J<NSAMPLES; J++){	
			double p0 = read_float64("out1_data");
			double p1 = read_float64("out1_data");
			double p2 = read_float64("out1_data");
			double p3 = read_float64("out1_data");
			double p4 = read_float64("out1_data");
			double p5 = read_float64("out1_data");
		fprintf(stdout, "%le,%le,%le,%le,%le,%le\n", p0, p1, p2, p3, p4, p5);
	//	}
	}*/
	double p0 = read_float64("out1_data");
	double p1 = read_float64("out1_data");
	double p2 = read_float64("out1_data");
	double p3 = read_float64("out1_data");
	double p4 = read_float64("out1_data");
	double p5 = read_float64("out1_data");

	fprintf(stdout, "best fit coeffs are = %le,%le,%le,%le,%le,%le\n", p0, p1, p2, p3, p4, p5);

//	uint32_t elapsed_time = read_uint32("elapsed_time_pipe");
//	fprintf(stdout,"Elapsed time = %d.\n", elapsed_time);

#ifdef SW
	PTHREAD_CANCEL(bestFit);
	close_pipe_handler();
	return(0);
#endif
}
