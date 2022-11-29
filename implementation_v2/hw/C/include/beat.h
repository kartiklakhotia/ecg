#ifndef __beat_h____
#define __beat_h____

void initFilteredResultBuffer();
void  pushIntoFilteredResultBuffer(uint32_t sample_index, int32_t sample_value);
int fetchCurrentBeat (uint32_t qrs_peak_index);
void getWrittenBeatStatus (uint32_t *lwi, uint32_t *wp);

uint8_t getResultBufferLock();
void releaseResultBufferLock(uint8_t v);

#endif
