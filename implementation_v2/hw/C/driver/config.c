#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>

#include "app_defines.h"
#include "tb_utils.h"
#include "hermite.h"
#include "config.h"

	
void sendConfigHF(REAL* hf)
{
	int I;
	for(I =0; I< NHERMITE * MEM_SIZE_PER_HF; I++)
	{
		tbSendReal(hf[I]);
		int H   = (I/MEM_SIZE_PER_HF);
		int SI  = (I % MEM_SIZE_PER_HF)/NSAMPLES;
		int idx = (I % MEM_SIZE_PER_HF) % NSAMPLES;

#ifdef READBACK
		REAL rb = tbGetReal();
		if(rb != hf[I])
			fprintf(stderr,"Error:sendConfigHF:");
#ifdef DEBUGPRINT
		else
			fprintf(stderr,"Info:sendConfigHF:");
		fprintf(stderr,"  sent hf %d, %d, %d =%lf, received %lf\n", H, SI, idx, hf[I], rb);
#endif
#else
#ifdef DEBUGPRINT
		fprintf(stderr,"Info:sendConfigHF:  sent hf %d, %d, %d =%lf\n", H, SI, idx, hf[I]);
#endif
#endif
		//usleep(1000);
	}
	fprintf(stderr,"Info:sent HF\n");
}

void sendConfigurationToHardware(Config* cfg)
{
	tbSendUint8(cfg->band_pass_filter_order);
#ifdef READBACK
	uint8_t bpfo = tbGetUint8();
	if(bpfo != cfg->band_pass_filter_order)
	{
		fprintf(stderr,"Error: BPF order mismatch expected =%d, read-back%d\n", 
				cfg->band_pass_filter_order, bpfo);
	}
	else
	{
		fprintf(stderr,"Info: BPF order expected =%d, read-back%d\n", 
				cfg->band_pass_filter_order, bpfo);
	}
#endif
	usleep(1000);

	int I;
	for(I = 0; I < cfg->band_pass_filter_order; I++)
	{
		tbSendUint32((uint32_t) cfg->band_pass_filter_coefficients[I]);
#ifdef READBACK
		uint32_t fc = tbGetUint32();
		if(fc != cfg->band_pass_filter_coefficients[I])
		{
			fprintf(stderr,"Error: BPF coeff mismatch expected =%d, read-back%d\n", 
					cfg->band_pass_filter_coefficients[I], fc);
		}
		else
		{
			fprintf(stderr,"Info: BPF coeff expected =%d, read-back%d\n", 
					cfg->band_pass_filter_coefficients[I], fc);
		}
#endif
		usleep(1000);
	}

	tbSendUint8(cfg->moving_average_filter_order);
#ifdef READBACK
	uint8_t marfo = tbGetUint8();
	if(marfo != cfg->moving_average_filter_order)
	{
		fprintf(stderr,"Error: MAR order mismatch expected =%d, read-back%d\n", 
				cfg->moving_average_filter_order, marfo);
	}
	else
	{
		fprintf(stderr,"Info: MAR order expected =%d, read-back%d\n", 
				cfg->moving_average_filter_order, marfo);
	}
#endif
	usleep(1000);

#ifndef FRONTEND
	sendConfigHF(cfg->hF);
#endif
}


int readBackAndCheckHf(REAL* hF)
{
	int err = 0;
	int I;
	for(I = 0; I < NHERMITE * MEM_SIZE_PER_HF; I++)
	{
		int H   = (I/MEM_SIZE_PER_HF);
		int SI  = (I % MEM_SIZE_PER_HF)/NSAMPLES;
		int idx = (I % MEM_SIZE_PER_HF) % NSAMPLES;

		REAL X = tbGetReal();
		if(X != hF[I])
		{
			fprintf(stderr,"Error: read back hF %d, %d,  [%d]  = %lf, expected %lf\n",
					H, SI, idx, (double) X, (double) hF[I]);	
			err = 1;
			break;
		}
	}

	if(!err)
	{
		fprintf(stderr,"Info: verified HF values sent to hardware\n");
	}
	return(err);
}

