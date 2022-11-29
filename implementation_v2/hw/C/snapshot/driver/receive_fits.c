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

	struct timespec tval;
	timespec_get(&tval, TIME_UTC);
	fprintf(stdout,"Info:readAndReportResult: for beat %d, arrival-time = %lf\n",
				beat_index,
				((double) tval.tv_sec) + (1.0e-9 * tval.tv_nsec));
}

int main(int argc, char* argv[])
{
	while(1)
	{
		readAndReportBeatResult();
	}

}
