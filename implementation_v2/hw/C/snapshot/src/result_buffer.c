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

void initFilteredResultBuffer()
{
	filtered_result_buffer.write_pointer = 0;
	filtered_result_buffer.last_written_index = 0;
	
	int I;
	for(I = 0; I < 128; I++)
		filtered_result_buffer.buffer[I] = 0;
}

void  pushIntoFilteredResultBuffer(uint32_t sample_index, int32_t sample_value)
{
	uint16_t wp = filtered_result_buffer.write_pointer;
	filtered_result_buffer.buffer[wp] = sample_value;
	filtered_result_buffer.last_written_index = sample_index;


	wp++;
	filtered_result_buffer.write_pointer = (wp & 0x3ff);
}

uint32_t getWritePointerInFilteredResultBuffer()
{
	return(filtered_result_buffer.write_pointer);
}

uint32_t getLastWrittenIndexInFilteredResultBuffer()
{
	return(filtered_result_buffer.last_written_index);
}


// idx must be between 0 and 1023.
int32_t getSampleValueFromFilteredResultBuffer(uint32_t idx)
{
#ifdef SW
	assert(idx < 1024);
#endif 
	return(filtered_result_buffer.buffer[idx]);
}

