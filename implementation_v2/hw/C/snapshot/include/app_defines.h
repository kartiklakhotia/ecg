#ifndef __app_defines_h____
#define __app_defines_h____


#ifdef USE_DOUBLE

	#define REAL double
	#define sendReal sendDouble
	#define getReal  getDouble
	#define tbSendReal tbSendDouble
	#define tbGetReal  tbGetDouble
	#define sendRealToHermiteFitter sendDoubleToHermiteFitter
	#define getRealForHermiteFitter getDoubleForHermiteFitter

#else

	#define REAL float
	#define sendReal sendFloat
	#define getReal  getFloat
	#define tbSendReal tbSendFloat
	#define tbGetReal  tbGetFloat

	#define sendRealToHermiteFitter sendFloatToHermiteFitter
	#define getRealForHermiteFitter getFloatForHermiteFitter

#endif

//
// two magic numbers for QRS peak detector.
//
#define QRS_EPSILON		 	10
#define QRS_PEAK_THRESHOLD	 	100000000

#define MIN_SIGMA  0.003*(1000.0/360.0)
#define MAX_SIGMA  0.004*(1000.0/360.0)

#define NHERMITE   6		// 6 hermite polynomials.
#define NSAMPLES   144
#define NSIGMAS	   10
#define SAMPLING_INTERVAL (1.0/360.0)

#define MEM_SIZE_PER_HF (NSAMPLES * NSIGMAS)


#endif
