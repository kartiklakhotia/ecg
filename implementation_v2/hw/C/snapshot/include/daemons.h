#ifndef _daemons_h____
#define _daemons_h____

void sendBestFitToOutput();

void initBandPassFilter(uint8_t filter_order);
int32_t applyBandPassFilter(int32_t sample);

void initMovingAverageFilter(uint8_t filter_order);
int32_t applyMovingAverageFilter(int32_t sample);

void initDerivativeFilter();
int32_t applyDerivativeFilter(int32_t sample);

void initQrsDetector ();
int32_t applyQrsDetector(uint32_t time_step, int32_t sample);

void initFilteredResultBuffer();
void  pushIntoFilteredResultBuffer(uint32_t sample_index, int32_t sample_value);
uint32_t getLastWrittenIndexInFilteredResultBuffer();
uint32_t getWritePointerInFilteredResultBuffer();
int32_t  getSampleValueFromFilteredResultBuffer(uint32_t idx);

void controllerDaemon();
void beatDaemon();
void hermiteFitterDaemon();

#endif
