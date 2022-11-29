#include <stdlib.h>
#include <Pipes.h>
#include <pipeHandler.h>
#ifdef SW
#include <assert.h>
#endif

#include "data_structs.h"
#include "utils.h"
#include "best_fit.h"
#include "daemons.h"
#include "beat.h"

ResultBuffer filtered_result_buffer;

#ifndef SW
void __aa_barrier__();
#else
void __aa_barrier__()
{
}
#endif


void initFilteredResultBuffer()
{
	filtered_result_buffer.write_pointer = 0;
	filtered_result_buffer.last_written_index = 0;

	write_uint8("result_buffer_lock", 1);
	
	int I;
	for(I = 0; I < 128; I++)
		filtered_result_buffer.buffer[I] = 0;
}

uint8_t getResultBufferLock()
{
	uint8_t rv = read_uint8("result_buffer_lock");
	return(rv);

}

void releaseResultBufferLock(uint8_t v)
{
	write_uint8("result_buffer_lock", v);
}

void  pushIntoFilteredResultBuffer(uint32_t sample_index, int32_t sample_value)
{
	uint8_t l = getResultBufferLock();
	__aa_barrier__();

	uint16_t wp = filtered_result_buffer.write_pointer;
	filtered_result_buffer.buffer[wp] = sample_value;

	__aa_barrier__();

	filtered_result_buffer.last_written_index = sample_index;


#ifdef SW
	// fprintf(stderr,"RBUF: [%d] = %d.\n", wp, sample_value);
#endif

	wp++;
	filtered_result_buffer.write_pointer = (wp & (RBUF_SIZE - 1));
	__aa_barrier__();

	releaseResultBufferLock(l);
}

void getWrittenBeatStatus (uint32_t *lwi, uint32_t *wp)
{
	uint8_t l = getResultBufferLock();
	__aa_barrier__();

	*wp  =  (filtered_result_buffer.write_pointer);
	*lwi =  (filtered_result_buffer.last_written_index);

	__aa_barrier__();
	releaseResultBufferLock(l);
	
}

// idx must be between 0 and 1023.
int32_t getSampleValueFromFilteredResultBuffer(uint32_t idx)
{
#ifdef SW
	assert(idx < RBUF_SIZE);
#endif
	
	int32_t rv = filtered_result_buffer.buffer[idx];
#ifdef SW
	// fprintf(stderr,"RBUF: %d = [%d].\n", rv, idx);
#endif
	return(rv);
}

