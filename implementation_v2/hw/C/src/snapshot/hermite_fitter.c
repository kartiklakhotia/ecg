#include <stdlib.h>
#include <unistd.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "app_defines.h"
#include "data_structs.h"
#include "utils.h"
#include "best_fit.h"
#include "beat.h"
#include "daemons.h"

extern uint8_t fetch_beat_done;
extern uint8_t best_fit_done;


// current beat for processing is maintained
// here.
REAL current_beat[144];
uint32_t current_peak_index;

// best fit
HermiteFit  best_hermite_fit;

void getCurrentBeat ()
{
#ifdef BACKEND
	current_peak_index = getUint32();
#else
	current_peak_index = read_uint32("current_peak_index_to_hermite_fitter");
#endif

	int I;
	for(I=0; I < NSAMPLES; I++)
	{

#ifdef BACKEND
		current_beat[I] = (REAL) (int32_t) getUint32();
#else
		current_beat[I] = (REAL) (int32_t) read_uint32("beat_to_hermite_fitter");
#endif

#ifdef SW
		fprintf(stderr,"BEAT: %d %lf\n", (current_peak_index + I)- 72, current_beat[I]);
#endif

	}
}



void hermiteFitterDaemon() 
{

#ifdef BACKEND
	initializeHermitePolynomials();
#else
	uint8_t s = read_uint8("start_hermite_fitter");
#ifdef SW
	fprintf(stderr,"Info: started hermiteFitterDaemon\n");
#endif

	#ifdef MONITORBACKEND
		write_uint32("backend_monitor_pipe", 0x10000000);
	#endif

#endif

	uint32_t beat_index = 0;
	while(1)
	{
		
		getCurrentBeat();

#ifndef BACKEND
#ifdef MONITORBACKEND
		write_uint32("backend_monitor_pipe", 0x20000000 | current_peak_index);
#endif
#endif

#ifdef SW
		fprintf(stderr,"Info:hermiteFitterDaemon: read qrs_peak=%d.\n", current_peak_index);
#endif

#ifdef SW
		fprintf(stderr,"Info:hermiteFitterDaemon: fetched current beat=%d.\n", beat_index);
#endif
		// the existing hermite fit code.
		// result is maintained in HermiteFit
		//
		best_fit_done = findBestFit();

#ifndef BACKEND
#ifdef MONITORBACKEND
		write_uint32("backend_monitor_pipe", 0x30000000);
#endif
#endif

		//
		// set up the output fit data structure.
		//
		best_hermite_fit.beat_index = beat_index;
		best_hermite_fit.qrs_peak   = current_peak_index;
		best_hermite_fit.valid_flag = 1;

#ifndef BACKEND
#ifdef MONITORBACKEND
		write_uint32("backend_monitor_pipe", 0x40000000);
#endif
#endif

#ifdef BACKEND
		sendBestFitToOutput();
#endif 
		beat_index++;
	}
}

