#ifndef _data_structs_h___
#define _data_structs_h___

#include "app_defines.h"

#define RBUF_SIZE   2048

typedef struct __FilterState128  {

	int32_t  filter_coefficients[128];
	int32_t  filter_samples[128];
	int32_t  filter_order;
	uint32_t write_pointer;
} FilterState128;

typedef struct __MovingAverageFilter64  {

	int32_t  filter_samples[64];
	int32_t  filter_order;
	int32_t  last_value;
	uint32_t write_pointer;

} MovingAverageFilter64;

typedef  struct __DerivativeFilter  {
	//
	// fixed coefficients 2,1,-1,-2
	//
	int32_t  filter_samples[4];
	uint32_t   write_pointer;
} DerivativeFilter;
void initDerivativeFilter();


// QRS detector.
typedef enum _QrsDetectorFsmState {
	RESET,
	LOOKINGFORPEAK,
	PROVISIONALPEAKFOUND,
	HALFPEAKFOUND
} QrsDetectorFsmState;

typedef struct _QrsDetector {

	QrsDetectorFsmState fsm_state;
	int32_t   peak_value;
	int32_t  peak_index;
	int32_t  half_peak_counter;
} QrsDetector;


//
// Band-pass-filter result buffer.
//
typedef struct __ResultBuffer {
	int32_t buffer[RBUF_SIZE];

	uint32_t write_pointer;
	uint32_t last_written_index;
} ResultBuffer;


typedef struct __HermiteFit {

	uint32_t beat_index;
	int32_t  qrs_peak;

	uint32_t best_sigma_index;
	REAL best_mse;
	REAL hermite_coefficients[NHERMITE];

	uint32_t valid_flag;
} HermiteFit;


#endif
