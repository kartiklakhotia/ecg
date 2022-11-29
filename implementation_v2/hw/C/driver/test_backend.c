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


int __verif_error__ = 0; 

Config hw_config;
void setAllOnesInHermitePolynomials();
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



// return 0 on success.
void sendBeatsAndReceiveFits(FILE* fp)
{

	int I = 0;
	uint32_t beat_index = 0;
	uint32_t qrs_peak_index = 180;
	int sample_id = 0;
	while(!feof(fp))
	{
		if(I == 0)
		{
			tbSendUint32(qrs_peak_index);
		}

		char sacr[16];
		int32_t t;
		REAL   rX;
#ifdef USE_DOUBLE
		int n = fscanf(fp, "%s %d %lf", sacr, &t, &rX);
#else
		int n = fscanf(fp, "%s %d %f", sacr, &t, &rX);
#endif

		if(n == 0)
			break;

		int32_t X = rX;
		tbSendUint32(X);

#ifdef DEBUGPRINT
		fprintf(stderr,"Sent sample[%d] = %d\n", sample_id, X);
#endif
		sample_id++;
		I++;
	
		if(I == NSAMPLES)
		{
			I = 0;
			fprintf(stderr,"Finished beat %d\n", beat_index);
			readAndReportBeatResult();


			beat_index++;
			qrs_peak_index += 180;
		}
	}
	fprintf(stderr,"Finished feeding file\n");
}


#ifdef SW
DEFINE_THREAD(hermiteFitterDaemon);
#endif

int main(int argc, char* argv[])
{
	float result;
	int I, J;

#ifndef FPGA
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s <sbeat-data-file>\n", argv[0]);
		return(1);
	}
#else
	if(argc < 3)
	{
		fprintf(stderr,"Usage: %s  <single-beat-data-file> <tty>\n", argv[0]);
		return(1);

	}

	setUartBlockingFlag(0);
	int tty_fd = setupDebugUartLink(argv[2]);
	if(tty_fd < 0)
	{
		fprintf(stderr,"Error: could not open uart %s\n", argv[2]);
		return (1);
	}
#endif

	beats_file_pointer = fopen(argv[1],"r");
	if(beats_file_pointer == NULL)
	{
		fprintf(stderr,"Could not open beat data set file %s.\n", argv[1]);
		return(1);
	}

#ifndef USEALLONES
	fillHermitePolynomialsInConfig();
#else
	setAllOnesInHermitePolynomials();
#endif

	signal(SIGINT,  Exit);
	signal(SIGTERM, Exit);

#ifdef SW
	//init_pipe_handler_with_log("pipehandler.log");
	init_pipe_handler();

	register_pipe("in_data",8,8,0);
	register_pipe("out_data",8,8,0);


	PTHREAD_DECL(hermiteFitterDaemon);
	PTHREAD_CREATE(hermiteFitterDaemon);
#endif

	
	// send the configuration to hardware.
	sendConfigHF(hw_config.hF);

	// send the beats file.
	sendBeatsAndReceiveFits(beats_file_pointer);

	while(1)
	{
		usleep(1000000);
	}

#ifdef SW
	close_pipe_handler();
#endif
	return(0);
}
