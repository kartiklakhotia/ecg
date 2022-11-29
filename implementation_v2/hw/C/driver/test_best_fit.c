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
#include "best_fit.h"
#include "config.h"
#include "daemons.h"
#ifdef FPGA
#include "uart_interface.h"
#endif


void ajit_fpu___init_aa_globals__(); 
int __verif_error__ = 0; 

Config hw_config;
FILE* beats_file_pointer = NULL;
void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

void readAndReportBeatResult()
{
	uint32_t beat_index = tbGetUint32();
	uint32_t qrs_peak_index = tbGetUint32();
	uint32_t best_sigma_index = tbGetUint32();
		
	fprintf(stdout, "FIT: %d %d %d ",
				beat_index, qrs_peak_index, best_sigma_index);

	REAL mse = tbGetReal();
	
	int H = 0;
	for(H = 0; H < NHERMITE; H++)
	{
		REAL c0 = tbGetReal();
		fprintf(stdout," %le ", c0);
	}
	fprintf(stdout," %le\n", mse);
	fflush(stdout);
}



void sendBeats(FILE* fp)
{
	int I = 0;
	while(!feof(fp))
	{
		int32_t X;
		int n = fscanf(fp, "%d", &X);

		if(n == 0)
			break;
		tbSendUint16((uint16_t) X);
		I++;
#ifdef DEBUGPRINT
		fprintf(stderr,"Sent sample[%d] = 0x%x\n", I, X);
#endif
	}
	fclose (fp);
}

void Sender()
{
	sendBeats(beats_file_pointer);
}
DEFINE_THREAD(Sender);


#ifdef SW

DEFINE_THREAD(controllerDaemon)
#ifndef FRONTEND
	DEFINE_THREAD(beatDaemon);
	DEFINE_THREAD(hermiteFitterDaemon);
#endif

#endif



void setBandPassInfoInConfig(FILE* fc)
{
	int filter_order = 0;
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

		hw_config.band_pass_filter_coefficients[filter_order] = x;
		filter_order++;
	}
	fclose(fc);

	hw_config.band_pass_filter_order = filter_order;
	fprintf(stderr,"Info: band-pass filter order=%d\n", filter_order);
}

int main(int argc, char* argv[])
{
	float result;
	int I, J;

#ifdef AA2C
	ajit_fpu___init_aa_globals__(); 
#endif

#ifndef FPGA
	if(argc < 4)
	{
		fprintf(stderr,"Usage: %s <moving-average-order> <filter-coeffs-file>  <single-beat-data-file>\n", 
				argv[0]);
		return(1);
	}
#else
	if(argc < 5)
	{
		fprintf(stderr,"Usage: %s <moving-average-order> <filter-coeffs-file>  <single-beat-data-file> <tty>\n", 
				argv[0]);
		return(1);

	}

	setUartBlockingFlag(1);
	int tty_fd = setupDebugUartLink(argv[4]);
	if(tty_fd < 0)
	{
		fprintf(stderr,"Error: could not open uart %s\n", argv[4]);
		return (1);
	}

#endif


	hw_config.moving_average_filter_order = atoi(argv[1]);
	if(hw_config.moving_average_filter_order > 64)
	{
		hw_config.moving_average_filter_order = 64;
	}
	fprintf(stderr,"Info: moving average filter order=%d\n", hw_config.moving_average_filter_order);
	
	FILE* filter_coeff_file = fopen(argv[2], "r");
	if(filter_coeff_file == NULL)
	{
		fprintf(stderr,"Could not open filter coeff file %s.\n", argv[2]);
		return(1);
	}
		
	setBandPassInfoInConfig(filter_coeff_file);

	beats_file_pointer = fopen(argv[3],"r");
	if(beats_file_pointer == NULL)
	{
		fprintf(stderr,"Could not open data set file %s.\n", argv[3]);
		return(1);
	}

	fillHermitePolynomialsInConfig();

	signal(SIGINT,  Exit);
	signal(SIGTERM, Exit);

#ifdef SW
	//init_pipe_handler_with_log("pipehandler.log");
	init_pipe_handler();

	register_pipe("in_data",8,8,0);
	register_pipe("out_data",8,8,0);
	register_pipe("backend_monitor_pipe",1,32,0);

#ifdef USE_DOUBLE
	register_pipe("beat_to_hermite_fitter",2,32,0);
#else
	register_pipe("beat_to_hermite_fitter",2,32,0);
#endif
	register_pipe("current_peak_index_to_hermite_fitter",1,32,0);
	register_pipe("qrs_detect_to_beat_daemon",1,32,0);

	//init_pipe_handler();

	PTHREAD_DECL(controllerDaemon);
	PTHREAD_CREATE(controllerDaemon);


#ifndef FRONTEND

	PTHREAD_DECL(beatDaemon);
	PTHREAD_CREATE(beatDaemon);

	PTHREAD_DECL(hermiteFitterDaemon);
	PTHREAD_CREATE(hermiteFitterDaemon);
#endif

#endif

	
	// send the configuration to hardware.
	sendConfigurationToHardware(&hw_config);

	// send the beats file.
	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);

	while(1)
	{
#ifdef FRONTEND
		int32_t pval;
		pval =  tbGetUint32();
		fprintf(stderr,"TBQRS: %d\n", pval);
#else
		readAndReportBeatResult();
#endif
	}

#ifdef SW
	close_pipe_handler();
#endif
	return(0);
}
