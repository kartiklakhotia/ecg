#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
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

Config hw_config;
FILE* beats_file_pointer = NULL;

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

void sendBeats(FILE* fp)
{
	struct timespec tval;
	timespec_get(&tval, TIME_UTC);
	fprintf(stderr,"Info:sendBeats: start time = %lf\n",
				((double) tval.tv_sec) + (1.0e-9 * tval.tv_nsec));

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

		usleep(1000);
	}
	timespec_get(&tval, TIME_UTC);
	fprintf(stderr,"Info:sendBeats: stop time = %lf\n",
				((double) tval.tv_sec) + (1.0e-9 * tval.tv_nsec));
	fclose (fp);
}

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


	if(argc < 5)
	{
		fprintf(stderr,"Usage: %s <moving-average-order> <filter-coeffs-file>  <single-beat-data-file> <tty>\n", 
				argv[0]);
		return(1);
	}

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

	beats_file_pointer = fopen(argv[3],"r");
	if(beats_file_pointer == NULL)
	{
		fprintf(stderr,"Could not open data set file %s.\n", argv[3]);
		return(1);
	}
		
	setBandPassInfoInConfig(filter_coeff_file);
	fillHermitePolynomialsInConfig();

	signal(SIGINT,  Exit);
	signal(SIGTERM, Exit);

	setUartBlockingFlag(1);
	int tty_fd = setupDebugUartLink(argv[4]);
	if(tty_fd < 0)
	{
		fprintf(stderr,"Error: could not open uart %s\n", argv[5]);
		return (1);
	}
	
	// send the configuration to hardware.
	sendConfigurationToHardware(&hw_config);

	// send the ecg data
	sendBeats(beats_file_pointer);

	return(0);
}
