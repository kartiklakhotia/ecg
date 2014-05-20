#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "hermite.h"
#include "bestFit.h"
#include "filter.h"
#include "qrsDet.h"
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
			write_float64("hF_input_pipe",oF);\
			hhf[idx + (SI*NSAMPLES)] = oF;\
		}}
	

void Sender(char* infile_arg)
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
	fprintf(stderr," Hermite Function Initiation complete.\n");
	
	FILE* frec = fopen(infile_arg, "r");
	if(frec == NULL)
	{
		fprintf(stderr, "could not open the record file %s\n", infile_arg);
		return;
	}
	double data;
	fscanf(frec, "%lf", &data);
	while(!feof(frec)){
		write_float64("det_input_pipe", data);
		fscanf(frec, "%lf", &data);
	}
	fclose(frec);
}


void Receiver()
{
	int count = 0;
	FILE* fout = fopen("characterized_data.txt", "w");
	while(1)
	{
		count++;
		int64_t best_sigma_index = read_uint64("bsi_output_pipe");
		double p0 = read_float64("dotP_output_pipe");
		double p1 = read_float64("dotP_output_pipe");
		double p2 = read_float64("dotP_output_pipe");
		double p3 = read_float64("dotP_output_pipe");
		double p4 = read_float64("dotP_output_pipe");
		double p5 = read_float64("dotP_output_pipe");
		fprintf(fout, " Best sigma= %f, (index = %"PRId64").\n", (MIN_SIGMA + (((float)(best_sigma_index))*(MAX_SIGMA - MIN_SIGMA)/NSIGMAS)), best_sigma_index);
		fprintf(fout, "best fit coeffs are = %le,%le,%le,%le,%le,%le\n\n\n", p0, p1, p2, p3, p4, p5);
		printf("characterized %d number of beats\n", count);
	}	
	fclose(fout);
}


#ifdef SW
DEFINE_THREAD(bestFit)
DEFINE_THREAD(qrsDet)
#endif
DEFINE_THREAD_WITH_ARG(Sender)
DEFINE_THREAD(Receiver)

int main(int argc, char* argv[])
{
	float result;
	int I, J;

	if(argc < 2)
	{
		fprintf(stderr,"Supply data set file.\n");
		return(1);
	}


	signal(SIGINT,  Exit);
	signal(SIGTERM, Exit);

#ifdef SW
	init_pipe_handler();
	PTHREAD_DECL(bestFit);
	PTHREAD_DECL(qrsDet);
	PTHREAD_CREATE(bestFit);
	PTHREAD_CREATE(qrsDet);
#endif
	PTHREAD_DECL(Sender);
	PTHREAD_DECL(Receiver);	
	PTHREAD_CREATE_WITH_ARG(Sender, argv[1]);
	PTHREAD_CREATE(Receiver);

	PTHREAD_JOIN(Sender);
	PTHREAD_CANCEL(Receiver);
	



#ifdef SW
	PTHREAD_CANCEL(qrsDet);
	PTHREAD_CANCEL(bestFit);
	close_pipe_handler();
	return(0);
#endif
}
