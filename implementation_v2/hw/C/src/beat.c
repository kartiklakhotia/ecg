#include <stdlib.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "app_defines.h"
#include "data_structs.h"
#include "utils.h"
#include "daemons.h"
#include "beat.h"

void __aa_barrier__();

// using the current peak index, we read
// the beat out of the result buffer.
extern ResultBuffer volatile filtered_result_buffer;
extern REAL current_beat[144];

	
//
// listen on qrs_peak and if ok, generate beat which is sent to 
// backend.
//
void beatDaemon ()
{
	while(1)
	{

		// get the qrs peak index.
		uint32_t qrs_peak_index = read_uint32("qrs_detect_to_beat_daemon");

		if(qrs_peak_index >= 72)
		// peak is sufficiently advanced.
		{

			uint32_t last_windex  = qrs_peak_index + 71;
			uint32_t first_windex = qrs_peak_index - 72;

			while(1)
			// spin until enough samples are available...
			{
				uint32_t wp, lwi;
				getWrittenBeatStatus(&lwi, &wp);

				if(lwi > last_windex)
				//
				// enough samples available?
				//
				{
					uint32_t move_back_amount = (lwi - first_windex) + 1;

					// find index at the beginning of the beat in 
					// the receiver buffer.
					uint32_t start_pointer = 
						((wp > move_back_amount) ?
					 	(wp - move_back_amount) :
					 	(wp + RBUF_SIZE - move_back_amount));

					write_uint32("current_peak_index_to_hermite_fitter", qrs_peak_index);

					uint32_t J;
					for(J = 0; J < 144; J++)
					{
						// bottom 10 bits.
						uint32_t K = (start_pointer + J) & (RBUF_SIZE - 1);

						// filtered result.
						int32_t  B = getSampleValueFromFilteredResultBuffer(K);
						write_uint32("beat_to_hermite_fitter",B);


					}
					break;
				}
				
			} // try till enough values have been filled.
		}

	}
}
